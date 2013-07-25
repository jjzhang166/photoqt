#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>

// A custom QTabWidget
class TabWidget : public QTabWidget {

	Q_OBJECT

public:
	explicit TabWidget(QWidget *parent = 0);
	~TabWidget();

	void makeBold(bool b) { bold = b; }
	void setBorderTop(QString col, int wid) { borderTopColor = col; borderTopWidth = wid; setCSS(); }
	void setBorderBot(QString col, int wid) { borderBotColor = col; borderBotWidth = wid; setCSS(); }
	void expand(bool exp) { expandTabs = exp; }

private:
	void setCSS();
	int tabWidth;
	bool bold;
	QString borderTopColor;
	int borderTopWidth;
	QString borderBotColor;
	int borderBotWidth;
	bool expandTabs;

protected:
	void resizeEvent(QResizeEvent *);

};

#endif // CUSTOMTABWIDGET_H
