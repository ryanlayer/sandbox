#include <iostream>
using namespace std;

class has_static
{
	public:
		static int N;
		int id;

		int get_static() {return id+N;}

		virtual void virtual_test();


};
int has_static::N = 0;

class A_has_static : public has_static
{
	public:
		A_has_static(int _id) {
			id = _id + 100;
		}

		void virtual_test() {
			++id;
		}

};


class B_has_static : public has_static
{
	public:
		B_has_static(int _id) {
			id = _id + 10;
		}

		void virtual_test() {
			--id;
		}

};


int main()
{
	has_static::N = 5;

	cout << has_static::N <<endl;

	A_has_static a(3);
	A_has_static b(7);
	B_has_static c(3);
	B_has_static d(7);
	cout << a.get_static() << endl;
	cout << b.get_static() << endl;
	cout << c.get_static() << endl;
	cout << d.get_static() << endl;
}
