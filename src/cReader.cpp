#include "cReader.h"
using namespace std;

//--- constructors
cReader::cReader()
{
	clear();
}

cReader::cReader(const string s)
{
	clear();
	open(s);
}

cReader::~cReader()
{
	close();
}


void cReader::clear()
{
	baseMesh.clear();
	mesh.clear();
	fat.clear();
	io.clear();

	fileName.clear();
	oldFileName.clear();
	// cache info
	origStartFrame = -1;
	totalFrames = -1;
	lastBaseFrame = -1;
	lastV = -1;
	lastE = -1;
	lastF = -1;
	lastUV = -1;
	lastChecksum = 0;

    // external user params
	setTimeOffset(0.0);
	setTimeScale(1.0);
	setLerp(1);
	setLoopAnim(1);
	cacheInternalTime = 0.0;

	stat = cStatus::ok;
}

ostream& operator<<(std::ostream& ostr, const cReader& b)
{
	OSTR_SET
	ostr << b.fileName << endl;
	return ostr;
}


cStatus cReader::open(const string fn)
{
	stat = cStatus::ok;

	if(fn == oldFileName || !fn.size() ) return cStatus::f_open_read_error;
	clear();
	fileName = fn;

	stat = io.open(fileName, cStatus::f_mode_read);
	if(stat.isError()) { return stat; }
	stat = io.read_header();
	if(stat.isError()) { return stat; }
	stat = io.checkSignature();
	if( stat.isError() ) { return cStatus::f_wrong_signature; }
	stat = io.checkVersion();
	if( stat.isError() ) { return cStatus::f_wrong_version; }
	stat = io.read_FAT(fat);
	if(stat.isError()) { return cStatus::f_wrong_fat; }

	// filling in totalFrames and origStartFrame
	totalFrames = fat.numFrames();
	ulint offset, checksum;
	int mayaFrameNum, cacheFrameNum, baseFrameNum, compressed, V, E, F, UV;
	fat.getFrame(0, offset, mayaFrameNum, cacheFrameNum, baseFrameNum, compressed, V, E, F, UV, checksum);
	origStartFrame = mayaFrameNum;
	oldFileName = fileName;
	return stat;
}

cStatus cReader::close()
{
	stat = cStatus::ok;
	clear();
	return stat;
}


bool cReader::isOpen()
{
	return io.isOpen();
}

bool cReader::isError()
{
	if(stat.isError()) return true; else return false;
}

cStatus cReader::lastError()
{
	return stat;
}

// time, tOffset, tScale, loopAnim, totalFrames
double cReader::getTime(double t, double tOffset, double tScale, int loopAnim, int totFrames)
{
	t = t-tOffset;
	if (tScale == 0) tScale = 0.00000001;
	t*=(1/tScale);

	// animation looping
	if (loopAnim)
	{
		if (t<0) t = abs(totFrames-1) - fmod(fabs(t), totFrames-1);
		if (t > totFrames-1) t = fmod(t, totFrames-1);
	}
	// TIME bounds check - in any case time shouldn't go beyond bounds
	if (t < 0) t=0;
	if (t > totFrames-1) t=(totFrames-1);
	cacheInternalTime = t;
	return(t);
}


// loads given cFrame
cStatus cReader::loadFrame(int num, cFrame &frm, int setLastVars)
{
	stat = cStatus::ok;
	ulint offset, checksum;
	int mayaFrame, cacheFrame, baseFrame, compressed, V, E, F, UV;
	fat.getFrame(num, offset, mayaFrame, cacheFrame, baseFrame, compressed, V, E, F, UV, checksum);

	Byte *b = NULL;
	stat = io.read_dump(offset, b);
	if(stat.isError()) return stat;
	frm.unpack(b);
	cUtil::delete_Byte(b);

	if(setLastVars)
	{
		lastV = V;
		lastE = E;
		lastF = F;
		lastUV = UV;
		lastChecksum = checksum;
	}
	return stat;
}


