#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>
#include <QGraphicsOpacityEffect>

class CustomLabel : public QLabel {
	Q_OBJECT

public:
	CustomLabel(const QString &text = "", QWidget *parent = 0);
	~CustomLabel();

	void setBold(bool b) { bold = b; setCSS(); }
	void setFontSize(QString fs) { fontsize = fs; setCSS(); }
	void setFontColor(QString col) { fontcolor = col; setCSS(); }
	void setBackgroundColor(QString col) { backgroundcolor = col; setCSS(); }
	void setBorder(int width, QString col) { borderwidth = width; bordercolor = col; setCSS(); }
	void setLineHeight(QString lh) { lineheight = lh; setCSS(); }

	void setToolTip(QString t);
	void setToolTipNoWrap(bool b);

	void setData(QString d) { data = d; }
	QString getData() { return data; }


private:
	void setCSS();
	QString fontsize;
	QString fontcolor;
	QString backgroundcolor;
	int borderwidth;
	QString bordercolor;
	bool bold;
	bool tooltipNoWrap;
	QString lineheight;


	QString data;

public slots:
	void setEnabled(bool);
	void setDisabled(bool b) { setEnabled(!b); }

protected:
	void mouseReleaseEvent(QMouseEvent *) {
		emit clicked();
	}
signals:
	void clicked();

};


#endif	// CUSTOMLABEL_H
