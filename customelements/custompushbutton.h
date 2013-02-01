#ifndef CUSTOMPUSHBUTTON_H
#define CUSTOMPUSHBUTTON_H

#include <QPushButton>

// A custom QPushButton, restyled
class CustomPushButton : public QPushButton {

	Q_OBJECT

public:
	explicit CustomPushButton(const QString &text = "", QWidget *parent = 0);
	~CustomPushButton();

	// Per default the button has rounded corners - can be disabled
	void removeRoundedCorners();

private:
	bool noRoundedCorners;

public slots:
	// When disabled, we need to adjust some colors to make it visible
	void setEnabled(bool);

};

#endif // CUSTOMPUSHBUTTON_H
