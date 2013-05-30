#include "application.h"

Application::Application(int &argc, char **argv) : QApplication(argc, argv) {
}

bool Application::notify(QObject *receiver, QEvent *event) {

	try {
		return QApplication::notify(receiver, event);
	} catch(std::exception& e) {
		showError(e.what());
	}
	this->restoreOverrideCursor();
	return false;

}

void Application::showError(const char *m) {
	QMessageBox msg;
	msg.setWindowTitle("ERROR reading image");
	msg.setText(QString("I'm sorry, I wasn't able to succesfully load and display this image.<br><br>Error message:<br>%1").arg(m));
	msg.setIcon(QMessageBox::Critical);
	msg.exec();
}

Application::~Application() { }
