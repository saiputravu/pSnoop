#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QResizeEvent>
#include <QFont>
#include <QString>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>

class Settings : public QWidget {
	Q_OBJECT;

	public:
		Settings(std::string config_file, QWidget *parent=nullptr);
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

		// Qt Objects
		QHBoxLayout *container;
		QVBoxLayout *settings_layout;
		QWidget *settings;
		QListWidget *categories;
		QPushButton *test;

	signals:

	public slots:
		void open_window();
		void resizeEvent(QResizeEvent *event);

};

#endif // SETTINGS_H
