/*
 * ExpressionTest.cpp
 *
 *  Created on: 2017年5月5日
 *      Author: arenx
 */

#include "ExpressionGrammar.hpp"

#include <vector>
#include <functional>
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"


using namespace ::std;
using namespace ::testing;

namespace arenx { namespace sqlite { namespace spirit { namespace ExpressionGrammarTest {

MATCHER(isValid, std::string(negation ? "dose not" : "dose") + " match grammar") {
	const string sql = arg;

	auto begin = sql.begin();
	auto end = sql.end();

	static ExpressionGrammar grammar;

	bool match = phrase_parse(begin, end, grammar, ::boost::spirit::ascii::space);

	if (!match || begin != end) {
//		throw runtime_error(boost::str( boost::format("invalid syntax of[%1%] at [%2%]") % sql % string(begin, end) ));
		cout << "failed at [" << string(begin, end) << "]" <<endl;
		return false;
	}

	return true;
}

class ExpressionGrammarTest  : public Test {
protected:

	virtual void SetUp() {
	}

	virtual void TearDown() {
	}
//};

	int testRecursiveMax = 3;
	int testDataLimit = 100; // -1 is no limit

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

	vector<string> aggregateFunctionNames = {
		"AVG",
		"COUNT",
		"GROUP_CONCAT",
		"MAX",
		"MIN",
		"SUM",
		"TOTAL"
	};

	vector<string> typeNames = {
		"INT",
		"INTEGER",
		"TINYINT",
		"SMALLINT",
		"MEDIUMINT",
		"BIGINT",
		"UNSIGNED BIG INT",
		"INT2",
		"INT8",
		"CHARACTER(20)",
		"VARCHAR(255)",
		"VARYING CHARACTER(255)",
		"NCHAR(55)",
		"NATIVE CHARACTER(70)",
		"NVARCHAR(100)",
		"TEXT",
		"CLOB",
		"BLOB",
		"REAL",
		"DOUBLE",
		"DOUBLE PRECISION",
		"FLOAT",
		"NUMERIC",
		"DECIMAL(10,5)",
		"BOOLEAN",
		"DATE",
		"DATETIME"
	};

	vector<string> randomAndTrimVector(const vector<string>& v){
		if (-1 == testDataLimit) {
			return v;
		}

		if (testDataLimit >= (int)v.size()) {
			return v;
		}

		static random_device rd;
		static mt19937 g(rd());

		vector<string> out = v;

		shuffle(out.begin(), out.end(), g);

		out.resize(testDataLimit);

		return out;
	};

	vector<string>  fullNames(int level){

		if (0 >= level) return vector<string>();

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

		results = randomAndTrimVector(results);

		return results;
	};

	vector<string>  literalValues(int level){

		if (0 >= level) return vector<string>();

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

		results = randomAndTrimVector(results);

		return results;
	};

	vector<string>  aggregateFunctions(int level){

		if (0 >= level) return vector<string>();

		vector<string> results;

		for (string& s1: aggregateFunctionNames) {

			results.push_back(s1 + "()");
			results.push_back(s1 + "(*)");

			for (string& s2: expressions(level-1)) {
				results.push_back(s1 + "(" + s2 + ")");
				results.push_back(s1 + "( DISTINCT " + s2 + ")");
				for (string& s3: expressions(level-1)) {
					results.push_back(s1 + "(" + s2 + ", " + s3 + ")");
					results.push_back(s1 + "( DISTINCT " + s2 + ", " + s3 + ")");
				};
			};
		}

		results = randomAndTrimVector(results);

		return results;
	};

	vector<string>  expressions(int level){

		if (0 >= level) return vector<string>();

		vector<string> results;

		for (string& s1: unaryOperators) {
			for (string& s2: literalValues(level-1)) {
				results.push_back(s1 + " " + s2);
			};
		}

		for (string& s1: aggregateFunctions(level-1)) {
			results.push_back(s1);
		}

		for (string& s1: expressions(level-1)) {
			results.push_back("(" + s1 + ")");
			for (string& s2: expressions(level-1)) {
				results.push_back("(" + s1 + ", " + s2 + ")");
				for (string& s3: expressions(level-1)) {
					results.push_back("(" + s1 + ", " + s2 + ", " + s3 + ")");
				}
			}
		}

		for (string& s1: expressions(level-1)) {
			for (string& s2: typeNames) {
				results.push_back("CAST (" + s1 + " AS " + s2 + ")");
			}
		}

		for (string& s1: literalValues(level-1)) {
			results.push_back(s1);
		}

		for (string& s1: bindParameters) {
			results.push_back(s1);
		}

		for (string& s1: fullNames(level-1)) {
			results.push_back(s1);
		}

		results = randomAndTrimVector(results);

		return results;
	};

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
	auto testCase = literalValues(testRecursiveMax);
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
	auto testCase = fullNames(testRecursiveMax);
	int passCount = 0;
	for (string& s: testCase) {
		ASSERT_THAT(s, isValid()) << passCount << "/" << (int)testCase.size();
		passCount++;
	}
}

TEST_F(ExpressionGrammarTest, _expression){
	auto testCase = expressions(testRecursiveMax);
	int passCount = 0;
	for (string& s: testCase) {
		ASSERT_THAT(s, isValid()) << passCount << "/" << (int)testCase.size();
		passCount++;
	}
}

TEST_F(ExpressionGrammarTest, _aggregate_function){
	auto testCase = aggregateFunctions(testRecursiveMax);
	int passCount = 0;
	for (string& s: testCase) {
		ASSERT_THAT(s, isValid()) << passCount << "/" << (int)testCase.size();
		passCount++;
	}
}



}}}}
