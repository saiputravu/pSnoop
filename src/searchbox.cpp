#include "searchbox.hpp"

SearchBox::SearchBox(PacketStream *stream, QWidget *parent) : stream(stream), QWidget(parent) {

	this->setMaximumHeight(40);

	this->textbox = new QLineEdit(this);
	this->submit = new QPushButton(this);
	this->h_splitter = new QSplitter(Qt::Horizontal, this);
	this->container = new QHBoxLayout;

	this->h_splitter->setChildrenCollapsible(false);
	this->submit->setText("Filter");

	// On submit pressed or enter pressed
	this->connect(this->submit, SIGNAL(clicked()),
			this, SLOT(on_submit()));
	this->connect(this->textbox, SIGNAL(returnPressed()),
			this, SLOT(on_submit()));

	this->connect(this->textbox, SIGNAL(textChanged(const QString &)),
			this, SLOT(query_highlighting()));

	this->h_splitter->addWidget(this->textbox);
	this->h_splitter->addWidget(this->submit);
	this->container->addWidget(this->h_splitter);
	this->setLayout(container);
}

SearchBox::~SearchBox() {
}

bool SearchBox::check_syntax(QString query_qstring) {
	// Parse string into a vector - also empty vector beforehand
	std::string query_string = query_qstring.toUtf8().constData();
	this->queries = std::vector<Parser::ast::query_struct>();
	return x3::parse(query_string.begin(), query_string.end(), Parser::expression, this->queries);
}

void SearchBox::set_packet_filter(Packet *packet, Parser::ast::query_struct query, bool chain) {
	// If packet is already filtered, ignore it (chains queries rather than treat them as OR)
	if (chain && packet->get_filtered()) {
		return;
	}
	
	// If query is a wildcard, match all 
	if (query.val == "*") {
		packet->set_filtered(false);
		return;
	}

	if (query.key == "protocol") {
		// Protocol is all uppercase so make the argument uppercase
		for (auto & c: query.val) c = std::toupper(c);
		packet->set_filtered( // Set opposite of the return value
				!filter_query(packet->get_protocol(), query)
				);

	} else if (query.key == "contains") {

	} else if (query.key == "length") {
		packet->set_filtered( // Set opposite of the return value
				!filter_query(packet->get_header_len(), query)
				);
	}
}

bool SearchBox::filter_query(std::string field, Parser::ast::query_struct query) {
	// String field overload
	// Parse the operator and the value fields on the packet.field
	// Returns if packet is valid 
	if (query.op == "==") 
		return field == query.val;
	else if (query.op == "!=")
		return field != query.val;

	return false; // Should never be reached
}

bool SearchBox::filter_query(int field, Parser::ast::query_struct query) {
	// Int field overload
	// Parse the operator and the value fields on the packet.field
	// Returns if packet is valid 
	int value = std::stoi(query.val);
	if (query.op == "==")
		return field == value;
	else if (query.op == ">=")
		return field >= value;
	else if (query.op == ">")
		return field > value; 
	else if (query.op == "<=")
		return field <= value;
	else if (query.op == "<")
		return field < value;
	else if (query.op == "!=")
		return field != value;

	return false; // Should never be reached 
}

void SearchBox::on_submit() {
	if (this->queries.size() > 0) {
		bool chain = this->queries.size() > 1; // check if multiple queries are chained
		
		if (chain) {
			// Unfilter all packets
			// This needs to be done as on consecutive chained queries it will ignore filtered packets on previous chained queries
			for (unsigned int i = 0; i < this->stream->size(); ++i)
				(*this->stream)[i]->set_filtered(false);
		}

		// Loop through each packet
		for (unsigned int i = 0; i < this->stream->size(); ++i) {
			// Apply each query onto that packet to set filtered flag
			for (auto const &query: this->queries) {
				this->set_packet_filter((*this->stream)[i], 
						query,
						chain);
			}
		}
		
		// Reload packet table after filtered packets
		if (this->stream->size() > 0)
			emit reload_packets(this->stream);

	} else if (this->textbox->text() == "") {
		// If query is empty, filter nothing
		for (unsigned int i = 0; i < this->stream->size(); ++i)
			(*this->stream)[i]->set_filtered(false);
		
		// Reload table
		if (this->stream->size() > 0)
			emit reload_packets(this->stream);
	}
}

void SearchBox::query_highlighting() {
	QString cur_query = this->textbox->text();
	if (cur_query.length() <= 0) {
		this->textbox->setStyleSheet("background:#ffffff;");
		return;
	}
	if (!this->check_syntax(cur_query))
		this->textbox->setStyleSheet("background:#ffcccb;");
	else
		this->textbox->setStyleSheet("background:#ccff99;");
}
