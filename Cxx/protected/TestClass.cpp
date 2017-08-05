/*
 * TestClass.cpp
 *
 *  Created on: Aug 5, 2017
 *      Author: linux
 */
#include "TestClass.h"
void TestClass::setname(const string &mname)
{
	name = mname;
}

void TestClass::setproname(const string &mname)
{
	proname = mname;
}
void TestClass::setpubname(const string &mname)
{
	pubname = mname;
}
void driveClass::drivesetname(string pri,string pro,string pub){
	//setname(pri);
	setproname(pro);
	setpubname(pub);
}

