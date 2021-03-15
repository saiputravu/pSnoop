#include "window.hpp"

Window::Window(QWidget *parent) : QMainWindow(parent) {
	this->init_general();
	this->init_menu();
	this->init_layout();

	// Tasks
	this->capture_thread = new CaptureThread(&this->capture);
}

Window::~Window() {
	// Stop capture if it's active and join thread
	this->capture_thread->kill();
	
	// Kill application as you don't need it without main window open
	QApplication::quit();
}

void Window::init_general() {
	// General Window Settings
	this->resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
	this->setMinimumSize(300, 300);
	this->setWindowTitle("pSnoopGUI - Sai");

	// Setup settings
	this->settings = new Settings("pSnoop.cfg");
	this->capture.get_packet_stream()->set_config(this->settings->get_config_dict()); // Give packet stream the settings dictionary

	// Setting application wide font
	QFont font(QString::fromStdString(this->settings->get("font")));
	font.setStyleHint(QFont::Monospace);
	font.setPointSize(10);
	if (this->settings->get("fontsize") != "Bad"){
		bool ok;
		int fontsize = QString::fromStdString(this->settings->get("fontsize")).toInt(&ok, 10);
		if (ok)
			font.setPointSize(fontsize);
	}

	QApplication::setFont(font);

	// Setting theme
	QApplication::setStyle("fusion");
}

void Window::init_menu() {
	// File Menu Actions
	this->open_action = new QAction("&Open", this);
	this->open_action->setShortcut(QKeySequence("Ctrl+o"));
	this->open_action->setStatusTip("Open a new .pcap file");
	this->connect(this->open_action, 
			&QAction::triggered, 
			this, 
			&Window::not_implemented);
	
	this->save_as_action = new QAction("&Save As", this);
	this->save_as_action->setShortcut(QKeySequence("Ctrl+s"));
	this->save_as_action->setStatusTip("Save file as ... ");
	this->connect(this->save_as_action, 
			&QAction::triggered, 
			this, 
			&Window::not_implemented);

	this->settings_action = new QAction("Preferences", this);
	this->settings_action->setShortcut(QKeySequence("Ctrl+p"));
	this->settings_action->setStatusTip("Open preferences ...");
	this->connect(this->settings_action, 
			&QAction::triggered, 
			this->settings, 
			&Settings::open_window);
	
	this->quit_action = new QAction("Quit", this);
	this->quit_action->setShortcut(QKeySequence("Ctrl+w"));
	this->quit_action->setStatusTip("Quit Application");
	this->connect(this->quit_action, 
			&QAction::triggered, 
			this, 
			&Window::on_exit);

	// File Menu
	this->file_menu = this->menuBar()->addMenu(QString("&File"));
	this->file_menu->addAction(open_action);
	this->file_menu->addAction(save_as_action);
	this->file_menu->addAction(settings_action);
	this->file_menu->addSeparator();
	this->file_menu->addAction(quit_action);

	// Edit Menu Actions
	this->copy_action = new QAction("&Copy Selection", this);
	this->copy_action->setShortcut(QKeySequence("Ctrl+c"));
	this->connect(this->copy_action, 
			&QAction::triggered, 
			this, 
			&Window::not_implemented);

	this->find_action = new QAction("&Find", this);
	this->find_action->setShortcut(QKeySequence("Ctrl+f"));
	this->find_action->setStatusTip("Find instances");
	this->connect(this->find_action, 
			&QAction::triggered, 
			this, 
			&Window::not_implemented);

	this->filter_action = new QAction("F&ilter", this);
	this->filter_action->setStatusTip("Select the filter");
	this->connect(this->filter_action, 
			&QAction::triggered, 
			this, 
			&Window::not_implemented);

	// Edit Menu
	this->edit_menu = this->menuBar()->addMenu(QString("&Edit"));
	this->edit_menu->addAction(copy_action);
	this->edit_menu->addAction(find_action);
	this->edit_menu->addAction(filter_action);

	// Capture Menu Actions
	this->select_interface_action = new QAction("&Select Interface", this);
	this->select_interface_action->setShortcut(QKeySequence("Ctrl+i"));
	this->select_interface_action->setStatusTip("Select the interface to capture packets on"); 
	this->connect(this->select_interface_action, 
			&QAction::triggered, 
			this, 
			&Window::select_interface);
	
	this->start_action = new QAction("&Begin Capture", this);
	this->start_action->setShortcut(QKeySequence("Ctrl+b"));
	this->connect(this->start_action, 
			&QAction::triggered, 
			this, 
			&Window::begin_capture);
	
	this->stop_action = new QAction("&End Capture", this);
	this->stop_action->setShortcut(QKeySequence("Ctrl+e"));
	this->connect(this->stop_action, 
			&QAction::triggered, 
			this, 
			&Window::end_capture);

	this->restart_action = new QAction("&Restart Capture", this);
	this->restart_action->setShortcut(QKeySequence("Ctrl+Shift+b"));
	this->connect(this->restart_action, 
			&QAction::triggered, 
			this, 
			&Window::restart_capture);

	this->capture_filter_action = new QAction("&Capture filter options", this);
	this->capture_filter_action->setStatusTip("Raw libpcap filter - filter before beginning capturing");
	this->connect(this->capture_filter_action, 
			&QAction::triggered, 
			this, 
			&Window::capture_filter);
	 
	this->statistics_action = new QAction("S&tatistics", this);
	this->statistics_action->setStatusTip("Open protocol statistics");
	this->connect(this->statistics_action, 
			&QAction::triggered, 
			this, 
			&Window::statistics);
	
	// Capture Menu
	this->capture_menu = this->menuBar()->addMenu(QString("&Capture"));
	this->capture_menu->addAction(select_interface_action);
	this->capture_menu->addAction(capture_filter_action);
	this->capture_menu->addAction(statistics_action);
	this->capture_menu->addSeparator();
	this->capture_menu->addAction(start_action);
	this->capture_menu->addAction(stop_action);
	this->capture_menu->addAction(restart_action);

	// Help Menu Actions
	this->about_action = new QAction("&About", this);
	this->connect(this->about_action, 
			&QAction::triggered, 
			this, 
			&Window::about);

	// Help Menu
	this->help_menu = this->menuBar()->addMenu(QString("&Help"));
	this->help_menu->addAction(this->about_action);
}

