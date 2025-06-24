#ifndef _core_defs
#define _core_defs
#pragma warning(disable:4267)	// possible loss of data in size_t conversion
#pragma warning(disable:4996)	// fopen unsafe

//#include <inttypes.h>
#include <stdint.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>		// arrays
#include <sstream>		// stringstream
#include <cmath>		// sqrt(), pow()
#include <algorithm>	// std::copy()
#include <typeinfo>		// typeid(T)
#include <ctime>		// time()
#include <windows.h>

#ifndef COUT_SET
	#define COUT_SET {cout.setf(ios::fixed); cout.precision(3);}
	#define OSTR_SET {ostr.setf(ios::fixed); ostr.precision(3);}
	// constants
	#define PI 3.1415926535
	#define _KB 1024
	#define _MB 1048576
	#define _GB 1073741824
#endif

// signature and version
#define PCH_VERSION 0.3f
#define PCH_SIGNATURE "JossPCH"

// cMesh constants
#define MESH_FULL (int)0		// all dataRecs are used
#define MESH_GEO_UV (int)10		// faces/pts/uvs
#define MESH_GEO (int)20		// faces/pts
#define MESH_PTS (int)30		// just pts
#define MESH_PTS_DELTA (int)40	// just pts
#define MESH_MATRIX (int)50		// delta matrix

// types
// size_t is usually "unsigned int"
typedef unsigned char Byte;
typedef signed short int int16;
typedef unsigned short int uint16;
typedef unsigned int uint;				// __int32, 4 Bytes, –2,147,483,648 to 2,147,483,647
//typedef long long int llint;			// __int64, 8 Bytes, –9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
typedef unsigned long long int ulint;	// unsigned __int64, 8 Bytes, 0 to 18,446,744,073,709,551,615
typedef unsigned __int64 uint64;

#define UINT_BITS sizeof(uint) * sizeof(Byte)
#define ULINT_BITS sizeof(ulint) * sizeof(Byte)

union uVar
{
	Byte b;
	int i;
	uint ui;
	ulint ul;
	float f;
	double d;
};


// sizes
#define Byte_sz sizeof(Byte)
#define int_sz sizeof(int)
#define uint_sz sizeof(uint)
//#define llint_sz sizeof(llint)
#define ulint_sz sizeof(ulint)
#define float_sz sizeof(float)
#define double_sz sizeof(double)

#define loop(n) for(int i=0; i<n; i++)
#define loop2(n) for(int j=0; j<n; j++)
#define loop3(n) for(int k=0; k<n; k++)
#define loop12(s, e) for(int i=s; i<e; i++)
#define loopu(n) for(uint i=0; i<n; i++)

// debug
#define _debug #ifdef _DEBUG cout << __FUNCTION__ << endl; #endif

#endif
