/***********************************************
 *
 * THIS CLASS IS CURRENTLY NOT USED!
 * THE CODE IS LEFT HERE, IN CASE THAT FEATURE
 * MIGHT BE IMPLEMENTED IN THE FUTURE...
 *
 * *********************************************/
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
