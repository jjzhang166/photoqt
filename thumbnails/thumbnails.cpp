#include "thumbnails.h"

Thumbnails::Thumbnails(QMap<QString, QVariant> set, bool verbose, QWidget *parent) : QWidget(parent) {

	this->setObjectName("viewThumbs");
	this->setStyleSheet("#viewThumbs { background: rgba(0,0,0,100); }");

	globSet = set;
	this->verbose = verbose;

	animateInAndOut = false;
	counttot = 0;
	countpos = -1;
	currentfile = "";

	view = new ThumbnailView(verbose, globSet);
	connect(view, SIGNAL(movedScroll()), this, SLOT(scrolledView()));

	thread = new ThumbThread();
	thread->verbose = verbose;
	connect(thread, SIGNAL(centerOnCurrent(bool)), this, SLOT(centerOnCurrent(bool)));
	connect(thread, SIGNAL(updateThumb(QImage,QString,int,int,int,bool)), this, SLOT(updateThumb(QImage,QString,int,int,int,bool)));

	// The animation instances
	ani = new QPropertyAnimation(this,"geometry");
	isShown = false;
	connect(ani, SIGNAL(finished()), this, SLOT(aniFinished()));

	// The geometries
	rectShown = QRect(0,0,10,10);
	rectHidden = QRect(0,0,10,10);

	// Adjust some properties of the small GraphicsView for Thumbnails
	if(globSet.value("ThumbnailKeepVisible").toBool()) {
		this->setGeometry(rectShown);
		isShown = true;
	} else
		this->setGeometry(rectHidden);

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(view);
	layout->setMargin(0);
	this->setLayout(layout);

}

void Thumbnails::loadDir(QString filepath) {

	view->scene.clear();
	allPixmaps.clear();
	allPixmapsPath.clear();

	currentfile = filepath;
	counttot = 0;

	QDir dir(QFileInfo(filepath).absolutePath());

	// These are the images known by PhotoQt
	QStringList flt = globSet.value("KnownFileTypes").toString().split(",");
	if(imageFilter.join("").trimmed().length() == 0)
		dir.setNameFilters(flt);
	else {
		QStringList flt_approved;
		foreach(QString f, imageFilter) {
			if(f.startsWith(".") && flt.contains(f.remove(0,1)))
				flt_approved.append("*" + f);
			else if(!f.startsWith("."))
				flt_approved.append("*" + f + "*");

		}
		dir.setNameFilters(flt_approved);
	}


	// Store a QFileInfoList and a QStringList with the filenames
	allImgsInfo = dir.entryInfoList(QDir::Files,QDir::IgnoreCase);

	// When opening an unknown file (i.e., one that doesn't match any set format), then we need to manually add it to the list of loaded images
	if(!allImgsInfo.contains(QFileInfo(currentfile))) allImgsInfo.append(QFileInfo(currentfile));

	// Sort images...
	bool asc = globSet.value("SortImagesAscending").toBool();
	if(globSet.value("SortImagesBy").toString() == "naturalname")
		std::sort(allImgsInfo.begin(),allImgsInfo.end(),(asc ? sort_naturalname : sort_naturalname_desc));
	else if(globSet.value("SortImagesBy").toString() == "date")
		std::sort(allImgsInfo.begin(),allImgsInfo.end(),(asc ? sort_date: sort_date_desc));
	else if(globSet.value("SortImagesBy").toString() == "size")
		std::sort(allImgsInfo.begin(),allImgsInfo.end(),(asc ? sort_size : sort_size_desc));
	else
		std::sort(allImgsInfo.begin(),allImgsInfo.end(),(asc ? sort_name : sort_name_desc));


	// Storing number of images
	counttot = allImgsInfo.length();

	if(!globSet.value("ThumbnailDisable").toBool())
		// Start loading directory
		startThread();

	QRectF r = view->scene.itemsBoundingRect();
	r.setX(0);
	r.setWidth(counttot*(globSet.value("ThumbnailSize").toInt()+globSet.value("ThumbnailSpacingBetween").toInt()));
	r.setHeight(globSet.value("ThumbnailSize").toInt()+globSet.value("ThumbnailLiftUp").toInt()-2);
	view->setSceneRect(r);


}

