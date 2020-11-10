#ifndef HEXVIEW_H
#define HEXVIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAbstractScrollArea>

class HexView : public QAbstractScrollArea {
	Q_OBJECT;

	public:
		HexView(QWidget *parent = nullptr);
		~HexView();

	private:

	signals:
	
	private slots:
};

#endif // HEXVIEW_H
