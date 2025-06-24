// DOUBLE CHECK MATRIX MULT, SMTH WRONG

#ifndef _cMatrix
#define _cMatrix
#include "core_defs.h"
#include "cUtil.h"
using namespace std;

// supports just a SQUARE matrices atm, need to add sizex()/sizey() procs, change operator()*, dot()
#define MATRIX_DIM 4
#define MATRIX_LEN (MATRIX_DIM*MATRIX_DIM)

class cMatrix: public cUtil
{
	public:
		cMatrix();
		cMatrix(vector<double>&);
		cMatrix(const cMatrix&);
		~cMatrix();

		void clear();
		int dim() const;
		int size() const;		// number of elements in internal array
		int dataSize() const;	// size in Bytes
		bool idOk(int) const;
		int rowcol2id(int, int) const;
		uint checksum() const;

		double& at(int);
		double& at(int, int);
		void set(int, double);
		void set(int, int, double);
		double get(int) const;
		double get(int, int) const;

		cMatrix& operator=(const cMatrix&);
		bool operator==(const cMatrix&) const;
		cMatrix operator*(const cMatrix&) const;
		//cVec3 operator*(const cMatrix&, const cVec3&) const;	// cVec3 supporting operator*(cMatrix) already
		double dot(int, int, const cMatrix&) const;	// dot product between matrix rows/cols
		//cMatrix operator+(const cMatrix&) const;

		void print() const;
		friend std::ostream& operator<<(std::ostream&, const cMatrix&);

		int write(const Byte*, const cMatrix&) const;
		int read(const Byte*, cMatrix&) const;

	private:
		vector<double> _m;
		double empty;
};

typedef cMatrix matrix;

#endif