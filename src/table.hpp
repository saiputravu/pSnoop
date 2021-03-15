#ifndef TABLE_H
#define TABLE_H

#include <QObject>
#include <QWidget>
#include <QScrollArea>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QSizePolicy>
#include <QStringList>
#include <QString>
#include <QColor>
#include <QKeyEvent>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>

#include <map>
#include <string>
#include <arpa/inet.h>

#include "settings.hpp"
#include "capture/packet.hpp"

class PacketTable : public QTableWidget {
	Q_OBJECT;

	public:
		PacketTable(QWidget *parent = nullptr, 
				unsigned int row_height = 0,
				int endian = 0);
		~PacketTable();

		// Getters 
		QStringList get_labels() { return this->labels; }

		// Methods
		void append(QStringList items);

	private:
		// Methods
		virtual void keyPressEvent(QKeyEvent *event);

		// Properties
		QStringList labels;
		unsigned int row_height;
		int endian;

	signals:

	public slots:
		void reload_packets(PacketStream *stream);
		void append_packet(Packet *packet);	// Connect this to a PacketStream signal
											// Re-cast to Packet *, QSignalMapper only accepts QObject objects
		

};

#endif // TABLE_H
