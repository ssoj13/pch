#include "cFrameTable.h"

//--- Constructors
cFrameTable::cFrameTable()
{
	clear();
}

cFrameTable::cFrameTable(cFrameTable &other)
{
	clear();
	data = other.data;
}

cFrameTable::cFrameTable(Byte* dump)
{
	clear();
	unpack(dump);
}

cFrameTable::~cFrameTable()
{
	clear();
}

void cFrameTable::initSig()
{
	_signature = "cFrameTable";
}

void cFrameTable::clearData()
{
	cContainer::clearData();
	addData("offset", D_ULINT);
	addData("mayaFrame", D_INT);
	addData("cacheFrame", D_INT);
	addData("baseFrame", D_INT);
	addData("compressed", D_INT);
	addData(S_V, D_INT);
	addData("e", D_INT);
	addData("f", D_INT);
	addData("uv", D_INT);
	addData("checksum", D_ULINT);
}




//--- member funcs
int cFrameTable::numFrames()
{
	int num = 0;
	int idx = dataId("offset");
	if(idOk(idx)) num = data[idx].size();
	return num;
}

void cFrameTable::addFrame(ulint offs, int mFrame, int cFrame, int bFrame, int comp, int v, int e, int f, int uv, ulint csum)
{
	// in theory there should be no situation when these fields is not initialized, but who knows...
	data[dataId("offset")].add((ulint)offs);
	data[dataId("mayaFrame")].add((int)mFrame);
	data[dataId("cacheFrame")].add((int)cFrame);
	data[dataId("baseFrame")].add((int)bFrame);
	data[dataId("compressed")].add((int)comp);
	data[dataId(S_V)].add((int)v);
	data[dataId("e")].add((int)e);
	data[dataId("f")].add((int)f);
	data[dataId("uv")].add((int)uv);
	data[dataId("checksum")].add((ulint)csum);
}

void cFrameTable::setFrame(int i, ulint offs, int mFrame, int cFrame, int bFrame, int comp, int v, int e, int f, int uv, ulint csum)
{
	if(i < dataSize("offset"))
	{
		data[dataId("offset")].set(i, (ulint)offs);
		data[dataId("mayaFrame")].set(i, (int)mFrame);
		data[dataId("cacheFrame")].set(i, (int)cFrame);
		data[dataId("baseFrame")].set(i, (int)bFrame);
		data[dataId("compressed")].set(i, (int)comp);
		data[dataId(S_V)].set(i, (int)v);
		data[dataId("e")].set(i, (int)e);
		data[dataId("f")].set(i, (int)f);
		data[dataId("uv")].set(i, (int)uv);
		data[dataId("checksum")].set(i, (ulint)csum);
	}
}

void cFrameTable::getFrame(int i, ulint &offs, int &mFrame, int &cFrame, int &bFrame, int &comp, int &v, int &e, int &f, int &uv, ulint &csum)
{
	if(i < dataSize("offset"))
	{
		data[dataId("offset")].get(i, offs);
		data[dataId("mayaFrame")].get(i, mFrame);
		data[dataId("cacheFrame")].get(i, cFrame);
		data[dataId("baseFrame")].get(i, bFrame);
		data[dataId("compressed")].get(i, comp);
		data[dataId(S_V)].get(i, v);
		data[dataId("e")].get(i, e);
		data[dataId("f")].get(i, f);
		data[dataId("uv")].get(i, uv);
		data[dataId("checksum")].get(i, csum);
	}
}

/*
int cFrameTable::searchMayaFrame(int mayaFrame, int type)
{
	int n, r = 0;
	for(uint i=0; i<size(); i++)
	{
		data[mayaFrame].get(i, n);
		if(n == mayaFrame)
		{
			data[cacheFrame].get(i, r);
			break;
		}
	}
	return r;
}
*/
