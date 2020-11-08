#include "table.hpp"

Table::Table(QWidget *parent, 
		unsigned width, unsigned height,
		unsigned int x, unsigned int y,
		unsigned row_height) {
	this->labels << "Frame"
		<< "Time"
		<< "Destination" 
		<< "Source" 
		<< "Protocol"
		<< "Information";

	this->table = new QTableWidget(parent);
	this->table->setRowCount(0);
	this->table->setColumnCount(this->labels.count());

	// Set labels 
	this->table->setHorizontalHeaderLabels(this->labels);

	// Resize table
	this->table->setFixedSize(width, height);

	// Set row height
	this->table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	this->table->verticalHeader()->setDefaultSectionSize(row_height);

	// Resizing column widths
	this->table->horizontalHeader()->setStretchLastSection(true);
}

Table::~Table() {

}

void Table::append(QStringList items) {
	if (items.count() != this->labels.count())
		return;
	
	// Add new row
	this->table->insertRow(this->table->rowCount());
	for (int i = 0; i < items.count(); ++i) {
		// Set each respective item
		this->table->setItem(this->table->rowCount() - 1, i,
				new QTableWidgetItem(items.at(i)));

		// Set background colour
		if (this->table->rowCount() & 1)
			this->table->item(this->table->rowCount() - 1, i)->setBackground(QColor(0xc2, 0xff, 0xeb));
	}
}

