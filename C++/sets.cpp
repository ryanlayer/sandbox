// constructing sets
#include <iostream>
#include <set>
using namespace std;

bool fncomp (int lhs, int rhs)
{
	return lhs<rhs;
}

struct classcomp {
	bool operator() (const int& lhs, const int& rhs) const {
		return lhs<rhs;
	}
};

int main ()
{
	set<int> first;                           // empty set of ints

	int myints[]= {10,20,30,40,50};
	set<int> second (myints,myints+5);        // pointers used as iterators

	set<int> third (second);                  // a copy of second

	set<int> fourth (second.begin(), second.end());  // iteratorctor.

	set<int,classcomp> fifth;                 // class as Compare

	bool(*fn_pt)(int,int) = fncomp;
	set<int,bool(*)(int,int)> sixth (fn_pt);  // function pointer as Compare

	return 0;
}

