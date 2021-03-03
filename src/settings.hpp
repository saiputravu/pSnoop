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
		void generate_default_config();

		static void clear_layout(QLayout *layout);

	private:
		// Properties
		std::string config_file = "\0";
		std::map<std::string, QString> config;

		void store_value(std::string key, std::string value);

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
		void appearance_tab();

		// Re-implemented
		void resizeEvent(QResizeEvent *event);

};

#endif // SETTINGS_H
