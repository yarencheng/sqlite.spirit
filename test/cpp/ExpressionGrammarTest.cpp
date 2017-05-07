/*
 * ExpressionTest.cpp
 *
 *  Created on: 2017年5月5日
 *      Author: arenx
 */

#include "ExpressionGrammar.hpp"

#include <vector>
#include "gtest/gtest.h"
#include "gmock/gmock.h"


using namespace ::std;
using namespace ::testing;

namespace arenx { namespace sqlite { namespace spirit { namespace ExpressionGrammarTest {

class ExpressionGrammarTest  : public Test {
protected:

	virtual void SetUp() {
	}

	virtual void TearDown() {
	}
};

MATCHER(isValid, std::string(negation ? "dose not" : "dose") + " match grammar") {
	const string sql = arg;

	auto begin = sql.begin();
	auto end = sql.end();

	ExpressionGrammar grammar;

	bool match = phrase_parse(begin, end, grammar, ::boost::spirit::ascii::space);

	if (!match || begin != end) {
//		throw runtime_error(boost::str( boost::format("invalid syntax of[%1%] at [%2%]") % sql % string(begin, end) ));
		cout << "failed at [" << string(begin, end) << "]" <<endl;
		return false;
	}

	return true;
}

vector<string> numberValues = {
	"1234567890",
	"111",
	"+111",
	"-111",
	"111.",
	"111.111",
	"111E111",
	"111E+111",
	"111E-111",
	"0x111"
};

vector<string> stringValues = {
	"\'\'",
	"\'a\'",
	"\'aa\'",
	"\'aaa\'",
	"\'a\'\'aa\'"
};

vector<string> bindParameters = {
	"?",
	"?123",
	"@123abc",
	":123abc",
	"$123abc",
};

vector<string> blobValues = {
	"x\'\'",
	"X\'\'",
	"x\'1234567890\'",
	"x\'abcdef\'",
	"x\'ABCDFE\'",
};

vector<string> names = {
	"a",
	"_",
	"aa",
	"a1",
	"a_",
	"a_1",
};

vector<string> unaryOperators = {
	"+",
	"-",
	"~",
	"NOT"
};

vector<string> binarayOperators = {
	"REGEXP",
	"MATCH",
	"GLOB",
	"LIKE",
	"AND",
	"OR",
	"IN",
	"IS",
	"IS NOT",
	"||",
	"<<",
	">>",
	"<=",
	">=",
	"==",
	"!=",
	"<>",
	"*",
	"/",
	"%",
	"+",
	"-",
	"&",
	"|",
	"<",
	">",
	"=",
};

auto fullNames = []{
	static vector<string> results;

	if (!results.empty()) return results;

	for (string& s1: names) {
		results.push_back(s1);
		for (string& s2: names) {
			results.push_back(s1 + "." + s2);
			for (string& s3: names) {
				results.push_back(s1 + "." + s2 + "." + s3);
			}
		}
	}

	return results;
};

auto literalValues = []{
	static vector<string> results;

	if (!results.empty()) return results;

	for (string& s1: numberValues) {
		results.push_back(s1);
	}

	for (string& s1: stringValues) {
		results.push_back(s1);
	}

	for (string& s1: blobValues) {
		results.push_back(s1);
	}

	results.push_back("NULL");

	return results;
};

auto expressions = []{
	static vector<string> results;

	if (!results.empty()) return results;

	for (string& s1: literalValues()) {
		results.push_back(s1);
	}

	for (string& s1: bindParameters) {
		results.push_back(s1);
	}

	for (string& s1: fullNames()) {
		results.push_back(s1);
	}

	return results;
};

auto all = []{
	static vector<string> results;

	if (!results.empty()) return results;

	for (string& s1: expressions()) {
		results.push_back(s1);
	}

	for (string& s1: unaryOperators) {
		for (string& s2: expressions()) {
			results.push_back(s1 + " " + s2);
		}
	}

	for (string& s1: expressions()) {
		for (string& s2: binarayOperators) {
			for (string& s3: expressions()) {
				results.push_back(s1 + " " + s2 + " " + s3);
			}
		}
	}

	return results;
};

TEST_F(ExpressionGrammarTest, _number_value){
	auto& testCase = numberValues;
	int passCount = 0;
	for (string& s: testCase) {
		ASSERT_THAT(s, isValid()) << passCount << "/" << (int)testCase.size();
		passCount++;
	}
}

TEST_F(ExpressionGrammarTest, _string_value){
	auto& testCase = stringValues;
	int passCount = 0;
	for (string& s: testCase) {
		ASSERT_THAT(s, isValid()) << passCount << "/" << (int)testCase.size();
		passCount++;
	}
}

TEST_F(ExpressionGrammarTest, _blob_value){
	auto& testCase = blobValues;
	int passCount = 0;
	for (string& s: testCase) {
		ASSERT_THAT(s, isValid()) << passCount << "/" << (int)testCase.size();
		passCount++;
	}
}

TEST_F(ExpressionGrammarTest, _literal_value){
	auto testCase = literalValues();
	int passCount = 0;
	for (string& s: testCase) {
		ASSERT_THAT(s, isValid()) << passCount << "/" << (int)testCase.size();
		passCount++;
	}
}

TEST_F(ExpressionGrammarTest, _bind_parameter){
	auto& testCase = bindParameters;
	int passCount = 0;
	for (string& s: testCase) {
		ASSERT_THAT(s, isValid()) << passCount << "/" << (int)testCase.size();
		passCount++;
	}
}

TEST_F(ExpressionGrammarTest, _name){
	auto& testCase = names;
	int passCount = 0;
	for (string& s: testCase) {
		ASSERT_THAT(s, isValid()) << passCount << "/" << (int)testCase.size();
		passCount++;
	}
}

TEST_F(ExpressionGrammarTest, _full_name){
	auto testCase = fullNames();
	int passCount = 0;
	for (string& s: testCase) {
		ASSERT_THAT(s, isValid()) << passCount << "/" << (int)testCase.size();
		passCount++;
	}
}

TEST_F(ExpressionGrammarTest, _expression){
	auto testCase = expressions();
	int passCount = 0;
	for (string& s: testCase) {
		ASSERT_THAT(s, isValid()) << passCount << "/" << (int)testCase.size();
		passCount++;
	}
}

TEST_F(ExpressionGrammarTest, _all){
	auto testCase = all();
	int passCount = 0;
	for (string& s: testCase) {
		ASSERT_THAT(s, isValid()) << passCount << "/" << (int)testCase.size();
		passCount++;
	}

//	EXPECT_THAT("@123abc REGEXP 111.", isValid());
//	EXPECT_THAT("@123abc AND 111.", isValid());
//	EXPECT_THAT("@123abc + 111.", isValid());
}



}}}}
