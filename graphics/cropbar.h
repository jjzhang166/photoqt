#ifndef CROPBAR_H
#define CROPBAR_H

#include <QWidget>
#include <QStyleOption>
#include <QPainter>

class CropBar : public QWidget {

	Q_OBJECT

public:
	explicit CropBar(QWidget *parent = 0);

protected:
	void paintEvent(QPaintEvent *);

};



#endif // CROPBAR_H
