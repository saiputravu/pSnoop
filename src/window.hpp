#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QResizeEvent>
#include <QSplitter>
#include <QVBoxLayout>
#include <QPushButton>
#include <QContextMenuEvent>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>

#include "table.hpp"

class Window : public QMainWindow {
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
		QVBoxLayout *container;
		QWidget *main_widget;

		// Methods
		void init_general();
		void init_menu();
		void init_layout();

		// Menus
		QMenu *file_menu;
		QMenu *help_menu;

		// File Menu Actions

		// Help Menu Actions
		
	signals:

	private slots:
		// Menu Slots
		// void about();

};

#endif // WINDOW_H
