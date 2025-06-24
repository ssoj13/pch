/*
dataRec - hardcoded data types
to get single data values you need to call "get(i, uint)", it's made this way to make polymorphism works
if you add() data of different type, dataRec changes it's type, and all previous data is cleared

TODO:
	2. bounds check on get/set/etc
	3. maybe internal error code

q: should i use methods to access internal structures from other methods?
*/

#include "cList.h"
using namespace std;

//--- Constructors
cList::cList() {clear();}
cList::cList(string n) {clear(); _name = n;}
cList::cList(string n, uint t) {clear(); _name = n; d_type = t;}
cList::cList(string n, uint t, int append) {clear(); _name = n; d_type = t; setAppend(append);}
cList::cList(string n, vector<Byte> d) {clear(); _name = n; d_Byte = d; d_type = D_BYTE;}
cList::cList(string n, vector<int> d) {clear(); _name = n; d_int = d; d_type = D_INT;}
cList::cList(string n, vector<uint> d) {clear(); _name = n; d_uint = d; d_type = D_UINT;}
cList::cList(string n, vector<ulint> d) {clear(); _name = n; d_ulint = d; d_type = D_ULINT;}
cList::cList(string n, vector<float> d) {clear(); _name = n; d_float = d; d_type = D_FLOAT;}
cList::cList(string n, vector<double> d) {clear(); _name = n; d_double = d; d_type = D_DOUBLE;}
cList::cList(string n, vector<vec3> d) {clear(); _name = n; d_vec3 = d; d_type = D_VEC3;}
cList::cList(string n, vector<matrix> d) {clear(); _name = n; d_matrix = d; d_type = D_MATRIX;}
cList::cList(string n, vector<string> d) {clear(); _name = n; d_string = d; d_type = D_STRING;}

cList::cList(const cList &b)
{
	*this = b;
}


cList::cList(Byte* dump)
{
	unpack(dump);
}


cList::~cList()
{
	clear();
}

void cList::init(const cList &b)
{
	clear();
	_name = b._name;
	d_type = b.d_type;
}

//--- service funcs

void cList::clear()
{
	_signature = "cList";
	_name.clear();
	setAppend(1);
	d_type = D_UNKNOWN;
	d_Byte.clear();
	d_int.clear();
	d_uint.clear();
	d_ulint.clear();
	d_float.clear();
	d_double.clear();
	d_vec3.clear();
	d_matrix.clear();
	d_string.clear();
}

void cList::clearData()
{
	d_Byte.clear();
	d_int.clear();
	d_uint.clear();
	d_ulint.clear();
	d_float.clear();
	d_double.clear();
	d_vec3.clear();
	d_matrix.clear();
	d_string.clear();
}

// number of elements
int cList::size() const
{
	int sz = 0;
	switch(d_type)
	{
		case D_BYTE: {sz = d_Byte.size(); break;}
		case D_INT: {sz = d_int.size(); break;}
		case D_UINT: {sz = d_uint.size(); break;}
		case D_ULINT: {sz = d_ulint.size(); break;}
		case D_FLOAT: {sz = d_float.size(); break;}
		case D_DOUBLE: {sz = d_double.size(); break;}
		case D_VEC3: {sz = d_vec3.size(); break;}
		case D_MATRIX: {sz = d_matrix.size(); break;}
		case D_STRING: {sz = d_string.size(); break;}
		case D_UNKNOWN: {break;}
		default: {break;}
	}
	return sz;
}

// data size in Bytes
int cList::dataSize() const
{
	switch(d_type)
	{
		case D_BYTE: {return( d_Byte.size() * sizeof(Byte) );}
		case D_INT: {return( d_int.size() * sizeof(int) );}
		case D_UINT: {return( d_uint.size() * sizeof(uint) );}
		case D_ULINT: {return( d_ulint.size() * sizeof(ulint) );}
		case D_FLOAT: {return( d_float.size() * sizeof(float) );}
		case D_DOUBLE: {return( d_double.size() * sizeof(double) );}
		case D_VEC3: {return( d_vec3.size() * vec3().dataSize() );}
		case D_MATRIX: {return( d_matrix.size() * matrix().dataSize() );}
		case D_STRING: {int str_sz = 0; loop((int)d_string.size()) {str_sz += d_string[i].size(); str_sz += int_sz;} return(str_sz);}	// uint is size of each string
		case D_UNKNOWN: {return(0);}
		default: {return(0);}
	}
}

