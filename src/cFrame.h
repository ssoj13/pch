/*
	TODO: Refactor to cContainer
*/

#ifndef _cFrame
#define _cFrame
#include "core_defs.h"
#include "cUtil.h"
#include "cMesh.h"
using namespace std;

class cFrame: public cUtil
{
	public:
		cFrame();
		cFrame(const cFrame&);
		cFrame(Byte*);
		~cFrame();

		void clear();

		int size() const;
		void resize(int);
		void add(const cMesh&);
		void erase(int);
		void print() const;
		void printDebug() const;
		friend std::ostream& operator<<(std::ostream&, const cFrame&);

		void joinMeshes(cMesh&);
		cMesh& at(int);
		void append(cFrame &b);
		cFrame& operator=(const cFrame&);
		cFrame operator+(cFrame&);
		cFrame& operator+=(cFrame &b);

		// packing
		int packedSize() const;
		Byte* pack(int&);	// allocates memory and returns *ptr and size of dump
		int pack(Byte*);	// packs right into preallocated memory, returns Bytes written(cPtr)
		int unpack(Byte*);	// returns unpacked Bytes
	protected:
		string _signature;
		vector<cMesh> data;
		cMesh nullMesh;
};

#endif
