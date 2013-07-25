#include "customtabwidget.h"
#include <QLayout>

TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent) {

	bold = false;
	borderTopColor = "";
	borderTopWidth = 0;
	borderBotColor = "";
	borderBotWidth = 0;
	expandTabs = true;

	tabBar()->setCursor(Qt::PointingHandCursor);

	QString css = "QTabWidget::pane {";
	css += "border-bottom: 4px double black;";
	css += "padding: 10px;";
	css += "padding-top: 20px;";
	css += "}";
	css += "QTabWidget::tab-bar {";
	css += "alignment: center;";
	css += "}";
	css += "QTabWidget::pane {";
	css += "position: absolute;";
	css += "top: -0.5em;";
	css += "}";

	this->setStyleSheet(css);

}


void TabWidget::resizeEvent(QResizeEvent *) {

	int tabCount = tabBar()->count();

	if(tabCount != 0) {

		int width = this->width()-4;

		tabWidth = width/tabCount - 14;

		setCSS();

	}

}

void TabWidget::setCSS() {


		QString css = "QTabBar::tab {";
		css += "background: rgba(30,30,30,150);";
		css += "color: rgb(150,150,150);";
		bold ? css += "font-weight: bold;" : css += "";
		css += QString("width: %1px;").arg(expandTabs ? tabWidth : 200);
		css += "padding: 5px;";
		borderTopWidth != 0 ? css += QString("border-top: %1px solid %2;").arg(borderTopWidth).arg(borderTopColor) : css += "";
		borderBotWidth != 0 ? css += QString("border-bottom: %1px solid %2;").arg(borderBotWidth).arg(borderBotColor) : css += "";
		css += "}";
		css += "QTabBar::tab:selected, QTabBar::tab:hover {";
		css += "background: rgba(103,103,103,150);";
		css += "color: white;";
		css += "}";

		tabBar()->setStyleSheet(css);

}


TabWidget::~TabWidget() { }
