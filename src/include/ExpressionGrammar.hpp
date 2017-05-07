/*
 * ExpressionGrammar.hpp
 *
 *  Created on: 2017年5月5日
 *      Author: arenx
 */

#pragma once

#include <vector>
#include <boost/spirit/include/qi.hpp>

namespace arenx {
namespace sqlite {
namespace spirit {

struct ExpressionGrammar : boost::spirit::qi::grammar<std::string::const_iterator, boost::spirit::ascii::space_type>
{
public:
	ExpressionGrammar();
	virtual ~ExpressionGrammar() = default;

private:
	boost::spirit::qi::rule<std::string::const_iterator, boost::spirit::ascii::space_type>
		_all
		, _expression
		, _literal_value
		, _number_value
		, _string_value
		, _blob_value
		, _bind_parameter
		, _name
		, _full_name
		, _unary_operator
		, _binary_oprator
		;
};

} /* namespace spirit */
} /* namespace sqlite */
} /* namespace arenx */

