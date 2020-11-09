#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QApplication>
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
#include <QKeySequence>
#include <QLabel>
#include <QFont>

#include <iostream>
#include <stdio.h>

#include "table.hpp"
#include "settings.hpp"

class Window : public QMainWindow {
	Q_OBJECT;

	public:
		Window(QWidget *parent = nullptr);
		~Window();

	protected:
		void resizeEvent(QResizeEvent *event);

	private:
		// General Properties
		Settings *settings;

		Table *packet_table;
		QSplitter *h_splitter;
		QSplitter *v_splitter;
		QVBoxLayout *container;
		QWidget *main_widget;

		// General Methods
		void init_general();
		void init_menu();
		void init_layout();

		// Menus
		QMenu *file_menu;
		QMenu *edit_menu;
		QMenu *capture_menu;
		QMenu *help_menu;

		// File Menu Actions
		QAction *open_action;
		QAction *save_as_action;
		QAction *settings_action;
		QAction *quit_action;

		// Edit Menu Actions
		QAction *copy_action;
		QAction *find_action;
		QAction *filter_action;

		// Capture Menu Actions
		QAction *select_interface_action;
		QAction *start_action;
		QAction *stop_action;
		QAction *restart_action;
		QAction *refresh_action;
		QAction *capture_filter_action;
		QAction *statistics_action;

		// Help Menu Actions
		QAction *about_action;
		void about();
		
	signals:

	private slots:
		// Menu Slots
		void not_implemented();

};

#endif // WINDOW_H
