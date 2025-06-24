#include "cContainer.h"

//--- Constructors
cContainer::cContainer()
{
	clear();
}


cContainer::cContainer(cContainer &other)
{
	clear();
	data = other.data;
}

cContainer::cContainer(Byte* dump)
{
	clear();
	unpack(dump);
}

cContainer::~cContainer()
{
	clear();
}

void cContainer::init(cContainer &other)
{
	clear();
	int sz = other.size();
	if(sz) data.resize(sz);
	loop(sz) data[i].init(other.data[i]);
}

void cContainer::initSig()
{
	_signature = "cContainer";
}

string cContainer::getSig() const
{
	return _signature;
}



//--- member funcs

// returns number of cLists in vector
int cContainer::size() const
{
	return(data.size());
}

void cContainer::clear()
{
	clearData();
	initSig();
}

void cContainer::clearData()
{
	data.clear();
	_checksum = 0;
}

int cContainer::dataId(string name) const
{
	loop(size())
	{
		if(data[i].getName() == name) return i;
	}
	return(-1);
}

bool cContainer::idOk(int d) const
{
	if(d>-1 && d<size()) return true; else return false;
}

bool cContainer::dataExist(string name) const
{
	if(idOk(dataId(name))) return true; else return false;
}

void cContainer::deleteData(string name)
{
	int idx = dataId(name);
	if(idOk(idx)) data.erase(data.begin()+idx);
}

void cContainer::listData(vector<string> &aList) const
{
	aList.clear();
	loop(size()) { aList.push_back(data[i].getName()); }
}
vector<string> cContainer::listData() const
{
	vector<string> l;
	listData(l);
	return l;
}

int cContainer::dataSize(int d) const
{
	int sz = 0;
	if(idOk(d)) sz = data[d].size();
	return sz;
}

int cContainer::dataSize(string name) const
{
	return dataSize( dataId(name) );
}


void cContainer::resizeData(string name, int sz)
{
	int d = dataId(name);
	if(idOk(d)) data[d].resize(sz);
}

void cContainer::append(cContainer& b)
{
	// making it idiotproof:
	if(!b.size()) return;	// nothing to append
	if(!size()) {*this = b; return;}	// this obj is empty, making just =
	if(size() != b.size() ) return;	// checking if we have the same number of dataRecords in both objects

	loop(b.size())
	{
		if( data[i].isType(b.data[i].getType()) ) data[i].append(b.data[i]);
	}
}

// just names and packedSize()
void cContainer::print() const
{
	COUT_SET
	cout << *this;
}

// _all_ content
void cContainer::printDebug() const
{
	COUT_SET
	cout << _signature << ", packed size: " << i2s(packedSize()) << "b" << endl;
	loop(size()) data[i].print();
}

ostream& operator<<(std::ostream& ostr, const cContainer& b)
{
	OSTR_SET
	ostr << b._signature << ": " << b.i2s(b.packedSize()) << endl;
	loop(b.size()) ostr << b.data[i].getName() << ": " << b.data[i].size() << endl;
	return ostr;
}


// set cLists
void cContainer::set(int d, cList &v)
{
	if(idOk(d)) { data[d] = v; }
}
// set cList by name
void cContainer::set(string s, cList &v)
{
	int d = dataId(s);
	if(idOk(d)) data[d] = v;
}

// append only if types are match
void cContainer::append(int d, cList &v)
{
	if(idOk(d))
	{
		if( data[d].isType(v.getType()) ) { data[d].append(v); }
	}
}

void cContainer::get(int d, cList &v) const
{
	if(idOk(d)) { v = data[d]; }
}

cList& cContainer::at(int d)
{
	if(idOk(d)) return data.at(d); else return nullRec;
}

cList& cContainer::at(string s)
{
	int d = dataId(s);
	if(idOk(d)) return data.at(d); else return nullRec;
}


