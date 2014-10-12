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

#ifndef THREADFORTHUMBNAILS_H
#define THREADFORTHUMBNAILS_H

#include <QThread>
#include <QImageWriter>
#include <QDateTime>
#include <QDir>
#include <QCryptographicHash>
#include <QtSql>
#include <iostream>

#include "../graphics/imagereader.h"

class ThumbThread : public QThread {

	Q_OBJECT

public:

	ThumbThread() {
		db = QSqlDatabase::database("thumbDB");
	}

	bool verbose;

	// Dynamic thumbnails?
	bool dynamicThumbs;

	// Some variables for the needed information
	int counttot;
	QFileInfoList allimgs;
	int height;
	int breakme;
	bool cacheEnabled;

	QSqlDatabase db;
	QString typeCache;

	int currentPos;

	QMap<QString,QVariant> globSet;

	// These are used in the thread, and are re-set when scrolling
	int createThisOne;
	int leftNextThb;
	int rightNextThb;

	// These variables are needed for dynamic thumbnails, storing the current position and how far to the left or right we have to create thumbnails
	int amountCreated;
	int viewWidth;
	int thbWidth;
	// Once a thumbnail is created and displayed, we don't need to do this again
	QList<int> posCreated;

	// Update the thumbnail position and data
	void newData(int pos, int thbw, int vieww) {
		// If a thumbnail was just being created as we interrupted it, we remove this thumbnail
		if(posCreated.indexOf(createThisOne) != -1)
			posCreated.removeAt(posCreated.indexOf(createThisOne));
		amountCreated = 0;
		currentPos = pos;
		thbWidth = thbw;
		viewWidth = vieww;
		createThisOne = 0;
		leftNextThb = -1;
		rightNextThb = -1;
	}

