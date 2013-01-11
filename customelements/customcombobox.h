#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H

#include <QComboBox>

// A custom styled combobox
class CustomComboBox : public QComboBox {
	Q_OBJECT
public:
	explicit CustomComboBox(QWidget *parent = 0);
	~CustomComboBox();

public slots:
	// When disabled, we need to adjust some colors to make it visible
	void setEnabled(bool);
	
};

#endif // CUSTOMCOMBOBOX_H