void Thumbnails::startThread() {

	int newpos = allImgsInfo.indexOf(QFileInfo(currentfile));
	if(newpos == -1) newpos = 0;

	thread->newData(newpos,
			globSet.value("ThumbnailSize").toInt()+globSet.value("ThumbnailSpacingBetween").toInt(),
			this->width(),
			globSet.value("ThumbnailPreloadFullDirectory").toBool(),
			globSet.value("ThumbnailPreloadNumber").toInt(),
			globSet.value("ThumbnailFilenameInstead").toBool(),
			globSet.value("ThumbnailDynamic").toInt());

	thread->verbose = verbose;

	thread->counttot = allImgsInfo.length();
	thread->allimgs = allImgsInfo;
	thread->height = globSet.value("ThumbnailSize").toInt();
	thread->typeCache = (globSet.value("ThbCacheFile").toBool() ? "files" : "database");

	thread->cacheEnabled = globSet.value("ThumbnailCache").toBool();

	thread->start();

}

void Thumbnails::updateThumb(QImage img, QString path, int origwidth, int origheight, int pos, bool preload) {

	// Some general settings that are needed multiple times later-on
	int size = globSet.value("ThumbnailSize").toInt();
	int spacing = globSet.value("ThumbnailSpacingBetween").toInt();
	int liftup = globSet.value("ThumbnailLiftUp").toInt();

	bool showFilename = globSet.value("ThumbnailWriteFilename").toBool();
	bool showDimensions = (globSet.value("ThumbnailWriteResolution").toBool() && origwidth != 0 && origheight != 0 && !globSet.value("ThbCacheFile").toBool());

	bool filenameOnly = globSet.value("ThumbnailFilenameInstead").toBool();
	int filenameOnlyFontsize = globSet.value("ThumbnailFilenameInsteadFontSize").toInt();

	int fontsize = globSet.value("ThumbnailFontSize").toInt();

	QString thumbpos = globSet.value("ThumbnailPosition").toString();

	// Preload thumbnail
	if(preload) {

		// Two transparent thumbnails
		QPixmap norm(size+spacing,size+liftup);
		norm.fill(Qt::transparent);
		QPainter paint(&norm);

		QPixmap hov(size+spacing,size+liftup);
		hov.fill(Qt::transparent);
		QPainter paintHov(&hov);

		// Both load an empty "?" thumbnail
		QIcon f(filenameOnly ? ":/img/nothumb.png" : ":/img/emptythumb.png");
		if(thumbpos == "Bottom") {

			paint.drawPixmap(spacing/2,liftup,size,size,f.pixmap(size,size));
			paintHov.drawPixmap(spacing/2,0,size,size,f.pixmap(size,size));

		} else if(thumbpos == "Top") {

			paint.drawPixmap(spacing/2,0,size,size,f.pixmap(size,size));
			paintHov.drawPixmap(spacing/2,liftup,size,size,f.pixmap(size,size));

		}

		// Write filename and/or dimensions
		QTextDocument txt;
		if(showFilename || showDimensions) {
			QString textdocTXT = QString("<center><div style=\"text-align: center; font-size: %1pt; font-weight: bold; color: white; background: %2; border-radius: 10px\">").arg(filenameOnly ? filenameOnlyFontsize : fontsize).arg(filenameOnly ? "transparent" : "rgba(0,0,0,80)");
			if(showFilename) textdocTXT += QFileInfo(path).fileName();
			if(showDimensions) {
				if(showFilename) textdocTXT += "<br><i>(";
				textdocTXT += QString("%1:%2").arg(origwidth).arg(origheight);
				if(showFilename) textdocTXT += ")</i>";
			}
			textdocTXT += "</div></center>";

			txt.setHtml(textdocTXT);
			txt.setTextWidth(size);
			if(thumbpos == "Bottom") {
				paint.translate(0,size*(filenameOnly ? 0.1 : ((showFilename && showDimensions) ? 0.55 : 0.70)));
				paintHov.translate(0,size*(filenameOnly ? 0.1 : ((showFilename && showDimensions) ? 0.55 : 0.70)));
			} else if(thumbpos== "Top") {
				paint.translate(0,(filenameOnly ? size*0.1 : size/8.0));
				paintHov.translate(0,(filenameOnly ? size*0.1 : size/8.0));
			}
			txt.drawContents(&paint);
			txt.drawContents(&paintHov);
		}

		paint.end();
		paintHov.end();

		// Create Pixmap item
		ThumbnailPixmapItem *pixitem = new ThumbnailPixmapItem;
		pixitem->path = path;
		pixitem->position = pos;
		pixitem->setData(1,pixitem->path);
		pixitem->setPixmap(norm,hov);
		pixitem->setPos(pos*(size+spacing),0);
		connect(pixitem, SIGNAL(clicked(QString)), this, SLOT(gotClick(QString)));

		// Add to list
		allPixmaps.insert(path, pixitem);
		allPixmapsPath.insert(pos, path);

		// And add to scene
		view->scene.addItem(pixitem);

	} else {

		// The thumbnail image
		QPixmap pix(img.width(),img.height());
		pix = QPixmap::fromImage(img);
		if(pix.height() > size)
			pix = pix.scaledToHeight(size);
		if(pix.width() > size)
			pix = pix.scaledToWidth(size);

		// The normal image
		QPixmap imgNorm(size+spacing,size+liftup);
		imgNorm.fill(Qt::transparent);
		QPainter paint(&imgNorm);
		paint.setCompositionMode(QPainter::CompositionMode_SourceOver);

		// The hover image
		QPixmap imgHov(size+spacing,size+liftup);
		imgHov.fill(Qt::transparent);
		QPainter paintSel(&imgHov);
		paintSel.setCompositionMode(QPainter::CompositionMode_SourceOver);

		// Draw actual thumbnail
		int xPix = (size-pix.width())/2;
		int yPix = (size-pix.height())/2;
		int wPix = pix.width();
		int hPix = pix.height();
		if(thumbpos == "Bottom") {

			paint.fillRect(0,liftup,size+spacing,size,QColor::fromRgba(qRgba(0,0,0,150)));
			paintSel.fillRect(0,0,size+spacing,size,QColor::fromRgba(qRgba(0,0,0,150)));

			paint.drawPixmap(xPix+spacing/2,liftup+yPix,wPix,hPix,pix);
			paintSel.drawPixmap(xPix+spacing/2,yPix,wPix,hPix,pix);

		} else if(thumbpos == "Top") {

			paint.fillRect(0,0,size+spacing,size,QColor::fromRgba(qRgba(0,0,0,150)));
			paintSel.fillRect(0,liftup,size+spacing,size,QColor::fromRgba(qRgba(0,0,0,150)));

			paint.drawPixmap(xPix+spacing/2,yPix,wPix,hPix,pix);
			paintSel.drawPixmap(xPix+spacing/2,yPix+liftup,wPix,hPix,pix);

		}

		// Write filename and/or dimensions
		QTextDocument txt;
		if(showFilename || showDimensions) {

			QString textdocTXT = QString("<center><div style=\"text-align: center; font-size: %1pt; font-wight: bold; color: white; background: rgba(0,0,0,150); border-radius: 10px\">").arg(fontsize);
			if(showFilename && showDimensions) textdocTXT += QFileInfo(path).fileName() + "<br><i>(" + QString("%1:%2").arg(origwidth).arg(origheight) + ")</i>";
			else if(showFilename) textdocTXT += QFileInfo(path).fileName();
			else if(showDimensions) textdocTXT += QString("%1:%2").arg(origwidth).arg(origheight);
			textdocTXT += "</div></center>";

			txt.setHtml(textdocTXT);
			txt.setTextWidth(size);
			if(globSet.value("ThumbnailPosition") == "Bottom") {
				paint.translate(0,size*((showFilename && showDimensions) ? 0.55 : 0.70));
				paintSel.translate(0,size*((showFilename && showDimensions) ? 0.55 : 0.70));
			} else if(globSet.value("ThumbnailPosition") == "Top") {
				paint.translate(0,size/8.0);
				paintSel.translate(0,size/8.0);
			}

			txt.drawContents(&paint);
			txt.drawContents(&paintSel);

		}

		paint.end();
		paintSel.end();

		// And update pixmaps (the check is a safety measure, shouldn't really be necessary though)
		if(allPixmaps.keys().contains(path))
			allPixmaps.value(path)->setPixmap(imgNorm,imgHov);

	}

}