void cList::print() const
{
	cout << *this;
}

ostream& operator<<(std::ostream& ostr, const cList& b)
{
	OSTR_SET
	ostr << b._signature << " \"" << b._name << "\",\ttype: " << b.d_type << ",\tsize: " << b.size() << ": ";
	switch(b.d_type)
	{
		case D_BYTE: {loop(b.size()) {ostr << b.d_Byte[i] << ", ";}; break;}
		case D_INT: {loop(b.size()) {ostr << b.d_int[i] << ", ";}; break;}
		case D_UINT: {loop(b.size()) {ostr << b.d_uint[i] << ", ";}; break;}
		case D_ULINT: {loop(b.size()) {ostr << b.d_ulint[i] << ", ";}; break;}
		case D_FLOAT: {loop(b.size()) {ostr << b.d_float[i] << ", ";}; break;}
		case D_DOUBLE: {loop(b.size()) {ostr << b.d_double[i] << ", ";}; break;}
		case D_VEC3: {loop(b.size()) {b.d_vec3[i].print(); ostr << ", ";}; break;}
		case D_MATRIX: {loop(b.size()) {b.d_matrix[i].print(); ostr << ", ";}; break;}
		case D_STRING: {loop(b.size()) {ostr << b.d_string[i] << ", ";}; break;}
		case D_UNKNOWN: {ostr << "D_UNKNOWN";}
	}
	ostr << endl;
	return ostr;
}


//--- data manipulations
void cList::setName(string n) { _name = n; }
string cList::getName(void) const { return(_name); }

void cList::setType(uint t) { if(size() && d_type != t) clearData(); d_type = t; }
uint cList::getType(void) const { return(d_type); }
bool cList::isType(uint t) const { if(d_type == t) return true; else return false; }

void cList::setAppend(int v) { _appendFlag = v; }
int cList::getAppend() const { return _appendFlag; }

void cList::reserve(int sz)
{
	switch(d_type)
	{
		case D_BYTE: {d_Byte.reserve(sz); break;}
		case D_INT: {d_int.reserve(sz); break;}
		case D_UINT: {d_uint.reserve(sz); break;}
		case D_ULINT: {d_ulint.reserve(sz); break;}
		case D_FLOAT: {d_float.reserve(sz); break;}
		case D_DOUBLE: {d_double.reserve(sz); break;}
		case D_VEC3: {d_vec3.reserve(sz); break;}
		case D_MATRIX: {d_matrix.reserve(sz); break;}
		case D_STRING: {d_string.reserve(sz); break;}
		case D_UNKNOWN: {break;}
	}
}

void cList::resize(int sz)
{
	switch(d_type)
	{
		case D_BYTE: {d_Byte.resize(sz); break;}
		case D_INT: {d_int.resize(sz); break;}
		case D_UINT: {d_uint.resize(sz); break;}
		case D_ULINT: {d_ulint.resize(sz); break;}
		case D_FLOAT: {d_float.resize(sz); break;}
		case D_DOUBLE: {d_double.resize(sz); break;}
		case D_VEC3: {d_vec3.resize(sz); break;}
		case D_MATRIX: {d_matrix.resize(sz); break;}
		case D_STRING: {d_string.resize(sz); break;}
		case D_UNKNOWN: {break;}
	}
}

void cList::erase(int elem)
{
	switch(d_type)
	{
		case D_BYTE: {d_Byte.erase(d_Byte.begin()+elem); break;}
		case D_INT: {d_int.erase(d_int.begin()+elem); break;}
		case D_UINT: {d_uint.erase(d_uint.begin()+elem); break;}
		case D_ULINT: {d_ulint.erase(d_ulint.begin()+elem); break;}
		case D_FLOAT: {d_float.erase(d_float.begin()+elem); break;}
		case D_DOUBLE: {d_double.erase(d_double.begin()+elem); break;}
		case D_VEC3: {d_vec3.erase(d_vec3.begin()+elem); break;}
		case D_MATRIX: {d_matrix.erase(d_matrix.begin()+elem); break;}
		case D_STRING: {d_string.erase(d_string.begin()+elem); break;}
		case D_UNKNOWN: {break;}
	}
}

