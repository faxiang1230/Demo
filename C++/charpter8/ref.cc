/*
 * ref.cc
 *
 *  Created on: 2017年5月6日
 *      Author: celine
 */
#include "ref.h"
using namespace std;


void reftest(){
	int origin;
	int &reforigin = origin;
	cout << "origin addr: " << &origin <<endl;
	cout << "reforigin addr: " << &reforigin << endl;

	free_throws ft = {"Mike", 5, 1, 3.0};
	set_pc(ft);
	cout << "ft: " << ft.percent << endl;

	string input,copy,result;
	cout << "Enter a String:  ";
	getline(cin, input);
	copy = input;
	cout << "enter the string:  " << input << endl;
	result = version1(input, "****");
	cout << "enhanced result:" << result << endl;
	cout << "origin input:" << input << endl;

	result = version2(input, "****");
	cout << "enhanced result:" << result << endl;
	cout << "origin input:" << input << endl;

	result = version3(copy, "****");
	cout << "enhanced result:" << result << endl;
	cout << "origin input:" << copy << endl;
	result = "xxxx";
	cout << "enhanced result:" << result << endl;
}


void set_pc(free_throws &ft){
	if(ft.attempts != 0) {
		ft.percent = 100.0f * float(ft.made)/float(ft.attempts);
	} else {
		ft.percent = 0;
	}
}

string version1(const string &s1, const string &s2){
	string temp;
	temp = s1 + s2 + s1;
	return temp;
}
const string & version2(string &s1, const string &s2){
	s1 = s1 + s2 + s1;
	return s1;
}
const string & version3(string &s1, const string &s2){
	string temp;
	temp = s1 + s2 + s1;
	return temp;
}
