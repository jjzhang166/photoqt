#ifndef OPENCRUMBS_H
#define OPENCRUMBS_H

#include <QWidget>
#include <QPushButton>
#include <QtDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QDir>

// The different crumbs item
class CrumbsPart : public QPushButton {
	Q_OBJECT
public:
	CrumbsPart(QString txt = "");
	~CrumbsPart();
	// The full path
	QString path;
signals:
	// It was clicked
	void clicked(QString);
protected:
	void mouseReleaseEvent(QMouseEvent *);

};

class Crumbs : public QWidget {
	Q_OBJECT
public:
	Crumbs(QWidget *parent = 0);
	~Crumbs();

	// That's the current path
	QString path;

	// The main layout of the bar
	QHBoxLayout *layout;

	// The different lists
	QList<CrumbsPart *> partList;
	QList<QLabel *> labelList;

	// How many crumbs show at maximum
	int numCrumbsShown;

	// If the number of parts of the path exceeds the maximum crumbs number, some are hidden on the left
	QLabel *thereArePartsToTheLeft;

	// Some navigation buttons
	QPushButton *goUp;
	QPushButton *goBack;
	QPushButton *goForw;
	QLabel *vertLine;

	// The history
	QStringList history;
	int historyPos;

	// Default: false - if the new dir is loaded from the history or not
	bool dirChangeInHistory;

public slots:
	// Set the crumbs
	void setCrumbs(QString path);
	// Go a director level up
	void goLevelUp();
	// Go back in history (if possible)
	void goBackwards();
	// Go forwards in history (if possible)
	void goForwards();

signals:
	// A path item has been clicked
	void clicked(QString);

};

#endif // OPENCRUMBS_H
