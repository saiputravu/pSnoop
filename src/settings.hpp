#ifndef SETTINGS_H
#define SETTINGS_H

#include <QFont>
#include <QString>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>

class Settings {
	public:
		Settings(std::string config_file);
		~Settings();

		// Getters 
		QString get(std::string key);
		
		// Methods
		void parse_file();

	private:
		// Properties
		std::string config_file;
		std::map<std::string, QString> config;

		void store_value(std::string key, std::string value);

};

#endif // SETTINGS_H
