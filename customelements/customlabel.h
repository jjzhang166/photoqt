/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

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
	void setItalic(bool i) { italic = i; setCSS(); }
	void setPadding(int p) { padding = p; setCSS(); }
	void setFontSize(int fs) { fontsize = fs; setCSS(); }
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
	int fontsize;
	QString fontcolor;
	QString backgroundcolor;
	int borderwidth;
	QString bordercolor;
	bool bold;
	bool italic;
	int padding;
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
