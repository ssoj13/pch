#include "core_classes.h"
#include "core_defs.h"


string rndStr(int length, uint seed)
{
	static string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	string result;
	result.resize(length);

	srand(seed);
	for (int i = 0; i < length; i++) result[i] = charset[rand() % charset.length()];
	return result;
}

int cioTest()
{
	cIO io;
	cUtil mops;
	cStatus stat;

	io.setName("c:/temp/aaaaaaaaa.tmp");
    io.setMode(cStatus::f_mode_write);
	stat = io.open();
	stat.msg("opening for read");
	FILE *f = io.getFile();
	stat = mops.writef(f, (uint)01);
	stat.msg("uint_fwrite");
	stat = mops.writef(f, string("huy_pizda_jigurda"));
	stat.msg("string_fwrite");
	stat = mops.writef(f, string("ololo"));
	stat.msg("string_fwrite-2");
	stat = io.close();
	stat.msg();

	io.setMode(cStatus::f_mode_read);
	stat = io.open();
	stat.msg();
	f = io.getFile();
	int t;
	stat = mops.readf(f, t);
	string st;
	stat = mops.readf(f, st);
	stat.msg();
	cout << t << ", " << st << endl;
	stat = mops.readf(f, st);
	stat.msg();
	cout << st << endl;
	return(0);
}

// initial cube mesh
cMesh getTestMesh()
{
	vector<vec3> pts;
	pts.push_back(vec3(-1, -1, -1));
	pts.push_back(vec3(1, -1, -1));
	pts.push_back(vec3(1, -1, 1));
	pts.push_back(vec3(-1, -1, 1));
	pts.push_back(vec3(-1, 1, -1));
	pts.push_back(vec3(1, 1, -1));
	pts.push_back(vec3(1, 1, 1));
	pts.push_back(vec3(-1, 1, 1));
	// facePoints
	vector<uint> fPts;
	fPts.push_back(0);
	fPts.push_back(1);
	fPts.push_back(2);
	fPts.push_back(3);
	//
	fPts.push_back(4);
	fPts.push_back(5);
	fPts.push_back(6);
	fPts.push_back(7);
	//
	fPts.push_back(1);
	fPts.push_back(2);
	fPts.push_back(6);
	fPts.push_back(5);
	//
	fPts.push_back(3);
	fPts.push_back(0);
	fPts.push_back(4);
	fPts.push_back(7);
	//
	fPts.push_back(0);
	fPts.push_back(1);
	fPts.push_back(4);
	fPts.push_back(5);
	//
	fPts.push_back(2);
	fPts.push_back(3);
	fPts.push_back(7);
	fPts.push_back(6);
	// faceCounts
	vector<uint> fCnt;
	fCnt.push_back(4);
	fCnt.push_back(4);
	fCnt.push_back(4);
	fCnt.push_back(4);
	fCnt.push_back(4);
	fCnt.push_back(4);
	// objs
	vector<string> objs;
	vector<int> os, oe;
	objs.push_back("cube1");
	os.push_back(0);
	oe.push_back(7);

	cMesh m;
	m.addData(S_PTS, D_VEC3);
	m.set(S_PTS, pts);
	m.addData(S_FCONNECTS, D_UINT);
	m.set(S_FCONNECTS, fPts);
    m.addData(S_FCOUNTS, D_UINT);
    m.set(S_FCOUNTS, fCnt);
	m.addAutoObject();
	return(m);
}

// stack of cubes
cMesh getBigTestMesh(int c)
{
	cMesh m1;
	m1 = getTestMesh();
	loop(c)
	{
		m1 += m1;
	}
	return m1;
}

// main mesh test
cMesh cMeshTest()
{
	cMesh m1;
	m1 = getBigTestMesh(1);
	cout << m1 << endl;
	m1.addAutoObject();
	//m1.print();

	int sz;
	Byte *b = m1.pack(sz);
	//cout << "PACKED:" << endl;
	//cout << "mem_info: " << m.mem_info(b) << endl << endl << endl << endl;

	cMesh m2;
	//cout << "UNPACKING:" << endl;
	m2.unpack(b);
	//m2.print();
	//cout << "UNPACKED:" << endl;
	cout << m2 << endl;

	m2 += m2;
	//m2.print();
	//cout << "INCREMENTED:" << endl;

	return m2;
}

int mesh_cio_test()
{
	cIO io;
	cUtil mops;
	cStatus stat;

	cMesh m1;
	m1 = getBigTestMesh(18);
	cout << m1.packedSize() << endl;
	m1.print();
    int sz;
	Byte *b = m1.pack(sz);

	//---------------------
	io.setName("c:/temp/aaaaaaaaa.tmp");
    io.setMode(cStatus::f_mode_write);
	stat = io.open();
	stat.msg("opening for read");
	FILE *f = io.getFile();
	stat = mops.writef(f, b);
    stat.msg("mem_fwrite");
	stat = io.close();
	stat.msg("fclose");
	delete [] b;

	io.setMode(cStatus::f_mode_read);
	stat = io.open();
	stat.msg("io.open");
	f = io.getFile();
	stat = io.readf(f, b);
	stat.msg("io.fread");
	stat = io.close();
	stat.msg("fclose");

	cMesh in(b);
	in.print();

	return(0);
}