// fast and fail checksum calculation, replace later with something reasonable
ulint cList::checksum() const
{
	cChecksum cs;
	switch(d_type)
	{
		case D_BYTE: { Byte v; loop(size()) { get(i, v); cs.add((uint)v); } break;}
		case D_INT: { int v; loop(size()) { get(i, v); cs.add((uint)v); } break;}
		case D_UINT: { uint v; loop(size()) { get(i, v); cs.add((uint)v); } break;}
		case D_ULINT: { ulint v; loop(size()) { get(i, v); cs.add((uint)v); } break;}
		case D_FLOAT: { float v; loop(size()) { get(i, v); cs.add((uint)v); } break;}
		case D_DOUBLE: { double v; loop(size()) { get(i, v); cs.add((uint)v); } break;}
		case D_VEC3: { vec3 v; loop(size()) { get(i, v); cs.add(v.checksum()); } break;}
		case D_MATRIX: { matrix v; loop(size()) { get(i, v); cs.add(v.checksum()); } break;}
		case D_STRING: { break;}
		case D_UNKNOWN: { break; }
	}
	return (cs.get());
}


// assigning whole array, setType() clearing old data if it was of different type
void cList::set(vector<Byte> &v) { setType(D_BYTE); d_Byte = v; }
void cList::set(vector<int> &v) { setType(D_INT); d_int = v; }
void cList::set(vector<uint> &v) { setType(D_UINT); d_uint = v; }
void cList::set(vector<ulint> &v) { setType(D_ULINT); d_ulint = v; }
void cList::set(vector<float> &v) { setType(D_FLOAT); d_float = v; }
void cList::set(vector<double> &v) { setType(D_DOUBLE); d_double = v; }
void cList::set(vector<vec3> &v) { setType(D_VEC3); d_vec3 = v; }
void cList::set(vector<matrix> &v) { setType(D_MATRIX); d_matrix = v; }
void cList::set(vector<string> &v) { setType(D_STRING); d_string = v; }
// set(idx, value) to change single elements
// NO RANGE CHECK PERFORMED, THIS IS POTENTIALLY DANGEROUS FUNCTION
// IF DIFFERENT TYPE PROVIDED, ARRAYS GETTING CLEARED, AND SETTING ARBITRARY ELEMENT IN EMPTY ARRAY = BIG BADABOOM
// IN THE CASE OF ARITHMETIC OPS WE NEED TO EXPLICITLY PROVIDE TYPE OF RESULT: (UINT)(a+b),
// OTHERWISE METHOD FOR OTHER D_TYPE WILL BE CALLED AND PROGRAM WILL CRASH
void cList::set(int i, Byte v) { if(isType(D_BYTE)) d_Byte[i] = v; }
void cList::set(int i, int v) { if(isType(D_INT)) d_int[i] = v; }
void cList::set(int i, uint v) { if(isType(D_UINT)) d_uint[i] = v; }
void cList::set(int i, ulint v) { if(isType(D_ULINT)) d_ulint[i] = v; }
void cList::set(int i, float v) { if(isType(D_FLOAT)) d_float[i] = v; }
void cList::set(int i, double v) { if(isType(D_DOUBLE)) d_double[i] = v; }
void cList::set(int i, vec3 v) { if(isType(D_VEC3)) d_vec3[i] = v; }
void cList::set(int i, matrix v) { if(isType(D_MATRIX)) d_matrix[i] = v; }
void cList::set(int i, string v) { if(isType(D_STRING)) d_string[i] = v; }

// add() funcs is clearing data and changing type of dataRec if other dataType was supplied
void cList::add(Byte v) { if(isType(D_BYTE)) d_Byte.push_back(v); }
void cList::add(int v) { if(isType(D_INT)) d_int.push_back(v); }
void cList::add(uint v) { if(isType(D_UINT)) d_uint.push_back(v); }
void cList::add(ulint v) { if(isType(D_ULINT)) d_ulint.push_back(v); }
void cList::add(float v) { if(isType(D_FLOAT)) d_float.push_back(v); }
void cList::add(double v) { if(isType(D_DOUBLE)) d_double.push_back(v); }
void cList::add(vec3 v) { if(isType(D_VEC3)) d_vec3.push_back(v); }
void cList::add(matrix v) { if(isType(D_MATRIX)) d_matrix.push_back(v); }
void cList::add(string v) { if(isType(D_STRING)) d_string.push_back(v); }

