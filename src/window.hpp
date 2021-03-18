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
#include <QListWidget>
#include <QTreeWidget>
#include <QLabel>

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QFileDialog>
#include <QFont>

#include <QPieSeries>
#include <QChartView>
#include <QChart>
#include <QPainter>
#include <QLegend>

#include <QSignalMapper>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QRunnable>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <map>

#include "table.hpp"
#include "settings.hpp"
#include "hexview.hpp"
#include "tree.hpp"
#include "searchbox.hpp"
#include "capture/networking.hpp"
#include "capture/packet.hpp"

QT_CHARTS_USE_NAMESPACE
class CaptureThread : public QThread {
	Q_OBJECT
	public:
		CaptureThread(Networking *capture) 
			: capture(capture) {
			
		}

		bool get_pause() { return this->b_pause; }

		void resume() {
			if (!this->b_pause)
				return;
			this->mutex.lock();
			this->b_pause = false;
			this->mutex.unlock();
			this->pause_condition.wakeAll();
		}

		void pause() {
			if (this->b_pause)
				return;
			this->mutex.lock();
			this->b_pause = true;
			this->mutex.unlock();
		}

		void kill() {
			// Don't use this method unless you want to start a new thread object
			if (!this->b_pause)
				this->resume();
			this->mutex.lock();
			this->dead = true;
			this->mutex.unlock();
		}	

		void run() override {
			this->resume(); // as it starts off as in pause
			while(!dead) {
				this->mutex.lock();
				if (this->b_pause)  {
					this->pause_condition.wait(&this->mutex); 
				}
				this->mutex.unlock();
				this->ret = this->capture->listen_next_packet();

				// Pause thread to show end
				if (ret == -2)
					this->pause();
			}
		}

	private:
		Networking *capture;
		bool b_pause = true;
		bool dead = false;
		int ret = 0;

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

		PacketTable *packet_table;
		HexView *hex_view;
		PacketTree *packet_tree;
		SearchBox *search_box;
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

		// Pie chart labels
		std::map<QPieSlice*,QString> chart_labels = std::map<QPieSlice*,QString>();
		
	signals:
		void begin_capture_signal(bool *active);

	private slots:
		// Menu Slots
		void not_implemented();
		void about();
		void on_exit();

		// File Menu Slots
		void select_file();
		void select_save_file();

		// Capture Menu Slots
		void select_interface();
		void select_interface_button(QWidget *list);
		void begin_capture();
		void end_capture();
		void restart_capture();
		void capture_filter();
		void capture_filter_button(QWidget *textbox);
		void statistics();
		void statistics_hover(QPieSlice *slice, bool state); // On hover, zoom pie slice

		// Pane related slots
		void load_packet_bytes(int row, int col);
		void load_packet_tree(int row, int col);
};

#endif // WINDOW_H
