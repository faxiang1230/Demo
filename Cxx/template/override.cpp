/*************************************************************************
    > File Name: override.cpp
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2017年07月05日 星期三 20时05分43秒
 ************************************************************************/

#include<iostream>
using namespace std;
struct box
{
	char marker[40];
	float height;
	float width;
	float length;
	float volume;
};

void display(const box &b){
	cout << b.marker << b.height << b.width << b.length << b.volume << endl;
}
int main(){
	box b = {"test", 1, 2, 3};
	display(b);
	return 0;
}
