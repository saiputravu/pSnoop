#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QResizeEvent>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QContextMenuEvent>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QKeySequence>
#include <QLabel>
#include <QFont>
#include <QScrollArea>
#include <QListWidget>
#include <QSignalMapper>

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QRunnable>

#include <QDebug>

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <thread>

#include "table.hpp"
#include "settings.hpp"
#include "hexview.hpp"
#include "searchbox.hpp"
#include "capture/networking.hpp"

class CaptureThread : public QThread {
	Q_OBJECT;
	public:
		CaptureThread(Networking *capture, bool *active) 
			: capture(capture), active(active) {
			
			// Setting internal variables to indicate whether packets are being captured
			//this->connect(this->capture, &Networking::packet_recv,
			//        [this](Packet *unused) {if (!this->running) this->running = true;} );
			//this->connect(this->capture, &Networking::stopped_recv,
			//        [this]() {if (this->running) this->running = false;} );
		}

		bool get_pause() { return this->b_pause; }

		void resume() {
			if (!this->b_pause)
				return;
			this->mutex.lock();
			this->b_pause = false;
			//*this->active = true;
			this->mutex.unlock();
			this->pause_condition.wakeAll();
		}

		void pause() {
			if (this->b_pause)
				return;
			this->mutex.lock();
			this->b_pause = true;
			//*this->active = false;
			this->mutex.unlock();
		}

		void kill() {
			if (!this->b_pause)
				this->resume();
			this->mutex.lock();
			this->dead = true;
			this->mutex.unlock();
		}	

		void run() override {
			while(!dead) {
				this->mutex.lock();
				if (this->b_pause)  {
					this->pause_condition.wait(&this->mutex); 
				} else {
				}
				this->mutex.unlock();
				this->capture->listen_next_packet();
			}
			qDebug("Killed listen");
		}

	private:
		Networking *capture;
		bool *active;
		bool b_pause = false;
		bool dead = false;
		bool run_once = false;

		QMutex mutex;
		QWaitCondition pause_condition;
};

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
		HexView *hex_view;
		QSplitter *h_splitter;
		QSplitter *v_splitter;
		QVBoxLayout *container;
		QWidget *main_widget;
		QWidget *left_restrictor;
		
		Networking capture;	// Object for networking side of application
		bool capture_active = false;
		CaptureThread *capture_thread = nullptr;

		// General Methods
		void init_general();
		void init_menu();
		void init_layout();
		void error_pop_up(std::string error,
				std::string title = "Error");

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
		
	signals:
		void begin_capture_signal(bool *active);

	private slots:
		// Menu Slots
		void not_implemented();
		void about();
		void on_exit();

		// Capture Menu Slots
		void select_interface();
		void select_interface_button(QWidget *list);
		void begin_capture();
		void end_capture();
		void restart_capture();
		void capture_filter();

		// Pane related slots
		void load_packet_bytes(int row, int col);
};

#endif // WINDOW_H
