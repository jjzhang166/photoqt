#ifndef CUSTOMRADIOBUTTON_H
#define CUSTOMRADIOBUTTON_H

#include <QRadioButton>

// A custom (styled) radiobutton
class CustomRadioButton : public QRadioButton {

	Q_OBJECT

public:
	explicit CustomRadioButton(const QString &text = "", QWidget *parent = 0);
	~CustomRadioButton();

};

#endif // CUSTOMRADIOBUTTON_H
