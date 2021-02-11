#include "hexview.hpp"

HexView::HexView(QWidget *parent, int cell_width) : QTableWidget(parent), cell_width(cell_width) {

	this->proto_item = new QTableWidgetItem();
	this->proto_item->setTextAlignment(Qt::AlignCenter);
	this->setItemPrototype(this->proto_item);

	this->setColumnCount(8 + 1);
	this->setRowCount(0);

	this->verticalHeader()->hide();
	this->horizontalHeader()->hide();
	this->setShowGrid(false);
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);

	for (int i = 0; i < 8; ++i)
		this->setColumnWidth(i, this->cell_width);
	this->setColumnWidth(8, 5*this->cell_width);
}

HexView::~HexView() {

}

void HexView::load_bytes(char *bytes, int len) {
	this->setRowCount(0);
	this->setRowCount((int)(len / 8) + 1);

	// Fill up the hex 
	char to_hex[3];
	for (int i = 0; i < len; ++i) {
		snprintf(to_hex, 3, "%02x", bytes[i]);
		this->setItem((int)(i/8), i%8,
				new QTableWidgetItem(QString(to_hex)));
	}

	// Fill up the ascii sections
	char ascii[17];
	for (int i = 0; i < (int)(len / 8); ++i) {
		memset(ascii, 0, 17);
		for (int j = 0; j < 8; ++j) {
			ascii[j*2] = ' ';
			if (bytes[j + i*8] >= 0x20 && bytes[j + i*8] <= 0x7e)
				ascii[j*2 + 1] = bytes[j + i*8];
			else
				ascii[j*2 + 1] = '.';
		}
		this->setItem(i, 8,
				new QTableWidgetItem(QString(ascii)));
	}
	
	// Fill last line if there is excess 
	if (len%8) {
		memset(ascii, 0, 17);
		for (int i = 0; i < 8 - (len % 8); ++i) {
			ascii[i*2] = ' ';
			if (bytes[8*(int)(len / 8) + i] >= 0x20 && bytes[8*(int)(len / 8) + i] <= 0x7e)
				ascii[i*2 + 1] = bytes[8*(int)(len / 8) + i];
			else
				ascii[i*2 + 1] = '.';
		}
		this->setItem(this->rowCount() - 1, 8,
				new QTableWidgetItem(QString(ascii)));
	}

	// Color grey alternative lines
	QColor col(0xd3, 0xd3, 0xd3);
	for (int i = 0; i < len; ++i) {
		if ( !((int)(i / 8) & 1) )
			this->item((int)(i/8), i%8)->setBackground(col);
	}
}
