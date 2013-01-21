#ifndef SETTINGSTABOTHERCONTEXT_H
#define SETTINGSTABOTHERCONTEXT_H

#include "settingstabothercontexttiles.h"
#include "../customelements/customscrollbar.h"
#include "../widgets/customconfirm.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QDir>

class Context : public QScrollArea {

	Q_OBJECT

public:
	Context(QWidget *parent = 0, bool verbose = false);
	~Context();

	bool verbose;

	// The main layout
	QVBoxLayout *lay;

	// All the entries
	QList<ContextTile *> allTiles;

	// The old index (needed fro drag&drop)
	int oldIndex;

	// Load/Save context menu
	void loadContext();
	void saveContext();

public slots:
	// Add a new entry
	void addNewEntry();

	// Remove an entry
	void deleteTile(int index);

protected:
	void mousePressEvent(QMouseEvent *);
	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);

};

#endif // SETTINGSTABOTHERCONTEXT_H