void Window::init_layout() {

	// Overall container	
	this->main_widget = new QWidget(this);
	this->main_widget->move(0, 15);
	this->main_widget->setFixedSize(this->width(), this->height() - 15);
	this->container = new QVBoxLayout;

	// Splitters / Dividers
	this->h_splitter = new QSplitter(Qt::Vertical, this);
	this->v_splitter = new QSplitter(Qt::Horizontal, this);

	// Packet table object & setup add new packet on receiving new packet
	this->packet_table = new PacketTable(this);
	this->connect(&this->capture, &Networking::packet_recv,
			this->packet_table, &PacketTable::append_packet);

	// HexView object
	this->hex_view = new HexView(this);
	this->hex_view->setMaximumSize(hex_view->get_cell_width() * (8 + 7),
			this->height()+4000);

	this->connect(this->packet_table, &PacketTable::cellClicked,
			this, &Window::load_packet_bytes,
			Qt::DirectConnection);

	// Filter query packet search box
	this->search_box = new SearchBox(this->capture.get_packet_stream(), this);
	this->connect(this->search_box, &SearchBox::reload_packets, 
			this->packet_table, &PacketTable::reload_packets);

	// Treeview for packet structure
	this->packet_tree = new PacketTree(this);
	this->packet_tree->add_tree_root(QString("Root1"), QString("Value1"));
	this->packet_tree->add_tree_root(QString("Root2"), QString("Value2"));

	this->packet_tree->add_tree_child((std::vector<unsigned int>){0}, QString("Child1"), QString("ValueChild1"));
	this->packet_tree->add_tree_child((std::vector<unsigned int>){0, 0}, QString("ChildChild1"), QString("ValueChildChild1"));

	this->connect(this->packet_table, &PacketTable::cellClicked, 
			this, &Window::load_packet_tree);

	// Right hand side
	this->h_splitter->addWidget(this->packet_table);
	this->h_splitter->addWidget(this->search_box);
	this->h_splitter->addWidget(this->packet_tree);

	// Left hand side
	this->v_splitter->addWidget(this->hex_view);
	this->v_splitter->addWidget(this->h_splitter);

	// Putting them together 
	this->container->addWidget(this->v_splitter);
	this->main_widget->setLayout(this->container);
}

void Window::resizeEvent(QResizeEvent *event) {
	
	// Resize everything on window resize event
	this->main_widget->setFixedSize(this->width(), this->height() - 15);

	// Call normal procedure
	QWidget::resizeEvent(event);
}

