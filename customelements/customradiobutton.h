#ifndef CUSTOMRADIOBUTTON_H
#define CUSTOMRADIOBUTTON_H

#include <QRadioButton>

// A custom (styled) radiobutton
class CustomRadioButton : public QRadioButton {

	Q_OBJECT

public:
	explicit CustomRadioButton(const QString &text = "", QWidget *parent = 0);
	~CustomRadioButton();

	void setBackgroundColor(QString col);
	void setColor(QString col);
	void setPadding(int pad);
	void setBorderRadius(int rad);
	void setHoverEffect(QString hovCol);

private:
	QString css;
	QString bgColour;
	QString hoverColour;
	bool hoveringEnabled;
	bool hovering;
	QString colour;
	int padding;
	int borderRadius;

	void setCSS();

private slots:
	void checkToggle();

protected:
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void mouseMoveEvent(QMouseEvent *);

};

#endif // CUSTOMRADIOBUTTON_H
