#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QListWidget>
#include <QResizeEvent>
#include <QComboBox>
#include <QButtonGroup>
#include <QFont>
#include <QString>
#include <QStyleFactory>
#include <QFileDialog>

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
		std::string get(std::string key);
		std::map<std::string, std::string> *get_config_dict() { return &this->config; }
		
		// Setters 
		void store_value(std::string key, std::string value) {
			this->config[key] = value;
		}

		// Methods
		void generate_default_config();

		static void clear_layout(QLayout *layout);

	private:
		// Properties
		std::string config_file = "./pSnoop.cfg";
		std::map<std::string, std::string> config;


		// Qt Objects
		QHBoxLayout *container;
		QVBoxLayout *settings_layout;
		QListWidget *categories;

	signals:

	public slots:
		void set_config_file(const QString &text) { this->config_file = text.toUtf8().constData(); }
		void parse_file();
		void open_window();

		// Tabs
		void tab_manager(QListWidgetItem *item);
		void general_tab();
		void general_tab_button(QLineEdit *filepath);
		void appearance_tab();

		// Re-implemented
		void resizeEvent(QResizeEvent *event);

};

#endif // SETTINGS_H
