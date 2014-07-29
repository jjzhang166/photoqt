#ifndef MANIPULATELABEL_H
#define MANIPULATELABEL_H

#include <QLabel>
#include <QMenu>

class ManipulateLabel : public QLabel {

	Q_OBJECT

public:
	ManipulateLabel(QWidget *parent = 0);
	~ManipulateLabel();

protected:
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

private:
	bool selectionStarted;
	QRect selectionRect;
	QMenu contextMenu;

private slots:
	void saveSlot();

};

#endif // MANIPULATELABEL_H
