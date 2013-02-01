#ifndef THREADFORTHUMBNAILS_H
#define THREADFORTHUMBNAILS_H

#include <QThread>
#include <QImageWriter>
#include <QImageReader>
#include <QtDebug>
#include <QDateTime>
#include <QDir>
#include <QCryptographicHash>
#include <QtSql>

class ThumbThread : public QThread {

	Q_OBJECT

public:
	bool verbose;

	// Dynamic thumbnails?
	bool dynamicThumbs;

	// Some variables for the needed information
	int counttot;
	QFileInfoList allimgs;
	int height;
	int breakme;
	bool cacheEnabled;

	QString dbName;

	QSqlDatabase db;
	QString typeCache;

	int currentPos;

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
	void updateThb(QImage img,QString path, int pos);

protected:
	void run() {

		amUpdatingData = false;

		// If type is not set to be "files" (i.e. set to "database" or not set at all), use database (default)
		if(typeCache != "files") {

			if(verbose) qDebug() << "thread: Using database cache";

			if(dbName != "thread" && dbName != "open")
				dbName = "thread";

			dbName += QString("%1").arg(QDateTime::currentMSecsSinceEpoch());

			db = QSqlDatabase::database("thumbDB");

			db.transaction();

		} else
			if(verbose) qDebug() << "thread: Using file cache";


		// Thumbnail creation starts out from the currently viewed position, alternatingly creating a thumbnail to the left and then to the right.
		amountCreated = 0;
		createThisOne = -1;
		leftNextThb = -1;
		rightNextThb = -1;

		// The actual number of thumbnail we have to create (plus 4, 2 for safety on either site)
		int numberThbs = viewWidth/thbWidth + 4;
		// If we don't stop whenever the thumbnails are out of range, then we just continue until all are eventually set up
		if(!dynamicThumbs)
			numberThbs = counttot;

		qDebug() << "THB_thread: Started! verbose:" << verbose << "- currentPox:" << currentPos << "- amountCreated:" << amountCreated << "- numberThumbs:" << numberThbs << "- amUpdatingData:" << amUpdatingData;

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
					// Note: We need to check for right strictly less than counttot, since counttot starts counting at 1, but Photo starts counting at 0
					if((createThisOne <= currentPos && rightNextThb < counttot) || leftNextThb < 0) {
						createThisOne = rightNextThb;
						++rightNextThb;
						if(leftNextThb < 0)
							++amountCreated;
					// Next thumbnail to the left
					} else if((createThisOne >= currentPos && leftNextThb >= 0) || rightNextThb >= counttot) {
						createThisOne = leftNextThb;
						--leftNextThb;
						if(rightNextThb >= counttot)
							++amountCreated;
					}

				}

