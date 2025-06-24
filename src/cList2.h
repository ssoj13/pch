#ifndef _cList
#define _cList

//! refactored: http://www.tutorialspoint.com/cplusplus/cpp_templates.htm

#define D_BYTE		0
#define D_INT		1
#define D_UINT		2
#define D_ULINT		3
#define D_FLOAT		4
#define D_DOUBLE	5
#define D_VEC3		6
#define D_MATRIX	7
#define D_STRING	8
#define D_VECTOR	9
#define D_BINARY	10

//#include <iostream>
//#include <vector>
//#include <cstdlib>
//#include <string>
//#include <stdexcept>
#include "core_defs.h"
#include "core_classes.h"
using namespace std;

template <class T>
class cList
{
	public:
		cList(string);				// initializing with either name
		cList(string, vector<T>);	// or name and vector<T>
		cList(const cList&);		// or make a copy
		//cList(Byte*);				// or unpack
		~cList();

		// service methods
		int size() const;			// vector size
		int dataSize() const;		// data size in Bytes
		void clear();				// clear everything
		void clearData();			// clear just data
		// output
		friend std::ostream& operator<<(std::ostream&, const cList&);
		// data ops
		void setName(string);
		string getName(void) const;
		void setAppend(int);
		int getAppend() const;
		void reserve(int);
		void resize(int);
		void erase(int);
		ulint checksum() const;

		// get/set
		T get(int) const;
		vector<T> get() const;
		// contents ops: add/remove/set
		void set(int, T);
		void set(vector<T>&);
		cList& operator=(vector<T>&);
		cList& operator=(const cList&);
		void append(T);
		void append(vector<T>&);
		void append(cList&);
		cList operator+(T&);
		cList operator+(vector<T>&);
		cList operator+(cList&);
		// math ops
		void sum(int, int, T);	// mass-summation: start, end, value
		void lerp(const cList&, double);	// interpolates data in this cList with given cList
		void lerp(const cList&, const cList&, double);	// interpolates A & B and takes the result
		cList slice(int, int);

		// serialization
		/*
		int packedSize() const;
		Byte* pack(int&) const;	// allocates memory and returns *ptr and size of dump
		int pack(const Byte*) const;	// packs right into preallocated memory, returns Bytes written(cPtr)
		int unpack(Byte*);	// returns unpacked Bytes
		*/

	private:
		string		d_signature;
		string		d_name;
		int			d_appendFlag;
		vector<T>	data;


		uint size();			// return size
		void push(T const&);	// push element
		void pop();				// pop element
		T top() const;			// return top element
		bool empty() const {return data.empty();}	// return true if empty
	private:
		vector<T> data;			// elements
};

template <class T>
uint cList<T>::size()
{
	// returns size_t
	return data.size();
}

template <class T>
void cList<T>::push(T const& v)
{
	// append copy of passed element
	data.push_back(v);
}

template <class T>
void cList<T>::pop()
{
	if (data.empty()) {throw out_of_range("cList<>::pop(): empty cList");}
	// remove last element
	data.pop_back();
}

template <class T>
T cList<T>::top() const
{
	if (data.empty()) {throw out_of_range("cList<>::top(): empty cList");}
	// return copy of last element
	return data.back();
}

int main()
{
	try
	{
		// manipulate int cList
		cout << "int list" << endl;
		cList<int>	lInt;  // cList of ints
		lInt.push(7);
		cout << lInt.top() <<endl;

		// manipulate string cList
		cout << "string list" << endl;
		cList<string> lString;    // cList of strings
		lString.push("hello");
		cout << lString.top() << std::endl;
		lString.pop();

		// manipulate string cList
		cout << "vec3 list" << endl;
		vec3 v(0,1,0);
		cList<vec3> lVec;
		lVec.push(v);
		lVec.push(v);
		lVec.push(v);
		cout << lVec.size() << ", " << lVec.top() << std::endl;
		lVec.pop();
		lVec.pop();
	}
	catch (exception const& ex)
	{
		cerr << "Exception: " << ex.what() <<endl;
		return -1;
	}
}

#endif