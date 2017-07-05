/*************************************************************************
    > File Name: template.cpp
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2017年07月05日 星期三 18时56分16秒
 ************************************************************************/

#include<iostream>
#include <string>
using namespace std;
struct box{
	char maker[40];
	int height;
	int width;
	int length;
	int volume;
};
template <class T> T Max(T &a, T &b);
template <> box Max(box &a, box &b){
	if(a.volume == 0) {
		a.volume = a.height * a.width * a.length;
	}
	if(b.volume == 0) {
		b.volume = b.height * b.width * b.length;
	}
	return a.volume > b.volume?a:b;
}
template <typename T> void Swap(T &a, T &b){
	T temp;
	temp = a;
	a = b;
	b = temp;
}

int main(){
	int i = 10, j =20;
	string x = "123";
	string y = "adb";
	box a = {"TestA", 1, 2, 3};
	box b = {"TestB", 4, 5, 6};
	cout << "before Swap: i:" << i << " j:" << j << endl;
	Swap(i, j);
	cout << "After Swap: i:" << i << " j:" << j << endl;

	cout << "before Swap: x:" << x << " y:" << y << endl;
	Swap(x, y);
	cout << "After Swap: x:" << x << " y:" << y << endl;
	
	cout << Max(a,b).maker << endl;
	return 0;
}