// --== ARRAY operations ==--
int cContainer::addData(cList& v) { if( !idOk(dataId(v.getName())) ) { data.push_back(v); return size()-1; } else return -1; }
int cContainer::addData(string name, int d_type) { data.push_back( cList(name, d_type) ); return size()-1; }
int cContainer::addData(string name, int d_type, int append) { data.push_back( cList(name, d_type, append) ); return size()-1; }
int cContainer::addData(string s, vector<Byte> &v) { int i = addData(s, D_BYTE); set(i, v); return i; }
int cContainer::addData(string s, vector<int> &v) { int i = addData(s, D_INT); set(i, v); return i; }
int cContainer::addData(string s, vector<uint> &v) { int i = addData(s, D_UINT); set(i, v); return i; }
int cContainer::addData(string s, vector<ulint> &v) { int i = addData(s, D_ULINT); set(i, v); return i; }
int cContainer::addData(string s, vector<float> &v) { int i = addData(s, D_FLOAT); set(i, v); return i; }
int cContainer::addData(string s, vector<double> &v) { int i = addData(s, D_DOUBLE); set(i, v); return i; }
int cContainer::addData(string s, vector<vec3> &v) { int i = addData(s, D_VEC3); set(i, v); return i; }
int cContainer::addData(string s, vector<string> &v) { int i = addData(s, D_STRING); set(i, v); return i; }
// setting an array
void cContainer::set(int d, vector<Byte> &v) { if(idOk(d)) data[d].set(v); }
void cContainer::set(int d, vector<int> &v) { if(idOk(d)) data[d].set(v); }
void cContainer::set(int d, vector<uint> &v) { if(idOk(d)) data[d].set(v); }
void cContainer::set(int d, vector<ulint> &v) { if(idOk(d)) data[d].set(v); }
void cContainer::set(int d, vector<float> &v) { if(idOk(d)) data[d].set(v); }
void cContainer::set(int d, vector<double> &v) { if(idOk(d)) data[d].set(v); }
void cContainer::set(int d, vector<vec3> &v) { if(idOk(d)) data[d].set(v); }
void cContainer::set(int d, vector<string> &v) { if(idOk(d)) data[d].set(v); }
// setting array by name
void cContainer::set(string s, vector<Byte> &v) { int d = dataId(s); if(idOk(d)) data[d].set(v); }
void cContainer::set(string s, vector<int> &v) { int d = dataId(s); if(idOk(d)) data[d].set(v); }
void cContainer::set(string s, vector<uint> &v) { int d = dataId(s); if(idOk(d)) data[d].set(v); }
void cContainer::set(string s, vector<ulint> &v) { int d = dataId(s); if(idOk(d)) data[d].set(v); }
void cContainer::set(string s, vector<float> &v) { int d = dataId(s); if(idOk(d)) data[d].set(v); }
void cContainer::set(string s, vector<double> &v) { int d = dataId(s); if(idOk(d)) data[d].set(v); }
void cContainer::set(string s, vector<vec3> &v) { int d = dataId(s); if(idOk(d)) data[d].set(v); }
void cContainer::set(string s, vector<string> &v) { int d = dataId(s); if(idOk(d)) data[d].set(v); }
// append an array
void cContainer::append(int d, vector<Byte> &v) { if(data[d].isType(D_BYTE)) data[d].append(v); }
void cContainer::append(int d, vector<int> &v) { if(data[d].isType(D_INT)) data[d].append(v); }
void cContainer::append(int d, vector<uint> &v) { if(data[d].isType(D_UINT)) data[d].append(v); }
void cContainer::append(int d, vector<ulint> &v) { if(data[d].isType(D_ULINT)) data[d].append(v); }
void cContainer::append(int d, vector<float> &v) { if(data[d].isType(D_FLOAT)) data[d].append(v); }
void cContainer::append(int d, vector<double> &v) { if(data[d].isType(D_DOUBLE)) data[d].append(v); }
void cContainer::append(int d, vector<vec3> &v) { if(data[d].isType(D_VEC3)) data[d].append(v); }
void cContainer::append(int d, vector<string> &v) { if(data[d].isType(D_STRING)) data[d].append(v); }
// getting array
void cContainer::get(int d, vector<Byte> &v) const { if(idOk(d)) data[d].get(v); }
void cContainer::get(int d, vector<int> &v) const { if(idOk(d)) data[d].get(v); }
void cContainer::get(int d, vector<uint> &v) const { if(idOk(d)) data[d].get(v); }
void cContainer::get(int d, vector<ulint> &v) const { if(idOk(d)) data[d].get(v); }
void cContainer::get(int d, vector<float> &v) const { if(idOk(d)) data[d].get(v); }
void cContainer::get(int d, vector<double> &v) const { if(idOk(d)) data[d].get(v); }
void cContainer::get(int d, vector<vec3> &v) const { if(idOk(d)) data[d].get(v); }
void cContainer::get(int d, vector<string> &v) const { if(idOk(d)) data[d].get(v); }
// getting array by name
void cContainer::get(string s, vector<Byte> &v) const { int d = dataId(s); if(idOk(d)) data[d].get(v); }
void cContainer::get(string s, vector<int> &v) const { int d = dataId(s); if(idOk(d)) data[d].get(v); }
void cContainer::get(string s, vector<uint> &v) const { int d = dataId(s); if(idOk(d)) data[d].get(v); }
void cContainer::get(string s, vector<ulint> &v) const { int d = dataId(s); if(idOk(d)) data[d].get(v); }
void cContainer::get(string s, vector<float> &v) const { int d = dataId(s); if(idOk(d)) data[d].get(v); }
void cContainer::get(string s, vector<double> &v) const { int d = dataId(s); if(idOk(d)) data[d].get(v); }
void cContainer::get(string s, vector<vec3> &v) const { int d = dataId(s); if(idOk(d)) data[d].get(v); }
void cContainer::get(string s, vector<string> &v) const { int d = dataId(s); if(idOk(d)) data[d].get(v); }
// add a single value to a range(index correction)
void cContainer::sum(int d, int s, int e, Byte v) { if(idOk(d)) data[d].sum(s, e, v); }
void cContainer::sum(int d, int s, int e, int v) { if(idOk(d)) data[d].sum(s, e, v); }
void cContainer::sum(int d, int s, int e, uint v) { if(idOk(d)) data[d].sum(s, e, v); }
void cContainer::sum(int d, int s, int e, ulint v) { if(idOk(d)) data[d].sum(s, e, v); }
void cContainer::sum(int d, int s, int e, float v) { if(idOk(d)) data[d].sum(s, e, v); }
void cContainer::sum(int d, int s, int e, double v) { if(idOk(d)) data[d].sum(s, e, v); }
void cContainer::sum(int d, int s, int e, vec3 v) { if(idOk(d)) data[d].sum(s, e, v); }


