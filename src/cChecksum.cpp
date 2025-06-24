/*
	Simple Fletcher-32 implementation from wikipedia
*/

#include "cChecksum.h"
using namespace std;

cChecksum::cChecksum()
{
	clear();
}

cChecksum::cChecksum(const cChecksum &b)
{
	*this = b;
}

cChecksum::~cChecksum()
{
}



// members
void cChecksum::clear()
{
	_fl1 = 0xffff;
	_fl2 = 0xffff;
	_i = 0;
}

void cChecksum::print() const
{
	COUT_SET
	cout << *this;
}

ostream& operator<<(std::ostream& ostr, const cChecksum& b)
{
	OSTR_SET
	ostr << b.get() << endl;
	return ostr;
}

void cChecksum::set(uint fl1, uint fl2, uint i)
{
	clear();
	_fl1 = fl1;
	_fl2 = fl2;
	_i = i;
}

ulint cChecksum::get() const
{
	/* Second reduction step to reduce sums to 16 bits */
	uint fl1 = (_fl1 & 0xffff) + (_fl1 >> 16);
	uint fl2 = (_fl2 & 0xffff) + (_fl2 >> 16);
	ulint r = fl2 << 16 | fl1;
	return r;
}

void cChecksum::add(uint v)
{
	if(_i > CS_MAX_SUMS)
	{
		_i = 0;
		_fl1 = (_fl1 & 0xffff) + (_fl1 >> 16);
		_fl2 = (_fl2 & 0xffff) + (_fl2 >> 16);
	}
	_fl1 += v;
	_fl2 += _fl1;
	_i++;
}

void cChecksum::append(const cChecksum &b)
{
	// reduce and loop reset on append()
	_fl1 = (_fl1 & 0xffff) + (_fl1 >> 16);
	_fl2 = (_fl2 & 0xffff) + (_fl2 >> 16);
	_fl1 += b._fl1;
	_fl2 += _fl1;
	_i = CS_MAX_SUMS+1;
}



// operators
cChecksum cChecksum::operator+(const cChecksum& b)
{
	cChecksum r(*this);
	r.append(b);
	return(r);
}

cChecksum& cChecksum::operator+=(const cChecksum &b)
{
	this->append(b);
	return(*this);
}

cChecksum& cChecksum::operator=(const cChecksum &b)
{
	if(this == &b) { return(*this); }	// check for self-assignment
	set(b._fl1, b._fl2, b._i);
	return(*this);
}

bool cChecksum::operator==(const cChecksum &b)
{
	if(get() == b.get()) return true; else return false;
}

bool cChecksum::operator!=(const cChecksum &b)
{
	if(get() != b.get()) return true; else return false;
}
