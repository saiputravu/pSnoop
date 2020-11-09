#include "table.hpp"

Table::Table(QWidget *parent, 
		unsigned int x, unsigned int y,
		unsigned row_height) : QTableWidget(parent) {
	this->labels << "Frame"
		<< "Time"
		<< "Destination" 
		<< "Source" 
		<< "Protocol"
		<< "Information";

	// this->table = new QTableWidget(parent);
	this->setRowCount(0);
	this->setColumnCount(this->labels.count());

	// Set labels 
	this->setHorizontalHeaderLabels(this->labels);

	// Set sorting enabled
	this->setSortingEnabled(true);

	// Resize table and set resizeable
	// this->resize();
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	// Set row height
	this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	this->verticalHeader()->setDefaultSectionSize(row_height);

	// Resizing column widths
	this->horizontalHeader()->setStretchLastSection(true);

	// Hide left column
	this->verticalHeader()->setVisible(false);

	// Set selection behaviour to rows
	this->setSelectionBehavior(QAbstractItemView::SelectRows);
}

Table::~Table() {

}

void Table::append(QStringList items) {
	if (items.count() != this->labels.count())
		return;
	
	// Add new row
	this->insertRow(this->rowCount());
	for (int i = 0; i < items.count(); ++i) {
		// Set each respective item
		this->setItem(this->rowCount() - 1, i,
				new QTableWidgetItem(items.at(i)));
		
		// Set Cell as Read-Only
		this->item(this->rowCount() - 1, i)->setFlags(this->item(this->rowCount() - 1, i)->flags() ^ Qt::ItemIsEditable);

		// Set background colour
		if (this->rowCount() & 1)
			this->item(this->rowCount() - 1, i)->setBackground(this->packet_colors["random"]);
	}
}

