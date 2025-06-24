/*!
	Watch data types, because which read() function called depends only on given data type
*/

#ifndef _cUtil
#define _cUtil
#include "core_defs.h"
#include "cStatus.h"
#include <windows.h>

class cUtil
{
	public:
	// offset = write(ptr, value)
	template <class T> int write(const Byte*, const T) const;
	// value = read(ptr, &offset)
	template <class T> int read(const Byte*, T&) const;

	// cStatus = fwrite(FILE, value)
	template <class T> cStatus writef(FILE*, const T) const;
	// cStatus = fread(FILE, &value)
	template <class T> cStatus readf(FILE*, T&) const;

	// misc
	int mem_info(const Byte*) const;

	// zlib
	Byte* mem_compress(Byte *ptr) const;
	Byte* mem_decompress(Byte *ptr) const;

	// clear data:
	static void delete_Byte(Byte*);

	// cyclic shifts
	static uint ror(uint, uint);
	static uint rol(uint, uint);

	// numbers conversion
	static std::string i2s(int i);
	static int s2i(std::string s);

	//clipboard
	static void toClip(const std::string &s);

	//debug
	static void dtrace();
	template <class T> bool isNaN(T) const;
	template <class T> T clamp(T, T, T) const;
};

// ---=== WRITE
// offset = write(ptr, value)
template <class T>
inline int cUtil::write(const Byte* ptr, const T val) const
{
	memcpy( (void*)ptr, &val, sizeof(val));
	return(sizeof(val));
}

// string specialization
template <>
inline int cUtil::write<>(const Byte* ptr, const std::string &str) const
{
	int sz = str.size();
	int ofs = write(ptr, sz);
	memcpy( (void*)(ptr+ofs), str.data(), sz);
	return(sizeof(sz)+sz);
}




// ---=== READ
// offset = read(ptr, &value)
template <class T>
inline int cUtil::read(const Byte *ptr, T &val) const
{
	memcpy(&val, ptr, sizeof(val));
	return sizeof(val);
}

// string spec
template <>
inline int cUtil::read<>(const Byte* ptr, std::string &str) const
{
	int sz, of;
	of = read(ptr, sz);					// str size
	str = std::string((char*)ptr+of, sz);	// creating string from memory
	return(of+sz);						// new ptr
}




//--- WRITE FILE
template <class T>
inline cStatus cUtil::writef(FILE *F, const T val) const
{
	int st = fwrite(&val, sizeof(val), 1, F);
	if(st == 1) return cStatus(cStatus::ok); else return cStatus(cStatus::f_write_error);
}

// string specialization
template <>
inline cStatus cUtil::writef<>(FILE *F, const std::string &str) const
{
	int sz, st;
	cStatus stat;
	sz = str.size();
	stat = writef(F, sz);
	if(stat.isError()) return(stat);
	st = fwrite(str.data(), sz, 1, F);
	if(st == 1) return cStatus(cStatus::ok); else return cStatus(cStatus::f_write_error);
}
// mem specialization
template <>
inline cStatus cUtil::writef<>(FILE *F, Byte* ptr) const
{
	int sz, st;
	read(ptr, sz);
	st = fwrite((void*)ptr, sz, 1, F);
	if(st == 1) return cStatus(cStatus::ok); else return cStatus(cStatus::f_write_error);
}



//--- READ FILE
template <class T>
inline cStatus cUtil::readf(FILE *F, T &val) const
{
	int st = fread(&val, sizeof(val), 1, F);
	if(st == 1) return cStatus(cStatus::ok); else return cStatus(cStatus::f_read_error);	
}
// string specialization
template <>
inline cStatus cUtil::readf<>(FILE *F, std::string &str) const
{
	cStatus stat;
	int sz, st;
	stat = readf(F, sz);			// read string size
	if(stat.isError()) return(stat);
	char *ptr = new char[sz];		// alloc mem
	st = fread(ptr, sz, 1, F);		// read buffer
	if(st != 1) return cStatus(cStatus::f_read_error);
	str = std::string(ptr, sz);			// create string
	delete [] ptr;
	return stat;
}
// Byte* spec
template <>
inline cStatus cUtil::readf<>(FILE *F, Byte* &ptr) const
{
	cStatus stat;
	int sz, ofs, st;
	ptr = NULL;						// initializing pointer just for case
	stat = readf(F, sz);			// read dump size
	if(stat.isError()) return(stat);
	ptr = new Byte[sz];				// alloc mem
	if(ptr == NULL) return(cStatus::mem_error);
	ofs = write(ptr, sz);				// write dump size into mem
	st = fread(ptr+ofs, sz-ofs, 1, F);	// fread() right into memory
	if(st != 1) { stat = cStatus::f_read_error; delete_Byte(ptr); } else stat = cStatus::ok;
	return stat;
}

template <class T> bool cUtil::isNaN(T n) const
{
	if(n != n) return true; else return false;
}

template <class T> T cUtil::clamp(T val, T _min, T _max) const
{
	return max(_min, min(val, _max));
}

#endif
