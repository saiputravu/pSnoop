#ifndef HEXVIEW_H
#define HEXVIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QColor>
#include <QString>
#include <QDebug>

#include <stdio.h>
#include <map>
#include <iostream>
#include <arpa/inet.h>

#include "capture/protocols.hpp"
#include "capture/Utils/utils.hpp"

class HexView : public QTableWidget {
	Q_OBJECT;

	public:
		HexView(QWidget *parent = nullptr, 
				int cell_width = 23);
		~HexView();

		// Getters 
		int get_cell_width() { return this->cell_width; }

		// Be careful of usage, this could lead to overflows
		void load_bytes(unsigned char *bytes, int len);

	private:
		int cell_width;
		QTableWidgetItem *proto_item;

	signals:
	
	public slots:
};

#endif // HEXVIEW_H
