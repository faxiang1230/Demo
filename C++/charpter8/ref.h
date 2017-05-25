/*
 * ref.h
 *
 *  Created on: 2017年5月6日
 *      Author: celine
 */

#ifndef REF_H_
#define REF_H_
#include <iostream>
#include <string>
#include "struct_example.h"
using namespace std;
void set_pc(free_throws &);
string version1(const string &s1, const string &s2);
const string & version2(string &s1, const string &s2);
const string & version3(string &s1, const string &s2);


#endif /* REF_H_ */
