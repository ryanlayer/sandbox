#include <iostream>
using namespace std;

template <class T>
class X {
	public:
	T a,b;
	X(T i, T j);
	X();
};

template <class T>
X<T>::X(T i, T j) {
		a = i;
		b = j;
}

template <class T>
X<T>::X(){
	;
}



template <class T>
class mypair {
	X<T> c;
	T a, b;
	public:
	mypair (T first, T second) {
		//x=X(1,1);
		a=first;
		b=second;
	}
	T getmax ();
};

template <class T>
T mypair<T>::getmax ()
{
	T retval;
	retval = a>b? a : b;
	return retval;
}


int main () {
	X<int> x(1,1); 
	mypair <int> myobject (100, 75);
	cout << myobject.getmax();
	return 0;
}
