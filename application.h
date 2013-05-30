#ifndef APPLICATION_H
#define APPLICATION_H

#include <QtGui>
#include <QApplication>
#include <QtDebug>
#include <QMessageBox>

class Application : public QApplication {
	Q_OBJECT

public:
	Application(int &argc, char **argv);
	~Application();

	// reimplemented from QApplication so we can throw exceptions in slots
	virtual bool notify(QObject * receiver, QEvent * event);

	void showError(const char *);

};







#endif // APPLICATION_H
