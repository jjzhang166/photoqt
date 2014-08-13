#include "cropbar.h"

CropBar::CropBar(QWidget *parent) : QWidget(parent) {

}




// Enabling styling of widget
void CropBar::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}
