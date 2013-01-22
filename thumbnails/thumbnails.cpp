#include "thumbnails.h"

Thumbnails::Thumbnails(QWidget *parent, bool v) : QWidget(parent) {

	verbose = v;

	QHBoxLayout *layout = new QHBoxLayout;
	this->setLayout(layout);

	// The total amount of images, current position, and the filepath of current item
	counttot = 0;
	countpos = 0;
	currentfile = "";

	// The view and scene
	view = new ThumbnailView;
	view->setScene(&scene);
	view->setCursor(Qt::ArrowCursor);
	scene.clear();

	this->setStyleSheet("background: transparent");

	scrollbar = new CustomScrollbar;
	view->setHorizontalScrollBar(scrollbar);

	// The animation instances
	ani = new QPropertyAnimation(this,"geometry");
	isShown = false;

	// The geometries
	rectShown = QRect(0,0,10,10);
	rectHidden = QRect(0,0,10,10);

	// The thumbnail thread
	thumbThread = new ThumbThread;
	thumbThread->verbose = verbose;
	connect(thumbThread, SIGNAL(updateThb(QImage,QString,int)), this, SLOT(updateThumb(QImage,QString,int)));

	layout->addWidget(view);

}

// The animation function
void Thumbnails::animate() {

	// Open widget
	if(ani->state() == QPropertyAnimation::Stopped && !isShown) {

		if(verbose) qDebug() << "thb: Animate in";

		ani->setDuration(500);
		isShown = true;

		ani->setStartValue(rectHidden);
		ani->setEndValue(rectShown);
		ani->setEasingCurve(QEasingCurve::InBack);
		ani->start();

		this->raise();

	// Close widget
	} else if(ani->state() == QPropertyAnimation::Stopped && isShown) {

		if(verbose) qDebug() << "thb: Animate out";

		ani->setDuration(500);
		isShown = false;

		ani->setStartValue(rectShown);
		ani->setEndValue(rectHidden);
		ani->setEasingCurve(QEasingCurve::OutBack);
		ani->start();
	}

}