// appends data to vector if _appendFlag == 1 (need that to implement global non-appendable attributes for cMesh)
void cList::append(vector<Byte> &v) { if(isType(D_BYTE) && getAppend()) d_Byte.insert(d_Byte.end(), v.begin(), v.end()); }
void cList::append(vector<int> &v) { if(isType(D_INT) && getAppend()) d_int.insert(d_int.end(), v.begin(), v.end()); }
void cList::append(vector<uint> &v) { if(isType(D_UINT) && getAppend()) d_uint.insert(d_uint.end(), v.begin(), v.end()); }
void cList::append(vector<ulint> &v) { if(isType(D_ULINT) && getAppend()) d_ulint.insert(d_ulint.end(), v.begin(), v.end()); }
void cList::append(vector<float> &v) { if(isType(D_FLOAT) && getAppend()) d_float.insert(d_float.end(), v.begin(), v.end()); }
void cList::append(vector<double> &v) { if(isType(D_DOUBLE) && getAppend()) d_double.insert(d_double.end(), v.begin(), v.end()); }
void cList::append(vector<vec3> &v) { if(isType(D_VEC3) && getAppend()) d_vec3.insert(d_vec3.end(), v.begin(), v.end()); }
void cList::append(vector<matrix> &v) { if(isType(D_MATRIX) && getAppend()) d_matrix.insert(d_matrix.end(), v.begin(), v.end()); }
void cList::append(vector<string> &v) { if(isType(D_STRING) && getAppend()) d_string.insert(d_string.end(), v.begin(), v.end()); }
void cList::append(cList &v)
{
	// in Maya adding empty string arrays causes vectors range error
	if(!v.size()) return;
	// also do not append two different non-empty datas
	if(size() && !isType(v.getType()) ) return;
	// initialize to other dataType if this one is empty
	if(!size()) init(v);
    switch(getType())
	{
		case D_BYTE:	{ append(v.d_Byte); break;}
		case D_INT:		{ append(v.d_int);  break;}
		case D_UINT:	{ append(v.d_uint);  break;}
		case D_ULINT:	{ append(v.d_ulint);  break;}
		case D_FLOAT:	{ append(v.d_float);  break;}
		case D_DOUBLE:	{ append(v.d_double);  break;}
		case D_VEC3:	{ append(v.d_vec3);  break;}
		case D_MATRIX:	{ append(v.d_matrix);  break;}
		case D_STRING:	{ append(v.d_string);  break;}
		case D_UNKNOWN:	{ break; }
		default: { break; }
	}
}

// get single
void cList::get(int i, Byte &v) const { if(isType(D_BYTE)) v = d_Byte[i]; else v = 0;}
void cList::get(int i, int &v) const { if(isType(D_INT)) v = d_int[i]; else v = 0;}
void cList::get(int i, uint &v) const { if(isType(D_UINT)) v = d_uint[i]; else v = 0;}
void cList::get(int i, ulint &v) const { if(isType(D_ULINT)) v = d_ulint[i]; else v = 0;}
void cList::get(int i, float &v) const { if(isType(D_FLOAT)) v = d_float[i]; else v = 0;}
void cList::get(int i, double &v) const { if(isType(D_DOUBLE)) v = d_double[i]; else v = 0;}
void cList::get(int i, vec3 &v) const { if(isType(D_VEC3)) v = d_vec3[i]; else v = vec3(0,0,0);}
void cList::get(int i, matrix &v) const { if(isType(D_MATRIX)) v = d_matrix[i]; else v = matrix();}
void cList::get(int i, string &v) const { if(isType(D_STRING)) v = d_string[i]; else v = string("");}
// get vector
void cList::get(vector<Byte> &v) const { v = d_Byte; }
void cList::get(vector<int> &v) const { v = d_int; }
void cList::get(vector<uint> &v) const { v = d_uint; }
void cList::get(vector<ulint> &v) const { v = d_ulint; }
void cList::get(vector<float> &v) const { v = d_float; }
void cList::get(vector<double> &v) const { v = d_double; }
void cList::get(vector<vec3> &v) const { v = d_vec3; }
void cList::get(vector<matrix> &v) const { v = d_matrix; }
void cList::get(vector<string> &v) const { v = d_string; }

