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

#include <map>
#include <string>

#include "settings.hpp"
#include "capture/packet.hpp"

class Table : public QTableWidget {
	Q_OBJECT;

	public:
		Table(QWidget *parent = nullptr, 
				unsigned int row_height = 10);
		~Table();

		// Getters 
		QStringList get_labels() { return this->labels; }

		// Methods
		void append(QStringList items);
		virtual void clear();

	private:
		// Methods
		virtual void keyPressEvent(QKeyEvent *event);

		// Properties
		QStringList labels;
		unsigned int row_height;

		// Colours for packets
		std::map<std::string, QColor> packet_colors =
		{
			{"random", QColor(0xc2, 0xff, 0xeb)},
			{"tcp", QColor(0x99, 0xc2, 0x4d)}
		};

	signals:

	public slots:
		void append_packet(Packet *packet);	// Connect this to a PacketStream signal
												// Re-cast to Packet *, QSignalMapper only accepts QObject objects
		

};

#endif // TABLE_H
