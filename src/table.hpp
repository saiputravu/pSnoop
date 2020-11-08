#ifndef TABLE_H
#define TABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QStringList>
#include <QString>
#include <QColor>

class Table : public QWidget {
	Q_OBJECT;

	public:
		Table(QWidget *parent, 
				unsigned width, unsigned height, 
				unsigned int x = 0, unsigned int y = 0, 
				unsigned row_height = 10);
		~Table();

		// Getters 
		QTableWidget *get_table() { return this->table; }	
		QStringList get_labels() { return this->labels; }

		void append(QStringList items);

	private:
		QTableWidget *table;
		QStringList labels;

	signals:

	private slots:
};

#endif // TABLE_H
