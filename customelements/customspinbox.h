#ifndef CUSTOMSPINBOX_H
#define CUSTOMSPINBOX_H

#include <QSpinBox>

// A custom (styled) spinbox
class CustomSpinBox : public QSpinBox {

	Q_OBJECT

public:
	explicit CustomSpinBox(QWidget *parent = 0);
	~CustomSpinBox();

};

#endif // CUSTOMSPINBOX_H
