#include "cMesh.h"
using namespace std;

//--- constructors
cMesh::cMesh()
{
	clear();
}

cMesh::cMesh(const cMesh &b)
{
    *this = b;
}

cMesh::cMesh(Byte* b)
{
	unpack(b);
}

cMesh::~cMesh()
{
	clear();
}

void cMesh::initSig()
{
	_signature = "cMesh";
}

void cMesh::clearData()
{
	cContainer::clearData();
	setMeshType(MESH_FULL);
	nullRec.setName("cMesh(): index bug in at()");
}

void cMesh::setMeshType(int t)
{
	int id = dataId(S_MESHTYPE);
	if(!idOk(id)) { id = addData(S_MESHTYPE, D_INT, 0); }
	add(id, t);
}

// adds one object to the list of objects with name, faceStart and faceEnd
// if we gonna use references here, we wont get automatic conversion of "explicit" arguments to string;
void cMesh::addObject(string obj, int os, int oe)
{
	int idO, idS, idE;
	idO = dataId(S_OBJS);
	if(!idOk(idO)) idO = addData(S_OBJS, D_STRING);
	add(idO, obj);

	idS = dataId(S_OBJ_S);
	if(!idOk(idS)) idS = addData(S_OBJ_S, D_INT);
	add(idS, os);

	idE = dataId(S_OBJ_E);
	if(!idOk(idE)) idE = addData(S_OBJ_E, D_INT);
	add(idE, oe);
}

// adds new object consists of all faces in object
void cMesh::addAutoObject()
{
	int idO = dataId(S_OBJS);
	if(idOk(idO))
	{
		string nm = "obj_" + cUtil::i2s(dataSize(idO)-1);
		addAutoObject(nm);
	}
}

// adds new object consists of all faces in object
void cMesh::addAutoObject(string nm)
{
	int idFC = dataId(S_FCOUNTS);
	if(idOk(idFC)) addObject(nm, 0, dataSize(idFC)-1);
}

void cMesh::optimizeMats()
{
	int matId = dataId(S_MATS);
	int fmId = dataId(S_FACEMATS);
	if(!idOk(matId) || dataSize(matId) < 2 || !idOk(fmId) || !dataSize(fmId)) return;

	string sg, empty = "";
	vector<int> from, to;
	vector<string> mats, newMats;

	get(matId, mats);
	// finding duplicate materials and their numbers
	loop((int)mats.size())
	{
		if(mats[i] == empty) continue;
		sg = mats[i];
		newMats.push_back(sg);
		int curMat = newMats.size()-1;
		// finding all matches starting from itself
		loop2((int)mats.size())
		{
			if(mats[j] == empty) continue;
			if(mats[j] == sg)
			{
				mats[j] = empty;
				from.push_back(j);
				to.push_back(curMat);
			}
		}
		mats[i] = empty;
	}
	// setting compressed material names
	set(matId, newMats);

	vector<int> fm;
	get(fmId, fm);
	// and replacing their numbers
	loop((int)fm.size())
	{
		loop2((int)from.size())
		{
			if(fm[i] == from[j])
			{
				fm[i] = to[j];
				break;
			}
		}
	}
	set(fmId, fm);
}


// --=== overloads ===--
// append is used by operator+
// by default merges absent data fields
// doesn't merges different meshTypes

