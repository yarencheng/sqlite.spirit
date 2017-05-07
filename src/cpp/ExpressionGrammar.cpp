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
using spirit::qi::no_case;
using spirit::qi::digit;
using spirit::qi::hex;
using spirit::qi::char_;
using spirit::qi::alnum;
using spirit::qi::alpha;
using phoenix::push_back;


ExpressionGrammar::ExpressionGrammar(): ExpressionGrammar::base_type(_all){

	_all
		= 	_expression
			>>	_binary_oprator
			>>	_expression
		|	_unary_operator
			>> _expression
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
		=	no_case [lit("0x")]
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
		;

	_string_value
		=	lit('\'')
			>>	*(	(	char_ - char_('\'')
					)
				^	lit("\'\'")
				)
			>> lit('\'')
		;

	_blob_value
		=	no_case[
				lit('x')
				>> 	lit('\'')
				>> 	*(	digit
					^ 	hex
					)
				>> 	lit('\'')
			]
		;

	_bind_parameter
		=	lit("?")
			>>	*uint_
		|	(	lit(":")
				|	lit("@")
				|	lit("$")
			)
			>>	+alnum
		;

	_name
		=	+(	alpha
			^	lit("_")
			)
			>>	*(	alnum
				^	lit("_")
				)
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
		=	lit('+')
		|	lit('-')
		|	lit('~')
		|	no_case[lit("NOT")]
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


}



} /* namespace spirit */
} /* namespace sqlite */
} /* namespace arenx */