void Thumbnails::centerOnCurrent(bool firstLoad) {

	if(globSet.value("ThumbnailCenterActive").toBool() || firstLoad)
		view->centerOn(allPixmaps.value(currentfile));
	else
		view->ensureVisible(allPixmaps.value(currentfile));

	allPixmaps.value(currentfile)->presented = true;
	allPixmaps.value(currentfile)->setHoverImg();

}

// Jump to first/last image in list
void Thumbnails::gotoFirstLast(QString side) {

	if(verbose) std::clog << "thb: Got to first/Last: " << side.toStdString() << std::endl;

	if(allImgsInfo.length()) {

		if(side == "first")
			gotClick(allImgsInfo.first().absoluteFilePath());
		else if(side == "last")
			gotClick(allImgsInfo.last().absoluteFilePath());

	}

}

// Update hover/norm pixmap
void Thumbnails::updateThbViewHoverNormPix(QString oldpath, QString newpath) {

	if(!globSet.value("ThumbnailDisable").toBool()) {

		if(oldpath != "" && allPixmaps.keys().contains(oldpath)) {
			allPixmaps.value(oldpath)->presented = false;
			allPixmaps.value(oldpath)->setNormalImg();
		}
		if(newpath != "" && allPixmaps.keys().contains(newpath)) {
			allPixmaps.value(newpath)->presented = true;
			allPixmaps.value(newpath)->setHoverImg();
		}

	}

}