void frameTableTest()
{
	cFrameTable f1, f2;
	f1.addFrame(1000, 13, 174, 1, 0, 100, 50, 30, 12, 1212);
	f1.addFrame(5323212423, 14, 175, -1, 0, 100, 50, 30, 12, 1212);
	f1.print();

	cout << "all f1 data names:" << endl;
	vector<string> l;
	f1.listData(l);
	loop((int)l.size()) cout << l[i] << ", ";
	cout << endl << endl;

	cout << "packing-unpacking" << endl;
	int sz;
	Byte* b = f1.pack(sz);
	f2.unpack(b);
	f2.print();
}

void vec3_test()
{
	vec3 a(0, 0, 0);
	vec3 b(1, 1, 1);
	vec3 c(2, 2, 2);
	vec3 d(21, 72, 32);
	vec3 t = a;
	t.lerp(c, 0.1f);
	t.print();
}

cChecksum cChecksum_test()
{
	cChecksum a;
	static const uint iarr[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	for(uint i=0; i<11; i++)
	{
		a.add(iarr[i]);
		a.print();
	}
	a.print();
	return a;
}

cChecksum cChecksum_test2()
{
	cChecksum a, b;
	for(uint i=0; i<100; i++)
	{
		a.add(i);
		if(i == 5) b.add(17); else b.add(i);
		cout << a.get() << "\t\t" << b.get() << endl;
	}
	b.print();
	return b;
}

void cProfileTest()
{
	cProfile prf;
	prf.start();
	loop(100000000){ulint u = i; u+=u;}
	prf.stop();
	cout.setf(ios::fixed);
	cout.precision(15);
	cout << prf.get();
}

void cContainerTest()
{
	cContainer a, b, c;
	a.addData("name", D_STRING);
	a.addData(S_PTS, D_VEC3);
	a.addData("counts", D_UINT);
	a.addData("conects", D_UINT);

	uint v1 = 13, v2 = 27;
	vec3 p(0, 0, 0);

	int idx = a.dataId("name");
	loop(10) a.add(idx, rndStr(5, 0));
	idx = a.dataId(S_PTS);
	loop(5) a.add(idx, p);
	idx = a.dataId("counts");
	loop(5) a.add(idx, v1);
	idx = a.dataId("conects");
	loop(5) a.add(idx, v2);

	a.print();
	int sz;
	Byte *ptr = a.pack(sz);
	b.unpack(ptr);
	b.print();

	cout << "printing C += A: " <<  endl;
	c += a;
	c.print();
	//c+= a;
	//c+= b;
	c.print();
}

cFrame cFrameTest()
{
	cFrame cf1, cf2;

	cMesh m = cMeshTest();

	cf1.add(m);
	cf1.add(m);
	cf1.printDebug();
	cout << "--== PRINTED cFrame1 ==--" << endl;

	int sz;
	Byte *ptr = cf1.pack(sz);
	cf2.unpack(ptr);
	cf2.printDebug();
	cout << "--== PRINTED cFrame2 ==--" << endl;
	return cf2;
}

void fstreamTest()
{
	ifstream fin;
	ofstream fout;
	fout.open("c:/aaa.log", ios::out | ios::app);
	if(!fout.is_open()) return;
	fout << cFrameTest();
	fout.close();
}

void cMatrixTest()
{
	cMatrix m1, m2, m3;
	cout << m1 << endl;
	m2 = m1;
	m2.set(0, 0, 3.14);
	m2.set(1, 0, 3.14);
	cout << m2 << endl;
	m3 = m1*m2;
	cout << m3 << endl;
}

void cMatrixMultTest()
{
	cMatrix m1, m2, m3;
	vec3 v(1, 0, 0);
	v *= m1;
	cout << v;
}

void cListTest()
{
	cList a("asdg", D_MATRIX);
	cout << "created d_matrix" << endl;

	loop(3) a.add(matrix());
	cout << "added 3 matrices in loop" << endl;

	cMatrix m1, m2, m3;
	a.get(0, m1);
	m1.set(0, 0, 3.14);
	m1.set(1, 0, 6.28);
	m1.set(2, 2, 1.11);
	a.set(1, m1);
	cout << "changed values in matrix" << endl;
	cout << a << endl;

	cout << "packing cList" << endl;
	int sz;
	Byte *ptr = a.pack(sz);
	cout << "packed" << endl;
	cList b(ptr);
	cout << "creating new cList from memory" << endl;
	cout << b;
}

int main(void)
{
	//cContainerTest();
	//frameTableTest();
	//cMeshTest();
	//cFrameTest();

	//fstreamTest();
	//cMatrixTest();
	cListTest();
	//cMatrixMultTest();

	cList a("pts", (int)10);
	cout << a.packedSize() << endl;

	cout << sizeof(ulint) << endl;

	return 0;
}
