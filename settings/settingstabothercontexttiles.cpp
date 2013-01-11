#include "settingstabothercontexttiles.h"

ContextTile::ContextTile(QString cmdTxt, QString descTxt, QWidget *parent) : QWidget(parent) {

	this->setMouseTracking(true);

	// The standard default stylesheet
	css = "font-weight: bold; color: black; border-radius: 5px; padding: 1px; font-size: 8pt;";

	// Some special stylesheet for hovered and normal
	cssBackgroundNorm = "background: rgba(255,255,255,100);";
	cssBackgroundHov = "background: rgba(255,255,255,200);";

	this->setStyleSheet(css + cssBackgroundNorm);

	// Main Layout
	QHBoxLayout *lay = new QHBoxLayout;

	lay->setSpacing(0);

	// Default index (i.e. invalid/unset)
	index = -1;

	// A label for dragging
	dragLabel = new QLabel("Click here to drag");
	dragLabel->setCursor(Qt::SizeAllCursor);
	dragLabel->setStyleSheet("background: transparent; color: #333333;");
	lay->addWidget(dragLabel);

	// A seperator
	QLabel *sep1 = new QLabel("|");
	sep1->setStyleSheet("background: transparent;");
	lay->addWidget(sep1);

	// The command (editable)
	cmd = new QLineEdit(cmdTxt);
	cmd->setStyleSheet("background: transparent");
	cmd->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	lay->addWidget(cmd);

	// A seperator
	QLabel *sep2 = new QLabel("|");
	sep2->setStyleSheet("background: transparent;");
	lay->addWidget(sep2);

	// The menu text (editable)
	desc = new QLineEdit(descTxt);
	desc->setStyleSheet("background: transparent");
	desc->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	lay->addWidget(desc);

	// A seperator
	QLabel *sep3 = new QLabel("|");
	sep3->setStyleSheet("background: transparent;");
	lay->addWidget(sep3);

	// Delete entry
	QPushButton *x = new QPushButton("x ");
	x->setFlat(true);
	x->setCursor(Qt::PointingHandCursor);
	x->setStyleSheet("background: transparent");
	lay->addWidget(x);
	connect(x, SIGNAL(clicked()), this, SLOT(pleaseDeleteMe()));

	lay->setMargin(3);
	this->setLayout(lay);



}

// Delete tile
void ContextTile::pleaseDeleteMe() {
	emit deleteMe(index);
}

void ContextTile::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}


ContextTile::~ContextTile() { }

