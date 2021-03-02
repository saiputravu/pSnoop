#include "settings.hpp"

Settings::Settings(std::string config_file, QWidget *parent) : config_file(config_file), QWidget(parent){
	// this->parse_file();
	
	this->setWindowTitle("Settings");

	container = new QHBoxLayout();

	settings_layout = new QVBoxLayout();
	categories = new QListWidget();
	this->categories->setMaximumWidth((int)(this->width() * 0.25));

	test = new QPushButton();
	test->setText("Submit");

	this->settings_layout->addWidget(test);

	this->container->addWidget(categories);
	this->container->addLayout(settings_layout);
	
	this->setLayout(container);
}

Settings::~Settings() {

}

void Settings::parse_file() {
	std::ifstream file(this->config_file);

	if (!file) 
		return;
	
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();

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
