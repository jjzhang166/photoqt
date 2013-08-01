#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>
#include <QGraphicsOpacityEffect>

class CustomLabel : public QLabel {
	Q_OBJECT

public:
	CustomLabel(const QString &text = "", QWidget *parent = 0);
	~CustomLabel();

	void setFontSize(QString fs) { fontsize = fs; setCSS(); }

private:
	void setCSS();
	QString fontsize;

public slots:
	void setEnabled(bool);

protected:
	void mouseReleaseEvent(QMouseEvent *e) {
		emit clicked();
	}
signals:
	void clicked();

};


#endif	// CUSTOMLABEL_H
