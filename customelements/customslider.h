#ifndef CUSTOMSLIDER_H
#define CUSTOMSLIDER_H

#include <QSlider>

// A custom (styled) slider
class CustomSlider : public QSlider {

	Q_OBJECT

public:
	explicit CustomSlider(Qt::Orientation orientation = Qt::Horizontal, QWidget *parent = 0);
	~CustomSlider();

protected:
	void mousePressEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);

};

#endif // CUSTOMSLIDER_H