// summation func for numeric data types
void cList::sum(int start, int end, Byte v) {if(isType(D_BYTE)) loop12(start, end) d_Byte[i] += v; }
void cList::sum(int start, int end, int v) {if(isType(D_INT)) loop12(start, end) d_int[i] += v;}
void cList::sum(int start, int end, uint v) {if(isType(D_UINT)) loop12(start, end) d_uint[i] += v;}
void cList::sum(int start, int end, ulint v) {if(isType(D_ULINT)) loop12(start, end) d_ulint[i] += v;}
void cList::sum(int start, int end, float v) {if(isType(D_FLOAT)) loop12(start, end) d_float[i] += v;}
void cList::sum(int start, int end, double v) {if(isType(D_DOUBLE)) loop12(start, end) d_double[i] += v;}
void cList::sum(int start, int end, vec3 v) {if(isType(D_VEC3)) loop12(start, end) d_vec3[i] += v;}
// do we need sum for matrix and string?

// lerp: quite a stupid type conversion, but it's the only way to calculate it without losing precision
void cList::lerp(const cList &b, double fraction)
{
	if(d_type != b.d_type || size() != b.size()) return;
	switch(d_type)
	{
		case D_BYTE: { loop(size()) {d_Byte[i] = (Byte)((double)d_Byte[i] + (((double)b.d_Byte[i] - (double)d_Byte[i]) * fraction));} break; }
		case D_INT: { loop(size()) {d_int[i] = (int)((double)d_Byte[i] + (((double)b.d_Byte[i] - (double)d_Byte[i]) * fraction));} break; }
		case D_UINT: { loop(size()) {d_uint[i] = (uint)((double)d_Byte[i] + (((double)b.d_Byte[i] - (double)d_Byte[i]) * fraction));} break; }
		case D_ULINT: { loop(size()) {d_ulint[i] = (ulint)((double)d_Byte[i] + (((double)b.d_Byte[i] - (double)d_Byte[i]) * fraction));} break; }
		case D_FLOAT: { loop(size()) {d_float[i] = (float)((double)d_Byte[i] + (((double)b.d_Byte[i] - (double)d_Byte[i]) * fraction));} break; }
		case D_DOUBLE: { loop(size()) {d_double[i] = (double)((double)d_Byte[i] + (((double)b.d_Byte[i] - (double)d_Byte[i]) * fraction));} break; }
		case D_VEC3: { loop(size()) {d_vec3[i].lerp(b.d_vec3[i], fraction);} break;}
		default: { break; }
	}
}

// takes inputs A & B and uses the result
void cList::lerp(const cList &a, const cList &b, double fraction)
{
	if(d_type != a.d_type || size() != a.size() || d_type != b.d_type || size() != b.size()) return;
	switch(d_type)
	{
		case D_BYTE: { loop(size()) {d_Byte[i] = (Byte)((double)a.d_Byte[i] + (((double)b.d_Byte[i] - (double)a.d_Byte[i]) * fraction));} break; }
		case D_INT: { loop(size()) {d_int[i] = (int)((double)a.d_Byte[i] + (((double)b.d_Byte[i] - (double)a.d_Byte[i]) * fraction));} break; }
		case D_UINT: { loop(size()) {d_uint[i] = (uint)((double)a.d_Byte[i] + (((double)b.d_Byte[i] - (double)a.d_Byte[i]) * fraction));} break; }
		case D_ULINT: { loop(size()) {d_ulint[i] = (ulint)((double)a.d_Byte[i] + (((double)b.d_Byte[i] - (double)a.d_Byte[i]) * fraction));} break; }
		case D_FLOAT: { loop(size()) {d_float[i] = (float)((double)a.d_Byte[i] + (((double)b.d_Byte[i] - (double)a.d_Byte[i]) * fraction));} break; }
		case D_DOUBLE: { loop(size()) {d_double[i] = (double)((double)a.d_Byte[i] + (((double)b.d_Byte[i] - (double)a.d_Byte[i]) * fraction));} break; }
		case D_VEC3: { loop(size()) {d_vec3[i] = a.d_vec3[i]; d_vec3[i].lerp(b.d_vec3[i], fraction);} break;}
		default: { break; }
	}
}


