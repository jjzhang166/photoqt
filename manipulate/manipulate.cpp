#include "manipulate.h"

ManipulateImage::ManipulateImage(QWidget *parent) : MyWidget("white", parent) {

	this->setVisibleArea(QSize(800,600));

	image = new ManipulateLabel;
	image->setScaledContents(true);

	QVBoxLayout *lay = new QVBoxLayout;
	lay->addWidget(image);

	this->setWidgetLayout(lay);

}

void ManipulateImage::manipulate(QString imageURL) {

	QPixmap p(imageURL);
	p = p.scaledToWidth(700);

	image->setPixmap(p);

	makeShow();

}

ManipulateImage::~ManipulateImage() { }
