#include "settings.hpp"

Settings::Settings(std::string config_file, QWidget *parent) : config_file(config_file), QWidget(parent){
	
	// Set window title 
	this->setWindowTitle("Settings");

	// Instantiate containers for layout structure
	this->container = new QHBoxLayout();
	this->settings_layout = new QVBoxLayout();
	this->categories = new QListWidget();
	this->categories->setMaximumWidth((int)(this->width() * 0.25));

	// Right hand side placeholders (settings options)
	QLabel *placeholder = new QLabel();
	placeholder->setText("Select a menu");
	placeholder->setAlignment(Qt::AlignCenter);
	this->settings_layout->addWidget(placeholder);

	// Setting categories 
	new QListWidgetItem("General", this->categories);
	new QListWidgetItem("Appearance", this->categories);
	this->connect(this->categories, &QListWidget::itemClicked,
			this, &Settings::tab_manager);

	// Layout configuration
	this->container->addWidget(categories);
	this->container->addLayout(settings_layout);
	this->setMinimumSize(500,500);
	this->setLayout(container);

	// Generate a default config file if there is not one in the current directory
	// Then parse the settings file
	this->parse_file();
}

Settings::~Settings() {

}

void Settings::generate_default_config() {
	// Mention new file is being written 
	QWidget *pop_up = new QWidget();
	QVBoxLayout *layout = new QVBoxLayout();
	QLabel *text = new QLabel();
	QPushButton *okay = new QPushButton();

	text->setText("New configuration file is being generated");
	okay->setText("Okay");
	pop_up->setWindowTitle("Warning : File not found");
	this->connect(okay, &QPushButton::clicked,
			pop_up, &QWidget::close);

	layout->addWidget(text);
	layout->addWidget(okay);
	pop_up->setLayout(layout);
	pop_up->show();

	// Open file stream and write default configs 
	std::ofstream stream;
	stream.open(this->config_file);
	stream	<< "font=JetBrainsMono" << std::endl
			<< "fontsize=8" << std::endl
			<< "version=1.0" << std::endl
			<< "UNKNOWN_fg=#f3e0ec" << std::endl // Unknown 
			<< "UNKNOWN_bg=#a81031" << std::endl
			<< "TCP_fg=#231f20" << std::endl // TCP 
			<< "TCP_bg=#afc9a6" << std::endl
			<< "UDP_fg=#231f20" << std::endl // UDP 
			<< "UDP_bg=#f4a261" << std::endl
			<< "ICMP_fg=#231f20" << std::endl // ICMP 
			<< "ICMP_bg=#74b3ce" << std::endl
			<< "IP_fg=#231f20" << std::endl // IP 
			<< "IP_bg=#847996" << std::endl
			<< "ARP_fg=#231f20" << std::endl // ARP
			<< "ARP_bg=#f58fa5" << std::endl
			<< "HTTP_fg=#d7f2ba" << std::endl // HTTP 
			<< "HTTP_bg=#264653" << std::endl
			<< "HTTPS_fg=#231f20" << std::endl // HTTPS
			<< "HTTPS_bg=#817e9f" << std::endl
			<< "TREEVIEW_ROOT_fg=#e9f1f7" << std::endl // Tree view root node 
			<< "TREEVIEW_CHILD_KEY_fg=#e9f1f7" << std::endl // Tree view child node key 
			<< "TREEVIEW_CHILD_VAL_fg=#e9f1f7" << std::endl // Tree view child node value
			<< "TREEVIEW_ROOT_bg=#412234" << std::endl
			<< "TREEVIEW_CHILD_KEY_bg=#6a6f8a" << std::endl
			<< "TREEVIEW_CHILD_VAL_bg=#9f4a54" << std::endl;
	stream.close();
	this->parse_file();
}

void Settings::parse_file() {
	std::ifstream file(this->config_file);

	if (!file) {
		this->generate_default_config();
		return;
	}
	
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	// Read every line in format <KEY>=<VALUE>
	std::string line;
	while (std::getline(buffer, line)) {
		std::stringstream line_buf(line);
		std::string key;
		if (std::getline(line_buf, key, '=')) {
			std::string value;
			if (std::getline(line_buf, value))
				this->config[key] = value;
		}
	}
}