// Load the directory
void Thumbnails::loadDir() {

	// Clear the scene
	scene.clear();

	// Clear all possibly existing pixmaps
	allPixmaps.clear();

	// Reset the counter
	counttot = 0;

	// Reset Info and Paths
	allImgsInfo.clear();
	allImgsPath.clear();


	// Get the current directory (and if for some reason photo got a directory, don't go up a level)
	QString currentdir = "";
	if(QFileInfo(currentfile).isFile())
		currentdir = QFileInfo(currentfile).absoluteDir().absolutePath();
	else
		currentdir = currentfile;

	if(verbose) qDebug() << "thb: Load directory:" << currentdir << "- nothb:" << noThumbs;

	// Get QDir instance
	QDir *dir = new QDir(currentdir);

	// These are the images known by photo
	QStringList flt = globSet.value("KnownFileTypes").toString().split(",");
	dir->setNameFilters(flt);

	// Store a QFileInfoList and a QStringList with the filenames
	allImgsInfo = dir->entryInfoList(QDir::NoFilter,QDir::IgnoreCase);

	for(int i = 0; i < allImgsInfo.length(); ++i)
		allImgsPath << allImgsInfo.at(i).absoluteFilePath();

	// Storing number of images
	counttot = allImgsPath.length();

	// If thumbnails aren't disabled
	if(!noThumbs) {

		// Use Image Thumbnails or Filename-Only Thumbnails
		bool filenameInsteadThumb = globSet.value("ThumbnailFilenameInstead").toBool();
		int filenameInsteadFontSize = globSet.value("ThumbnailFilenameInsteadFontSize").toInt();

		int normsqu = globSet.value("ThumbnailSize").toInt();
		int border = globSet.value("ThumbnailBorderAround").toInt();
		int liftup = globSet.value("ThumbnailLiftUp").toInt();

		for(int i = 0; i < counttot; ++i) {

			// The normal image pixmap
			QPixmap imgNorm(normsqu-border,normsqu-border+liftup);
			imgNorm.fill(Qt::transparent);
			QPainter paint(&imgNorm);
			paint.setCompositionMode(QPainter::CompositionMode_SourceOver);
			QPen pen(Qt::white);
			paint.setPen(pen);
			QIcon f;
			if(!filenameInsteadThumb)
				f.addFile(":/img/emptythumb.png");
			else
				f.addFile(":/img/nothumb.png");
			paint.drawPixmap((border)/2,liftup+(border)/2,normsqu-border,normsqu-border,f.pixmap(normsqu-border,normsqu-border));
			QTextDocument txt;
			if(filenameInsteadThumb) {
				txt.setHtml(QString("<center><div style=\"text-align: center; font-size: %1pt; font-wight: bold; color: white; background: none;\">" + allImgsInfo.at(i).fileName() + "</div></center>").arg(filenameInsteadFontSize));
				txt.setTextWidth(normsqu);
				paint.translate(0,liftup);
				txt.drawContents(&paint);
			} else {
				txt.setHtml("<center><div style=\"text-align: center; font-size: 8pt; font-wight: bold; color: white; background: rgba(0,0,0,200); border-radius: 10px;\">" + allImgsInfo.at(i).fileName() + "</div></center>");
				txt.setTextWidth(normsqu);
				paint.translate(3,normsqu*2/3.0);
				txt.drawContents(&paint);
			}
			paint.end();

			// The hover image pixmap
			QPixmap imgHov(normsqu-border,normsqu-border+liftup);
			imgHov.fill(Qt::transparent);
			QPainter paint2(&imgHov);
			paint2.setCompositionMode(QPainter::CompositionMode_SourceOver);
			paint2.setPen(pen);
			paint2.drawPixmap((border)/2,(border)/2,normsqu-border,normsqu-border,f.pixmap(normsqu-border,normsqu-border));
			if(filenameInsteadThumb) {

				if(verbose) qDebug() << "thb: Use filename thumbs";

				txt.setHtml(QString("<center><div style=\"text-align: center; font-size: %1pt; font-wight: bold; color: white; background: none;\">" + allImgsInfo.at(i).fileName() + "</div></center>").arg(filenameInsteadFontSize));
				txt.setTextWidth(normsqu);
				paint2.translate(0,0);
				txt.drawContents(&paint2);

			} else {
				txt.setHtml("<center><div style=\"text-align: center; font-size: 8pt; font-wight: bold; color: white; background: rgba(0,0,0,200); border-radius: 10px;\">" + allImgsInfo.at(i).fileName() + "</div></center>");
				txt.setTextWidth(normsqu);
				paint2.translate(3,normsqu*2/3.0);
				txt.drawContents(&paint2);
			}
			paint2.end();

			// Create the pixmapitem, set pixmaps and adjust the position
			ThumbnailPixmapItem *pix = new ThumbnailPixmapItem;
			pix->path = allImgsPath.at(i);
			pix->setPixmap(imgNorm,imgHov);
			pix->setPos(i*normsqu,0);
			connect(pix, SIGNAL(clicked(QString)), this, SLOT(gotClick(QString)));

			// Add to list
			allPixmaps.append(pix);

			// And add to scene
			scene.addItem(pix);

		}

		// Adjust scene rect
		scene.setSceneRect(scene.itemsBoundingRect());

		// If image thumbnails are wanted, start the thread
		if(!filenameInsteadThumb) {

			if(verbose) qDebug() << "thb: Start loading thumbs";

			// Set and start the thumbnail thread
			thumbThread->counttot = allImgsPath.length();
			thumbThread->allimgs.clear();
			thumbThread->allimgs.append(allImgsInfo);
			thumbThread->height = globSet.value("ThumbnailSize").toInt();
			if(globSet.value("ThumbnailCache").toBool())
				thumbThread->cacheEnabled = true;
			else
				thumbThread->cacheEnabled = false;
			thumbThread->breakme = false;
			thumbThread->dbName = "thumb";
			if(globSet.value("ThbCacheFile").toBool())
				thumbThread->typeCache = "files";
			else
				thumbThread->typeCache = "database";
			if(allImgsInfo.length())
				thumbThread->start();

		}

	}

}

// Interrupt the thumbnail creation
void Thumbnails::stopThbCreation() {

	thumbThread->breakme = true;

}

