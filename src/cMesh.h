#ifndef _cMesh
#define _cMesh
#include "core_defs.h"
#include "cContainer.h"
using namespace std;

/*
//TODO: mapping of faces into pts array
//probably worth to implement face-centric scheme
// where mesh is just a list of faces with links to internal points array

//meshType: base, vertex, deltaVertex, matrix
//matrix

//geo
	p
	faceCounts
	faceConnects

//uv
	uvSets
	uvConnects
	uvCounts
	u
	v

//normals
	normals

//mats
	mat
	faceMats

//TODO: remove objs since objects are split by classes now
//objs
	objs
	objS
	objE
*/

#define S_MESHTYPE "meshType"			// int
#define S_PTS "p"						// vec3
#define S_FCOUNTS "faceCounts"			// uint
#define S_FCONNECTS "faceConnects"		// uint
#define S_OBJS "objs"					// string
#define S_OBJ_S "obj_s"					// int
#define S_OBJ_E "obj_e"					// int
#define S_U "u"							// double
#define S_V "v"							// double
#define S_UVCONNECTS "uvConnects"		// uint
#define S_UVCOUNTS "uvCounts"			// uint
#define S_UVSETS "uvSets"				// string
#define S_NORMALS "normals"				// vec3
#define S_MATS "mats"					// string
#define S_FACEMATS "faceMats"			// int
#define S_MATRIX "matrix"				// matrix

class cMesh: public cContainer
{
	public:
		cMesh();
		cMesh(const cMesh&);
		cMesh(Byte*);
		~cMesh();

		virtual void initSig();
		virtual void clearData();
		void setMeshType(int);

		// i don't need any special methods to get/set data fields now
		// providing just some methods _critical_ for meshes
		// mats
		void addObject(string, int, int);
		void addAutoObject();
		void addAutoObject(string);

		//--- manipulations
		void optimizeMats();
		// append is used by operator+
		virtual void append(cMesh&);
		
		/// riddle array here is of the size(faces) length, with 0/1 components
		/// faces with 1 in riddle[face] are copied while faces with 0 are out
		void riddleCopy(const cMesh&, cMesh&, vector<int>&);
		cMesh slice(int, int);

		void checkNanPts(vector<int>&);
};

#endif
