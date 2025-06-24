/*
Simple checksum class for all data types with internal position counter
*/

#ifndef _cChecksum
#define _cChecksum
#include "core_defs.h"

#define CS_MAX_SUMS 359

class cChecksum
{
	public:
		cChecksum();
		cChecksum(const cChecksum&);
		~cChecksum();

		void clear();
		void print() const;
		friend std::ostream& operator<<(std::ostream&, const cChecksum&);
		void set(uint, uint, uint);
		ulint get() const;
		void add(uint);
		void append(const cChecksum&);

		cChecksum operator+(const cChecksum&);
		cChecksum& operator+=(const cChecksum&);
		cChecksum& operator=(const cChecksum&);
		bool operator==(const cChecksum&);
		bool operator!=(const cChecksum&);
		bool operator!=(const uint);

	private:
		uint _i;
		uint _fl1;
		uint _fl2;
};

#endif
