#include "window.hpp"

Window::Window(QWidget *parent) : QWidget(parent) {
	this->init();
}

Window::~Window() {

}

void Window::init() {
	this->resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

	// Overall container
	this->container = new QVBoxLayout;

	// Horizontal splitter
	this->h_splitter = new QSplitter(Qt::Vertical, this);

	// Vertical splitter 
	this->v_splitter = new QSplitter(Qt::Horizontal, this);

	// Packet table object
	this->packet_table = new Table(this);

	// Place holders 
	QPushButton *button = new QPushButton(this);
	button->setText("HelloWorld");
	QPushButton *button2 = new QPushButton(this);
	button2->setText("HelloWorld2");
	QPushButton *button3 = new QPushButton(this);
	button3->setText("HelloWorld3");
	
	// Right hand side
	this->h_splitter->addWidget(this->packet_table);
	this->h_splitter->addWidget(button);
	this->h_splitter->addWidget(button2);

	// Left hand side
	this->v_splitter->addWidget(button3);
	this->v_splitter->addWidget(h_splitter);

	// Putting them together 
	this->container->addWidget(v_splitter);
	this->setLayout(container);

	QStringList a;
	a << "1"
		<< "Thurday idk"
		<< "127.0.0.1"
		<< "127.0.0.1"
		<< "HTTP"
		<< "GET HTTP/1.1  / ajsdaiowoiwdaoioidsspoaddasppkaso";
	
	for (int i = 0; i < 30; ++i)
		this->packet_table->append(a);
}

void Window::resizeEvent(QResizeEvent *event) {
	
	// Call normal procedure
	QWidget::resizeEvent(event);
}
