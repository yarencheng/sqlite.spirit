/*
 * ExpressionTest.cpp
 *
 *  Created on: 2017年5月5日
 *      Author: arenx
 */

#include "Expression.hpp"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace ::std;
using namespace ::testing;

namespace arenx { namespace sqlite { namespace spirit { namespace HelloWorldTest {

class ExpressionTest  : public Test {
protected:

	virtual void SetUp() {
	}

	virtual void TearDown() {
	}
};


TEST_F(ExpressionTest, hello){

	Expression expression1 = Expression::parse("111");

	cout << "expression1=" << expression1._aaa << endl;

	Expression expression2 = Expression::parse("aaa");

	cout << "expression2=" << expression2._aaa << endl;
}



}}}}
