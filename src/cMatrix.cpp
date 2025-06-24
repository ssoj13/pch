#include "cMatrix.h"


//--- Constructors
cMatrix::cMatrix()
{
	clear();
}

cMatrix::cMatrix(vector<double> &b)
{
	clear();
	if(size() == (int)b.size()) _m = b;
}

cMatrix::cMatrix(const cMatrix &b)
{
	clear();
	if(size() == b.size()) _m = b._m;
}

cMatrix::~cMatrix() {}





//--- member funcs
void cMatrix::clear()
{
	empty = -1.0f;
	_m.resize(size());
	loop(size()) _m[i] = 0.0f;
	loop(dim()) at(i, i) = 1.0f;
}

int cMatrix::dim() const
{
	return MATRIX_DIM;
}

int cMatrix::size() const
{
	return MATRIX_LEN;
}

int cMatrix::dataSize() const
{
	return MATRIX_LEN * sizeof(double);
}

bool cMatrix::idOk(int id) const
{
	if(id >= 0 && id < size()) return true; else return false;
}

// using zero-based row and column numbers
// TODO: row/col out of range check?
int cMatrix::rowcol2id(int row, int col) const
{
	int id = (row * dim()) + col;
	return id;
}

uint cMatrix::checksum() const
{
	uint sum = 0;
	double dsum = 0;
	loop(size()) dsum += get(i)*i;
	sum = uint(dsum);
	return sum;
}


double& cMatrix::at(int d)
{
	if(idOk(d)) return _m.at(d); else return empty;
}

double& cMatrix::at(int row, int col)
{
	int d = rowcol2id(row,col);
	if(idOk(d)) return _m.at(d); else return empty;
}

void cMatrix::set(int d, double v)
{
	if(idOk(d)) _m[d] = v;
}

void cMatrix::set(int row, int col, double v)
{
	int d = rowcol2id(row,col);
	if(idOk(d)) _m[d] = v;
}

double cMatrix::get(int d) const
{
	/*double v = empty;
	if(idOk(d)) v = _m[d];
	return v;*/
	return(_m[d]);
}

double cMatrix::get(int row, int col) const
{
	double v = empty;
	int d = rowcol2id(row,col);
	if(idOk(d)) v = _m[d];
	return v;
}

cMatrix& cMatrix::operator=(const cMatrix &b)
{
	if(this == &b){return(*this);}	// check for self-assignment
	_m = b._m;
	return(*this);
}

bool cMatrix::operator==(const cMatrix &b) const
{
	if(size() != b.size()) return false;
	loop(size())
	{
		if(get(i) != b.get(i)) return false;
	}
	return true;
}

// square matrix mult
cMatrix cMatrix::operator*(const cMatrix &b) const
{
	cMatrix r = *this;
	loop(dim())	// rows
	{
		loop2(b.dim())	// columns
		{
			r.set(i, j, r.dot(i, j, b) );
		}
	}
	return r;
}

// square matrix (row * column) dot product
double cMatrix::dot(int row, int col, const cMatrix &b) const
{
	if(dim() != b.dim()) return empty;

	double r = 0;
	loop(dim())	// rows
	{
		r += get(row, i) * b.get(i, col);
	}
	return r;
}


void cMatrix::print() const
{
	cout << *this;
}

ostream& operator<<(std::ostream& ostr, const cMatrix& b)
{
	OSTR_SET
	loop(b.dim())	// rows
	{
		cout << "[";
		loop2(b.dim())	// columns
		{
			ostr << b.get(i,j) << ", ";
		}
		cout << "]" << endl;
	}
	return ostr;
}

// Each data class has a write() and read() functions to dump/read itself to a chunk of memory
int cMatrix::write(const Byte* ptr, const cMatrix &m) const
{
	uint ofs = 0;
	loop(size())
	{
		ofs += cUtil::write(ptr+ofs, m.get(i));
	}
	return(ofs);
}

int cMatrix::read(const Byte* ptr, cMatrix &m) const
{
	int ofs = 0;
	loop(size())
	{
		ofs += cUtil::read(ptr+ofs, m.at(i));
	}
	return(ofs);
}
