#include "dropdownmenuitem.h"

// A custom QLabel used for the menu
MainMenuItem::MainMenuItem(QString txt, QString icon, QString setData, int closeMe, bool clickable) : QLabel(txt) {

	this->setStyleSheet("background: black");

	// Enable mouse tracking for hover effect
	this->setMouseTracking(true);

	ssNormal = "QLabel { background: transparent; border: none; color: grey; } QLabel:disabled { color: white; font-weight: bold }";
	ssHover = "QLabel { background: transparent; border: none; color: white; } QLabel:disabled { color: white; font-weight: bold; }";

	// Set the default stylesheet
	MainMenuItem::setStyleSheet(ssNormal);

	// Set the transmitted data
	data = setData.trimmed();
	close = closeMe;
	this->setEnabled(clickable);
	setIcon(icon);

	this->setCursor(Qt::PointingHandCursor);

}

// Get all the needed events and connect them to the slots
bool MainMenuItem::event(QEvent *e) {

	if(e->type() == QEvent::MouseMove)
		MainMenuItem::setStyleSheet(ssHover);
	else if(e->type() == QEvent::Leave)
		MainMenuItem::setStyleSheet(ssNormal);

	if(e->type() == QEvent::MouseButtonPress)
		emit clicked();

	QList<QEvent::Type> types;
	types << QEvent::MouseButtonDblClick;
	types << QEvent::MouseButtonPress;
	types << QEvent::MouseButtonRelease;

	if(!types.contains(e->type()))
		QLabel::event(e);

	return e->isAccepted();

}

// Add an icon to the front of the string
void MainMenuItem::setIcon(QString icon) {

	if(icon != "")
		this->setText("<img height=\"13\" src=\"" + icon + "\">&nbsp;" + this->text());

}

MainMenuItem::~MainMenuItem() { }
