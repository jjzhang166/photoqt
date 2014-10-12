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

#ifndef GLOBSHORTCUTS_H
#define GLOBSHORTCUTS_H

#include <QObject>
#include <QMap>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QVariant>
#include <iostream>

class Shortcuts : public QObject {

	Q_OBJECT

public:

	bool verbose;

	// Store all the shortcuts
	QMap<QString,QList<QVariant> > allKeyShortcuts;
	QMap<QString,QList<QVariant> > allMouseShortcuts;

	// The version (set by mainwindow.cpp)
	QString version;

	// Load all shortcuts
	void loadSH() {

		if(verbose) std::clog << "Loading Shortcuts" << std::endl;

		allKeyShortcuts.clear();
		allMouseShortcuts.clear();

		QFile file(QDir::homePath() + "/.photoqt/shortcuts");
		if(!file.exists())
			setDefault();
		else {
			file.open(QIODevice::ReadOnly);
			QTextStream in(&file);

			// Go through all the shortcuts
			QStringList all = in.readAll().split("\n");
			for(int i = 0; i < all.size(); ++i) {

				if(!all.at(i).startsWith("Version=") && all.at(i).trimmed() != "") {

					QStringList sh = all.at(i).split("::");

					// Check if it's a mouse action
					bool mouse = false;
					QString tmp = sh.at(1);
					if(tmp.startsWith("[M]"))
						mouse = true;

					// This list stores two values: close, and command
					QList<QVariant> listNew;
					listNew << bool(sh.at(0).toInt()) << sh.at(2);

					if(mouse)
						allMouseShortcuts.insert(tmp,listNew);
					else
						allKeyShortcuts.insert(tmp,listNew);

				}

			}

			file.close();
		}

	}

	// Save the current shortcuts
	void saveSH() {

		if(verbose) std::clog << "sh: Save Shortcuts" << std::endl;

		// The version is always the first line
		QString content = QString("Version=%1\n").arg(version);

		// Store all key shortcuts
		QMapIterator<QString,QList<QVariant> > i(allKeyShortcuts);
		while(i.hasNext()) {
			i.next();
			QString cmd = i.value().at(1).toString();
			if(cmd == "__openOld")
				cmd = "__open";
			content += QString("%1::%2::%3\n").arg(i.value().at(0).toInt()).arg(i.key()).arg(cmd);
		}

		// Store all mouse shortcuts
		QMapIterator<QString,QList<QVariant> > m(allMouseShortcuts);
		while(m.hasNext()) {
			m.next();
			QString cmd = m.value().at(1).toString();
			if(cmd == "__openOld")
				cmd = "__open";
			content += QString("%1::%2::%3\n").arg(m.value().at(0).toInt()).arg(m.key()).arg(cmd);
		}

		// And save file
		QFile file(QDir::homePath() + "/.photoqt/shortcuts");
		file.remove();
		file.open(QIODevice::WriteOnly);
		QTextStream out(&file);
		out << content;
		file.close();

		// Tell that shortcuts have been updated
		emit updatedShortcuts();

	}

public slots:

	// Set the set of default shortcuts
	void setDefault() {

		if(verbose) std::clog << "sh: Set default shortcuts" << std::endl;

		allKeyShortcuts.clear();
		allMouseShortcuts.clear();

		allKeyShortcuts.insert("O",QList<QVariant>() << false << "__open");
		allKeyShortcuts.insert("Ctrl+O",QList<QVariant>() << false << "__open");
		allKeyShortcuts.insert("Right",QList<QVariant>() << false << "__next");
		allKeyShortcuts.insert("Space",QList<QVariant>() << false << "__next");
		allKeyShortcuts.insert("Left",QList<QVariant>() << false << "__prev");
		allKeyShortcuts.insert("Backspace",QList<QVariant>() << false << "__prev");
		allKeyShortcuts.insert("+",QList<QVariant>() << false << "__zoomIn");
		allKeyShortcuts.insert("Ctrl++",QList<QVariant>() << false << "__zoomIn");
		allKeyShortcuts.insert("-",QList<QVariant>() << false << "__zoomOut");
		allKeyShortcuts.insert("Ctrl+-",QList<QVariant>() << false << "__zoomOut");
		allKeyShortcuts.insert("0",QList<QVariant>() << false << "__zoomReset");
		allKeyShortcuts.insert("1",QList<QVariant>() << false << "__zoomActual");
		allKeyShortcuts.insert("Ctrl+1",QList<QVariant>() << false << "__zoomActual");
		allKeyShortcuts.insert("R",QList<QVariant>() << false << "__rotateR");
		allKeyShortcuts.insert("L",QList<QVariant>() << false << "__rotateL");
		allKeyShortcuts.insert("Ctrl+0",QList<QVariant>() << false << "__rotate0");
		allKeyShortcuts.insert("Ctrl+H",QList<QVariant>() << false << "__flipH");
		allKeyShortcuts.insert("Ctrl+V",QList<QVariant>() << false << "__flipV");
		allKeyShortcuts.insert("Ctrl+X",QList<QVariant>() << false << "__scale");
		allKeyShortcuts.insert("Ctrl+E",QList<QVariant>() << false << "__hideMeta");
		allKeyShortcuts.insert("E",QList<QVariant>() << false << "__settings");
		allKeyShortcuts.insert("F2",QList<QVariant>() << false << "__rename");
		allKeyShortcuts.insert("Ctrl+C",QList<QVariant>() << false << "__copy");
		allKeyShortcuts.insert("Ctrl+M",QList<QVariant>() << false << "__move");
		allKeyShortcuts.insert("Delete",QList<QVariant>() << false << "__delete");
		allKeyShortcuts.insert("I",QList<QVariant>() << false << "__about");
		allKeyShortcuts.insert("S",QList<QVariant>() << false << "__stopThb");
		allKeyShortcuts.insert("Ctrl+R",QList<QVariant>() << false << "__reloadThb");
		allKeyShortcuts.insert("Escape",QList<QVariant>() << false << "__hide");
		allKeyShortcuts.insert("Q",QList<QVariant>() << false << "__close");
		allKeyShortcuts.insert("Ctrl+Q",QList<QVariant>() << false << "__close");
		allKeyShortcuts.insert("Home",QList<QVariant>() << false << "__gotoFirstThb");
		allKeyShortcuts.insert("End",QList<QVariant>() << false << "__gotoLastThb");
		allKeyShortcuts.insert("M", QList<QVariant>() << false << "__slideshow");
		allKeyShortcuts.insert("Shift+M", QList<QVariant>() << false << "__slideshowQuick");
		allKeyShortcuts.insert("W", QList<QVariant>() << false << "__wallpaper");

		allMouseShortcuts.insert("[M] Ctrl+Wheel Down",QList<QVariant>() << false << "__zoomOut");
		allMouseShortcuts.insert("[M] Ctrl+Wheel Up",QList<QVariant>() << false << "__zoomIn");
		allMouseShortcuts.insert("[M] Ctrl+Middle Button",QList<QVariant>() << false << "__zoomReset");
		allMouseShortcuts.insert("[M] Right Button",QList<QVariant>() << false << "__showContext");

		saveSH();

	}

signals:
	// Tell everybody that the shortcuts have been changed
	void updatedShortcuts();

};

#endif // GLOBSHORTCUTS_H