				// If this thumbnail hasn't yet been set up
				// FOR ANYTHING WE DO BELOW WE CHECK THE BOOLEAN amUpdatingData, BECAUSE IF THAT'S TRUE, THEN THE DATA IS CURRENTLY BEING UPDATED AND THAT COULD POSSIBLE RESULT IN SOMETHING BAD IF WE TRY TO EMIT A NEW THUMB...
				if(!posCreated.contains(createThisOne)) {

					posCreated.append(createThisOne);

					// Create the md5 hash for the thumbnail file
					QByteArray path = "";
					if(!amUpdatingData) path = "file://" + allimgs.at(createThisOne).absoluteFilePath().toAscii();

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

					bool loaded = false;
					bool wasoncecreated = false;

					// If files in ~/.thumbnails/ shall be used, then do use them
					if(typeCache == "files") {

						// If there exists a thumbnail of the current file already
						if(QFile(QDir::homePath() + "/.thumbnails/" + td + "/" + md5 + ".png").exists() && cacheEnabled) {

							if(verbose && !amUpdatingData) qDebug() << "thread: Loading existing thumb from file:" << createThisOne;

							p.load(QDir::homePath() + "/.thumbnails/" + td + "/" + md5 + ".png");

							int mtime = p.text("Thumb").remove("MTime:").trimmed().toInt();
							loaded = true;

							if(!amUpdatingData && int(allimgs.at(createThisOne).lastModified().toTime_t()) != mtime)
								loaded = false;

						// Otherwise load thumbnail from original (using QImageReader for better performance)
						}

					// otherwise use the database (default)
					} else {

						ts = 256;

						QSqlQuery query(db);
						if(!amUpdatingData) query.exec(QString("SELECT thumbnail,filelastmod FROM Thumbnails WHERE filepath='%1'").arg(allimgs.at(createThisOne).absoluteFilePath()));
						if(query.next()) {

							if(!amUpdatingData && query.value(query.record().indexOf("filelastmod")).toInt() == int(allimgs.at(createThisOne).lastModified().toTime_t())) {
								if(verbose && !amUpdatingData) qDebug() << "thread: Loading existing thumb from db:" << createThisOne;
								QByteArray b;
								b = query.value(query.record().indexOf("thumbnail")).toByteArray();
								p.loadFromData(b);
								loaded = true;
							}

							wasoncecreated = true;

						}

						query.clear();

					}

					// If file wasn't loaded from file or database, then it doesn't exist yet (or isn't up-to-date anymore) and we have to create it
					if(!loaded) {

						if(verbose && !amUpdatingData) qDebug() << "thread: Creating new thumb:" << createThisOne;

						QImageReader reader;
						if(!amUpdatingData) reader.setFileName(allimgs.at(createThisOne).absoluteFilePath());
						int readerWidth = reader.size().width();
						int readerHeight = reader.size().height();

						if(readerWidth > ts) {
							float q = ts/(readerWidth*1.0);
							readerWidth *= q;
							readerHeight *= q;
						}
						if(readerHeight > ts) {
							float q = ts/(readerHeight*1.0);
							readerWidth *= q;
							readerHeight *= q;
						}

						if(!amUpdatingData) reader.setScaledSize(QSize(readerWidth,readerHeight));

						if(!amUpdatingData) p = reader.read();

						if(typeCache == "files") {

							// If the file itself wasn't read from the thumbnails folder, is not a temporary file, and if the original file isn't at thumbnail size itself
							if(!amUpdatingData && cacheEnabled & !allimgs.at(createThisOne).absoluteFilePath().startsWith(QDir::homePath() + "/.thumbnails/") && !allimgs.at(createThisOne).absoluteFilePath().startsWith(QDir::tempPath()) && (p.height() > 127 || p.width() > 127)) {

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
									qDebug() << "ERROR CREATING NEW THUMBNAIL FILE";
								// Delete temporary file
								QFile(QDir::tempPath() + "/" + md5 + "__photo.png").remove();

							}

						} else {

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
								query2.prepare("UPDATE Thumbnails SET filepath=:path,thumbnail=:thb,filelastmod=:mod,thumbcreated=:crt WHERE filepath=:path");
							else
								query2.prepare("INSERT INTO Thumbnails(filepath,thumbnail,filelastmod,thumbcreated) VALUES(:path,:thb,:mod,:crt)");

							if(!amUpdatingData) query2.bindValue(":path",allimgs.at(createThisOne).absoluteFilePath());
							query2.bindValue(":thb",b);
							if(!amUpdatingData) query2.bindValue(":mod",allimgs.at(createThisOne).lastModified().toTime_t());
							query2.bindValue(":crt",QDateTime::currentMSecsSinceEpoch());
							if(!amUpdatingData) query2.exec();
							if(!amUpdatingData && query2.lastError().text().trimmed().length())
								qDebug() << "ERR" << createThisOne << ":" << query2.lastError().text().trimmed();
							query2.clear();

						}

					}

					// Send out signal with all the data
					if(!breakme && !amUpdatingData)
						emit updateThb(p,allimgs.at(createThisOne).absoluteFilePath(),createThisOne);

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
