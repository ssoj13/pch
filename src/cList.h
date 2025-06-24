/*
TODO:
1. operator==
*/

#ifndef _cList
#define _cList
#include "core_defs.h"
#include "cStatus.h"
#include "cChecksum.h"
#include "cUtil.h"
#include "cVec.h"

using namespace std;

// data types
#define D_UNKNOWN	0
#define D_BYTE		1
#define D_INT		2
#define D_UINT		3
#define D_ULINT		4
#define D_FLOAT		5
#define D_DOUBLE	6
#define D_VEC3		7
#define D_MATRIX	8
#define D_STRING	9

class cList: public cUtil
{
	public:
		cList();
		cList(string);
		cList(string, uint);		// name, d_type
		cList(string, uint, int);	// name, d_type, append
		cList(string, vector<Byte>);
		cList(string, vector<int>);
		cList(string, vector<uint>);
		cList(string, vector<ulint>);
		cList(string, vector<float>);
		cList(string, vector<double>);
		cList(string, vector<vec3>);
		cList(string, vector<matrix>);
		cList(string, vector<string>);
		cList(const cList&);
		cList(Byte*);
		~cList();

		// initializes object to other object data type and name, but skips data
		void init(const cList&);

		// service funcs
		void clear();
		void clearData();
		void print() const;
		friend std::ostream& operator<<(std::ostream&, const cList&);
		int size() const;
		int dataSize() const;
		// data ops
		void setName(string);
		string getName(void) const;
		void setType(uint);
		uint getType(void) const;
		bool isType(uint) const;
		void setAppend(int);
		int getAppend() const;
		void reserve(int);
		void resize(int);
		void erase(int);
		ulint checksum() const;

		// assigning whole array
		void set(vector<Byte>&);
		void set(vector<int>&);
		void set(vector<uint>&);
		void set(vector<ulint>&);
		void set(vector<float>&);
		void set(vector<double>&);
		void set(vector<vec3>&);
		void set(vector<matrix>&);
		void set(vector<string>&);
		// set single value
		void set(int, Byte);
		void set(int, int);
		void set(int, uint);
		void set(int, ulint);
		void set(int, float);
		void set(int, double);
		void set(int, vec3);
		void set(int, matrix);
		void set(int, string);
		// add single value
		void add(Byte);
		void add(int);
		void add(uint);
		void add(ulint);
		void add(float);
		void add(double);
		void add(vec3);
		void add(matrix);
		void add(string);
		// append vector
		void append(vector<Byte>&);
		void append(vector<int>&);
		void append(vector<uint>&);
		void append(vector<ulint>&);
		void append(vector<float>&);
		void append(vector<double>&);
		void append(vector<vec3>&);
		void append(vector<matrix>&);
		void append(vector<string>&);
		void append(cList&);
		// get data, fills in reference to variable, made that way to make polymorphism working
		void get(int, Byte&) const;
		void get(int, int&) const;
		void get(int, uint&) const;
		void get(int, ulint&) const;
		void get(int, float&) const;
		void get(int, double&) const;
		void get(int, vec3&) const;
		void get(int, matrix&) const;
		void get(int, string&) const;
		// get array
		void get(vector<Byte>&) const;
		void get(vector<int>&) const;
		void get(vector<uint>&) const;
		void get(vector<ulint>&) const;
		void get(vector<float>&) const;
		void get(vector<double>&) const;
		void get(vector<vec3>&) const;
		void get(vector<matrix>&) const;
		void get(vector<string>&) const;
		// mass-summation
		void sum(int, int, Byte);
		void sum(int, int, int);
		void sum(int, int, uint);
		void sum(int, int, ulint);
		void sum(int, int, float);
		void sum(int, int, double);
		void sum(int, int, vec3);
		void sum(int, int, matrix);
		// lerp
		void lerp(const cList&, double);	// interpolates data in this cList with given cList
		void lerp(const cList&, const cList&, double);	// interpolates A & B and takes the result
		cList slice(int, int);

		// overloads
		cList& operator=(vector<Byte>&);
		cList& operator=(vector<int>&);
		cList& operator=(vector<uint>&);
		cList& operator=(vector<ulint>&);
		cList& operator=(vector<float>&);
		cList& operator=(vector<double>&);
		cList& operator=(vector<vec3>&);
		cList& operator=(vector<matrix>&);
		cList& operator=(vector<string>&);
		cList& operator=(const cList&);

		cList operator+(vector<Byte>&);
		cList operator+(vector<int>&);
		cList operator+(vector<uint>&);
		cList operator+(vector<ulint>&);
		cList operator+(vector<float>&);
		cList operator+(vector<double>&);
		cList operator+(vector<vec3>&);
		cList operator+(vector<matrix>&);
		cList operator+(vector<string>&);
		cList operator+(cList&);

		// serialization
		int packedSize() const;
		Byte* pack(int&) const;	// allocates memory and returns *ptr and size of dump
		int pack(const Byte*) const;	// packs right into preallocated memory, returns Bytes written(cPtr)
		int unpack(Byte*);	// returns unpacked Bytes

	private:
		string _signature;
		string _name;
		int _appendFlag;
		uint d_type;
		vector<Byte>	d_Byte;
		vector<int>		d_int;
		vector<uint>	d_uint;
		vector<ulint>	d_ulint;
		vector<float>	d_float;
		vector<double>	d_double;
		vector<vec3>	d_vec3;
		vector<matrix>	d_matrix;
		vector<string>	d_string;
};

#endif
