#include <QtGui/QApplication>
#include "mainwindow.h"
#include <iostream>
#include <QTranslator>

int main(int argc, char *argv[]) {

	QCoreApplication::setApplicationName("photo");

	// This string holds the current version
	QString globVersion = "0.9beta";

	// A help message for the command line
	QString hlp = "\nPhoto v" + globVersion + " - created by Lukas Spies (photoQt@ymail.com) - License: GPL\n";
	hlp += "Photo is a fast, simple, good looking, yet powerfull and highly configurable image viewer.\n\n";

	hlp += "Usage: photo [options|file]\n\n";

	hlp += "Options:\n";
	hlp += "\t--h, --help\t\tThis help message\n\n";

	hlp += ">> Start-up options:\n\n";

	hlp += "\t--start-in-tray\t\tStart Photo hidden to the system tray\n";
	hlp += "\t--no-thumbs\t\tDon't load thumbnails (Navigation through folder is still possible)\n\n";

	hlp += ">> Remote Controlling:\n\n";

	hlp += "\t--open\t\t\tOpens the open file dialog (also shows Photo if hidden)\n";
	hlp += "\t--toggle\t\tToggles Photo - hides Photo if visible, shows if hidden\n";
	hlp += "\t--show\t\t\tShows Photo (does nothing if already shown)\n";
	hlp += "\t--hide\t\t\tHides Photo (does nothing if already hidden)\n\n";

	hlp += ">> Remote controlling w/ filename needed:\n\n";

	hlp += "\t--no-thumbs [filename]\t\tDon't load thumbnails (Navigation through folder is still possible)\n";
	hlp += "\t--thumbs [filename]\t\tReversing a '--no-thumbs' (thumbnails are enabled by default)\n\n";

	hlp += "\n   Enjoy Photo :-)\n\n\n";

	// This file is updated by a running instance of Photo every 500 milliseconds - check
	QFile chk(QDir::homePath() + "/.photo/running");
	QString all = "";
	if(chk.open(QIODevice::ReadOnly)) {
		QTextStream in(&chk);
		all = in.readAll();
	}

	QStringList allArgs;
	for(int i = 0; i < argc; ++i)
		allArgs.append(argv[i]);

	QStringList knownArgs;
	knownArgs << "--open";
	knownArgs << "--no-thumbs";
	knownArgs << "--thumbs";
	knownArgs << "--toggle";
	knownArgs << "--show";
	knownArgs << "--hide";
	knownArgs << "--start-in-tray";

	// If photo was started with "--h" or "--help", show help message
	if(allArgs.contains("--help") || allArgs.contains("-help") || allArgs.contains("--h") || allArgs.contains("-h")) {

		std::cout << hlp.toStdString();

		return 0;

	// If an instance of photo is running, we check for command line arguments
	} else if(QDateTime::currentMSecsSinceEpoch() - all.toLongLong() < qint64(520)) {

		qDebug() << "Running instance of Photo detected...";

		// This is the content of the file used to communicate with running photo instance
		QString cont = "";

		// This boolean is set to true if an unknown command is used
		bool err = false;

		for(int i = 1; i < allArgs.length(); ++i) {

			if(knownArgs.contains(allArgs.at(i)))
				cont += allArgs.at(i) + "\n";
			else if(allArgs.at(i).startsWith("-"))
				err = true;
			else {
				QString filename = allArgs.at(i);
				if(!filename.startsWith("/"))
					filename = QFileInfo(filename).absoluteFilePath();
				if(filename != QApplication::applicationFilePath())
					cont += "-f-" + filename;
			}

		}

		// If photo is called without any arguments, "show" is used
		if(allArgs.length() == 1)
			cont = "--show";

		// If only correct arguments were used
		if(!err) {
			// Write the commands into this file, which is checked regularly by a running instance of photo
			QFile f(QDir::homePath() + "/.photo/cmd");
			f.remove();
			if(f.open(QIODevice::WriteOnly)) {
				QTextStream out(&f);
				out << cont;
				f.close();
			} else
				qDebug() << "ERROR! Couldn't write to file '~/.photo/cmd'. Unable to communicate with running process";

		// If an uncorrect argument was used
		} else
			std::cout << hlp.toStdString();


		return 0;

	// If photo isn't running and no command line argument (besides filename and "--start-in.tray") was used
	} else {

		bool err = false;

		for(int i = 0; i < allArgs.length(); ++i) {
			if(allArgs.at(i).startsWith("-") && !knownArgs.contains(allArgs.at(i)))
				err = true;
		}

		if(err == true) {

			std::cout << hlp.toStdString();

			// Nothing after this return will be executed (Photo will simply quit)
			return 0;
		}

		// This boolean stores if photo needs to be minimized to the tray
		bool startintray = allArgs.contains("--start-in-tray");

		// If photo is supposed to be started minimized in system tray
		if(startintray) {
			// If the option "Use Tray Icon" in the settings is not set, we set it
			QFile set(QDir::homePath() + "/.photo/settings");
			if(set.open(QIODevice::ReadOnly)) {
				QTextStream in(&set);
				QString all = in.readAll();
				if(all.contains("TrayIcon=0")) {
					all.replace("TrayIcon=0","Trayicon=1");
					set.close();
					set.remove();
					set.open(QIODevice::WriteOnly);
					QTextStream out(&set);
					out << all;
					set.close();
				} else
					set.close();
			} else
				qDebug() << "Unable to ensure TrayIcon is enabled - make sure it is enabled!!";
		}

		QApplication a(argc, argv);

		// LOAD THE TRANSLATOR
		QTranslator trans;

		// Check if languaged is changed to one of the possible translations
		QFile fileSettings(QDir::homePath() + "/.photo/settings");
		if(fileSettings.open(QIODevice::ReadOnly)) {
			QTextStream in(&fileSettings);
			QString all = in.readAll();
			if(all.contains("Language=") && !all.contains("Language=en")) {
				QString code = all.split("Language=").at(1).split("\n").at(0).trimmed();
				trans.load(":/lang/photo_" + code);
				a.installTranslator(&trans);
				qDebug() << "Loading Translation:" << code;
			}
			fileSettings.close();
		} else
			qDebug() << "ERROR! Couldn't read settings file.";


		// Ensure that the config folder exists
		QDir dir(QDir::homePath() + "/.photo");
		if(!dir.exists())
			dir.mkdir(QDir::homePath() + "/.photo");

		// This int holds 1 if photo was updated and 2 if it's newly installed
		int update = 0;

		// Check if the settings file exists. If not, create a file with default settings (i.e. empty file, settings and defaults are handled by globalsettings.h)
		QFile file(QDir::homePath() + "/.photo/settings");
		if(!file.exists()) {
			if(!file.open(QIODevice::WriteOnly))
				qDebug() << "ERROR: Couldn't write settings file! Please ensure that you have read&write access to home directory";
			else {
				qDebug() << "Creating basic settings file";
				QTextStream out(&file);
				out << "Version=" + globVersion + "\n";
				file.close();
			}

			update = 2;

		// If file does exist, check if it is from a previous version -> photo was updated
		} else {
			if(!file.open(QIODevice::ReadWrite))
				qDebug() << "ERROR: Couldn't read settings file! Please ensure that you have read&write access to home directory";
			else {
				QTextStream in(&file);
				QString all = in.readAll();

				// If it contains no "Version=" (photo 0.1)
				if(!all.contains("Version=")) {
					file.close();
					file.remove();
					file.open(QIODevice::ReadWrite);
					in << "Version=" + globVersion  + "\n" << all;
					update = 1;
				// If it doesn't contain current version (some previous version)
				} else if(!all.contains("Version=" + globVersion + "\n")) {
					file.close();
					file.remove();
					file.open(QIODevice::ReadWrite);
					QStringList allSplit = all.split("\n");
					allSplit.removeFirst();
					QString allFile = "Version=" + globVersion + "\n" + allSplit.join("\n");
					in << allFile;
					update = 1;
				}

				file.close();

			}
		}

		// Check if thumbnail database exists. If not, create it
		QFile database(QDir::homePath() + "/.photo/thumbnails");
		if(!database.exists()) {

			qDebug() << "Create Database";

			QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "thumbDB");
			db.setDatabaseName(QDir::homePath() + "/.photo/thumbnails");
			if(!db.open())
				qDebug() << "ERROR: Couldn't open thumbnail database:" << db.lastError().text().trimmed();
			QSqlQuery query(db);
			query.prepare("CREATE TABLE Thumbnails (filepath TEXT,thumbnail BLOB, filelastmod INT, thumbcreated INT)");
			query.exec();
			if(query.lastError().text().trimmed().length())
				qDebug() << "ERROR (Creating Thumbnail Datbase):" << query.lastError().text().trimmed();
			query.clear();


		} else {

			// Opening the thumbnail database
			QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","thumbDB");
			db.setDatabaseName(QDir::homePath() + "/.photo/thumbnails");
			if(!db.open())
				qDebug() << "ERROR: Couldn't open thumbnail database:" << db.lastError().text().trimmed();

		}


		// Previous versions of Photo checked here also for the shortcuts file. We don't need to do that anymore, since all shortcuts including the defaults are handled by shortcuts.h


		// The main image has a (very basic) context menu. Internal commands are not yet possible, will hopefuly come in next version
		QFile contextmenu(QDir::homePath() + "/.photo/contextmenu");
		if(!contextmenu.exists()) {
			if(contextmenu.open(QIODevice::WriteOnly)) {

				QTextStream out(&contextmenu);

				QString def = "dolphin %d\nOpen in Dolphin\n\ngimp %f\nEdit with Gimp\n\ngwenview %f\nOpen in GwenView";
				out << def;

				contextmenu.close();

			} else
				qDebug() << "ERROR: Couldn't create contextmenu file";
		}


		// The Window has to be initialised *AFTER* the checks above to ensure that the settings exist and are updated and can be loaded
		MainWindow w;
		w.show();
		if(!startintray)
			w.showFullScreen();
		else
			w.hide();

		QString file_str = "";

		// Look for a filename possibly passed on by the user
		for(int i = 0; i < allArgs.length(); ++i) {
			if(!allArgs.at(i).startsWith("-")) {
				QString filename = allArgs.at(i);
				filename = QFileInfo(filename).absoluteFilePath();
				if(filename != QApplication::applicationFilePath()) {
					qDebug() << filename;
					file_str = filename;
					break;
				}
			}
		}


		// Possibly disable thumbnails
		if(allArgs.contains("--no-thumbs")) {
			QMap<QString,QVariant> upd;
			upd.insert("ThumbnailDisable",true);
			w.globSet->settingsUpdated(upd);
		}

		// Set the file to open that the user might have passed on
		w.globVar->currentfile = file_str;

		if(QFile::exists(QDir::homePath() + "/.photo/update"))
			update = 1;

		if(QFile::exists(QDir::homePath() + "/.photo/install"))
			update = 2;

		w.globVar->startupMessageInstallUpdateShown = update;
		w.startUpTimer->start();


		return a.exec();

	}
}