// TODO: create universal append(): check the list of datas and append all of them in loop,
// applying special appends for special types of data like faceVerts, etc
void cMesh::append(cMesh &b)
{
	string d;
	int id1, id2;
	int start, ofs, mTypeA, mTypeB;

	d = S_MESHTYPE;	// we ALWAYS have meshType in cMesh class
	get(dataId(S_MESHTYPE), 0, mTypeA);
	b.get(b.dataId(S_MESHTYPE), 0, mTypeB);
	// if it's different meshTypes - do nothing
	if(mTypeA != mTypeB) return;

	// objs - mesh_full, mesh_geo_uv, mesh_geo, mesh_pts
	// geo - mesh_full, mesh_geo_uv, mesh_geo, mesh_pts
	// uvs - mesh_full, mesh_geo_uv
	// normals - mesh_full
	// mats - mesh_full, mesh_geo_uv, mesh_geo
	// matrix???
	switch(mTypeA)
	{
		case MESH_FULL:
		{
			// objs: appending first because next step we gonna overwrite faceCounts on polys append
			d = S_OBJS;
			start = dataSize(d);
			ofs = dataSize(S_FCOUNTS);
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }
			d = S_OBJ_S;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); data.at(id1).sum(start, dataSize(id1), (int)ofs); } // increasing old.Start/End by old.faceCount
			d = S_OBJ_E;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); data.at(id1).sum(start, dataSize(id1), (int)ofs); } // watch "ofs" type, since it may call wrong .sum(d_type) method

			// geo
			d = S_PTS;
			start = dataSize(S_FCONNECTS);
			ofs = dataSize(d);
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }
			d = S_FCONNECTS;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); data.at(id1).sum(start, dataSize(id1), (uint)ofs);}
			d = S_FCOUNTS;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }

			// UVs
			start = dataSize(S_UVCONNECTS);
			ofs = dataSize(S_U);
			d = S_U;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }
			d = S_V;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }
			d = S_UVCONNECTS;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); data.at(id1).sum(start, dataSize(id1), (uint)ofs);} // fixing uvConnects: from old uvConnects.size to new uvConnects.size: add old UV.size to all links
			d = S_UVCOUNTS;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }
			d = S_UVSETS;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }

			// normals
			d = S_NORMALS;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }

			// mats
			start = dataSize(S_FACEMATS);
			ofs = dataSize(S_MATS);
			d = S_MATS;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }
			d = S_FACEMATS;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); data.at(id1).sum(start, dataSize(id1), (int)ofs);}
			break;
		}
		case MESH_GEO_UV:
			{
			// objs: appending first because next step we gonna overwrite faceCounts on polys append
			d = S_OBJS;
			start = dataSize(d);
			ofs = dataSize(S_FCOUNTS);
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }
			d = S_OBJ_S;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); data.at(id1).sum(start, dataSize(id1), (uint)ofs); } // increasing old.Start/End by old.faceCount
			d = S_OBJ_E;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); data.at(id1).sum(start, dataSize(id1), (uint)ofs); } // watch "ofs" type, since it may call wrong .sum(d_type) method

			// geo
			d = S_PTS;
			start = dataSize(S_FCONNECTS);
			ofs = dataSize(d);
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }
			d = S_FCONNECTS;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); data.at(id1).sum(start, dataSize(id1), (uint)ofs);}
			d = S_FCOUNTS;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }

			// UVs
			start = dataSize(S_UVCONNECTS);
			ofs = dataSize(S_U);
			d = S_U;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }
			d = S_V;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }
			d = S_UVCONNECTS;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); data.at(id1).sum(start, dataSize(id1), (uint)ofs);} // fixing uvConnects: from old uvConnects.size to new uvConnects.size: add old UV.size to all links
			d = S_UVCOUNTS;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }
			d = S_UVSETS;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }
			break;
		}
		case MESH_GEO:
		{
			// objs: appending first because next step we gonna overwrite faceCounts on polys append
			d = S_OBJS;
			start = dataSize(d);
			ofs = dataSize(S_FCOUNTS);
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }
			d = S_OBJ_S;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); data.at(id1).sum(start, dataSize(id1), (uint)ofs); } // increasing old.Start/End by old.faceCount
			d = S_OBJ_E;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); data.at(id1).sum(start, dataSize(id1), (uint)ofs); } // watch "ofs" type, since it may call wrong .sum(d_type) method

			// geo
			d = S_PTS;
			start = dataSize(S_FCONNECTS);
			ofs = dataSize(d);
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }
			d = S_FCONNECTS;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); data.at(id1).sum(start, dataSize(id1), (uint)ofs);}
			d = S_FCOUNTS;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }
			break;
		}
		case MESH_PTS:
		{
			// geo
			d = S_PTS;
			id1 = dataId(d); id2 = b.dataId(d);
			if(!idOk(id1) && b.idOk(id2)) { addData(b.at(id2)); } else if(idOk(id1) && b.idOk(id2)) { data.at(id1).append(b.at(id2)); }
		}
	}// END Switch

}// END appendMesh()


cMesh cMesh::slice(int, int)
{
	cMesh m;
	return(m);
}


/// riddle array here is of the size(faces) length, with 0/1 components
/// faces with 1 in riddle[face] are copied while faces with 0 are out
void cMesh::riddleCopy(const cMesh &a, cMesh &b, vector<int> &riddle)
{
	int idP = a.dataId(S_PTS);
	int idFCnt = a.dataId(S_FCOUNTS);
	int idFConn = a.dataId(S_FCONNECTS);
	int numFaces = a.dataSize(idFCnt);
	if( !idOk(idP) || !idOk(idFCnt) || !idOk(idFConn) || ((int)riddle.size() < numFaces) ) return;

	vector<vec3> aPts, bPts;
	vector<uint> aFCounts, aFConnects, bFCounts, bFConnects;
	a.get(idP, aPts);
	a.get(idP, aFCounts);
	a.get(idP, aFConnects);

	vector<int> cpFlag(aPts.size(), -1);	// flags for copied vertices
	int conn = 0;	// index in fConnects
	loop(numFaces)
	{
		if(riddle[i]) //COPY FACE
		{
				loop2((int)aFCounts[i])	// iterating face points
				{
					int ptIndex = aFConnects[conn];	// point index in main array
					if(cpFlag[ptIndex] == -1)		// if point is not copied yet:
					{
						int newIndex = bPts.size();		// index of a point in new array
						bFConnects.push_back(newIndex);	// adding it to bfconnects
						cpFlag[ptIndex] = newIndex;		// setting that index in cpFlags
						bPts.push_back(aPts[ptIndex]);	// and adding point itself
					}
					else { bFConnects.push_back( cpFlag[ptIndex] ); } // just adding point in new array to bfconnects
					conn++;
				}
				bFCounts.push_back(aFCounts[i]);
		}
		else { conn += aFCounts[i]; } // NOT COPY FACE
	}
	b.clear();
	b.setMeshType(MESH_GEO);
	b.addData(S_PTS, bPts);
	b.addData(S_FCOUNTS, bFCounts);
	b.addData(S_FCONNECTS, bFConnects);
}

void cMesh::checkNanPts(vector<int> &nanPts)
{
	nanPts.clear();	
	int id = dataId(S_PTS);
	if(!idOk(id)) return;
	vector<vec3> pts;
	get(id, pts);
	loop((int)pts.size())
	{
		if( isNaN(pts[i].x()) || isNaN(pts[i].y()) || isNaN(pts[i].z()) ) nanPts.push_back(i);
	}
}