cStatus cReader::getFrame(double extTime)
{
	stat = cStatus::ok;

	double t = getTime(extTime, timeOffset, timeScale, loopAnim, totalFrames);

	// sanity check
	if(t < 0 || t >= totalFrames) {return cStatus::error;}

	int t_low = (int)floor(t);
	int t_high = (int)ceil(t);
	double fraction = t - t_low;

	// getting frame data
	ulint offset1, checksum1;
	int mayaFrame1, cacheFrame1, baseFrame1, compressed1, V1, E1, F1, UV1;
	fat.getFrame(t_low, offset1, mayaFrame1, cacheFrame1, baseFrame1, compressed1, V1, E1, F1, UV1, checksum1);
	ulint offset2, checksum2;
	int mayaFrame2, cacheFrame2, baseFrame2, compressed2, V2, E2, F2, UV2;
	fat.getFrame(t_high, offset2, mayaFrame2, cacheFrame2, baseFrame2, compressed2, V2, E2, F2, UV2, checksum2);

	// note: we can only have baseFrame on t_low
	int t_low_loaded = 0;
	int bf = -1;
	if(baseFrame1 == -1) bf = t_low;	// if t_low is a baseFrame
	else if(baseFrame1 > -1) bf = baseFrame1;

	cFrame frame1, frame2;
	cMesh mesh1, mesh2;
	// LOADING BASEFRAME (AND MAYBE T_LOW)
	if(bf != lastBaseFrame)
	{
		loadFrame(bf, frame1, 1);	// temporarily using cFrame1
		frame1.joinMeshes(baseMesh);
		frame1.clear();
		baseMesh.optimizeMats();
		mesh = baseMesh;
		lastBaseFrame = bf;
		if(t_low == bf) t_low_loaded = 2;	// t_low in mesh
	}
	// LOADING T_LOW IF NEEDED
	if(!t_low_loaded)
	{
		loadFrame(t_low, frame1, 1);
		frame1.joinMeshes(mesh1);
		frame1.clear();
		t_low_loaded = 1;	// t_low in mesh1
	}

	// JUST SETTING PTS FROM T_LOW (IF IT'S NOT A BFRAME)
	if (checksum1 != checksum2 || !useLerp || t_low == t_high) 
	{
		if(t_low_loaded == 1)
		{
			int p = mesh.dataId(S_PTS);
			int p1 = mesh1.dataId(S_PTS);
			if(mesh.idOk(p) && mesh1.idOk(p1)) mesh.set(p, mesh1.at(p1));
		}
	}
	// OR LOADING T_HIGH AND LEPR()
	else
	{
		loadFrame(t_high, frame2, 0);
		frame2.joinMeshes(mesh2);
		frame2.clear();
		if(t_low_loaded == 1)	// t_low in mesh1
		{
			int p = mesh.dataId(S_PTS);
			int p1 = mesh1.dataId(S_PTS);
			int p2 = mesh2.dataId(S_PTS);
			if(mesh.idOk(p) && mesh1.idOk(p1) && mesh2.idOk(p2))
			{
				mesh.at(p).lerp( mesh1.at(p1), mesh2.at(p2), fraction );
			}
		}
		else if(t_low_loaded == 2)	// t_low in mesh already
		{
			int p = mesh.dataId(S_PTS);
			int p2 = mesh2.dataId(S_PTS);
			if(mesh.idOk(p) && mesh2.idOk(p2))
			{
				mesh.at(p).lerp( mesh2.at(p2), fraction );
			}
		}
	}
	mesh1.clear();
	mesh2.clear();
	return stat;
}


cMesh& cReader::getMesh()
{
	return mesh;
}


void cReader::getMesh(cMesh &m)
{
	m = mesh;
}


// converts mats/faceMats into MEL
vector<string> cReader::mats2mel(const cMesh &mesh)
{
	stat = cStatus::ok;

	vector<string> mats, comps, mel;
	vector<int> fMats;
	int matsId = mesh.dataId(S_MATS);
	int fmId = mesh.dataId(S_FACEMATS);
	if(!mesh.idOk(matsId) || !mesh.idOk(fmId)) return mel;
	mesh.get(matsId, mats);
	mesh.get(fmId, fMats);

	comps.resize(mats.size());
	int last, st = 0, en = 0;
	if(fMats.size()) last = fMats[0];
	// iterating thru faceMats, collecting range pairs "f[0:100]"
	for(uint i=1; i<fMats.size(); i++)
	{
		if(fMats[i] != last)
		{
			string comp = " ($obj+\".f[" + cUtil::i2s(st) + ":" + cUtil::i2s(en) + "]\")";
			comps[last].append(comp);
			last = fMats[i];
			st = i;
		}
		en = i;
	}
	// pushing last pair
	if(st < en && st != en)
	{
		string comp = " ($obj+\".f[" + cUtil::i2s(st) + ":" + cUtil::i2s(en) + "]\")";
		comps[last].append(comp);
	}

	
	mel.push_back("\nstring $tt[] = `ls -sl`;");
	mel.push_back("string $obj = $tt[0];");
	for(uint i=0; i<comps.size(); i++)
	{
		string sg = mats[i];
		mel.push_back("//" + cUtil::i2s(i) + ":");
		mel.push_back("{\n\tstring $sg = \"" + sg + "\";");
		mel.push_back("\tif(!objExists($sg))\n\t{");
		mel.push_back("\t\t$sg = `sets -renderable true -empty -name $sg`;");
		mel.push_back("\t\tstring $mat = `shadingNode -asShader lambert`;");
		mel.push_back("\t\tfloat $r=rand(0,1), $g=rand(0,1), $b=rand(0,1);");
		mel.push_back("\t\tsetAttr ($mat+\".color\") -type double3 $r $g $b;");
		mel.push_back("\t\tconnectAttr -f ($mat+\".outColor\") ($sg+\".surfaceShader\");\n\t}");
		mel.push_back("\tsets -e -forceElement $sg " + comps[i] + ";\n}");
	}

	//string script;
	//loop(mel.size()) script += mel[i] + "\n";
	//cUtil::toClip(script);
	//msg("matMap script copied to clipboard");
	return mel;
}

void cReader::setTimeOffset(double v) { timeOffset = v; }
void cReader::setTimeScale(double v) { timeScale = v; }
void cReader::setLerp(int v) { useLerp = v; }
void cReader::setLoopAnim(int v) { loopAnim = v; }
double cReader::getInternalTime() { return cacheInternalTime; }	// set by getTime()

int cReader::getOrigStartFrame() { return origStartFrame; }
int cReader::getTotalFrames() { return totalFrames; }
int cReader::getLastBaseframe() { return lastBaseFrame; }
int cReader::getLastV() { return lastV; }
int cReader::getLastE() { return lastE; }
int cReader::getLastF() { return lastF; }
int cReader::getLastUV() { return lastUV; }
ulint cReader::getLastChecksum() { return lastChecksum; }
