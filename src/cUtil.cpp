// cUtil is a base class to provide unified memory/file IO/some service functions for instanced classes

#include "cUtil.h"
using namespace std;

//returns dump size(first uint)
int cUtil::mem_info(const Byte* ptr) const
{
	int ofs;
	return( read(ptr, ofs) );
}


// TODO zlib funcs
Byte* cUtil::mem_compress(Byte* ptr) const
{
	return(ptr);
}

Byte* cUtil::mem_decompress(Byte* ptr) const
{
	return(ptr);
}

//--- Delete data
void cUtil::delete_Byte(Byte* b)
{
	if(NULL != b)
	{
		delete [] b;
		b = NULL;
	}
}

// cyclic shifts
// ROtate Right
uint cUtil::ror(uint v, uint bits)
{
	uint r = v >> bits | v << (UINT_BITS-bits);
	return(r);
}
// ROtate Left
uint cUtil::rol(uint v, uint bits)
{
	uint r = v << bits | v >> (UINT_BITS-bits);
	return(r);
}

// numbers conversion
std::string cUtil::i2s(int i)
{
	stringstream s;
	s << i;
	return s.str();
}

int cUtil::s2i(std::string s)
{
	return stoi(s);
}

void cUtil::toClip(const string &s)
{
	OpenClipboard(0);
	EmptyClipboard();	
	HGLOBAL hg=GlobalAlloc(GMEM_MOVEABLE,s.size());
	if (!hg)
	{
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg),s.c_str(),s.size());
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT,hg);
	CloseClipboard();
	GlobalFree(hg);
}

// debug
void cUtil::dtrace()
{
	#ifdef _DEBUG
		cout << __FILE__ << ", line " << __LINE__ << ": " << __FUNCTION__ << endl;
	#endif
}
