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

	// Parse the settings file
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
			<< "colorscheme=black" << std::endl;
	stream.close();
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
				this->config[key] = QString(value.c_str());
		}
	}
}

QString Settings::get(std::string key) {
	if (this->config.find(key) == this->config.end()) 
		return QString("Bad");
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
	file_label->setText("Config file");
	file_submit->setText("OK");
	filepath->setText(QString::fromUtf8(this->config_file.c_str()));

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
			this, &Settings::parse_file);
	this->connect(filepath, &QLineEdit::textEdited, 
			this, &Settings::set_config_file);
}

void Settings::appearance_tab() {
	clear_layout(this->settings_layout);
	
	QHBoxLayout *style_layout = new QHBoxLayout();
	QComboBox *style_selection = new QComboBox();
	QLabel *style_label = new QLabel();


	// Options 
	style_selection->addItems(QStyleFactory::keys());

	style_label->setText("GUI Style");

	// Layouts 
	style_layout->addWidget(style_label);
	style_layout->addWidget(style_selection);	

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




