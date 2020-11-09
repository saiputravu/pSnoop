#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QDesktopWidget>
#include <QResizeEvent>
#include <QSplitter>
#include <QVBoxLayout>
#include <QPushButton>

#include "table.hpp"

class Window : public QWidget {
	Q_OBJECT;

	public:
		Window(QWidget *parent = nullptr);
		~Window();

	protected:
		void resizeEvent(QResizeEvent *event);

	private:
		// Properties
		Table *packet_table;
		QSplitter *h_splitter;
		QSplitter *v_splitter;
		QVBoxLayout* container;

		// Methods
		void init();
		
	signals:

	private slots:

};

#endif // WINDOW_H
