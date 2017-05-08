/*
 * Expression.cpp
 *
 *  Created on: 2017年5月5日
 *      Author: arenx
 */

#include "ExpressionGrammar.hpp"

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/support_utree.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/phoenix.hpp>
#include <boost/bind.hpp>

namespace arenx {
namespace sqlite {
namespace spirit {

using namespace ::boost::spirit;
namespace phoenix = boost::phoenix;
using spirit::lit;
using spirit::_1;
using spirit::uint_;
using spirit::as_string;
using spirit::lexeme;
using spirit::qi::no_case;
using spirit::qi::digit;
using spirit::qi::hex;
using spirit::qi::char_;
using spirit::qi::alnum;
using spirit::qi::alpha;
using phoenix::push_back;


ExpressionGrammar::ExpressionGrammar(): ExpressionGrammar::base_type(_all){

	_all
		= 	_unary_operator
			>> _all
		|	_aggregate_function
		|	_expression
			>>	*(	_binary_oprator
					>>	_all
				)
		|	_expression
			>>	lit("ISNULL")
		|	_expression
		;


	_expression
		=	_literal_value
		|	_bind_parameter
		|	_full_name
		;

	_literal_value
		=	_number_value
		|	_string_value
		|	_blob_value
		|	no_case[lit("NULL")]
		;

	_number_value
		=	lexeme[
				no_case [lit("0x")]
				>>	+hex
			|	-(	lit('+')
				| 	lit('-')
				)
				>>	(	(	+digit
							>>	-(	lit('.')
									>>	*digit
								)
						)
					|	(	lit('.')
							>>	+digit
						)
					)
				>>	-(	no_case [lit('E')]
						>>	-(	lit('+')
							|	lit('-')
							)
						>>	+digit
					)
			]
		;

	_string_value
		=	lexeme[
				lit('\'')
				>>	*(	(	char_ - char_('\'')
						)
					^	lit("\'\'")
					)
				>> lit('\'')
			]
		;

	_blob_value
		=	lexeme[
				no_case[
					lit('x')
					>> 	lit('\'')
					>> 	*(	digit
						^ 	hex
						)
					>> 	lit('\'')
				]
			]
		;

	_bind_parameter
		=	lexeme[
				lit("?")
				>>	*uint_
			|	(	lit(":")
					|	lit("@")
					|	lit("$")
				)
				>>	+alnum
			]
		;

	_name
		=	lexeme[
				+(	alpha
				^	lit("_")
				)
				>>	*(	alnum
					^	lit("_")
					)
			]
		;

	_full_name
		=	-(	_name
				>>	lit(".")
			)
			>>	-(	_name
					>>	lit(".")
				)
			>>	_name
		;

	_unary_operator
		=	lexeme[
				lit('+')
			|	lit('-')
			|	lit('~')
			|	no_case[lit("NOT")]
			]
		;

	_binary_oprator
		=	no_case [
					lit("REGEXP")
				| 	lit("MATCH")
				| 	lit("GLOB")
				| 	lit("LIKE")
				| 	lit("AND")
				| 	lit("OR")
				| 	lit("IN")
				| 	(lit("IS")
					>> -lit("NOT"))
				|	lit("||")
				| 	lit("<<")
				| 	lit(">>")
				| 	lit("<=")
				| 	lit(">=")
				| 	lit("==")
				| 	lit("!=")
				| 	lit("<>")
				| 	lit('*')
				| 	lit('/')
				| 	lit('%')
				| 	lit('+')
				| 	lit('-')
				| 	lit('&')
				| 	lit('|')
				| 	lit('<')
				| 	lit('>')
				| 	lit('=')
			]
		;

	_aggregate_function
		=	_aggregate_function_name
			>>	lit("(")
			>>	-(
					-no_case[lit("DISTINCT")]
					>>	_all
					>>	*(	lit(",")
							>>	_all
						)
				|	lit("*")
				)
			>>	lit(")")
		;

	_aggregate_function_name
		=	no_case[
				lit("AVG")
			|	lit("COUNT")
			|	lit("GROUP_CONCAT")
			|	lit("MAX")
			|	lit("MIN")
			|	lit("SUM")
			|	lit("TOTAL")
			]
		;

	_type_name
		=	no_case[
				lit("UNSIGNED")
				>>	+lit(" ")
				>>	lit("BIG")
				>>	+lit(" ")
				>>	lit("INT")
			|	lit("DOUBLE")
				>>	-lit("PRECISION")
			|	(	-(	lit("VARYING")
					|	lit("NATIVE ")
					)
					>>	lit("CHARACTER")
				|	lit("NVARCHAR")
				|	lit("VARCHAR")
				|	lit("NCHAR")
				)
				>>	lit("(")
				>>	+digit
				>>	lit(")")
			|	lit("DECIMAL")
				>>	lit("(")
				>>	+digit
				>>	lit(",")
				>>	+digit
				>>	lit(")")
			|	lit("MEDIUMINT")
			|	lit("SMALLINT")
			|	lit("DATETIME")
			|	lit("INTEGER")
			|	lit("TINYINT")
			|	lit("NUMERIC")
			|	lit("BOOLEAN")
			|	lit("BIGINT")
			|	lit("FLOAT")
			|	lit("INT2")
			|	lit("INT8")
			|	lit("TEXT")
			|	lit("BLOB")
			|	lit("REAL")
			|	lit("DATE")
			|	lit("INT")
			]
		;

	BOOST_SPIRIT_DEBUG_NODE(_all);
	BOOST_SPIRIT_DEBUG_NODE(_expression);
	BOOST_SPIRIT_DEBUG_NODE(_number_value);
	BOOST_SPIRIT_DEBUG_NODE(_string_value);
	BOOST_SPIRIT_DEBUG_NODE(_blob_value);
	BOOST_SPIRIT_DEBUG_NODE(_literal_value);
	BOOST_SPIRIT_DEBUG_NODE(_bind_parameter);
	BOOST_SPIRIT_DEBUG_NODE(_name);
	BOOST_SPIRIT_DEBUG_NODE(_full_name);
	BOOST_SPIRIT_DEBUG_NODE(_unary_operator);
	BOOST_SPIRIT_DEBUG_NODE(_binary_oprator);
	BOOST_SPIRIT_DEBUG_NODE(_aggregate_function);
	BOOST_SPIRIT_DEBUG_NODE(_aggregate_function_name);
	BOOST_SPIRIT_DEBUG_NODE(_type_name);

}



} /* namespace spirit */
} /* namespace sqlite */
} /* namespace arenx */
