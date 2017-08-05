//============================================================================
// Name        : test.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;
#include "TestClass.h"
int main() {
	TestClass t;
	//t.setname("private");
	//t.setproname("protected");
	t.setpubname("public");
	//cout << t.name << endl;
	cout << t.pubname << endl;
	//cout << t.proname << endl;
	driveClass d;
	d.drivesetname("pri","pro","pub");
	cout << d.pubname << endl;
	//cout << d.proname << endl;
	return 0;
}
