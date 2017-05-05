/*
 * Expression.cpp
 *
 *  Created on: 2017年5月5日
 *      Author: arenx
 */

#include "Expression.hpp"

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/support_utree.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/phoenix.hpp>
#include <boost/bind.hpp>

namespace arenx {
namespace sqlite {
namespace spirit {

using namespace ::std;
using namespace ::boost::spirit;
using spirit::qi::alnum;
using spirit::qi::lit;
using spirit::qi::_1;


Expression Expression::parse(const string& sql){
	utree ut;

	std::string::const_iterator iter = sql.begin();
	std::string::const_iterator end = sql.end();

	Expression expression;
	ExpressionGrammar grammar(expression);

	bool match = phrase_parse(iter, end, grammar, ascii::space);

	cout << "match=" << match << endl;
	cout << "std::string(iter, end)=" << std::string(iter, end) << endl;

	return expression;
}

void fn1(int const& s){
	cout << "fn1() s=" << s << endl;
}

void fn2(string const& s){
	cout << "fn2() s=" << s << endl;
}

Expression::ExpressionGrammar::ExpressionGrammar(Expression& ex): ExpressionGrammar::base_type(_expression){

	_expression
		=	int_										[boost::phoenix::bind(&fn1, _1)]
		|	boost::spirit::as_string[lit("aaa")]		[boost::phoenix::ref(ex._aaa) = _1]
		;

	BOOST_SPIRIT_DEBUG_NODE(_expression);
}

Expression::Expression() {
	// TODO Auto-generated constructor stub

}

} /* namespace spirit */
} /* namespace sqlite */
} /* namespace arenx */
