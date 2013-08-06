#ifndef CUSTOMSPINBOX_H
#define CUSTOMSPINBOX_H

#include <QSpinBox>
#include <QGraphicsOpacityEffect>

// A custom (styled) spinbox
class CustomSpinBox : public QSpinBox {

	Q_OBJECT

public:
	explicit CustomSpinBox(QWidget *parent = 0);
	~CustomSpinBox();

public slots:
	void setEnabled(bool);

private:
	void setCSS();

};

#endif // CUSTOMSPINBOX_H
