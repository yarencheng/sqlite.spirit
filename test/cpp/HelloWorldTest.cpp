/*
 * HelloWorldTest.cpp
 *
 *  Created on: 2017年4月23日
 *      Author: arenx
 */

#include "HelloWorld.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace ::std;
using namespace ::testing;

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Invoke;
using ::testing::_;

namespace arenx { namespace orm { namespace HelloWorldTest {

class HelloWorldTest  : public Test {
protected:

	virtual void SetUp() {
	}

	virtual void TearDown() {
	}
};


TEST_F(HelloWorldTest, hello){


}



}}}
