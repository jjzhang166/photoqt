#include "opencrumbs.h"

Crumbs::Crumbs(QWidget *parent) : QWidget(parent) {

	// The current total path
	path = "";

	// No history at startup (obviously)
	history.clear();
	historyPos = -1;

	// This is only set to true when new directory is loaded from history
	dirChangeInHistory = false;

	// The number of crumbs shown is sset later by the settings
	numCrumbsShown = 0;

	// The main layout
	layout = new QHBoxLayout;
	this->setLayout(layout);

	// The navigation buttons
	goUp = new QPushButton;
	goUp->setIcon(QIcon::fromTheme("go-up"));
	goUp->setStyleSheet("background:transparent");
	goUp->setIconSize(QSize(25,25));
	goBack = new QPushButton;
	goBack->setIcon(QIcon::fromTheme("go-previous"));
	goBack->setStyleSheet("background:transparent");
	goBack->setIconSize(QSize(25,25));
	goForw = new QPushButton;
	goForw->setIcon(QIcon::fromTheme("go-next"));
	goForw->setStyleSheet("background:transparent");
	goForw->setIconSize(QSize(25,25));
	vertLine = new QLabel("|");
	vertLine->setStyleSheet("background: transparent; color: white");
	connect(goUp, SIGNAL(clicked()), this, SLOT(goLevelUp()));
	connect(goBack, SIGNAL(clicked()), this, SLOT(goBackwards()));
	connect(goForw, SIGNAL(clicked()), this, SLOT(goForwards()));

	// Add the navigation to the main layout
	layout->addWidget(goBack);
	layout->addWidget(goUp);
	layout->addWidget(goForw);
	layout->addSpacing(5);
	layout->addWidget(vertLine);
	layout->addSpacing(5);


	// The total number of parts of the path exceed the total amount of crumbs to be shown - some are hidden on the left
	thereArePartsToTheLeft = new QLabel("..... /");
	thereArePartsToTheLeft->setStyleSheet("color: white; font-size: 12pt; background: transparent");

	// Setup all labels and crumbs, add them to the layout and the  hide them. Whenever a new path is loaded, the needed amount of crumbs are shown and set, the rest is hidden
	for(int i = 0; i < 30; ++i) {
		CrumbsPart *crumb = new CrumbsPart;
		QLabel *label = new QLabel("/");
		label->setStyleSheet("color: white; font-size: 12pt; background: transparent");
		partList.append(crumb);
		labelList.append(label);

		if(i == 0)
			layout->addWidget(thereArePartsToTheLeft);
		thereArePartsToTheLeft->hide();

		layout->addWidget(partList.at(i));
		if(i != partList.length())
			layout->addWidget(labelList.at(i));

		partList.at(i)->hide();
		labelList.at(i)->hide();

		connect(crumb, SIGNAL(clicked(QString)), this, SIGNAL(clicked(QString)));
	}

	layout->addStretch();

}

// Set a new path
void Crumbs::setCrumbs(QString p) {

	if(p.endsWith("/"))
		p = p.remove(p.length()-1,1);

	path = p;

	// Hide every previously possivbly shown crumbs
	for(int i = 0; i < partList.length(); ++i) {
		partList.at(i)->hide();
		labelList.at(i)->hide();
	}

	int skip = 0;

	// Split the path into its components
	QStringList parts = p.split("/");
	if(parts.length() > numCrumbsShown)
		skip = parts.length() - numCrumbsShown;

	// The first item is the top lever "ROOT" item
	if(parts.at(0) == "") {
		parts.removeFirst();
		parts.insert(0,"ROOT");
	}

	if(parts.last() == "")
		parts.removeLast();

	QString path = "/";

	int j = 0;

	if(skip != 0)
		thereArePartsToTheLeft->show();
	else
		thereArePartsToTheLeft->hide();

	// Set the paths to the crumbs
	for(int i = 0; i < parts.length(); ++i) {
		// skip>0 means, that this item is hidden
		if(skip == 0) {
			if(parts.at(i) != "ROOT")
				path += parts.at(i) + "/";
			partList.at(j)->setText(parts.at(i));
			partList.at(j)->path = path;
			partList.at(j)->show();
			if(j-1 >= 0)
				labelList.at(j-1)->show();
			++j;
		} else {
			if(parts.at(i) != "ROOT")
				path += parts.at(i) + "/";
			--skip;
		}
	}

	// Update the history
	if(!dirChangeInHistory) {
		if(historyPos+1 < history.length()) {
			int len = history.length();
			for(int i = historyPos+1; i < len; ++i)
				history.removeLast();
		}
		history.append(path);
		++historyPos;
	} else
		dirChangeInHistory = false;

	// Add a stretch to the layout to make everything align at the left
	layout->addStretch();

	// You can only go up, if you're not already at the top
	if(p == "")
		goUp->setEnabled(false);
	else
		goUp->setEnabled(true);

	// You can only go back if there is a previous item
	if(historyPos == 0)
		goBack->setEnabled(false);
	else
		goBack->setEnabled(true);

	// You can only go forwards if there is a next item
	if(historyPos+1 == history.length())
		goForw->setEnabled(false);
	else
		goForw->setEnabled(true);

}

// Go a level up
void Crumbs::goLevelUp() {

	QStringList parts = path.split("/");
	parts.removeLast();

	if(parts.length() == 1 && parts.at(0) == "") {
		parts.clear();
		parts.append("/");
	}

	if(parts.join("/") != "")
		emit clicked(parts.join("/"));

}

// Go back in the history
void Crumbs::goBackwards() {

	if(historyPos > 0 && historyPos <= history.length()) {
		--historyPos;
		dirChangeInHistory = true;
		emit clicked(history.at(historyPos));
	}

}

// Go forward in the history
void Crumbs::goForwards() {

	if(historyPos+1 < history.length()) {
		++historyPos;
		dirChangeInHistory = true;
		emit clicked(history.at(historyPos));
	}

}

Crumbs::~Crumbs() { }


// The single crumbs themselves
CrumbsPart::CrumbsPart(QString txt) : QPushButton(txt) {

	path = "";

	this->setStyleSheet("color: white; background: transparent; font-weight: bold; font-size: 12pt");

}

void CrumbsPart::mouseReleaseEvent(QMouseEvent *e) {

	emit clicked(path);

	QPushButton::mouseReleaseEvent(e);

}

CrumbsPart::~CrumbsPart() { }
