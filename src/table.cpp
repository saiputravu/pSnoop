#include "table.hpp"

Table::Table(QWidget *parent, 
		unsigned int row_height,
		int endian) : QTableWidget(parent), row_height(row_height), endian(endian){
	this->labels << "Frame"
		<< "Time"
		<< "Source" 
		<< "Destination" 
		<< "Protocol"
		<< "Information";

	// this->table = new QTableWidget(parent);
	this->setRowCount(0);
	this->setColumnCount(this->labels.count());

	// Set labels 
	this->setHorizontalHeaderLabels(this->labels);

	// Set sorting disabled - this messes up some features
	this->setSortingEnabled(false);

	// Set dividers to invisible
	this->setShowGrid(false);

	// Resize table and set resizeable
	// this->resize();
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	// Set row height
	// Set it as 2% of screen height
	if (this->row_height == 0) {
		QRect screen = QApplication::desktop()->screenGeometry();
		this->row_height = (int)(screen.height() * 0.02);
	}
	this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	this->verticalHeader()->setDefaultSectionSize(this->row_height);

	// Resizing column widths
	this->horizontalHeader()->setStretchLastSection(true);

	// Hide left column
	this->verticalHeader()->setVisible(false);

	// Set selection behaviour to rows
	this->setSelectionBehavior(QAbstractItemView::SelectRows);

}

Table::~Table() {

}

void Table::keyPressEvent(QKeyEvent *event) {
	// Super class implementation
	QTableWidget::keyPressEvent(event);

	if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) {
		emit this->cellClicked(this->currentRow(),
				this->currentColumn());
	}
}

void Table::clear() {
	// Super class implementation 
	QTableWidget::clear();

		this->labels << "Frame"
		<< "Time"
		<< "Source" 
		<< "Destination" 
		<< "Protocol"
		<< "Information";

	// this->table = new QTableWidget(parent);
	this->setRowCount(0);
	this->setColumnCount(this->labels.count());

	// Set labels 
	this->setHorizontalHeaderLabels(this->labels);

	// Set sorting disabled - this messes up some features
	this->setSortingEnabled(false);

	// Set dividers to invisible
	this->setShowGrid(false);

	// Resize table and set resizeable
	// this->resize();
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	// Set row height
	this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	this->verticalHeader()->setDefaultSectionSize(this->row_height);

	// Resizing column widths
	this->horizontalHeader()->setStretchLastSection(true);

	// Hide left column
	this->verticalHeader()->setVisible(false);

	// Set selection behaviour to rows
	this->setSelectionBehavior(QAbstractItemView::SelectRows);
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

void Table::append_packet(Packet *packet) {
	// Frame, time, src_ip, dst_ip, protocol, general info
	QStringList items;
	items.append(QString::number(packet->get_frame())); 
	items.append(QString::fromUtf8(Utils::convert_time(packet->get_header_timestamp())));
	unsigned int src, dest;
	src  = packet->get_ip_header()->source;
	dest = packet->get_ip_header()->dest;
	if (!endian) {
	// swap endianness (wifi)
		src = ntohl(src);
		dest = ntohl(dest);
	}

	items.append(QString::fromStdString(Utils::convert_ip(src)));
	items.append(QString::fromStdString(Utils::convert_ip(dest)));
	items.append(QString::fromStdString(packet->get_protocol()));
	items.append(QString::fromStdString(packet->get_info()));

	this->append(items);
}