// Got a click on an item
void Thumbnails::gotClick(QString path) {

	if(verbose) std::clog << "thb: gotClick:" << path.toStdString() << std::endl;

	updateThbViewHoverNormPix(currentfile,path);
	setCurrentfile(path);
	countpos = getImageFilePathIndexOf(path);

	emit loadNewImg(path);

}

// The view has been scrolled (needed for dynamic thumbnail creation)
void Thumbnails::scrolledView() {

	// Stop thread
	if(thread->isRunning()) {
		thread->abort();
		thread->wait();
	}

	// Get new path/position
	QString pixPath = "";
	int newpos = -1;

	int thbsize = globSet.value("ThumbnailSize").toInt();
	if(view->mapToScene(view->rect()).boundingRect().contains(QRect(allPixmaps.value(currentfile)->pos().toPoint(), QSize(thbsize,thbsize))))
		newpos = allPixmaps.value(currentfile)->position;
	else {
		QPoint center = view->viewport()->visibleRegion().boundingRect().center();
		// We get the position of the item in view center
		QGraphicsItem *pix = (QGraphicsItem*)view->itemAt(center);
		pixPath = pix->data(1).toString();
		if(pixPath.trimmed() == "") {
			int v = view->scrollbar->value();
			int size = globSet.value("ThumbnailSize").toInt();
			newpos = v/(size+globSet.value("ThumbnailSpacingBetween").toInt())
					+ view->width()/(2*size);
		} else
			newpos = getImageFilePathIndexOf(pixPath);
	}

	if(verbose) std::clog << "Central item: " << pixPath.toStdString() << std::endl;

	// Update data
	thread->updateData(newpos,
			   globSet.value("ThumbnailSize").toInt(),
			   view->width(),
			   globSet.value("ThumbnailPreloadFullDirectory").toBool(),
			   globSet.value("ThumbnailPreloadNumber").toInt(),
			   globSet.value("ThumbnailFilenameInstead").toBool(),
			   globSet.value("ThumbnailDynamic").toInt());

	// Start thread
	thread->start();


}

