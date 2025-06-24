#ifndef _cIO
#define _cIO
#include "core_defs.h"
#include "cList.h"
#include "cUtil.h"
#include "cFrameTable.h"

#define fseek _fseeki64
#define ftell _ftelli64

/*
usage:
cio("c:\aaa.pch" status::read) -> read_header() -> read_FAT() -> read_dump(int frame) -> close()
cio("c:\aaa.pch" status::write) -> write_header() -> write_dump() -> write_FAT() -> close()

file structure:
	header
	fat_offset
	frame
	...
	frame
	FAT
	eof

we need to handle compress/decompress
also class have to keep current file position all the time
*/

// take a note that all file offsets should be ULINT
class cIO: public cUtil
{
	public:
		cIO();
		cIO(std::string, int);
		~cIO();

		void print() const;
		friend std::ostream& operator<<(std::ostream&, const cIO&);
		void setName(const std::string&);
		std::string getName() const;
		void setMode(int);
		int getMode() const;
		FILE* getFile() const;
		bool isOpen() const;
		cStatus getStatus() const;
		std::string getSignature() const;
		double getVersion() const;
		cStatus checkSignature();	// checks signature against one in core_defs.h, returns cStatus
		cStatus checkVersion();		// checks version against one in core_defs.h, returns cStatus

		void clear();
		cStatus open();
		cStatus open(const std::string&, int);
		cStatus close();
		cStatus read_header();
		cStatus write_header(); // we have to fill in FAT_offset_offset here
		cStatus read_FAT(cFrameTable&);
		cStatus write_FAT(const cFrameTable&);
		cStatus write_dump(Byte*);
		cStatus read_dump(const ulint, Byte*&);
		ulint getFilePos() const;
		cStatus setFilePos(const ulint);

	private:
		// class vars
		int fmode;
		std::string fname;
		FILE *F;
		int file_opened;
		ulint filePos;		// current position in file, all methods should set it
		cStatus status;		// internal last status code
		// file vars
		std::string signature;
		double version;
		// cache table:
		ulint FAT_ptr;		// where to write fat offset ulint in file
		int FAT_len;
};

#endif
