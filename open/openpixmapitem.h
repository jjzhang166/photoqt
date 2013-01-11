#ifndef OPENPIXMAPITEM_H
#define OPENPIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include <QEvent>
#include <QtDebug>
#include <QMouseEvent>
#include <QGraphicsSceneEvent>
#include <QCursor>

class OpenPixmap : public QObject, public QGraphicsPixmapItem {

	Q_OBJECT

public:
	OpenPixmap(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
	~OpenPixmap();

	// The two pixmaps for normal and hover
	QPixmap normal;
	QPixmap hover;

	// The type of item it is
	QString itemType;

	// The path of the item
	QString path;

	int pos;

	void setNormal();
	void setHover();

public slots:
	void setPixmap(const QPixmap &pixmapNormal,const QPixmap &pixmapHover, int position = 1);

protected:
	bool sceneEvent(QEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *);

signals:
	void clicked(QString type, QString path);
	void newHover(QString path);

};

#endif // OPENPIXMAPITEM_H
