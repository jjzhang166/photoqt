#include "quicksettings.h"

QuickSettings::QuickSettings(QMap<QString, QVariant> set, bool v, QWidget *parent) : QWidget(parent) {

	verbose = v;

	// The global settings
	globSet = set;

	// style widget
	this->setStyleSheet("QWidget { border-radius: 8px; border-bottom-right-radius: 0px; border-top-right-radius: 0px; background-color: rgba(0, 0, 0, 200); }");
	this->setContentsMargins(10,10,10,10);

	// Initiate and set layout
	central = new QVBoxLayout;
	this->setLayout(central);

	// Setup animation
	ani = new QPropertyAnimation(this,"geometry");

	// A boolean storing if widget is shown or hidden
	isShown = false;

	// block widget in certain circumstances
	blockAll = false;

	// Store if widget shall be triggered my mouse movement
	mouseTrickerEnable = true;

	// The dimensions of the widget (shown and hidden)
	rectShown = QRect(0,50,300,450);
	rectHidden = QRect(-10,-10,10,10);

	// Initially the widget it hidden
	this->setGeometry(rectHidden);




	CustomLabel *title = new CustomLabel(tr("Quick Settings"));
	title->setAlignment(Qt::AlignCenter);
	title->setFontSize(13);
	title->setBold(true);

	CustomLabel *desc = new CustomLabel(tr("Change settings with one click. They are saved and applied immediately. If you're unsure what a setting does, check the full settings for descriptions."));

	composite = new CustomCheckBox(tr("Enable composite"));
	composite->setChecked(globSet.value("Composite").toBool());
	connect(composite, SIGNAL(toggled(bool)), this, SLOT(settingChanged()));
	trayicon = new CustomCheckBox(tr("Minimize to system tray"));
	trayicon->setChecked(globSet.value("TrayIcon").toBool());
	connect(trayicon, SIGNAL(toggled(bool)), this, SLOT(settingChanged()));
	loopthroughfolder = new CustomCheckBox(tr("Loop through folder"));
	loopthroughfolder->setChecked(globSet.value("LoopThroughFolder").toBool());
	connect(loopthroughfolder, SIGNAL(toggled(bool)), this, SLOT(settingChanged()));
	windowmode = new CustomCheckBox(tr("Window mode"));
	windowmode->setChecked(globSet.value("WindowMode").toBool());
	connect(windowmode, SIGNAL(toggled(bool)), this, SLOT(settingChanged()));
	clickonempty = new CustomCheckBox(tr("Close on click on background"));
	clickonempty->setChecked(globSet.value("CloseOnGrey").toBool());
	connect(clickonempty, SIGNAL(toggled(bool)), this, SLOT(settingChanged()));
	thumbnailskeepvisible = new CustomCheckBox(tr("Keep thumbnails visible"));
	thumbnailskeepvisible->setChecked(globSet.value("ThumbnailKeepVisible").toBool());
	connect(thumbnailskeepvisible, SIGNAL(toggled(bool)), this, SLOT(settingChanged()));
	thumbnailsdynamic = new CustomCheckBox(tr("Dynamic thumbnails"));
	thumbnailsdynamic->setChecked(globSet.value("ThumbnailDynamic").toBool());
	connect(thumbnailsdynamic, SIGNAL(toggled(bool)), this, SLOT(settingChanged()));

	CustomPushButton *showsettings = new CustomPushButton(tr("Show full settings"));
	QHBoxLayout *showsettingsLay = new QHBoxLayout;
	showsettingsLay->addStretch();
	showsettingsLay->addWidget(showsettings);
	showsettingsLay->addStretch();

	QSignalMapper *mapper = new QSignalMapper;
	mapper->setMapping(showsettings, "__settings");
	connect(showsettings, SIGNAL(clicked()), mapper, SLOT(map()));
	connect(showsettings, SIGNAL(clicked()), this, SLOT(animate()));
	connect(mapper, SIGNAL(mapped(QString)), this, SIGNAL(emulateShortcut(QString)));



	central->addWidget(title);
	central->addSpacing(10);
	central->addWidget(desc);

	central->addSpacing(15);
	central->addWidget(composite);
	central->addSpacing(10);
	central->addWidget(trayicon);
	central->addSpacing(10);
	central->addWidget(loopthroughfolder);
	central->addSpacing(10);
	central->addWidget(windowmode);
	central->addSpacing(10);
	central->addWidget(clickonempty);
	central->addSpacing(10);
	central->addWidget(thumbnailskeepvisible);
	central->addSpacing(10);
	central->addWidget(thumbnailsdynamic);
	central->addSpacing(20);
	central->addLayout(showsettingsLay);

	central->addStretch();


}


void QuickSettings::settingChanged() {

	globSet.remove("Composite");
	globSet.insert("Composite",composite->isChecked());

	globSet.remove("TrayIcon");
	globSet.insert("TrayIcon",trayicon->isChecked());

	globSet.remove("LoopThroughFolder");
	globSet.insert("LoopThroughFolder",loopthroughfolder->isChecked());

	globSet.remove("WindowMode");
	globSet.insert("WindowMode",windowmode->isChecked());

	globSet.remove("CloseOnGrey");
	globSet.insert("CloseOnGrey",clickonempty->isChecked());

	globSet.remove("ThumbnailKeepVisible");
	globSet.insert("ThumbnailKeepVisible",thumbnailskeepvisible->isChecked());

	globSet.remove("ThumbnailDynamic");
	globSet.insert("ThumbnailDynamic",thumbnailsdynamic->isChecked());

	emit updateSettings(globSet);

}


void QuickSettings::setRect(QRect fullscreen) {

	rectShown = QRect(fullscreen.width()-300, (fullscreen.height()-450)/3, 300, 450);
	rectHidden = QRect(fullscreen.width()+rectShown.width(),rectShown.y(),rectShown.width(),rectShown.height());

}

void QuickSettings::makeShow() {
	if(!isShown) animate();
}

void QuickSettings::makeHide() {
	if(isShown) animate();
}

// Animate open/close the widget
void QuickSettings::animate() {

	// Open widget
	if(!isShown) {

		if(ani->state() != QPropertyAnimation::Stopped)
			ani->targetObject()->setProperty(ani->propertyName(),ani->endValue());

		ani->setDuration(600);
		isShown = true;

		ani->setStartValue(rectHidden);
		ani->setEndValue(rectShown);
		ani->setEasingCurve(QEasingCurve::InBack);
		ani->start();

		this->raise();

	// Close widget
	} else if(isShown) {

		if(ani->state() != QPropertyAnimation::Stopped)
			ani->targetObject()->setProperty(ani->propertyName(),ani->endValue());

		ani->setDuration(500);
		isShown = false;

		ani->setStartValue(rectShown);
		ani->setEndValue(rectHidden);
		ani->setEasingCurve(QEasingCurve::OutBack);
		ani->start();

	}

}

// Make the widget styleable
void QuickSettings::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}
