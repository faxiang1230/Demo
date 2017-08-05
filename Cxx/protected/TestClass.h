/*
 * TestClass.h
 *
 *  Created on: Aug 5, 2017
 *      Author: linux
 */

#ifndef TESTCLASS_H_
#define TESTCLASS_H_

#include <string>
using std::string;
class TestClass{
private:
	string name;
	void setname(const string &mname);
protected:
	string proname;
	void setproname(const string &mname);
public:
	string pubname;
	void setpubname(const string &mname);
};

class driveClass:public TestClass
{
public:
	void drivesetname(string pri,string pro,string pub);
};
#endif /* TESTCLASS_H_ */