void Thumbnails::setGlobSet(QMap<QString, QVariant> set) {

	 globSet = set;
	 view->globSet = set;

	 QString qtfiles = globSet.value("KnownFileTypesQt").toString();
	 QString qtfiles_extras = globSet.value("KnownFileTypesQtExtras").toString();
	 thread->qtfiles = qtfiles;
	 if(qtfiles_extras.trimmed().length() != 0) thread->qtfiles += "," + qtfiles_extras;
	 thread->gmfiles = globSet.value("KnownFileTypesGm").toString();

}

// FOR SORTING, WE HAVE ALL FUNCTIONS FOR DESCENDING AND ASCENDING CASE, AS THIS IS FASTER THAN REVERSING THE ORDER
// AFTERWARDS (PARTICULARLY FOR DIRECTORIES WITH A LARGE NUMBER OF FILES

bool Thumbnails::sort_name(const QFileInfo &s1fileinfo, const QFileInfo &s2fileinfo) {
	return(s1fileinfo.fileName().compare(s2fileinfo.fileName(), Qt::CaseInsensitive) <= 0);
}
bool Thumbnails::sort_name_desc(const QFileInfo &s1fileinfo, const QFileInfo &s2fileinfo) {
	return(s2fileinfo.fileName().compare(s1fileinfo.fileName(), Qt::CaseInsensitive) <= 0);
}
bool Thumbnails::sort_date(const QFileInfo &s1fileinfo, const QFileInfo &s2fileinfo) {
	return(s1fileinfo.created().secsTo(s2fileinfo.created()) >= 0);
}
bool Thumbnails::sort_date_desc(const QFileInfo &s1fileinfo, const QFileInfo &s2fileinfo) {
	return(s1fileinfo.created().secsTo(s2fileinfo.created()) < 0);
}
bool Thumbnails::sort_size(const QFileInfo &s1fileinfo, const QFileInfo &s2fileinfo) {
	return(s1fileinfo.size() >= s2fileinfo.size());
}
bool Thumbnails::sort_size_desc(const QFileInfo &s1fileinfo, const QFileInfo &s2fileinfo) {
	return(s1fileinfo.size() < s2fileinfo.size());
}

