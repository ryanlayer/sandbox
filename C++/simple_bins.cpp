#include <vector>
#include <cstdio>

using namespace std;

template <class T>
class SimpleElement
{
	public:
		int start, end;
		T value;
};


template <class T>
class SimpleBins
{
	vector<SimpleElement<T> > v;
	public:

	SimpleBins();
};

int main()
{
	SimpleBins<int> sb;
}