void Window::select_interface() {
	// Window setup
	QWidget *pop_up = new QWidget();
	pop_up->setMinimumSize(400,200);
	
	// List of interfaces setup
	QListWidget *list = new QListWidget();
	int i = 0;
	while (this->capture.get_device(i)) {
		new QListWidgetItem(this->capture.get_device(i)->name, list);
		i++;
	}
	
	// Select button 
	QPushButton *select = new QPushButton("Select\nInterface");
	select->setMinimumHeight(150);

	QSignalMapper *sig_mapper = new QSignalMapper(pop_up);
	this->connect(select, SIGNAL(clicked()), sig_mapper, SLOT(map()));
	sig_mapper->setMapping(select, list);
	this->connect(sig_mapper, SIGNAL(mapped(QWidget *)), this, SLOT(select_interface_button(QWidget *)));

	// Layout setup
	QHBoxLayout *layout = new QHBoxLayout(pop_up);
	layout->addWidget(list);
	layout->addWidget(select);

	pop_up->show();
}

void Window::select_interface_button(QWidget *list) {
	// Select interface in the networking object
	QListWidget *list_widget = (QListWidget *)list;
	this->capture.setup_device(list_widget->currentRow());
	QString title;
	title.append(QString::fromLatin1("Interface : "));
	title.append(QString::fromLatin1(this->capture.get_device(list_widget->currentRow())->name));
	this->setWindowTitle(title);
}

void Window::capture_filter() {	
	if (!this->capture_thread->get_pause()) {
		error_pop_up("You can only set a capture filter when not listening!");
		return;
	} else if (!strncmp("\0", this->capture.get_cur_device(), 1)) { // Check if interface selected
		error_pop_up("You must select an interface first!");
		return;
	}

	QWidget *pop_up = new QWidget();
	QHBoxLayout *layout = new QHBoxLayout();
	QLineEdit *textbox = new QLineEdit();
	QPushButton *submit = new QPushButton();
	
	submit->setText("Set filter");
	QSignalMapper *sig_mapper = new QSignalMapper(pop_up);
	this->connect(submit, SIGNAL(clicked()), sig_mapper, SLOT(map()));
	sig_mapper->setMapping(submit, textbox);
	this->connect(sig_mapper, SIGNAL(mapped(QWidget *)), this, SLOT(capture_filter_button(QWidget *)));

	layout->addWidget(textbox);
	layout->addWidget(submit);
	pop_up->setLayout(layout);
	pop_up->show();
}

void Window::capture_filter_button(QWidget *textbox) {
	QLineEdit *query_box = (QLineEdit *)textbox;
	std::string query = query_box->text().toUtf8().constData();
	int ret = this->capture.set_filter(query.c_str());
	if (ret != 0) {
		error_pop_up("Invalid capture filter");
		return;
	}
	query_box->parentWidget()->close();
}

void Window::statistics() {
	if (!this->capture_thread->get_pause()) {
		this->error_pop_up("Please stop capturing first!");
		return;
	}

	QWidget *pop_up = new QWidget();
	QPieSeries *series = new QPieSeries();
	QChart *chart = new QChart();
	QChartView *chart_view = new QChartView(chart);
	QVBoxLayout *layout = new QVBoxLayout();

	// Calculations
	std::map<std::string, unsigned int>protocol_count = {};
	for (int i = 0; i < this->capture.get_packet_stream()->size(); ++i) {
		Packet *pkt = (*this->capture.get_packet_stream())[i];
		if (protocol_count.find(pkt->get_protocol()) == protocol_count.end()) 
			// Key doesn't exist 
			protocol_count[pkt->get_protocol()] = 1;
		else
			// Key exists
			protocol_count[pkt->get_protocol()] += 1;
	}

	// Settings 
	pop_up->setWindowTitle("Packet Protocol Statistics");
	for (auto &i : protocol_count) 
		series->append(QString::fromStdString(i.first), i.second);

	chart->addSeries(series);
	chart->setTitle("Protocol Statistics");
	chart->setAnimationOptions(QChart::AllAnimations);

	chart->legend()->setMarkerShape(QLegend::MarkerShapeCircle);
	chart->legend()->setShowToolTips(true);
	chart->legend()->show();

	chart_view->setChart(chart);
	chart_view->setRenderHint(QPainter::Antialiasing);
	
	// Fill up labels as a sort of cache for later use
	for (auto &i : series->slices())
		this->chart_labels[i] = i->label();

	// Connections
	this->connect(series, &QPieSeries::hovered,
			this, &Window::statistics_hover);

	// Layout structure
	layout->addWidget(chart_view);
	
	pop_up->resize(600,600);
	pop_up->setLayout(layout);
	pop_up->show();
}