cList cList::slice(int start, int end)
{
	cList r;
	r._name = _name;
	r.d_type = d_type;
	uint s, e, sz;
	sz = size();
	s = start<end?start:end;
	e = start<end?end:start;
    e = e>sz?sz:e;
	switch(d_type)
	{
		case D_BYTE:	{ r.d_Byte.insert(r.d_Byte.end(), d_Byte.begin()+s, d_Byte.begin()+e); break; }
		case D_INT:		{ r.d_int.insert(r.d_int.end(), d_int.begin()+s, d_int.begin()+e); break; }
		case D_UINT:	{ r.d_uint.insert(r.d_uint.end(), d_uint.begin()+s, d_uint.begin()+e); break; }
		case D_ULINT:	{ r.d_ulint.insert(r.d_ulint.end(), d_ulint.begin()+s, d_ulint.begin()+e); break; }
		case D_FLOAT:	{ r.d_float.insert(r.d_float.end(), d_float.begin()+s, d_float.begin()+e); break; }
		case D_DOUBLE:	{ r.d_double.insert(r.d_double.end(), d_double.begin()+s, d_double.begin()+e); break; }
		case D_VEC3:	{ r.d_vec3.insert(r.d_vec3.end(), d_vec3.begin()+s, d_vec3.begin()+e); break; }
		case D_MATRIX:	{ r.d_matrix.insert(r.d_matrix.end(), d_matrix.begin()+s, d_matrix.begin()+e); break; }
		case D_STRING:	{ r.d_string.insert(r.d_string.end(), d_string.begin()+s, d_string.begin()+e); break; }
		case D_UNKNOWN:	{ break; }
	}
	return(r);
}


//--- overloads
cList& cList::operator=(const cList& b)
{
	if(this == &b){return(*this);}	// check for self-assignment
	clear();
	_name = b._name;
	_appendFlag = b._appendFlag;
	d_type = b.d_type;
	switch(d_type)
	{
		case D_BYTE: { d_Byte = b.d_Byte; break; }
		case D_INT: { d_int = b.d_int; break; }
		case D_UINT: { d_uint = b.d_uint; break; }
		case D_ULINT: { d_ulint = b.d_ulint; break; }
		case D_FLOAT: { d_float = b.d_float; break; }
		case D_DOUBLE: { d_double = b.d_double; break; }
		case D_VEC3: { d_vec3 = b.d_vec3; break; }
		case D_MATRIX: { d_matrix = b.d_matrix; break; }
		case D_STRING: { d_string = b.d_string; break; }
	}
	return(*this);
}
cList& cList::operator=(vector<Byte>& b) { setType(D_BYTE); d_Byte = b; return(*this); }
cList& cList::operator=(vector<int>& b) { setType(D_INT); d_int = b; return(*this); }
cList& cList::operator=(vector<uint>& b) { setType(D_UINT); d_uint = b; return(*this); }
cList& cList::operator=(vector<ulint>& b) { setType(D_ULINT); d_ulint = b; return(*this); }
cList& cList::operator=(vector<float>& b) { setType(D_FLOAT); d_float = b; return(*this); }
cList& cList::operator=(vector<double>& b) { setType(D_DOUBLE); d_double = b; return(*this); }
cList& cList::operator=(vector<vec3>& b) { setType(D_VEC3); d_vec3 = b; return(*this); }
cList& cList::operator=(vector<matrix>& b) { setType(D_MATRIX); d_matrix = b; return(*this); }
cList& cList::operator=(vector<string>& b) { setType(D_STRING); d_string = b; return(*this); }


cList cList::operator+(cList& b)
{
	cList r = *this;
	// we do not trying to add data of another type - we just returning object intact
	if(r.size() && r.d_type != b.d_type) return(r); else r.setType(b.d_type);
	if(!r._name.size()) r._name = b._name;
	r.append(b);
	return(r);
}
// append performs type/append checks
cList cList::operator+(vector<Byte> &v) { cList r = *this; r.append(v); return(r); }
cList cList::operator+(vector<int> &v) { cList r = *this; r.append(v); return(r); }
cList cList::operator+(vector<uint> &v) { cList r = *this; r.append(v); return(r); }
cList cList::operator+(vector<ulint> &v) { cList r = *this; r.append(v); return(r); }
cList cList::operator+(vector<float> &v) { cList r = *this; r.append(v); return(r); }
cList cList::operator+(vector<double> &v) { cList r = *this; r.append(v); return(r); }
cList cList::operator+(vector<vec3> &v) { cList r = *this; r.append(v); return(r); }
cList cList::operator+(vector<matrix> &v) { cList r = *this; r.append(v); return(r); }
cList cList::operator+(vector<string> &v) { cList r = *this; r.append(v); return(r); }


//--- serialization
int cList::packedSize() const
{
	if(!_name.size()) {return(0);} // if we got no _name - return 0
	int h_sz, d_sz;
	//     dumpLen +sigLen + _signature +        nameLen +_name +        d_type + _appendFlag
	h_sz = int_sz + int_sz + _signature.size() + int_sz + _name.size() + int_sz + int_sz;
	//      numData +[data]
	d_sz =  int_sz + dataSize();
	return(h_sz + d_sz);
}