// Update a thumbnail
void Thumbnails::updateThumb(QImage img, QString path, int pos) {

	if(verbose) qDebug() << "thb: Update thumb:" << pos << "-" << path;

	// Default size
	int size = globSet.value("ThumbnailSize").toInt();
	int border = globSet.value("ThumbnailBorderAround").toInt();
	int liftup = globSet.value("ThumbnailLiftUp").toInt();

	// The thumbnail image
	QPixmap pix(img.width(),img.height());
	pix = QPixmap::fromImage(img);
	if(pix.height() > size-border)
		pix = pix.scaledToHeight(size-border);
	if(pix.width() > size-border)
		pix = pix.scaledToWidth(size-border);

	// The normal image
	QPixmap imgNorm(size,size+liftup);
	imgNorm.fill(Qt::transparent);
	QPainter paint(&imgNorm);
	paint.setCompositionMode(QPainter::CompositionMode_SourceOver);
	int xPix = (size-pix.width())/2;
	int yPix = (size-pix.height())/2;
	int wPix = pix.width();
	int hPix = pix.height();
	if(globSet.value("ThumbnailPosition") == "Bottom") {
		paint.fillRect(0,liftup,size,size,QColor::fromRgba(qRgba(0,0,0,150)));
		paint.drawPixmap(xPix,liftup+yPix,wPix,hPix,pix);
	} else if(globSet.value("ThumbnailPosition") == "Top") {
		paint.fillRect(0,0,size,size,QColor::fromRgba(qRgba(0,0,0,150)));
		paint.drawPixmap(xPix,yPix,wPix,hPix,pix);
	}
	QTextDocument txt;
	txt.setHtml("<center><div style=\"text-align: center; font-size: 7pt; font-wight: bold; color: white; background: rgba(0,0,0,150); border-radius: 10px\">" + QFileInfo(path).fileName() + "</div></center>");
	txt.setTextWidth(size);
	if(globSet.value("ThumbnailPosition") == "Bottom")
		paint.translate(0,size*3/4.0);
	else if(globSet.value("ThumbnailPosition") == "Top")
		paint.translate(0,size/8.0);
	txt.drawContents(&paint);
	paint.end();

	// The hover image
	QPixmap imgHov(size,size+liftup);
	imgHov.fill(Qt::transparent);
	QPainter paintSel(&imgHov);
	paintSel.setCompositionMode(QPainter::CompositionMode_SourceOver);
	int xSel = (size-pix.width())/2;
	int ySel = (size-pix.height())/2;
	int wSel = pix.width();
	int hSel = pix.height();
	if(globSet.value("ThumbnailPosition") == "Bottom") {
		paintSel.fillRect(0,0,size,size,QColor::fromRgba(qRgba(0,0,0,150)));
		paintSel.drawPixmap(xSel,ySel,wSel,hSel,pix);
	} else if(globSet.value("ThumbnailPosition") == "Top") {
		paintSel.fillRect(0,liftup,size,size,QColor::fromRgba(qRgba(0,0,0,150)));
		paintSel.drawPixmap(xSel,ySel+liftup,wSel,hSel,pix);
	}
	if(globSet.value("ThumbnailPosition") == "Bottom")
		paintSel.translate(0,size*3/4.0);
	else if(globSet.value("ThumbnailPosition") == "Top")
		paintSel.translate(0,size/8.0);
	txt.drawContents(&paintSel);
	paintSel.end();

	if(allPixmaps.length() >= pos) {
		allPixmaps.at(pos)->setPixmap(imgNorm,imgHov);
		allPixmaps.at(pos)->path = path;
	}

}

// Got a click on an item
void Thumbnails::gotClick(QString path) {

	if(verbose) qDebug() << "thb: gotClick:" << path;

	updateThbViewHoverNormPix(currentfile,path);

	emit loadNewImg(path);

}

// Update hover/norm pixmap
void Thumbnails::updateThbViewHoverNormPix(QString oldpath, QString newpath) {

	if(!noThumbs) {

		if(oldpath != "" && allImgsPath.indexOf(oldpath) != -1) {
			allPixmaps.at(allImgsPath.indexOf(oldpath))->presented = false;
			allPixmaps.at(allImgsPath.indexOf(oldpath))->setNormalImg();
		}
		if(newpath != "" && allImgsPath.indexOf(newpath) != -1) {
			allPixmaps.at(allImgsPath.indexOf(newpath))->presented = true;
			allPixmaps.at(allImgsPath.indexOf(newpath))->setHoverImg();
		}

	}

}

// Jump to first/last image in list
void Thumbnails::gotoFirstLast(QString side) {

	if(verbose) qDebug() << "thb: Got to first/Last:" << side;

	if(allImgsPath.length()) {

		if(side == "first")
			gotClick(allImgsPath.first());
		else if(side == "last")
			gotClick(allImgsPath.last());

	}

}

// Enabling styling of widget
void Thumbnails::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}



Thumbnails::~Thumbnails() { }
