#include "window.hpp"
#include "table.hpp"

Window::Window(QWidget *parent) : QWidget(parent) {
	this->init();
}

Window::~Window() {

}

void Window::init() {
	this->resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

	Table t(this, this->width(), 300);

	QStringList a;
	a << "1"
		<< "Thurday idk"
		<< "127.0.0.1"
		<< "127.0.0.1"
		<< "HTTP"
		<< "GET HTTP/1.1 /";
	
	t.append(a);
}
