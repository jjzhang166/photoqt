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
		_working =false;
		_abort = false;
	}

	bool verbose;

	// Dynamic thumbnails?
	int dynamicThumbs;

	// Filename only on thumbnails?
	bool filenameOnly;

	// Some variables for the needed information
	int counttot;
	int currentPos;
	QFileInfoList allimgs;
	int height;
	bool loadFullDirectory;
	unsigned int preloadNumber;

	QSqlDatabase db;
	QString typeCache;
	bool cacheEnabled;

	QString qtfiles;
	QString qtfiles_extras;
	QString gmfiles;

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
	QList<int> preloadCreated;

	// Update the thumbnail position and data
	void newData(int pos, int thbw, int vieww, bool loadfulldir, int preload, bool filename, int smartdyn) {
		amountCreated = 0;
		currentPos = pos;
		thbWidth = thbw;
		viewWidth = vieww;
		loadFullDirectory = loadfulldir;
		preloadNumber = preload;
		createThisOne = 0;
		leftNextThb = -1;
		rightNextThb = -1;
		posCreated.clear();
		preloadCreated.clear();
		filenameOnly = filename;
		dynamicThumbs = smartdyn;
	}

	void updateData(int pos, int thbw, int vieww, bool loadfulldir, int preload, bool filename, int smartdyn) {
		amountCreated = 0;
		currentPos = pos;
		thbWidth = thbw;
		viewWidth = vieww;
		createThisOne = pos;
		loadFullDirectory = loadfulldir;
		preloadNumber = preload;
		leftNextThb = -1;
		rightNextThb = -1;
		filenameOnly = filename;
		dynamicThumbs = smartdyn;
	}

	// Posibility to abort thread
	void abort() {
		if (_working)
		    _abort = true;
	}

private:
	bool _abort;
	bool _working;

signals:
	// Whenever a thumbnail is succesfully created, send it to main thread
	void updateThumb(QImage img,QString path, int dimensionWidth, int dimensionHeight, int pos, bool preload);
	void centerOnCurrent(bool);

