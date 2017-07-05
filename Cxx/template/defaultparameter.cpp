/*************************************************************************
    > File Name: defaultparameter.cpp
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2017年07月05日 星期三 20时02分41秒
 ************************************************************************/

#include<iostream>
using namespace std;
void song(const char *name = "Mo.test", int times = 1);
int main(){
	song();
	return 0;
}

void song(const char *name, int times){
	cout << name[times];
}
