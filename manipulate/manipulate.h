#ifndef MANIPULATE_H
#define MANIPULATE_H

#include "../customelements/customlabel.h"
#include "../customelements/custompushbutton.h"
#include "../widgets/mywidget.h"
#include "manipulatelabel.h"

#include <QRubberBand>

#include <QMenu>
#include <QFileDialog>

class ManipulateImage : public MyWidget {

	Q_OBJECT

public:
	explicit ManipulateImage(QWidget *parent = 0);
	~ManipulateImage();

	void manipulate(QString imageURL);

private:
	ManipulateLabel *image;
//	QPoint rubberOrig;
//	QRubberBand *rubberBand;


//	bool selectionStarted;
//	   QRect selectionRect;
//	   QMenu contextMenu;

protected:
//	void mousePressEvent(QMouseEvent *e);
//	void mouseMoveEvent(QMouseEvent *e);
//	void mouseReleaseEvent(QMouseEvent *e);


//	void paintEvent(QPaintEvent *e);
//	    void mousePressEvent(QMouseEvent *e);
//	    void mouseMoveEvent(QMouseEvent *e);
//	    void mouseReleaseEvent(QMouseEvent *e);

//private slots:
//    void saveSlot();

};


#endif // MANIPULATE_H
