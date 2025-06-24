#ifndef _cGenericType
#define _cGenericType

/*
Each elementary type has getter/setter:
  - set()
  - get()

*size()	// size in Bytes
*data()	// returns data ptr

operator<<	// is it worth to make serialization thru <<?

operator+	// append
operator-	// diff
operator=	// assign
lerp()
*/

#include "core_defs.h"
#include "core_classes.h"

template <class T>
class cGenericType
{
	public:
		void set(T v) {d = v;}
		T get() const {return d;}
		size_t size() const {return sizeof(d);}
		void* data() const {return (void*)&d;}
		friend std::ostream& operator<<(std::ostream& ostr, const cGenericType<T> &b)
		{
			OSTR_SET
			ostr << d;
			return ostr;
		}
	private:
		T d;
};

int main(void)
{
	cGenericType<string> v;
	v.set(std::string("huypizdajigurda"));
	auto var = v.get();
	cout << var << endl;
	cout << v.data() << endl;
	cout << v.size() << endl;
}

#endif