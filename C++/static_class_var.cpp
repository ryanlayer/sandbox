#include <iostream>
using namespace std;

class has_static
{
	public:
		static int N;
		int id;

		has_static(int _id)
		{
			id = _id;
		}

		int get_static() {return id+N;}

};

int has_static::N = 0;

int main()
{
	has_static::N = 5;
	cout << has_static::N <<endl;

	has_static a(3);
	has_static b(7);
	cout << a.get_static() << endl;
	cout << b.get_static() << endl;
}
