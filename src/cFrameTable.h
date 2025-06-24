/*
	Basically it's just a cContainer with 3 functions to add/set/get frameData
*/

#ifndef _cFrameTable
#define _cFrameTable
#include "core_defs.h"
#include "cContainer.h"
#include "cMesh.h"
#include "cUtil.h"

using namespace std;

class cFrameTable: public cContainer
{
	public:
		cFrameTable();
		cFrameTable(cFrameTable&);
		cFrameTable(Byte*);
		~cFrameTable();

		virtual void initSig();
		virtual void clearData();

		/*
		ulint frameOffset
		int FRAME_MESH
		mayaFrame
		cacheFrame
		lastBaseFrame
		compression
		V
		E
		F
		T
		UV
		checksum
        */
        int numFrames();
		void addFrame(ulint, int, int, int, int, int, int, int, int, ulint);
		void setFrame(int, ulint, int, int, int, int, int, int, int, int, ulint);
		void getFrame(int, ulint&, int&, int&, int&, int&, int&, int&, int&, int&, ulint&);
};

#endif