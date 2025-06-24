#include "cio.h"
using namespace std;

//--- constructors
cIO::cIO()
{
	file_opened = 0;
	clear();
}

cIO::cIO(string fn, int md)
{
	file_opened = 0;
	clear();
	setName(fn);
	setMode(md);
	status = open();
}

cIO::~cIO()
{
	clear();
}


//--- initialization members
void cIO::clear()
{
	close();
	file_opened = 0;
	fmode = cStatus::f_mode_read;
	fname.clear();
	signature = PCH_SIGNATURE;
	version = PCH_VERSION;
	FAT_ptr = 0L;
	FAT_len = 0;
	status = cStatus::ok;
}

void cIO::print() const
{
	cout << *this;
}

ostream& operator<<(std::ostream& ostr, const cIO& b)
{
	OSTR_SET
	ostr << b.fname << ", mode: " << b.fmode << ", status: " << b.status << endl;
	return ostr;
}

void cIO::setName(const string &n)
{
	fname = n;
}
string cIO::getName() const
{
	return(fname);
}

// set/getMode returns cStatus-compatible modes
void cIO::setMode(int m)
{
	fmode = m;
}
int cIO::getMode() const
{
	return(fmode);
}

FILE* cIO::getFile() const
{
	return(F);
}

ulint cIO::getFilePos() const
{
	return ftell(F);
}

cStatus cIO::setFilePos(ulint offset)
{
	int st = fseek(F, offset, SEEK_SET);
	if(st) return cStatus::f_seek_error; else return cStatus::ok;
}

bool cIO::isOpen() const
{
	if(file_opened) return true; else return false;
}

cStatus cIO::getStatus() const
{
	return status;
}

string cIO::getSignature() const
{
	return signature;
}

cStatus cIO::checkSignature()
{
	if(signature != PCH_SIGNATURE) status = cStatus(cStatus::f_wrong_signature); else status = cStatus(cStatus::ok);
	return status;
}

double cIO::getVersion() const
{
	return version;
}

cStatus cIO::checkVersion()
{
	if(version != PCH_VERSION) status = cStatus(cStatus::f_wrong_version); else status = cStatus(cStatus::ok);
	return status;
}


//--- IO
cStatus cIO::open()
{
	if(file_opened) close();
	string md;
	if(fmode == cStatus::f_mode_read) {md = "rb";}
	else if(fmode == cStatus::f_mode_write) {md = "wb";}
	else {return(cStatus::f_mode_wrong);}

	F = fopen(fname.c_str(), md.c_str());
	if(!F && md == "rb") {status = cStatus::f_open_read_error; return status;}
	else if(!F && md == "wb") {status = cStatus::f_open_write_error; return status;}
	else status = cStatus::ok;
	file_opened = 1;
	filePos = getFilePos();
	return status;
}

cStatus cIO::open(const string &fn, int fm)
{
	setName(fn);
	setMode(fm);
	status = open();
	return status;
}

cStatus cIO::close()
{
	if(file_opened) fclose(F);
	file_opened = 0;
	filePos = 0;
	status = cStatus::ok;
	return status;
}

cStatus cIO::write_header()
{
	setFilePos(0);
	status = writef(F, signature);
	status = writef(F, version);
	FAT_ptr = getFilePos();
	status = writef(F, FAT_ptr); // empty ulint - FAT_offset placeholder
	filePos = getFilePos();
	return(status);
}

cStatus cIO::read_header()
{
	setFilePos(0);
	status = readf(F, signature);
	if(status.isError()) return status;
	status = readf(F, version);
	if(status.isError()) return status;
	status = readf(F, FAT_ptr);
	if(status.isError()) return status;
	filePos = getFilePos();
	return(status);
}

// writes FAT at current file position and rewrites FAT_offset in file
// filePos is kept at the end of FAT
cStatus cIO::write_FAT(const cFrameTable &fat)
{
	int sz;
	Byte *b = fat.pack(sz);
	// writing FAT
	ulint fat_offset = getFilePos();
	status = writef(F, b);
	if(status.isError()) return status;
	delete_Byte(b);
	// writing offset into FAT_ptr
	filePos = getFilePos();
	setFilePos(FAT_ptr);	// FAT_ptr is set by write_header()
	status = writef(F, fat_offset);
	if(status.isError()) return status;
	setFilePos(filePos);
	return(status);
}

// reads and unpacks FAT from a file, filePos is kept intact
cStatus cIO::read_FAT(cFrameTable &fat)
{
	filePos = getFilePos();
	setFilePos(FAT_ptr);	// FAT_ptr is set by read_header()
	Byte *b;
	status = readf(F, b);	// we don't need to know dump size, first Byte of dump is always dumpSize
	if(status.isError()) return status;
	status = fat.unpack(b);
	if(status.isError()) return status;
	delete_Byte(b);
	setFilePos(filePos);
	return(status);
}

// basically need these two functions just for getFilePos()
cStatus cIO::write_dump(Byte* dump)
{
	status = writef(F, dump);
	filePos = getFilePos();
	return(status);
}
cStatus cIO::read_dump(const ulint offset, Byte* &dump)
{
	setFilePos(offset);
	status = readf(F, dump);
	filePos = getFilePos();
	return status;
}



/*template <>
inline cStatus cUtil::readf<Byte* &>(FILE *F, Byte* &ptr) const
{
	cStatus stat;
	uint sz, ofs, st;
	stat = readf(F, sz);				// read dump size
	if(stat.isError()) return(stat);
	ptr = new Byte[sz];				// alloc mem
	if(ptr == NULL) return(cStatus::mem_error);
	ofs = write(ptr, sz);					// write dump size into mem
	st = fread(ptr+ofs, sz-ofs, 1, F);	// fread() right into memory
	if(st != 1) { stat = cStatus::f_read_error; delete_Byte(ptr); } else stat = cStatus::ok;
	return stat;
}*/