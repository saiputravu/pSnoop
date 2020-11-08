#include "window.hpp"

Window::Window(QWidget *parent) : QWidget(parent) {
	this->init();
}

Window::~Window() {

}

void Window::init() {
	this->resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

	this->packet_table = new Table(this, this->width(), (unsigned int)300);

	QStringList a;
	a << "1"
		<< "Thurday idk"
		<< "127.0.0.1"
		<< "127.0.0.1"
		<< "HTTP"
		<< "GET HTTP/1.1  / ajsdaiowoiwdaoioidsspoaddasppkaso";
	
	this->packet_table->append(a);
	this->packet_table->append(a);
	this->packet_table->append(a);
}

void Window::resizeEvent(QResizeEvent *event) {
	
	// Resize packet_table
	this->packet_table->get_table()->setFixedSize(this->width(), this->packet_table->get_height());

	// Call normal procedure
	QWidget::resizeEvent(event);
}