// --== SINGLE VALUE operations ==--
// set single value
void cContainer::set(int d, int idx, Byte v) { data[d].set(idx, v); }
void cContainer::set(int d, int idx, int v) { data[d].set(idx, v); }
void cContainer::set(int d, int idx, uint v) { data[d].set(idx, v); }
void cContainer::set(int d, int idx, ulint v) { data[d].set(idx, v); }
void cContainer::set(int d, int idx, float v) { data[d].set(idx, v); }
void cContainer::set(int d, int idx, double v) { data[d].set(idx, v); }
void cContainer::set(int d, int idx, vec3 v) { data[d].set(idx, v); }
void cContainer::set(int d, int idx, string v) { data[d].set(idx, v); }
// add single value to the end of array
void cContainer::add(int d, Byte v) { data[d].add(v); }
void cContainer::add(int d, int v) { data[d].add(v); }
void cContainer::add(int d, uint v) { data[d].add(v); }
void cContainer::add(int d, ulint v) { data[d].add(v); }
void cContainer::add(int d, float v) { data[d].add(v); }
void cContainer::add(int d, double v) { data[d].add(v); }
void cContainer::add(int d, vec3 v) { data[d].add(v); }
void cContainer::add(int d, string v) { data[d].add(v); }
// get single value
void cContainer::get(int d, int idx, Byte &v) const { data[d].get(idx, v); }
void cContainer::get(int d, int idx, int &v) const { data[d].get(idx, v); }
void cContainer::get(int d, int idx, uint &v) const { data[d].get(idx, v); }
void cContainer::get(int d, int idx, ulint &v) const { data[d].get(idx, v); }
void cContainer::get(int d, int idx, float &v) const { data[d].get(idx, v); }
void cContainer::get(int d, int idx, double &v) const { data[d].get(idx, v); }
void cContainer::get(int d, int idx, vec3 &v) const { data[d].get(idx, v); }
void cContainer::get(int d, int idx, string &v) const { data[d].get(idx, v); }



