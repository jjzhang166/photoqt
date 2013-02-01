#ifndef CUSTOMSCROLLBAR_H
#define CUSTOMSCROLLBAR_H

#include <QScrollBar>
#include <QTimeLine>
#include <QStyleOption>
#include <QPainter>
#include <QTimer>
#include <QtDebug>
#include <QStylePainter>
#include <QPaintEvent>

// This is a customized scrollbar that fades out when not in use
class CustomScrollbar : public QScrollBar {

	Q_OBJECT

public:
	explicit CustomScrollbar(QWidget *parent = 0);
	~CustomScrollbar();

	// Per default the scrollbar fades out when not used. On demand it can be forced to stay always visible
	void keepAlwaysVisible(bool keepVisible);

private:
	// This timer is the time after it is stopped being used before it fades out
	QTimer *hideTimer;

	// Booleans storing if bar is hovered or clicked
	bool hovered;
	bool clicked;

	// The alpha values (full shown and the current state (updated when fading out)
	int alphaShow;
	int alphaCur;
	// The timeline for fading out
	QTimeLine *fadeOut;

	bool dontfadeout;

private slots:
	// Start Fading out
	void hideTimerTimeout();
	// Step by step fading out
	void fadeOutStep();

public slots:
	// Set scrollbar full shown
	void setScrollbarShown();

protected:
	// needed mouse events
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void leaveEvent(QEvent *);
	void enterEvent(QEvent *);

};

#endif // CUSTOMSCROLLBAR_H
