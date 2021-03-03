#ifndef SEARCHBOX_H 
#define SEARCHBOX_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QHBoxLayout>
#include <QString>
#include <QStringList>
#include <QAction>

#include <iostream>
#include <string>
#include <vector>
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/algorithm/string.hpp>

#include "capture/packet.hpp"

namespace x3 = boost::spirit::x3;

// Inline prevents being linked in main object file at compilation
namespace Parser {
	namespace ast {
		struct query_struct {
			std::string key, op, val;
		};
		using boost::fusion::operator<<;
	}

	template <typename T> static auto as (auto p) {
		// Cast x3 object p as T
		return x3::rule<struct tag, T> {"as"} = p;
	}

	// Unused, was for attaching multiple queries
	// inline static const auto concat = x3::string("||") | x3::string("&&");

	inline static const auto string_key = x3::string("protocol")
		| x3::string("contains");
	inline static const auto string_operator = x3::string("==") | x3::string("!=");
	inline static const auto string_value = as<std::string>(+x3::char_("a-zA-Z0-9\*"));

	inline static const auto int_key = x3::string("length");
	inline static const auto int_operator = x3::string(">=") | x3::string("<=") |
		x3::string("<") | x3::string("==") | 
		x3::string(">") | x3::string("!=");
	inline static const auto int_value = as<std::string>(+x3::char_("0-9\*"));

	inline x3::rule<class query, ast::query_struct> const query = "query";
	inline static const auto query_def = 
		(string_key >> x3::omit[-x3::ascii::space] >> string_operator >> x3::omit[-x3::ascii::space] >> string_value) | 
		(int_key >> x3::omit[-x3::ascii::space] >> int_operator >> x3::omit[-x3::ascii::space] >> int_value);

	inline x3::rule<class expression, std::vector<ast::query_struct>> const expression = "expression";
	inline static const auto expression_def = query % x3::omit[-x3::ascii::space >> "&&" >> -x3::ascii::space];

	BOOST_SPIRIT_DEFINE(query, expression);
}

BOOST_FUSION_ADAPT_STRUCT(Parser::ast::query_struct,
		key, op, val);

class SearchBox : public QWidget {
	Q_OBJECT;

	public:
		SearchBox(PacketStream *stream, QWidget *parent = nullptr);
		~SearchBox();
		
		// Methods
		bool check_syntax(QString query_qstring);
		static void set_packet_filter(Packet *packet, Parser::ast::query_struct query, bool chain=false);

		static bool filter_query(int field, Parser::ast::query_struct query);
		static bool filter_query(std::string field, Parser::ast::query_struct query);

	private:
		// Attributes 
		QLineEdit *textbox;
		QPushButton *submit;
		QSplitter *h_splitter;
		QHBoxLayout *container;

		std::vector<Parser::ast::query_struct> queries;
		PacketStream *stream;
	
	signals:
		void reload_packets(PacketStream *stream);

	public slots:
		void on_submit();
		void query_highlighting();
};

#endif // SEARCHBOX_H
