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
	void setPadding(int pad) { padding = pad; setCSS(); }
	void setRGBA(int r, int g, int b, int a) { rgba = QString("rgba(%1,%2,%3,%4)").arg(r).arg(g).arg(b).arg(a); setCSS(); }
	void setFontSize(QString fs) { fontsize = fs; setCSS(); }
	void setBold(bool b) { bold = b; setCSS(); }

private:
	bool hover;
	bool enabled;
	int padding;
	QString fontsize;
	QString rgba;
	bool bold;

	void setCSS();

public slots:
	// When disabled, we need to adjust some colors to make it visible
	void setEnabled(bool);

protected:
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);

};

#endif // CUSTOMPUSHBUTTON_H
