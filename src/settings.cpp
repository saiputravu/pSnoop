#include "settings.hpp"

Settings::Settings(std::string config_file) : config_file(config_file) {
	this->parse_file();
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
