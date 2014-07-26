#ifndef CUSTOMLINE_H
#define CUSTOMLINE_H

#include <QFrame>

class CustomLine : public QFrame {
public:
	explicit CustomLine(QString color = "white", Qt::Orientation o = Qt::Horizontal) {
		if(o == Qt::Vertical) {
			this->setFixedWidth(1);
			this->setFrameShape(QFrame::VLine);
		} else {
			this->setFixedHeight(1);
			this->setFrameShape(QFrame::HLine);
		}
		this->setFrameShadow(QFrame::Sunken);
		this->setStyleSheet("background-color: " + color);
	}

};

#endif // CUSTOMLINE_H
