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
	// Some variables for the needed information
	int counttot;
	QFileInfoList allimgs;
	int height;
	int breakme;
	bool cacheEnabled;

	QString dbName;

	QSqlDatabase db;
	QString typeCache;

signals:
	// Whenever a thumbnail is succesfully created, send it to main thread
	void updateThb(QImage img,QString path, int pos);

protected:
	void run() {

		// If type is not set to be "files" (i.e. set to "database" or not set at all), use database (default)
		if(typeCache != "files") {

			if(dbName != "thread" && dbName != "open")
				dbName = "thread";

			dbName += QString("%1").arg(QDateTime::currentMSecsSinceEpoch());

			db = QSqlDatabase::database("thumbDB");

			db.transaction();

		}

		for(int i = 0; i < counttot; ++i) {

			// Create the md5 hash for the thumbnail file
			QByteArray path = "file://" + allimgs.at(i).absoluteFilePath().toAscii();
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

					p.load(QDir::homePath() + "/.thumbnails/" + td + "/" + md5 + ".png");

					int mtime = p.text("Thumb").remove("MTime:").trimmed().toInt();
					loaded = true;

					if(int(allimgs.at(i).lastModified().toTime_t()) != mtime)
						loaded = false;

				// Otherwise load thumbnail from original (using QImageReader for better performance)
				}

			// otherwise use the database (default)
			} else {

				ts = 256;

				QSqlQuery query(db);
				query.exec(QString("SELECT thumbnail,filelastmod FROM Thumbnails WHERE filepath='%1'").arg(allimgs.at(i).absoluteFilePath()));
				if(query.next()) {

					if(query.value(query.record().indexOf("filelastmod")).toInt() == int(allimgs.at(i).lastModified().toTime_t())) {
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

				QImageReader reader(allimgs.at(i).absoluteFilePath());
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

				reader.setScaledSize(QSize(readerWidth,readerHeight));

				p = reader.read();

				if(typeCache == "files") {

					// If the file itself wasn't read from the thumbnails folder, is not a temporary file, and if the original file isn't at thumbnail size itself
					if(cacheEnabled & !allimgs.at(i).absoluteFilePath().startsWith(QDir::homePath() + "/.thumbnails/") && !allimgs.at(i).absoluteFilePath().startsWith(QDir::tempPath()) && (p.height() > 127 || p.width() > 127)) {

						// We use a QImageWriter (faster, metainfo support) - the path is a temporary path (for reason, see below)
						QImageWriter writer(QDir::tempPath() + "/" + md5 + "__photo.png","png");

						// The following meta information is required by the standard
						writer.setText("Thumb::MTime",QString("%1").arg(allimgs.at(i).lastModified().toTime_t()));

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

					query2.bindValue(":path",allimgs.at(i).absoluteFilePath());
					query2.bindValue(":thb",b);
					query2.bindValue(":mod",allimgs.at(i).lastModified().toTime_t());
					query2.bindValue(":crt",QDateTime::currentMSecsSinceEpoch());
					query2.exec();
					if(query2.lastError().text().trimmed().length())
						qDebug() << "ERR" << i << ":" << query2.lastError().text().trimmed();
					query2.clear();

				}

			}

			// Send out signal with all the data
			if(!breakme)
				emit updateThb(p,allimgs.at(i).absoluteFilePath(),i);

			// Stop thumbnail creation
			if(breakme)
				break;

		}

		// Close database if it was needed
		if(typeCache != "files")
			db.commit();

	}

};

#endif // THREADFORTHUMBNAILS_H