protected:
	void run() {

		// Set thread to working
		_working = true;
		_abort = false;

		if(preloadNumber < viewWidth/thbWidth + 10) preloadNumber = viewWidth/thbWidth + 10;

		// Start and end pos of required thumbnails
		int startpos = currentPos-(preloadNumber/2);
		if(startpos < 0) startpos = 0;
		int endpos = currentPos+(preloadNumber/2);
		if(endpos > counttot) endpos = counttot;

		if(loadFullDirectory) {
			startpos = 0;
			endpos = counttot;
		}

		// This stores if the dir was newly loaded
		// It is set to false if any preload-thumbs have already been setup
		bool newlyLoaded = (preloadCreated.length()==0);

		// Setup preload thumbnails
		for(int i = startpos; i < endpos; ++i) {

			// If we should stop - stop!
			if (_abort) return;

			if(!preloadCreated.contains(i)) {
				emit updateThumb(QImage(), allimgs.at(i).absoluteFilePath(), 200, 200, i, true);
				preloadCreated << i;
			}

		}

		// If it's a newly loaded directory, we center on active thumbnail
		if(newlyLoaded)
			emit centerOnCurrent(true);

		// If we stick with filename-only thumbnails -> stop here
		if(filenameOnly) return;

		// If we are requested to stop: stop!
		if (_abort) return;

		// If type is not set to be "files" (i.e. set to "database" or not set at all), use database (default)
		if(typeCache != "files") {

			if(verbose) std::clog << "thread: Using database cache" << std::endl;

			db.transaction();

		} else
			if(verbose) std::clog << "thread: Using file cache" << std::endl;


		// Thumbnail creation starts out from the currently viewed position, alternatingly creating a thumbnail to the left and then to the right.
		amountCreated = 0;
		createThisOne = currentPos;
		leftNextThb = -1;
		rightNextThb = -1;

		// The actual number of thumbnail we have to create (plus 10, 5 for safety on either site)
		int numberThbs = viewWidth/thbWidth + 10;

		// If we don't stop whenever the thumbnails are out of range, then we just continue until all are eventually set up
		if(dynamicThumbs == 0 || numberThbs > counttot)
			numberThbs = counttot;

		if(verbose) std::clog << "Thumbnail thread started! v: " << verbose << " - curPos: " << currentPos << " - amountCreated: " << amountCreated << " - numberThumbs: " << numberThbs << std::endl;

		// We add all thumbnails up and stop when we reach the total number
		int stopCriteria = ((dynamicThumbs == 2) ? preloadNumber : numberThbs);
		while(amountCreated < stopCriteria) {

			// If we should stop - stop!
			if (_abort) return;

			// We don't check for abort request after here, as we should finish loading the thumbnail anyways...

			// Another thumbnail created
			++amountCreated;

			// createThisOne==-1 means, first thumbnail
			if(createThisOne == currentPos && amountCreated == 1) {
				leftNextThb = createThisOne-1;
				rightNextThb = createThisOne+1;
			} else if(createThisOne == -1 || (createThisOne == 0 && leftNextThb == -1 && rightNextThb == -1)) {
				createThisOne = currentPos;
				leftNextThb = createThisOne-1;
				rightNextThb = createThisOne+1;
			} else {

				// Next thumbnail to the right
				// Note: We need to check for right strictly less than counttot, since counttot starts counting at 1, but PhotoQt starts counting at 0
				if((createThisOne <= currentPos && rightNextThb < counttot) || leftNextThb < 0) {
					createThisOne = rightNextThb;
					++rightNextThb;
					if(verbose) std::clog << "Creating RIGHT thumbnails" << std::endl;
				// Next thumbnail to the left
				} else if((createThisOne >= currentPos && leftNextThb >= 0) || rightNextThb >= counttot) {
					createThisOne = leftNextThb;
					--leftNextThb;
					if(verbose) std::clog << "Creating LEFT thumbnails" << std::endl;
				}

			}


			// If this thumbnail hasn't yet been set up
			if(!posCreated.contains(createThisOne) && createThisOne != -1 && createThisOne < counttot) {

				// Create the md5 hash for the thumbnail file
				QByteArray path = "";
				path = "file://" + allimgs.at(createThisOne).absoluteFilePath().toLatin1();

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

				int origwidth = -1;
				int origheight = -1;

				bool wasoncecreated = false;

				// If files in ~/.thumbnails/ shall be used, then do use them
				if(typeCache == "files" && cacheEnabled) {

					// If there exists a thumbnail of the current file already
					if(QFile(QDir::homePath() + "/.thumbnails/" + td + "/" + md5 + ".png").exists() && cacheEnabled) {

						if(verbose) std::clog << "thread: Loading existing thumb from file: " << createThisOne << std::endl;

						p.load(QDir::homePath() + "/.thumbnails/" + td + "/" + md5 + ".png");

						uint mtime = p.text("Thumb").remove("MTime:").trimmed().toInt();
						posCreated.append(createThisOne);

						if(allimgs.at(createThisOne).lastModified().toTime_t() != mtime)
							posCreated.removeLast();

					// Otherwise load thumbnail from original
					}

				// otherwise use the database (default)
				} else if(cacheEnabled) {

					ts = 256;

					QSqlQuery query(db);
					query.exec(QString("SELECT thumbnail,filelastmod,origwidth,origheight FROM Thumbnails WHERE filepath='%1'").arg(allimgs.at(createThisOne).absoluteFilePath()));
					if(query.next()) {

						if(query.value(query.record().indexOf("filelastmod")).toUInt() == allimgs.at(createThisOne).lastModified().toTime_t()) {
							if(verbose) std::clog << "thread: Loading existing thumb from db: " << createThisOne << std::endl;
							QByteArray b;
							b = query.value(query.record().indexOf("thumbnail")).toByteArray();
							p.loadFromData(b);
							origwidth = query.value(query.record().indexOf("origwidth")).toInt();
							origheight = query.value(query.record().indexOf("origheight")).toInt();
							posCreated.append(createThisOne);
						}

						wasoncecreated = true;

					}

					query.clear();

				}

				// If file wasn't loaded from file or database, then it doesn't exist yet (or isn't up-to-date anymore) and we have to create it
				if(!posCreated.contains(createThisOne) && amountCreated < numberThbs) {

					posCreated.append(createThisOne);

					if(verbose) std::clog << "thread: Creating new thumb: " << createThisOne << std::endl;

					ImageReader image(verbose);
					image.qtfiles = qtfiles;
					image.gmfiles = gmfiles;
					p = image.readImage(allimgs.at(createThisOne).absoluteFilePath(),0,false,QSize(ts,ts));

					origwidth = image.origSize.width();
					origheight = image.origSize.height();

					if(typeCache == "files") {

						// If the file itself wasn't read from the thumbnails folder, is not a temporary file, and if the original file isn't at thumbnail size itself
						if(cacheEnabled && !allimgs.at(createThisOne).absoluteFilePath().startsWith(QDir::homePath() + "/.thumbnails/") && !allimgs.at(createThisOne).absoluteFilePath().startsWith(QDir::tempPath()) && (p.height() > 127 || p.width() > 127)) {

							// We use a QImageWriter (faster, metainfo support) - the path is a temporary path (for reason, see below)
							QImageWriter writer(QDir::tempPath() + "/" + md5 + "__photo.png","png");

							// The following meta information is required by the standard
							writer.setText("Thumb::MTime",QString("%1").arg(allimgs.at(createThisOne).lastModified().toTime_t()));

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

						query2.bindValue(":path",allimgs.at(createThisOne).absoluteFilePath());
						query2.bindValue(":thb",b);
						query2.bindValue(":mod",allimgs.at(createThisOne).lastModified().toTime_t());
						query2.bindValue(":crt",QDateTime::currentMSecsSinceEpoch());
						query2.bindValue(":origw",origwidth);
						query2.bindValue(":origh",origheight);
						query2.exec();
						if(query2.lastError().text().trimmed().length())
							std::cerr << "ERROR #" << createThisOne << ": " << query2.lastError().text().trimmed().toStdString() << std::endl;
						query2.clear();

					}

				}

				// Send out signal with all the data
				if(posCreated.contains(createThisOne))
					emit updateThumb(p,allimgs.at(createThisOne).absoluteFilePath(),origwidth,origheight,createThisOne, false);

			}

		}

		// Close database if it was needed
		if(typeCache != "files")
			db.commit();

		_working = false;

	}

};

#endif // THREADFORTHUMBNAILS_H
