#include "window.hpp"

Window::Window(QWidget *parent) : QMainWindow(parent) {
	this->init_general();
	this->init_menu();
	this->init_layout();
}

Window::~Window() {

}

void Window::init_general() {
	// General Window Settings
	this->resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
	this->setMinimumSize(300, 300);
	this->setWindowTitle("pSnoopGUI - Sai");
}

void Window::init_menu() {
	// File Menu Actions
	// File Menu
	this->file_menu = this->menuBar()->addMenu(QString("&File"));
	
	// Help Menu Actions
	// Help Menu
	this->help_menu = this->menuBar()->addMenu(QString("&Help"));

}

void Window::init_layout() {

	// Overall container	
	this->main_widget = new QWidget(this);
	this->main_widget->move(0, 15);
	this->main_widget->setFixedSize(this->width(), this->height() - 15);
	this->container = new QVBoxLayout;

	// Splitters / Dividers
	this->h_splitter = new QSplitter(Qt::Vertical, this);
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
	this->main_widget->setLayout(container);
	

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
	
	// Resize everything on window resize event
	this->main_widget->setFixedSize(this->width(), this->height() - 15);

	// Call normal procedure
	QWidget::resizeEvent(event);
}
