/*
*/

#ifndef _cStatus
#define _cStatus
#include "core_defs.h"

class cStatus
{
	public:
		cStatus();
		cStatus(int);
		~cStatus();
		bool isError() const;				// returns 1 if any error and 0 if not
		int get() const;					// just returns current status
		void set(const int);				// just returns current status
		std::string getMsg() const;			// gets error msg for internal status code
		std::string getMsg(const int) const;	// gets error msg for given status code
		void errMsg() const;				// prints message only if status != ok
		void msg() const;					// prints error msg to cout
		void msg(const std::string) const;	// prints error msg and user message to cout
		void msg(const int) const;			// prints error msg to cout, taking external error code
		friend std::ostream& operator<<(std::ostream&, const cStatus&);
		cStatus& operator=(const cStatus&);
		cStatus& operator=(const int);
		bool operator==(const cStatus&) const;
		bool operator!=(const cStatus&) const;
		bool operator!=(const int) const;

		// error codes:
		static const int ok = 0;			// generic Ok
		static const int error = 1;			// generic Error
		static const int mem_error = 10;	// general memory error
		static const int sig_error = 20;	// signature doesnt match
		static const int nodata = 30;		// no data in dump

		static const int f_mode_undef = 50;			// cio mode: undef
		static const int f_mode_read = 51;			// cio mode: read
		static const int f_mode_write = 52;			// cio mode: write
		static const int f_mode_wrong = 53;			// wrong mode given to cio()
		static const int f_open_read_error = 54;
		static const int f_open_write_error = 55;
		static const int f_read_error = 56;
		static const int f_write_error = 57;
		static const int f_seek_error = 58;

		static const int f_wrong_signature = 60;
		static const int f_wrong_version = 61;
		static const int f_wrong_fat = 62;

		// maya writer args
		// ...

	private:
		int status;			
};

#endif