void Window::statistics_hover(QPieSlice *slice, bool state) {
	if (state) {
		slice->setExploded(true);
    	slice->setLabel(this->chart_labels[slice] + QString(" %1%").arg(100*slice->percentage(), 0, 'f', 2));
		slice->setLabelVisible(true);
	} else {
		slice->setExploded(false);
    	slice->setLabel(this->chart_labels[slice]);
		slice->setLabelVisible(false);
	}
}

void Window::begin_capture() {
	if (this->capture_active) {
		this->error_pop_up("Capture already active!");
	} else if (strncmp("\0", this->capture.get_cur_device(), 1)) { // Check if interface selected

		// Run the thread if never run before
		// Else resume thread
		if (!this->capture_thread->isRunning()) {
			this->capture_thread->start();
		} else {
			this->capture_thread->resume();
		}

		this->setWindowTitle(QString("Live Capturing: ") + QString(this->capture.get_cur_device()));
	}
	 else {
		this->error_pop_up ("Please select an interface first!");
	}
}

void Window::end_capture () {
	this->setWindowTitle("Ending live capture ... ");

	// Pause thread 
	this->capture_thread->pause();

	this->setWindowTitle("Ended live capture ... ");
}

void Window::restart_capture () {
	this->end_capture();

	// Reset everything 
	this->packet_table->setRowCount(0);
	this->capture.clear_packets();
	this->setWindowTitle("Reseting packet table ... ");

	this->begin_capture();
}

void Window::load_packet_bytes(int row, int col) {	
	int frame = this->packet_table->item(row, 0)->text().toInt();
	Packet *pkt = (*this->capture.get_packet_stream())[frame];
	
	// Load bytes in the hexview 
	this->hex_view->load_bytes(pkt->get_data(), 
			pkt->get_header_len());
}

void Window::load_packet_tree(int row, int col) {
	int frame = this->packet_table->item(row, 0)->text().toInt();
	Packet *pkt = (*this->capture.get_packet_stream())[frame];
	
	// Load tree into the treeview
	this->packet_tree->load_tree(pkt->get_tree());
}

void Window::error_pop_up (std::string error, std::string title) {
	QFont local_font(QString::fromStdString(this->settings->get("font")));
	local_font.setPointSize(10);

	QWidget *pop_up = new QWidget();
	QVBoxLayout *container = new QVBoxLayout();
	QLabel *label = new QLabel(pop_up);
	QPushButton *button = new QPushButton(pop_up);

	pop_up->setWindowTitle(QString::fromStdString(title));
	label->setText(QString::fromStdString(error));
	button->setText("Okay");
	this->connect(button, SIGNAL(clicked()), pop_up, SLOT(close()));

	container->addWidget(label);
	container->addWidget(button);
	pop_up->setLayout(container);
	pop_up->show();
}

void Window::not_implemented() {
	this->error_pop_up("This has not been implemented yet.", "Feature Not Implemented");
}

void Window::about() {
	QFont local_font(QString::fromStdString(this->settings->get("font")));
	local_font.setBold(true);
	local_font.setPointSize(12);

	QWidget *pop_up = new QWidget();
	pop_up->setWindowTitle("About");
	pop_up->setFixedSize(400, 300);

	QVBoxLayout *container = new QVBoxLayout;
	QLabel *header = new QLabel("pSnoop GUI Version");
	header->setFont(local_font);

	QLabel *about_label = new QLabel("designed and developed by Sai\n"
			"This is a packet sniffing tool coded in C++\n"
			"using LibPcap and Qt\n");
	local_font.setBold(false);
	local_font.setPointSize(9);
	about_label->setFont(local_font);

	QPushButton *quit_button = new QPushButton();
	quit_button->setText("Okay");
	quit_button->setFont(local_font);
	this->connect(quit_button, SIGNAL(clicked()), pop_up, SLOT(close()));

	container->addWidget(header);
	container->addWidget(about_label);
	container->addWidget(quit_button);
	pop_up->setLayout(container);

	pop_up->show();
}

void Window::on_exit() {
	// Implement a prompt to save work 
	QApplication::exit();
}
