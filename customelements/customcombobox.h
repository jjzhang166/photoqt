#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H

#include <QComboBox>
#include <QGraphicsOpacityEffect>

// A custom styled combobox
class CustomComboBox : public QComboBox {
	Q_OBJECT
public:
	explicit CustomComboBox(QWidget *parent = 0);
	~CustomComboBox();

	void setFontSize(int fsize);
	void setBorder(int width, QString colour);

private:
	int fontsize;
	bool enabled;

	int borderWidth;
	QString borderColour;

	void setCSS();

private slots:
	void updateToolTip(QString);

public slots:
	// When disabled, we need to adjust some colors to make it visible
	void setEnabled(bool);
	
};

#endif // CUSTOMCOMBOBOX_H
