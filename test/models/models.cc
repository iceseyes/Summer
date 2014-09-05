/*
 * Summer - /Summer/test/models/models.cc
 * author: Massimo Bianchi 2014
 */

#include <summer/data/Model.h>

#include <gtest/gtest.h>

#include <string>

using namespace std;
using namespace summer::data;

class ModelTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		m["one"] = 5;
	}

	Model m;
};

TEST_F(ModelTest, AnyCastCheck) {
	int a = boost::any_cast<int>(m["one"]);
	ASSERT_EQ(5, a) << "key <one> must by integer 5.";
}

TEST_F(ModelTest, SimpleGet) {
	ASSERT_EQ(5, m.get<int>("one")) << "key <one> must by integer 5.";

	int a = m.get<int>("one");
	ASSERT_EQ(5, a) << "key <one> must by integer 5.";

	ASSERT_THROW(
		std::string b = m.get<std::string>("one"),
		boost::bad_any_cast
	) << "key <one> is integer not string! Bad_any_cast expected.";
}

TEST_F(ModelTest, AdvGet) {
	m.get<int>("one") = 10;
	ASSERT_EQ(10, m.get<int>("one")) << "now, key <one> must by integer 10.";

	int &a = m.get<int>("one");
	a = 150;
	ASSERT_EQ(150, m.get<int>("one")) << "now, key <one> must by integer 10.";

	m["one"] = std::string("two");
	ASSERT_EQ("two", m.get<std::string>("one")) << "now, key <one> must by string <two>.";

	ASSERT_THROW(
		m.get<int>("one"),
		boost::bad_any_cast
	) << "key <one> is string not integer! Bad_any_cast expected.";
}
