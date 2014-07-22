/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "graphicsitem.h"
#include <iostream>

// This item is used for displaying the big image
GraphicsItem::GraphicsItem(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {

	this->setTransformationMode(Qt::SmoothTransformation);

	// This movie is used for animated images
	mov = new QMovie;

	// The default transition setting is 0
	transitionSetting = 0;

	addTransBar = 0;

	// These booleans initially are set to false
	curRotated = false;
	prevRotated = false;

	itemZoomed = false;

	// Whenever a new image in the animation is reade, set it as a pixmap
	connect(mov, SIGNAL(updated(QRect)), this, SLOT(movieNextImg()));

	// Catch movie errors
	connect(mov, SIGNAL(error(QImageReader::ImageReaderError)), this, SLOT(movieError(QImageReader::ImageReaderError)));

	// Setting up a timeline (adjusted according to user settings)
	trans = new QTimeLine(300,this);
	trans->setFrameRange(0,30);
	connect(trans, SIGNAL(frameChanged(int)), this, SLOT(composePixmap(int)));

}

// When the settings have changed (called from mainwindow.cpp)
void GraphicsItem::transitionSetChange(int t) {

	transitionSetting = t;
	trans->setDuration(t > 0 ? t*100 : 1);

}

// Set the new movie image as pixmap
void GraphicsItem::movieNextImg() {
	this->setPixmap(mov->currentPixmap(),true,false,true);
}

// Set the movie and start it
void GraphicsItem::setMovie(QString m, int width, int height) {

	mov->stop();
	mov->setFileName(m);
	mov->setScaledSize(QSize(width,height));
	this->setPixmap(mov->currentPixmap(),true);
	mov->start();

}

// Set a (normal) pixmap
void GraphicsItem::setPixmap(const QPixmap &pixmap, bool dontStopMov, bool smoothTransition, bool movieUpdate, bool transitionUpdate) {

	QPixmap thisPixmap = this->pixmap();

	// For every frame in a transition, we do add a bar to each (that would cause the image to slide down every time a little more).
	QPixmap pix(pixmap);
	if(!transitionUpdate) {
		pix = QPixmap(pixmap.width(), pixmap.height()+addTransBar);
		pix.fill(QColor(0,0,0,1));
		QPainter p(&pix);
		p.drawPixmap(0,0,pixmap.width(),pixmap.height(),pixmap);
		p.end();
	}


	// Stop the movie, if function itself isn't called by the movie
	if(!dontStopMov && mov->state() == QMovie::Running) {
		mov->stop();
		QPixmap movPix(mov->currentPixmap().width(), mov->currentPixmap().height()+addTransBar);
		movPix.fill(Qt::transparent);
		QPainter movP(&movPix);
		movP.drawPixmap(0,0,mov->currentPixmap().width(),mov->currentPixmap().height(),mov->currentPixmap());
		movP.end();
		QGraphicsPixmapItem::setPixmap(movPix);
	}

	// If the movie emmited a change in its pixmap and the transition is running, simply change the current item for the transition
	if(movieUpdate && trans->state() == QTimeLine::Running) {
		curItem = pix;

	// If a transition is supposed to be done
	} else if(smoothTransition && transitionSetting != 0 && !curRotated && !prevRotated && !itemZoomed) {

		// Stop a possible previous transition and set the previous pixmap
		if(trans->state() == QTimeLine::Running) {
			trans->stop();
			QGraphicsPixmapItem::setPixmap(curItem);
			prevItem = curItem;

		// No transition currently running, update the prevItem only
		} else {
			prevItem = thisPixmap;
		}

		// The new current item
		curItem = pix;

		// Start the transition
		trans->start();

	// And if none of the above applied, we simply call the default setPixmap() function
	} else
		QGraphicsPixmapItem::setPixmap(pix);

	emit updateSceneBigRect();

}

// Composing the pixmap
void GraphicsItem::composePixmap(int i) {

	// We make the background pixmap big enough to fit the previous and current pixmap
	QPixmap newitem(qMax(curItem.width(),prevItem.width()),qMax(curItem.height(),prevItem.height()));
	newitem.fill(QColor(0,0,0,1));

	// If the new item is smaller than the previous one, we have to fade out the previous one, but leave a patch as big as the new pixmap to prevent an ugly bug (background showing through)
	if(prevItem.width() > curItem.width() || prevItem.height() > curItem.height()) {

		// Draw the patch onto newitem

		QPainter p(&newitem);

		int x = 0;
		int y = 0;
		if(curItem.width() < prevItem.width())
			x = (prevItem.width()-curItem.width())/2;
		if(curItem.height() < prevItem.height())
			y = (prevItem.height()-curItem.height())/2;
		p.drawPixmap(x,y,curItem.width(),curItem.height()-addTransBar,prevItem,x,y,curItem.width(),curItem.height());
		p.end();


	}

	QPainter prevPaint(&newitem);

	// Increase the opacity of the previous items only if it's bigger than the new one
	if(prevItem.width() > curItem.width() || prevItem.height() > curItem.height())
		prevPaint.setOpacity(1-i/30.0);
	prevPaint.setCompositionMode(QPainter::CompositionMode_SourceOver);

	// Position the previous item if either its width or height is smaller than the new item
	int x = 0;
	int y = 0;
	if(prevItem.width() < curItem.width())
		x = (curItem.width()-prevItem.width())/2;
	if(prevItem.height() < curItem.height())
		y = (curItem.height()-prevItem.height())/2;
	prevPaint.drawPixmap(x,y,prevItem.width(),prevItem.height(),prevItem);
	prevPaint.end();

	QPainter newPaint(&newitem);

	// Slowly decrease the opacity
	newPaint.setOpacity(i/30.0);
	newPaint.setCompositionMode(QPainter::CompositionMode_SourceOver);

	// Adjust the position
	x = (curItem.width() < prevItem.width()) ? (prevItem.width()-curItem.width())/2 : 0;
	y = (curItem.height() < prevItem.height()) ? (prevItem.height()-curItem.height())/2 : 0;

	newPaint.drawPixmap(x,y,curItem.width(),curItem.height(),curItem);
	newPaint.end();

	// And set the new pixmap as part of the transition
	setPixmap(newitem,true,false,false,true);

}

// Catch movie errors
void GraphicsItem::movieError(QImageReader::ImageReaderError r) {
	std::cerr << "ERROR (Animated Image): " << r << std::endl;
}

void GraphicsItem::setTransBarHeight(int height) {
	addTransBar = height;
}

GraphicsItem::~GraphicsItem() { }