	// This boolean is set to true whenever the data gets updated and the thread is not changing anything during this time
	bool amUpdatingData;

signals:
	// Whenever a thumbnail is succesfully created, send it to main thread
	void updateThb(QImage img,QString path, int dimensionWidth, int dimensionHeight, int pos);

protected:
	void run() {

		amUpdatingData = false;

		// If type is not set to be "files" (i.e. set to "database" or not set at all), use database (default)
		if(typeCache != "files") {

			if(verbose) std::clog << "thread: Using database cache" << std::endl;

			db.transaction();

		} else
			if(verbose) std::clog << "thread: Using file cache" << std::endl;


		// Thumbnail creation starts out from the currently viewed position, alternatingly creating a thumbnail to the left and then to the right.
		amountCreated = 0;
		createThisOne = -1;
		leftNextThb = -1;
		rightNextThb = -1;

		posCreated.clear();

		// The actual number of thumbnail we have to create (plus 4, 2 for safety on either site)
		int numberThbs = viewWidth/thbWidth + 4;
		// If we don't stop whenever the thumbnails are out of range, then we just continue until all are eventually set up
		if(!dynamicThumbs || numberThbs > counttot)
			numberThbs = counttot;

		if(verbose) std::clog << "Thumbnail thread started! v: " << verbose << " - curPos: " << currentPos << " - amountCreated: " << amountCreated << " - numberThumbs: " << numberThbs << " - amUpdatingData: " << amUpdatingData << std::endl;

		// We add all thumbnails up and stop when we reach the total number
		while(amountCreated < numberThbs || amUpdatingData) {

			if(!amUpdatingData) {

				// Another one created
				++amountCreated;

				// createThisOne==-1 means, first thumbnail
				if(createThisOne == -1 || (createThisOne == 0 && leftNextThb == -1 && rightNextThb == -1)) {
					createThisOne = currentPos;
					leftNextThb = createThisOne-1;
					rightNextThb = createThisOne+1;
				} else {
					// Next thumbnail to the right
					// Note: We need to check for right strictly less than counttot, since counttot starts counting at 1, but PhotoQt starts counting at 0
					if((createThisOne <= currentPos && rightNextThb < counttot) || leftNextThb < 0) {
						createThisOne = rightNextThb;
						++rightNextThb;
						if(leftNextThb < 0 && numberThbs != counttot)
							++amountCreated;
						if(verbose) std::clog << "Creating RIGHT thumbnails" << std::endl;
					// Next thumbnail to the left
					} else if((createThisOne >= currentPos && leftNextThb >= 0) || rightNextThb >= counttot) {
						createThisOne = leftNextThb;
						--leftNextThb;
						if(rightNextThb >= counttot && numberThbs != counttot)
							++amountCreated;
						if(verbose) std::clog << "Creating LEFT thumbnails" << std::endl;
					}

				}

				// If this thumbnail hasn't yet been set up
				// FOR ANYTHING WE DO BELOW WE CHECK THE BOOLEAN amUpdatingData, BECAUSE IF THAT'S TRUE, THEN THE DATA IS CURRENTLY BEING UPDATED AND THAT COULD POSSIBLE RESULT IN SOMETHING BAD IF WE TRY TO EMIT A NEW THUMB...
				if(!posCreated.contains(createThisOne) && createThisOne != -1) {

					posCreated.append(createThisOne);

					// Create the md5 hash for the thumbnail file
					QByteArray path = "";
					if(!amUpdatingData) path = "file://" + allimgs.at(createThisOne).absoluteFilePath().toLatin1();

					QByteArray md5 = QCryptographicHash::hash(path,QCryptographicHash::Md5).toHex();

					// Prepare the QImage (QPixmap cannot be used, since its use is not safe outside main thread and would most likely lead to a crash)
					QImage p;

					// Depending on the set size of the thumbnails, choose the right thumbnail resolution
					int ts = 256;
					QString td = "large";
					if(height < 129) {
						ts = 128;
						td = "normal";
					}

					int origwidth;
					int origheight;

					bool loaded = false;
					bool wasoncecreated = false;

					// If files in ~/.thumbnails/ shall be used, then do use them
					if(typeCache == "files" && cacheEnabled) {

						// If there exists a thumbnail of the current file already
						if(QFile(QDir::homePath() + "/.thumbnails/" + td + "/" + md5 + ".png").exists() && cacheEnabled) {

							if(verbose && !amUpdatingData) std::clog << "thread: Loading existing thumb from file: " << createThisOne << std::endl;

							p.load(QDir::homePath() + "/.thumbnails/" + td + "/" + md5 + ".png");

							int mtime = p.text("Thumb").remove("MTime:").trimmed().toInt();
							loaded = true;

							if(!amUpdatingData && int(allimgs.at(createThisOne).lastModified().toTime_t()) != mtime)
								loaded = false;

						// Otherwise load thumbnail from original (using QImageReader for better performance)
						}

					// otherwise use the database (default)
					} else if(cacheEnabled) {

						ts = 256;

						QSqlQuery query(db);
						if(!amUpdatingData) query.exec(QString("SELECT thumbnail,filelastmod,origwidth,origheight FROM Thumbnails WHERE filepath='%1'").arg(allimgs.at(createThisOne).absoluteFilePath()));
						if(query.next()) {

							if(!amUpdatingData && query.value(query.record().indexOf("filelastmod")).toInt() == int(allimgs.at(createThisOne).lastModified().toTime_t())) {
								if(verbose && !amUpdatingData) std::clog << "thread: Loading existing thumb from db: " << createThisOne << std::endl;
								QByteArray b;
								b = query.value(query.record().indexOf("thumbnail")).toByteArray();
								p.loadFromData(b);
								origwidth = query.value(query.record().indexOf("origwidth")).toInt();
								origheight = query.value(query.record().indexOf("origheight")).toInt();
								loaded = true;
							}

							wasoncecreated = true;

						}

						query.clear();

					}

					// If file wasn't loaded from file or database, then it doesn't exist yet (or isn't up-to-date anymore) and we have to create it
					if(!loaded) {

						if(verbose && !amUpdatingData) std::clog << "thread: Creating new thumb: " << createThisOne << std::endl;


						QString qtfiles = globSet.value("KnownFileTypesQt").toString();
						QString qtfilesextras = globSet.value("KnownFileTypesQtExtras").toString();
						if(qtfiles != "" && qtfilesextras != "") qtfiles += ",";
						qtfiles += qtfilesextras;

						QString gmfiles = globSet.value("KnownFileTypesGm").toString();
						QString gmfilesextras = globSet.value("KnownFileTypesGmExtras").toString();
						if(gmfiles != "" && gmfilesextras != "") gmfiles += ",";
						gmfiles += gmfilesextras;

						ImageReader image(verbose);
						image.qtfiles = qtfiles;
						image.gmfiles = gmfiles;
						p = image.readImage(allimgs.at(createThisOne).absoluteFilePath(),0,false,QSize(ts,ts));

						origwidth = image.origSize.width();
						origheight = image.origSize.height();

						if(typeCache == "files") {

							// If the file itself wasn't read from the thumbnails folder, is not a temporary file, and if the original file isn't at thumbnail size itself
							if(!amUpdatingData && cacheEnabled && !allimgs.at(createThisOne).absoluteFilePath().startsWith(QDir::homePath() + "/.thumbnails/") && !allimgs.at(createThisOne).absoluteFilePath().startsWith(QDir::tempPath()) && (p.height() > 127 || p.width() > 127)) {

								// We use a QImageWriter (faster, metainfo support) - the path is a temporary path (for reason, see below)
								QImageWriter writer(QDir::tempPath() + "/" + md5 + "__photo.png","png");

								// The following meta information is required by the standard
								if(!amUpdatingData) writer.setText("Thumb::MTime",QString("%1").arg(allimgs.at(createThisOne).lastModified().toTime_t()));

								// We write the temporary file
								writer.write(p);

								// If the file still doesn't exist, copy it to the right location (>> protection from concurrency)
								if(QFile(QDir::homePath() + "/.thumbnails/" + td + "/" + md5 + ".png").exists())
									QFile(QDir::homePath() + "/.thumbnails/" + td + "/" + md5 + ".png").remove();

								if(!QFile(QDir::tempPath() + "/" + md5 + "__photo.png").copy(QDir::homePath() + "/.thumbnails/" + td + "/" + md5 + ".png"))
									std::cerr << "ERROR creating new thumbnail file!" << std::endl;
								// Delete temporary file
								QFile(QDir::tempPath() + "/" + md5 + "__photo.png").remove();

							}

						} else if(cacheEnabled) {

							db.close();
							db.open();

							QSqlQuery query2(db);

							QByteArray b;
							QBuffer buf(&b);
							buf.open(QIODevice::WriteOnly);

							// If file has transparent areas, we save it as png to preserver transparency. Otherwise we choose jpg (smaller)
							if(p.hasAlphaChannel())
								p.save(&buf,"PNG");
							else
								p.save(&buf,"JPG");

							// If it was once created, i.e. if the file changed (i.e. if last mod date changed), then we have to update it
							if(wasoncecreated)
								query2.prepare("UPDATE Thumbnails SET filepath=:path,thumbnail=:thb,filelastmod=:mod,thumbcreated=:crt,origwidth=:origw,origheight=:origh WHERE filepath=:path");
							else
								query2.prepare("INSERT INTO Thumbnails(filepath,thumbnail,filelastmod,thumbcreated,origwidth,origheight) VALUES(:path,:thb,:mod,:crt,:origw,:origh)");

							if(!amUpdatingData) query2.bindValue(":path",allimgs.at(createThisOne).absoluteFilePath());
							if(!amUpdatingData) query2.bindValue(":thb",b);
							if(!amUpdatingData) query2.bindValue(":mod",allimgs.at(createThisOne).lastModified().toTime_t());
							if(!amUpdatingData) query2.bindValue(":crt",QDateTime::currentMSecsSinceEpoch());
							if(!amUpdatingData) query2.bindValue(":origw",origwidth);
							if(!amUpdatingData) query2.bindValue(":origh",origheight);
							if(!amUpdatingData) query2.exec();
							if(!amUpdatingData && query2.lastError().text().trimmed().length())
								std::cerr << "ERROR #" << createThisOne << ": " << query2.lastError().text().trimmed().toStdString() << std::endl;
							query2.clear();

						}

					}

					// Send out signal with all the data
					if(!breakme && !amUpdatingData)
						emit updateThb(p,allimgs.at(createThisOne).absoluteFilePath(),origwidth,origheight,createThisOne);

				}

				// Stop thumbnail creation
				if(breakme)
					break;

			}

			// Close database if it was needed
			if(typeCache != "files")
				db.commit();

		}

	}

};

#endif // THREADFORTHUMBNAILS_H
