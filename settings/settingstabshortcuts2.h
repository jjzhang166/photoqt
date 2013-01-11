#ifndef SETTINGSTABSHORTCUTS2_H
#define SETTINGSTABSHORTCUTS2_H

#include "customelements/customscrollbar.h"
#include "customelements/customcheckbox.h"
#include "settings/settingstabshortcutstiles.h"
#include "settings/settingstabshortcutskeydetect.h"

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollArea>


class SettingsTabShortcuts2 : public QWidget {

	Q_OBJECT

public:
	SettingsTabShortcuts2(QWidget *parent = 0);
//	~SettingsTabShortcuts2();


	bool setup;

	QMap<QString,int> keysFORkeySH;
	QMap<int, QList<QVariant> > keySH;

	QMap<QString,int> keysFORkeySHnew;
	QMap<int, QList<QVariant> > keySHnew;


	/*********************
	 * GENERAL * GENERAL *
	 * GENERAL * GENERAL *
	 * GENERAL * GENERAL *
	 *********************/

	QStringList navigationIntern;
	QStringList navigationInternRead;

	CustomScrollbar *scrollbarNavigationSET;
	QScrollArea *scrollNavigationSET;
	QWidget *scrollWidgNavigationSET;
	QGridLayout *gridNavigationSET;

	CustomScrollbar *scrollbarNavigationAVAIL;
	QScrollArea *scrollNavigationAVAIL;
	QWidget *scrollWidgNavigationAVAIL;
	QGridLayout *gridNavigationAVAIL;

	int navigationSetSHrow;
	int navigationSetSHcol;

	QList<ShortcutsTiles *> allNavigationExist;


	/*************************
	 * IMAGE * IMAGE * IMAGE *
	 * IMAGE * IMAGE * IMAGE *
	 * IMAGE * IMAGE * IMAGE *
	 *************************/


	QStringList imageIntern;
	QStringList imageInternRead;

	CustomScrollbar *scrollbarImageSET;
	QScrollArea *scrollImageSET;
	QWidget *scrollWidgImageSET;
	QGridLayout *gridImageSET;

	CustomScrollbar *scrollbarImageAVAIL;
	QScrollArea *scrollImageAVAIL;
	QWidget *scrollWidgImageAVAIL;
	QGridLayout *gridImageAVAIL;

	int imageSetSHrow;
	int imageSetSHcol;

	QList<ShortcutsTiles *> allImageExist;


	/**********************
	 * FILE * FILE * FILE *
	 * FILE * FILE * FILE *
	 * FILE * FILE * FILE *
	 **********************/

	QStringList fileIntern;
	QStringList fileInternRead;

	CustomScrollbar *scrollbarFileSET;
	QScrollArea *scrollFileSET;
	QWidget *scrollWidgFileSET;
	QGridLayout *gridFileSET;

	CustomScrollbar *scrollbarFileAVAIL;
	QScrollArea *scrollFileAVAIL;
	QWidget *scrollWidgFileAVAIL;
	QGridLayout *gridFileAVAIL;

	int fileSetSHrow;
	int fileSetSHcol;

	QList<ShortcutsTiles *> allFileExist;


	/*************************
	 * OTHER * OTHER * OTHER *
	 * OTHER * OTHER * OTHER *
	 * OTHER * OTHER * OTHER *
	 *************************/

	QStringList otherIntern;
	QStringList otherInternRead;

	CustomScrollbar *scrollbarOtherSET;
	QScrollArea *scrollOtherSET;
	QWidget *scrollWidgOtherSET;
	QGridLayout *gridOtherSET;

	CustomScrollbar *scrollbarOtherAVAIL;
	QScrollArea *scrollOtherAVAIL;
	QWidget *scrollWidgOtherAVAIL;
	QGridLayout *gridOtherAVAIL;

	int otherSetSHrow;
	int otherSetSHcol;

	QList<ShortcutsTiles *> allOtherExist;



	ShortcutKeyDetect *keyDetect;


	void setShortcuts(QString category);

	void loadAllShortcuts();


	void saveShortcuts();


public slots:
	void addSH(QString exe, QString cat, QString keycombo = "");
	void removeSH(int row, int col, QString cat, int posInList);

	void detectCombo(QString keycombo, QString cat, int posInList);

	void renewShortcut(QString cat, int posInList);

	void setupAvailShortcuts();

private:
	void paintEvent(QPaintEvent *);
};

#endif // SETTINGSTABSHORTCUTS2_H
