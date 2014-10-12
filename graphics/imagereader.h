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

#ifndef IMAGEREADER_H
#define IMAGEREADER_H

#include <QObject>
#include <QImage>
#include <QImageReader>
#include <QtDebug>
#include <QPainter>
#include <QTextDocument>
#include <QFileInfo>

#ifdef GM
#include <GraphicsMagick/Magick++/Image.h>
#endif

class ImageReader : public QObject {

public:
	ImageReader(bool v);
	~ImageReader();

	bool verbose;

	QString qtfiles;
	QString gmfiles;

	QImage imageRead;

	QImage readImage(QString filename, int rotation, bool zoomed, QSize maxSize, bool dontscale = false);


	float scaleImg1;
	float scaleImg2;

	bool animatedImg;
	QString fileformat;
	QSize origSize;

	bool doIUseMagick(QString filename);
};

#endif // IMAGEREADER_H
