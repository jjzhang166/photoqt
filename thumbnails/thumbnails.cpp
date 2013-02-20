#include "thumbnails.h"

Thumbnails::Thumbnails(QWidget *parent, bool v, QMap<QString,QVariant> set) : QWidget(parent) {

	this->setObjectName("viewThumbs");

	globSet = set;

	verbose = v;

	QHBoxLayout *layout = new QHBoxLayout;
	this->setLayout(layout);

	// The total amount of images, current position, and the filepath of current item
	counttot = 0;
	countpos = 0;
	currentfile = "";

	// The view and scene
	view = new ThumbnailView(globSet);
	view->setDragMode(QGraphicsView::ScrollHandDrag);
	view->setMouseTracking(true);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	// Load needed thumbnails (dynamic thumbnail creation)
	connect(view, SIGNAL(movedScroll()), this, SLOT(scrolledView()));

	this->setStyleSheet("background: transparent");

	// The animation instances
	ani = new QPropertyAnimation(this,"geometry");
	isShown = false;
	connect(ani, SIGNAL(finished()), this, SLOT(aniFinished()));

	// The geometries
	rectShown = QRect(0,0,10,10);
	rectHidden = QRect(0,0,10,10);

	// The thumbnail thread
	thumbThread = new ThumbThread;
	thumbThread->verbose = verbose;
	connect(thumbThread, SIGNAL(updateThb(QImage,QString,int)), this, SLOT(updateThumb(QImage,QString,int)));

	layout->addWidget(view);

	// Adjust some properties of the small GraphicsView for Thumbnails
	if(globSet.value("ThumbnailKeepVisible").toBool()) {
		this->setGeometry(rectShown);
		isShown = true;
	} else
		this->setGeometry(rectHidden);

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

void Thumbnails::aniFinished() {

//	if(animateInAndOut && isShown)
//		QTimer::singleShot(400, this, SLOT(animate()));

	animateInAndOut = false;

}

// Load the directory
void Thumbnails::loadDir() {

	// Clear the scene
	view->scene.clear();

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
			pix->setData(1,pix->path);
			pix->setPixmap(imgNorm,imgHov);
			pix->setPos(i*normsqu,0);
			connect(pix, SIGNAL(clicked(QString)), this, SLOT(gotClick(QString)));

			// Add to list
			allPixmaps.append(pix);

			// And add to scene
			view->scene.addItem(pix);

		}

		// Adjust scene rect
		view->scene.setSceneRect(view->scene.itemsBoundingRect());

		newlyLoadedDir = true;

	}

}

void Thumbnails::startThread() {

	// If image thumbnails are wanted, start the thread
	if(!globSet.value("ThumbnailFilenameInstead").toBool() && newlyLoadedDir) {

		newlyLoadedDir = false;

		if(verbose) qDebug() << "thb: Start loading thumbs";

		view->thbWidth = globSet.value("ThumbnailSize").toInt();


		int newpos = 0;

		if(view->scene.width() > this->width()) {
			QPoint center = view->viewport()->visibleRegion().boundingRect().center();
			if(center != QPoint(0,0)) {
				ThumbnailPixmapItem *pix = (ThumbnailPixmapItem*)view->itemAt(center);
				QString pixpath = pix->path;
				newpos = allImgsPath.indexOf(pixpath);
			} else
				newpos = -1;
			if(newpos == -1) {
				// Get new middle position
				newpos = allImgsPath.indexOf(currentfile);
				if(newpos < 0)
					newpos = 0;
				if(newpos >= counttot)
					newpos = counttot-1;
			}
		}
		thumbThread->currentPos = newpos;

		thumbThread->verbose = verbose;
		// Set and start the thumbnail thread
		thumbThread->counttot = allImgsPath.length();
		thumbThread->allimgs.clear();
		thumbThread->allimgs.append(allImgsInfo);
		thumbThread->viewWidth = this->width();
		// This is cleared here, because the run() function in the tread is also called for updates only (i.e. this list needs to be preserved in that case)
		thumbThread->posCreated.clear();
		thumbThread->thbWidth = globSet.value("ThumbnailSize").toInt();
		thumbThread->dynamicThumbs = globSet.value("ThumbnailDynamic").toBool();
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

	// We set this boolean to true, and this causes drawImg() in mainwindow.cpp NOT to ensure the visibility of the item (it already is visible). Before occasionally this led to the thumbnailview "jumping" a little to the right/left ensuring the visibility.
	thumbLoadedThroughClick = true;

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

void Thumbnails::setRect(QRect rect) {

	// Adjust the thumbnail geometry
	rectShown = QRect(rect);
	if(rect.y() != 0)
		rectHidden = QRect(0,rect.bottomLeft().y(),rect.width(),rect.height());
	else
		rectHidden = QRect(0,0,rect.width(),-rect.height());

	qDebug() << rectShown << "-" << rectHidden << isShown;

	if(isShown) this->setGeometry(rectShown);
	else this->setGeometry(rectHidden);

}

void Thumbnails::makeShow() {

	qDebug() << "SHOW" << isShown;

	if(!isShown)
		animate();

}

void Thumbnails::makeHide() {

	if(isShown)
		animate();

}

// The view has been scrolled (needed for dynamic thumbnail creation)
void Thumbnails::scrolledView(bool forceUpdate) {

	// We get the position of the item in view center
	QGraphicsItem *pix = (QGraphicsItem*)view->itemAt(view->viewport()->visibleRegion().boundingRect().center());
	QString pixPath = pix->data(1).toString();
	int newpos = allImgsPath.indexOf(pixPath);

	// And submit data to thread
	// The amUpdatingData bool is true as long as data is updated, and the thread is sitting idle in that time
	thumbThread->amUpdatingData = true;
	// Set data
	thumbThread->newData(newpos,globSet.value("ThumbnailSize").toInt(),view->width());

	// Finished
	thumbThread->amUpdatingData = false;
	// If thread wasn't even running, start it
	if(!thumbThread->isRunning())
		thumbThread->start();


}

// Enabling styling of widget
void Thumbnails::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}


void Thumbnails::ensureThumbLoad() {

	view->scrollbar->setValue(view->scrollbar->value()+1);

}


Thumbnails::~Thumbnails() { }
