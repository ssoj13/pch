#ifndef _cReader
#define _cReader
#include "core_defs.h"
#include "cIO.h"
#include "cMesh.h"
#include "cFrame.h"
#include "cFrameTable.h"
using namespace std;

class cReader
{
	public:
		cReader();
		cReader(const string);	// name
		cReader(const string, int, int, int);	// name, lerp, uv, normals
		~cReader();

		void clear();
        friend std::ostream& operator<<(std::ostream&, const cReader&);

		cStatus open(const string);		// initializes cache, backups oldFileName, reads FAT
		cStatus close();				// safely closes cache
		bool isOpen();
		bool isError();
		cStatus lastError();

		double getTime(double, double, double, int, int);	// time, tOffset, tScale, loopAnim, totalFrames
		cStatus loadFrame(int, cFrame&, int);	// low-level func, loads physical cFrames
		cStatus getFrame(double);	// encapsulates all frame loading processes - loading, baseFrame, lerping, setting lastStats, result is in "mesh" member
		void getLastStats();
		cMesh& getMesh();		// returns a REFERENCE to internal cMesh
		void getMesh(cMesh&);	// returns a COPY - slow

		void setTimeOffset(double);
		void setTimeScale(double);
		void setLerp(int);
		void setLoopAnim(int);
		double getInternalTime();

		int getOrigStartFrame();
		int getTotalFrames();
		int getLastV();
		int getLastE();
		int getLastF();
		int getLastUV();
		ulint getLastChecksum();
		int getLastBaseframe();
		vector<string> mats2mel(const cMesh &mesh);

	protected:
		cIO io;
		cFrameTable fat;
		cMesh baseMesh, mesh;
		cStatus stat;

		// external user params
		double timeOffset;
		double timeScale;
		int useLerp;
		int loopAnim;
		double cacheInternalTime;	// set by getTime()

		string fileName;
		string oldFileName;
		// cache info
		int origStartFrame;
		int totalFrames;
		int lastBaseFrame;
		int lastV;
		int lastE;
		int lastF;
		int lastUV;
		ulint lastChecksum;
};

#endif
