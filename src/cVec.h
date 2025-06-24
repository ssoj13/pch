/// templated vector3
// TODO: rewrite for dynamic size vectors with "vector<T>" storage
#ifndef _cVec
#define _cVec
#include "core_defs.h"
#include "cUtil.h"
#include "cMatrix.h"
using namespace std;

template <class T> class cVec: public cUtil
{
	public:
		cVec();
		cVec(T, T, T);
		cVec(const cVec&);
		~cVec();

		void clear();
		int dataSize() const;		// dataSize() = size of data in Bytes
		//uint packedSize() const;	// size of data + headers + other for packing, made for compatibility
		uint checksum() const;
		T x() const;
		T y() const;
		T z() const;
		T length() const;
		void normalize();
		void zero();
		void set(T, T, T);
		void setx(T);
		void sety(T);
		void setz(T);
		void lerp(const cVec&, double); 		// lerp and store vector inside
		cVec& operator=(const cVec&);
		cVec operator+(const cVec&) const;
		cVec& operator+=(cVec&);
		cVec operator-(const cVec&) const;
		cVec operator*(const double) const;		// scale
		T operator*(const cVec&) const;			// dot
		cVec& operator*=(const cMatrix&);
		cVec operator*(const cMatrix&) const;	// matrix transform
		cVec operator^(const cVec&) const;		// cross
		void print() const;
		int write(const Byte*, const cVec&) const;
		int read(const Byte*, cVec&) const;
		// the syntax of this "ostream operator<<" is a total bulshit, but seems it works...
		friend std::ostream& operator<<(std::ostream& ostr, const cVec<T> &b)
		{
			OSTR_SET
			ostr << "[" << b.x() << ", " << b.y() << ", " << b.z() << "]";
			return ostr;
		}
	private:
		T _x, _y, _z;
};

//--- Constructors
template <class T>
cVec<T>::cVec()
{
	clear();
}

template <class T>
cVec<T>::cVec(T x, T y, T z)
{
	set( (T)x, (T)y, (T)z );
}

template <class T>
cVec<T>::cVec(const cVec<T> &v)
{
	_x = v._x;
	_y = v._y;
	_z = v._z;
}

template <class T>
cVec<T>::~cVec() {}

//--- member funcs
template <class T>
void cVec<T>::clear()
{
	_x = _y = _z = 0;
}

template <class T>
int cVec<T>::dataSize() const
{
	return (sizeof(T)*3);
}

/*template <class T>
uint cVec<T>::packedSize()
{
	return (sizeof(T)*3);
}*/

template <class T>
uint cVec<T>::checksum() const
{
	uint sum = (uint)x() + (uint)y()*2 + (uint)z()*3;
	return sum;
}

template <class T>
T cVec<T>::x() const {return(_x);}
template <class T>
T cVec<T>::y() const {return(_y);}
template <class T>
T cVec<T>::z() const {return(_z);}

template <class T>
T cVec<T>::length() const
{
	double len = abs( sqrt( pow(x(),2) + pow(y(),2) + pow(z(),2) ) );
	return((T)len);
}

template <class T>
void cVec<T>::normalize()
{
	T len = length();
	_x /= len;
	_y /= len;
	_z /= len;
}

template <class T>
void cVec<T>::zero() { _x = _y = _z = (T)0; }

template <class T>
void cVec<T>::set(T x, T y, T z)
{
	_x = (T)x;
	_y = (T)y;
	_z = (T)z;
}

template <class T>
void cVec<T>::setx(T x) { _x = (T)x; }
template <class T>
void cVec<T>::sety(T y) { _y = (T)y; }
template <class T>
void cVec<T>::setz(T z) { _z = (T)z; }


template <class T>
void cVec<T>::lerp(const cVec &b, double fraction)
{
	*this = *this + ((b - *this) * (T)fraction);
}

template <class T>
cVec<T>& cVec<T>::operator=(const cVec<T> &b)
{
	if(this == &b){return(*this);}	// check for self-assignment
	_x = b._x;
	_y = b._y;
	_z = b._z;
	return(*this);
}

// sum
template <class T>
cVec<T> cVec<T>::operator+(const cVec<T> &b) const
{
	cVec v(_x+b._x, _y+b._y, _z+b._z);
	return(v);
}

template <class T>
cVec<T>& cVec<T>::operator+=(cVec<T> &b)
{
	*this = *this+b;
	return(*this);
}

// difference
template <class T>
cVec<T> cVec<T>::operator-(const cVec<T> &b) const
{
	cVec v(_x-b._x, _y-b._y, _z-b._z);
	return(v);
}

// scale
template <class T>
cVec<T> cVec<T>::operator*(const double fraction) const
{
	cVec<T> v;
	v._x = _x * (T)fraction;
	v._y = _y * (T)fraction;
	v._z = _z * (T)fraction;
	return v;
}

// dot
template <class T>
T cVec<T>::operator*(const cVec<T> &b) const
{
	T crs = _x*b._x + _y*b._y + _z*b._z;
	return crs;
}

// cross
template <class T>
cVec<T> cVec<T>::operator^(const cVec<T> &b) const
{
	cVec v;
	v._x = _y*b._z - _z*b._y;
	v._y = _z*b._x - _x*b._z;
	v._z = _x*b._y - _y*b._x;
	return v;
}

template <class T>
cVec<T>& cVec<T>::operator*=(const cMatrix &m)
{
	T rx, ry, rz, rw;
	rx = (T)m.get(0,0)*x() + (T)m.get(0,1)*y() + (T)m.get(0,2)*z() + (T)m.get(0,3)*(T)1.0;
	ry = (T)m.get(1,0)*x() + (T)m.get(1,1)*y() + (T)m.get(1,2)*z() + (T)m.get(1,3)*(T)1.0;
	rz = (T)m.get(2,0)*x() + (T)m.get(2,1)*y() + (T)m.get(2,2)*z() + (T)m.get(2,3)*(T)1.0;
	rw = (T)m.get(3,0)*x() + (T)m.get(3,1)*y() + (T)m.get(3,2)*z() + (T)m.get(3,3)*(T)1.0;
	if(rw != (T)1.0) { rx /= rw; ry /= rw; rz /= rw; } // homogenization
	_x = rx;
	_y = ry;
	_z = rz;
	return(*this);
}

template <class T>
cVec<T> cVec<T>::operator*(const cMatrix &m) const
{
	cVec<T> r = *this;
	r *= m;
	return r;
}

template <class T>
void cVec<T>::print() const
{
	cout << *this;
}

template <class T>
int cVec<T>::write(const Byte* ptr, const cVec<T> &v) const
{
	int ofs = 0;
	ofs += cUtil::write(ptr+ofs, v.x());
	ofs += cUtil::write(ptr+ofs, v.y());
	ofs += cUtil::write(ptr+ofs, v.z());
	return(ofs);
}

template <class T>
int cVec<T>::read(const Byte* ptr, cVec<T> &v) const
{
	T tx, ty, tz;
	int ofs = 0;
	ofs += cUtil::read(ptr+ofs, tx);
	ofs += cUtil::read(ptr+ofs, ty);
	ofs += cUtil::read(ptr+ofs, tz);
	v.set(tx, ty, tz);
	return ofs;
}

typedef cVec<float> vec3;
//typedef cVec<double> vec3;

#endif