cContainer& cContainer::operator=(const cContainer &b)
{
	if(this == &b) return(*this);
	_signature = b._signature;
	_checksum = b._checksum;
	data = b.data;
	return(*this);
}

// need to do something with checksum upon + and +=
// maybe recalculate
cContainer cContainer::operator+(cContainer &b)
{
	cContainer r = *this;
	r.append(b);
	return(r);
}

cContainer& cContainer::operator+=(cContainer &b)
{
	this->append(b);
	return(*this);
}

//--- Packing
int cContainer::packedSize() const
{
	int h_sz = 0, d_sz = 0, sz;
	// dumpLen +    sigLen + _signature +        checksum + numRecs + dataRec offsets
	h_sz = int_sz + int_sz + _signature.size() + ulint_sz + int_sz + data.size()*int_sz;
	loop(size()) d_sz += data[i].packedSize();
	sz = h_sz + d_sz;
	return(sz);
}

Byte* cContainer::pack(int &sz) const	// allocates memory and packs
{
	sz = packedSize();
	if(!sz) {return(NULL);}
	Byte *dump = new Byte[packedSize()];
	if(dump == NULL) {sz = 0; return(NULL);}	// if memory error - return 0 - do something later with error processing
	sz = pack(dump);
	return(dump);
}


int cContainer::pack(Byte *dump) const	// packing into preallocated memory, returning packed size
{
	int sz, cPtr = 0, offsets = 0;
	if(dump == NULL) {return(0);}	// if we got null *ptr - do nothing, return zero incrment
	sz = packedSize();
	cPtr += write(dump+cPtr, sz);				// dumpLen
	cPtr += write(dump+cPtr, _signature);		// signature
	cPtr += write(dump+cPtr, _checksum);		// _checksum
	cPtr += write(dump+cPtr, (int)data.size());	// numRecs - it should be INT
	offsets = cPtr;
	cPtr += size() * int_sz;
	loop(size())
	{
		write(dump+offsets+(i*uint_sz), cPtr);	// writing offset to data dump
		cPtr += data[i].pack(dump+cPtr);		// writing packed dump
	}
	return(cPtr);
}

int cContainer::unpack(Byte* dump)
{
	clear();	// always clearing to be sure we have all the dynamic dataRecs initialized
	int sz, cPtr = 0, offsets, ofs;
	string sig;
	cPtr += read(dump+cPtr, sz);		// dumpLen
	cPtr += read(dump+cPtr, sig);		// signature
	if (sig != _signature) return(cStatus::sig_error);
	cPtr += read(dump+cPtr, _checksum);	// _checksum
	cPtr += read(dump+cPtr, sz);		// numRecs
	data.resize(sz);
	offsets = cPtr;
	cPtr += sz * int_sz;
	loop(sz)
	{
		read(dump+offsets+(i*int_sz), ofs);
		data[i].unpack(dump+ofs);
	}
	return(cStatus::ok);
}