std::string Settings::get(std::string key) {
	if (this->config.find(key) == this->config.end()) 
		return std::string("Bad");
	return this->config[key];
}

void Settings::resizeEvent(QResizeEvent *event) {
	QWidget::resizeEvent(event);

	this->categories->setMaximumWidth((int)(this->width()*0.25));
}

void Settings::open_window() {
	this->show();
}

void Settings::tab_manager(QListWidgetItem *item) {
	if (item->text() == "General") 
		this->general_tab();
	else if (item->text() == "Appearance")
		this->appearance_tab();
}

void Settings::general_tab() {
	// Clear settings options section 
	clear_layout(this->settings_layout);

	// New settings options
	QHBoxLayout *config_file_option = new QHBoxLayout();
	QLineEdit *filepath = new QLineEdit();
	QPushButton *file_submit = new QPushButton();
	QLabel *file_label = new QLabel();

	QWidget *padding = new QWidget();

	// Set default text
	file_label->setText("Config");
	file_submit->setText("Select File");
	filepath->setText(QString::fromUtf8(this->config_file.c_str()));
	filepath->setText(QString::fromStdString(this->config_file));

	// Layouts
	config_file_option->addWidget(file_label);
	config_file_option->addWidget(filepath);
	config_file_option->addWidget(file_submit);

	this->settings_layout->addLayout(config_file_option);	
	// Set padding to take up bottom space
	padding->setSizePolicy(
			QSizePolicy::Expanding,
			QSizePolicy::Preferred);
	this->settings_layout->addWidget(padding);

	// Any connections / callbacks 
	this->connect(file_submit, &QPushButton::clicked,
			[=](){this->general_tab_button(filepath);});
}

void Settings::general_tab_button(QLineEdit *filepath) {
	QFileDialog::Options options;
	QString selected_filter;

	QString filename = QFileDialog::getOpenFileName(this,
		"Open",
		filepath->text(),
		"Config Files (*.CFG *.CONFIG);;All Files (*)",
		&selected_filter,
		options);

	if (!filename.isEmpty()) {
		this->config_file = filename.toUtf8().constData();
		this->parse_file();
		filepath->setText(QString::fromStdString(this->config_file));
	}
}

void Settings::appearance_tab() {
	clear_layout(this->settings_layout);
	
	QVBoxLayout *style_layout = new QVBoxLayout();

	QVBoxLayout *udp_layout = new QVBoxLayout();
	
	QHBoxLayout *udp_fg_layout = new QHBoxLayout();
	QLineEdit *udp_fg_textbox = new QLineEdit();
	QLabel *udp_fg_label = new QLabel();

	QHBoxLayout *udp_bg_layout = new QHBoxLayout();
	QLineEdit *udp_bg_textbox = new QLineEdit();
	QLabel *udp_bg_label = new QLabel();

	udp_fg_label->setText("UDP Foreground");
	udp_bg_label->setText("UDP Background");

	udp_fg_layout->addWidget(udp_fg_textbox);
	udp_fg_layout->addWidget(udp_fg_label);
	udp_bg_layout->addWidget(udp_bg_textbox);
	udp_bg_layout->addWidget(udp_bg_label);
	udp_layout->addLayout(udp_fg_layout);
	udp_layout->addLayout(udp_bg_layout);


	// Options

	// Layouts 
	style_layout->addLayout(udp_layout);	

	// Main layout
	this->settings_layout->addLayout(style_layout);

	QWidget *padding = new QWidget();
	padding->setSizePolicy(
			QSizePolicy::Expanding,
			QSizePolicy::Preferred);
	this->settings_layout->addWidget(padding);
}

void Settings::clear_layout(QLayout *layout) {
	if (layout) {
        while(layout->count() > 0){
            QLayoutItem *item = layout->takeAt(0);
			if (item->layout()) {
				clear_layout(item->layout());	
			} else {
				QWidget* widget = item->widget();
				if(widget)
					delete widget;
				delete item;
			}
        }
    }
}