// Algorithm used for sorting a directory using natural sort
// Credits to: http://www.qtcentre.org/archive/index.php/t-21411.html
bool Thumbnails::sort_naturalname(const QFileInfo& s1fileinfo,const QFileInfo& s2fileinfo) {

	const QString s1 = s1fileinfo.fileName();
	const QString s2 = s2fileinfo.fileName();

	// ignore common prefix..
	int i = 0;

	while ((i < s1.length()) && (i < s2.length()) && (s1.at(i).toLower() == s2.at(i).toLower()))
		++i;
	++i;

	// something left to compare?
	if ((i < s1.length()) && (i < s2.length())) {

		// get number prefix from position i - doesnt matter from which string
		int k = i-1;

		//If not number return native comparator
		if(!s1.at(k).isNumber() || !s2.at(k).isNumber()) {

			//Two next lines
			//E.g. 1_... < 12_...
			if(s1.at(k).isNumber())
				return false;
			if(s2.at(k).isNumber())
				return true;
			return QString::compare(s1, s2, Qt::CaseSensitive) < 0;
		}

		QString n = "";
		k--;

		while ((k >= 0) && (s1.at(k).isNumber())) {
			n = s1.at(k)+n;
			--k;
		}

		// get relevant/signficant number string for s1
		k = i-1;
		QString n1 = "";
		while ((k < s1.length()) && (s1.at(k).isNumber())) {
			n1 += s1.at(k);
			++k;
		}

		// get relevant/signficant number string for s2
		//Decrease by
		k = i-1;
		QString n2 = "";
		while ((k < s2.length()) && (s2.at(k).isNumber())) {
			n2 += s2.at(k);
			++k;
		}

		// got two numbers to compare?
		if (!n1.isEmpty() && !n2.isEmpty())
			return (n+n1).toInt() < (n+n2).toInt();
		else {
			// not a number has to win over a number.. number could have ended earlier... same prefix..
			if (!n1.isEmpty())
				return false;
			if (!n2.isEmpty())
				return true;
			return s1.at(i) < s2.at(i);
		}
	} else {
		// shortest string wins
		return s1.length() < s2.length();
	}
}
bool Thumbnails::sort_naturalname_desc(const QFileInfo& s1fileinfo,const QFileInfo& s2fileinfo) {

	const QString s2 = s1fileinfo.fileName();
	const QString s1 = s2fileinfo.fileName();

	// ignore common prefix..
	int i = 0;

	while ((i < s1.length()) && (i < s2.length()) && (s1.at(i).toLower() == s2.at(i).toLower()))
		++i;
	++i;

	// something left to compare?
	if ((i < s1.length()) && (i < s2.length())) {

		// get number prefix from position i - doesnt matter from which string
		int k = i-1;

		//If not number return native comparator
		if(!s1.at(k).isNumber() || !s2.at(k).isNumber()) {

			//Two next lines
			//E.g. 1_... < 12_...
			if(s1.at(k).isNumber())
				return false;
			if(s2.at(k).isNumber())
				return true;
			return QString::compare(s1, s2, Qt::CaseSensitive) < 0;
		}

		QString n = "";
		k--;

		while ((k >= 0) && (s1.at(k).isNumber())) {
			n = s1.at(k)+n;
			--k;
		}

		// get relevant/signficant number string for s1
		k = i-1;
		QString n1 = "";
		while ((k < s1.length()) && (s1.at(k).isNumber())) {
			n1 += s1.at(k);
			++k;
		}

		// get relevant/signficant number string for s2
		//Decrease by
		k = i-1;
		QString n2 = "";
		while ((k < s2.length()) && (s2.at(k).isNumber())) {
			n2 += s2.at(k);
			++k;
		}

		// got two numbers to compare?
		if (!n1.isEmpty() && !n2.isEmpty())
			return (n+n1).toInt() < (n+n2).toInt();
		else {
			// not a number has to win over a number.. number could have ended earlier... same prefix..
			if (!n1.isEmpty())
				return false;
			if (!n2.isEmpty())
				return true;
			return s1.at(i) < s2.at(i);
		}
	} else {
		// shortest string wins
		return s1.length() < s2.length();
	}
}

void Thumbnails::makeShow() {

	if(!isShown)
		animate();

}

void Thumbnails::makeHide() {

	if(isShown)
		animate();

}

// The animation function
void Thumbnails::animate() {

	// Open widget
	if(!isShown) {

		if(ani->state() != QPropertyAnimation::Stopped)
			ani->targetObject()->setProperty(ani->propertyName(),ani->endValue());

		ani->setDuration(500);
		isShown = true;

		ani->setStartValue(rectHidden);
		ani->setEndValue(rectShown);
		ani->setEasingCurve(QEasingCurve::InBack);
		ani->start();

		this->raise();

	// Close widget
	} else if(isShown) {

		if(ani->state() != QPropertyAnimation::Stopped)
			ani->targetObject()->setProperty(ani->propertyName(),ani->endValue());

		ani->setDuration(500);
		isShown = false;

		ani->setStartValue(rectShown);
		ani->setEndValue(rectHidden);
		ani->setEasingCurve(QEasingCurve::OutBack);
		ani->start();
	}

}

void Thumbnails::aniFinished() {

	animateInAndOut = false;

}

void Thumbnails::setRect(QRect rect) {

	// Adjust the thumbnail geometry
	rectShown = QRect(rect);
	if(rect.y() != 0)
		rectHidden = QRect(0,rect.bottomLeft().y(),rect.width(),rect.height());
	else
		rectHidden = QRect(0,0,rect.width(),-rect.height());

	if(isShown) this->setGeometry(rectShown);
	else this->setGeometry(rectHidden);

}


// Enabling styling of widget
void Thumbnails::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

Thumbnails::~Thumbnails() { }
