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

void HexView::load_bytes(unsigned char *bytes, int len) {
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

	// Print output on every packet click
	
	//Utils::hexdump(bytes, len);
	//struct ip_header *i_header = (struct ip_header *)(bytes + sizeof(ether_header));
	//struct tcp_header *t_header = (struct tcp_header *)(bytes + sizeof(struct ether_header) +
	//        4*(unsigned int)(i_header->type_helen & 0x0f));
	//unsigned int payload_offset = (unsigned int)(sizeof(struct ether_header) +
	//            4*(unsigned int)(i_header->type_helen&0x0f) +
	//            4*(unsigned int)(htons(t_header->offset_resv_control) >> 12));
	//char *payload = (char *)(bytes + payload_offset);
	//Utils::hexdump((unsigned char *)payload, len-payload_offset); 
	//printf("Payload:\n%s\n", payload);
	//printf("Source port: [%d] | Destination port: [%d]\n", htons(t_header->source_port), htons(t_header->dest_port));
	//printf("Sequence: [%x] | Ack: [%x]\n", htonl(t_header->seq), htonl(t_header->ack));
	//printf("Flags: ");
    //
	//unsigned short control = htons(t_header->offset_resv_control);
	//if (control & 0b100000)
	//    printf("URG ");
	//if (control & 0b010000)
	//    printf("ACK ");
	//if (control & 0b001000)
	//    printf("PSH ");
	//if (control & 0b000100)
	//    printf("RST ");
	//if (control & 0b000010)
	//    printf("SYN ");
	//if (control & 0b000001)
	//    printf("FIN ");
	//printf("\n");
	//printf("Data offset: [%x | %d]\n", control&0xf000, control>>12);
}
