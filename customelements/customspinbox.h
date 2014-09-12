#ifndef CUSTOMSPINBOX_H
#define CUSTOMSPINBOX_H

#include <QSpinBox>
#include <QGraphicsOpacityEffect>

// A custom (styled) spinbox
class CustomSpinBox : public QSpinBox {

	Q_OBJECT

public:
	explicit CustomSpinBox(QWidget *parent = 0);
	~CustomSpinBox();

private:
	QString backgroundCol;
	QString selectionCol;
	QString borderCol;
	int borderWidth;
	QString fontcolor;

public slots:
	void setEnabled(bool);
	void setBackground(QString col, QString selectionCol) { backgroundCol = col; this->selectionCol = selectionCol; setCSS(); }
	void setBorder(QString col, int w) { borderCol = col; borderWidth = w; setCSS(); }
	void setFontColor(QString col) { fontcolor = col; setCSS(); }

private:
	void setCSS();

};

#endif // CUSTOMSPINBOX_H
