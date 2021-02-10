#include "searchbox.hpp"

SearchBox::SearchBox(QWidget *parent) : QWidget(parent) {

	this->setMaximumHeight(40);

	this->textbox = new QLineEdit(this);
	this->submit = new QPushButton(this);
	this->h_splitter = new QSplitter(Qt::Horizontal, this);
	this->container = new QHBoxLayout;

	this->h_splitter->setChildrenCollapsible(false);
	this->submit->setText("Filter");

	this->connect(this->submit, SIGNAL(clicked()),
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

void SearchBox::on_submit() {
	//QString query = this->textbox->text();
	//QStringList q = query.split(" ", QString::SkipEmptyParts);
	//std::cout << "Query submitted: [";
	//for (int i = 0; i < q.size(); ++i) {
	//    std::cout << "\"" << q[i].toUtf8().constData() << "\", ";
	//}
	//std::cout << "]" << std::endl;
	
	if (this->queries.size() > 0) {
		std::cout << std::endl << "Success" << std::endl;
		for (auto const &i : this->queries) 
			std::cout << "\t[QUERY]: " << i.key << " | " << i.op << " | " << i.val << std::endl;
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
