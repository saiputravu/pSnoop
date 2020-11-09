#ifndef TABLE_H
#define TABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QSizePolicy>
#include <QStringList>
#include <QString>
#include <QColor>

#include <map>
#include <string>

class Table : public QTableWidget {
	Q_OBJECT;

	public:
		Table(QWidget *parent, 
				unsigned int x = 0, unsigned int y = 0, 
				unsigned row_height = 10);
		~Table();

		// Getters 
		QStringList get_labels() { return this->labels; }

		void append(QStringList items);

	private:
		// Properties
		QStringList labels;

		// Colours for packets
		std::map<std::string, QColor> packet_colors = 
		{
			{"random", QColor(0xc2, 0xff, 0xeb)},
			{"tcp", QColor(0x99, 0xc2, 0x4d)}
		};

	signals:

	private slots:
};

#endif // TABLE_H
