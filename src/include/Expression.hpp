/*
 * Expression.hpp
 *
 *  Created on: 2017年5月5日
 *      Author: arenx
 */

#pragma once

#include <boost/spirit/include/qi.hpp>

namespace arenx {
namespace sqlite {
namespace spirit {

class Expression {
public:
	static Expression parse(const std::string& sql);

	Expression();
	virtual ~Expression() = default;

private:
	struct ExpressionGrammar : boost::spirit::qi::grammar<std::string::const_iterator, boost::spirit::ascii::space_type>
	{
	public:
		boost::spirit::qi::rule<std::string::const_iterator, boost::spirit::ascii::space_type> _expression;
		ExpressionGrammar(Expression& ex);
		virtual ~ExpressionGrammar() = default;
	};
public:
	std::string _aaa;
};

} /* namespace spirit */
} /* namespace sqlite */
} /* namespace arenx */
