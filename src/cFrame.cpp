#include "cFrame.h"
using namespace std;

//--- constructors
cFrame::cFrame()
{
	clear();
}

cFrame::cFrame(const cFrame &b)
{
	*this = b;
}

cFrame::cFrame(Byte* b)
{
	unpack(b);
}

cFrame::~cFrame()
{
	clear();
}


void cFrame::clear()
{
	data.clear();
	_signature = "cFrame";
	nullMesh.addData("cFrame(): index bug in at()", D_INT, 0);
}

int cFrame::size() const
{
	return data.size();
}

void cFrame::resize(int sz)
{
	data.resize(sz);
}

void cFrame::add(const cMesh &m)
{
	data.push_back(m);
}

void cFrame::erase(int d)
{
	if(d < (int)data.size()) data.erase(data.begin()+d);
}

void cFrame::print() const
{
	COUT_SET
	cout << *this;
}

ostream& operator<<(std::ostream& ostr, const cFrame& b)
{
	OSTR_SET
	ostr << b._signature << ": " << b.size() << " elements, " << b.i2s(b.packedSize()) << " Bytes packed" << endl;
	loop(b.size()) ostr << b.data[i];
	return ostr;
}

void cFrame::printDebug() const
{
	COUT_SET
	cout << _signature << ": " << size() << " elements, " << i2s(packedSize()) << " Bytes packed" << endl;
	loop(size()) data[i].printDebug();
}

// filters out and join cMeshes from cFrame
void cFrame::joinMeshes(cMesh &m)
{
	m.clear();
	loop(size())
	{
		if( at(i).dataExist("meshType") )
		{
			m.append(at(i));
		}
	}
	m.optimizeMats();
}


cMesh& cFrame::at(int d)
{
	if(d < size()) return data.at(d); else return nullMesh;
}

// operators
cFrame& cFrame::operator=(const cFrame &b)
{
	if(this == &b) return(*this);
	data = b.data;
	return(*this);
}

void cFrame::append(cFrame &b)
{
	data.insert( data.end(), b.data.begin(), b.data.end() );
}

// need to do something with checksum upon + and +=
cFrame cFrame::operator+(cFrame &b)
{
	cFrame r = *this;
	append(b);
	return(r);
}

cFrame& cFrame::operator+=(cFrame &b)
{
	this->append(b);
	return(*this);
}


//--- Packing
int cFrame::packedSize() const
{
	int h_sz = 0, d_sz = 0, sz;
	// dumpLen +     sigLen +  _signature +      numRecs + cContainer offsets
	h_sz = int_sz + int_sz + _signature.size() + int_sz + data.size()*int_sz;
	loop(size()) d_sz += data[i].packedSize();
	sz = h_sz + d_sz;
	return(sz);
}


int cFrame::pack(Byte *dump)	// packing into preallocated memory, returning packed size
{
	if(dump == NULL) {return(0);}	// if we got null *ptr - do nothing, return zero incrment
	int sz, offsets, cPtr = 0;
	sz = packedSize();
	cPtr += write(dump+cPtr, sz);				// dumpLen
	cPtr += write(dump+cPtr, _signature);		// signature
	cPtr += write(dump+cPtr, (int)data.size());	// numRecs
	offsets = cPtr;
	cPtr += size() * int_sz;
	loop(size())
	{
		write(dump+offsets+(i*uint_sz), cPtr);	// writing offset to data dump
		cPtr += data[i].pack(dump+cPtr);		// writing packed dump
	}
	return(cPtr);
}

Byte* cFrame::pack(int &sz)		// allocates memory and packs
{
	sz = packedSize();
	if(!sz) {return(NULL);}
	Byte *dump = new Byte[packedSize()];
	if(dump == NULL) {sz = 0; return(NULL);}	// if memory error - return 0 - do something later with error processing
	sz = pack(dump);
	return(dump);
}




int cFrame::unpack(Byte* dump)
{
	clear();	// always clearing to be sure we have all the dynamic dataRecs initialized
	int sz, cPtr = 0, offsets, ofs;
	string sig;
	cPtr += read(dump+cPtr, sz);		// dumpLen
	cPtr += read(dump+cPtr, sig);		// signature
	if (sig != _signature) return(cStatus::sig_error);
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
