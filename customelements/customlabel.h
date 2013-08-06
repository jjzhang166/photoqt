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
	void setToolTipColor(QString ttcol) { tooltipColor = ttcol; setCSS(); }
	void setFontColor(QString col) { fontcolor = col; setCSS(); }

private:
	void setCSS();
	QString fontsize;
	QString fontcolor;
	QString tooltipColor;
	bool bold;

public slots:
	void setEnabled(bool);
	void setDisabled(bool b) { setEnabled(!b); }

protected:
	void mouseReleaseEvent(QMouseEvent *e) {
		emit clicked();
	}
signals:
	void clicked();

};


#endif	// CUSTOMLABEL_H