Byte* cList::pack(int &sz) const	// using Byte* to avoid "pointer-arith" warning
{
	sz = packedSize();
	if(!sz) {return(NULL);}
	Byte *dump = new Byte[packedSize()];
	if(dump == NULL) {sz = 0; return(NULL);}	// if memory error - return 0 - do something later with error processing
	sz = pack(dump);
	return(dump);
}


// packs into preallocated memory, returns size packed(cPtr)
// if we got _name but no data, just header will be written
int cList::pack(const Byte *dump) const
{
	uint sz, cPtr = 0;
	if(dump == NULL) {return(0);}	// if we got null *ptr - do nothing, return zero incrment
	sz = packedSize();
	if(!sz) {return(0);}			// if we got no data - do nothing
	cPtr += write(dump+cPtr, sz);			// dumpLen
	cPtr += write(dump+cPtr, _signature);	// _signature
	cPtr += write(dump+cPtr, _name);		// _name
	cPtr += write(dump+cPtr, d_type);		// d_type
	cPtr += write(dump+cPtr, _appendFlag);	// _appendFlag
	cPtr += write(dump+cPtr, size());		// dataNum
	switch(d_type)
	{
		case D_BYTE:	{ loop(size()) cPtr += write(dump+cPtr, d_Byte[i]); break; }
		case D_INT:		{ loop(size()) cPtr += write(dump+cPtr, d_int[i]); break; }
		case D_UINT:	{ loop(size()) cPtr += write(dump+cPtr, d_uint[i]); break; }
		case D_ULINT:	{ loop(size()) cPtr += write(dump+cPtr, d_ulint[i]); break; }
		case D_FLOAT:	{ loop(size()) cPtr += write(dump+cPtr, d_float[i]); break; }
		case D_DOUBLE:	{ loop(size()) cPtr += write(dump+cPtr, d_double[i]); break; }
		case D_VEC3:	{ loop(size()) cPtr += d_vec3[i].write(dump+cPtr, d_vec3[i]); break; }
		case D_MATRIX:	{ loop(size()) cPtr += d_matrix[i].write(dump+cPtr, d_matrix[i]); break; }
		case D_STRING:	{ loop(size()) cPtr += write(dump+cPtr, d_string[i]); break; }
	}
	return(cPtr);
}

// unpacks dump into internal structures
// returns unpacked Bytes size that can be used as offset in chain-unpacking
int cList::unpack(Byte* dump)
{
	clear();	
	int sz, cPtr = 0;
	string sig;
	cPtr += read(dump+cPtr, sz);		// dumpLen
	cPtr += read(dump+cPtr, sig);		// _signature
	if (sig != _signature) return(cStatus::sig_error);
	cPtr += read(dump+cPtr, _name);		// _name
	cPtr += read(dump+cPtr, d_type);	// d_type
	cPtr += read(dump+cPtr, _appendFlag);	// _appendFlag
	cPtr += read(dump+cPtr, sz);		// dataNum
	if(!sz) {clear();return(0);}		// if we got 0 data - do not unpack at all
	resize(sz);
	switch(d_type)
	{
		case D_BYTE:	{ loop(sz) {cPtr += read(dump+cPtr, d_Byte[i]);}; break; }
		case D_INT:		{ loop(sz) {cPtr += read(dump+cPtr, d_int[i]);}; break; }
		case D_UINT:	{ loop(sz) {cPtr += read(dump+cPtr, d_uint[i]);}; break; }
		case D_ULINT:	{ loop(sz) {cPtr += read(dump+cPtr, d_ulint[i]);}; break; }
		case D_FLOAT:	{ loop(sz) {cPtr += read(dump+cPtr, d_float[i]);}; break; }
		case D_DOUBLE:	{ loop(sz) {cPtr += read(dump+cPtr, d_double[i]);}; break; }
		case D_VEC3:	{ loop(sz) {cPtr += d_vec3[i].read(dump+cPtr, d_vec3[i]);}; break; }
		case D_MATRIX:	{ loop(sz) {cPtr += d_matrix[i].read(dump+cPtr, d_matrix[i]);}; break; }
		case D_STRING:	{ loop(sz) {cPtr += read(dump+cPtr, d_string[i]);}; break; }
	}
	return(cPtr);
}
