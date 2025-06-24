#ifndef _cContainer
#define _cContainer
#include "core_defs.h"
#include "cUtil.h"
#include "cList.h"
using namespace std;

class cContainer: public cUtil
{
	public:
		cContainer();
		cContainer(cContainer&);
		cContainer(Byte*);
		~cContainer();
		void init(cContainer&);	// initializes object to other object data types and names, but skips data
		
		virtual void initSig();	// initializing a signature for this class, derived classes have to override it
		string getSig() const;

		int size() const;
		int dataSize() const;
		virtual void clear();
		virtual void clearData();
		void print() const;
		void printDebug() const;
		friend std::ostream& operator<<(std::ostream&, const cContainer&);

		// data info
		int dataId(string) const;		// returns index of named data in main array or -1 if not found
		bool idOk(int) const;			// checks if id valid, i.e. > -1 and less than size()
		bool dataExist(string name) const;	// returns true if dataName exist and false if not
		void deleteData(string);
		void listData(vector<string>&) const;	// lists all available data names, array-by-ref
		vector<string> listData() const;	// slower copy version
		int dataSize(string) const;			// returns size of given particular dataName
		int dataSize(int) const;			// returns size of data by index
		void resizeData(string, int);		// resizes data field(for cFrameTable to be sure we have enough frames)

		// cList operations
		void set(int, cList&);
		void set(string, cList&);
		virtual void append(int, cList&);
		void get(int, cList&) const;
		cList& at(int);
		cList& at(string);

		// adding data fields
		int addData(string, int);		// (name, d_type)
		int addData(string, int, int);	// (name, d_type, _appendFlag)
		int addData(cList&);			// directly copying cList, returns -1 if duplicate data name found
		// adding a dataField along with data itself
		int addData(string, vector<Byte>&);
		int addData(string, vector<int>&);
		int addData(string, vector<uint>&); 
		int addData(string, vector<ulint>&);
		int addData(string, vector<float>&);
		int addData(string, vector<double>&);
		int addData(string, vector<vec3>&);
		int addData(string, vector<string>&);
		// --== ARRAY operations ==--
		// setting an array
		void set(int, vector<Byte>&);
		void set(int, vector<int>&);
		void set(int, vector<uint>&);
		void set(int, vector<ulint>&);
		void set(int, vector<float>&);
		void set(int, vector<double>&);
		void set(int, vector<vec3>&);
		void set(int, vector<string>&);
		// setting an array by name, if no name exist - create one(?)
		// need just vector version due to speed limitations
		void set(string, vector<Byte>&);
		void set(string, vector<int>&);
		void set(string, vector<uint>&);
		void set(string, vector<ulint>&);
		void set(string, vector<float>&);
		void set(string, vector<double>&);
		void set(string, vector<vec3>&);
		void set(string, vector<string>&);
		// append an array
		void append(int, vector<Byte>&);
		void append(int, vector<int>&);
		void append(int, vector<uint>&);
		void append(int, vector<ulint>&);
		void append(int, vector<float>&);
		void append(int, vector<double>&);
		void append(int, vector<vec3>&);
		void append(int, vector<string>&);
		// getting vector
		void get(int, vector<Byte>&) const;
		void get(int, vector<int>&) const;
		void get(int, vector<uint>&) const;
		void get(int, vector<ulint>&) const;
		void get(int, vector<float>&) const;
		void get(int, vector<double>&) const;
		void get(int, vector<vec3>&) const;
		void get(int, vector<string>&) const;
		// getting vector by name
		void get(string, vector<Byte>&) const;
		void get(string, vector<int>&) const;
		void get(string, vector<uint>&) const;
		void get(string, vector<ulint>&) const;
		void get(string, vector<float>&) const;
		void get(string, vector<double>&) const;
		void get(string, vector<vec3>&) const;
		void get(string, vector<string>&) const;
		// add a single value to a range(index correction)
		void sum(int, int, int, Byte);
		void sum(int, int, int, int);
		void sum(int, int, int, uint);
		void sum(int, int, int, ulint);
		void sum(int, int, int, float);
		void sum(int, int, int, double);
		void sum(int, int, int, vec3);
		void sum(int, int, int, string);


		// --== SINGLE VALUE operations ==--
		// add single value to the end of array
		void add(int, Byte);
		void add(int, int);
		void add(int, uint);
		void add(int, ulint);
		void add(int, float);
		void add(int, double);
		void add(int, vec3);
		void add(int, string);
		// set single value
		void set(int, int, Byte);
		void set(int, int, int);
		void set(int, int, uint);
		void set(int, int, ulint);
		void set(int, int, float);
		void set(int, int, double);
		void set(int, int, vec3);
		void set(int, int, string);
		// get single value: dataId, index, val
		void get(int, int, Byte&) const;
		void get(int, int, int&) const;
		void get(int, int, uint&) const;
		void get(int, int, ulint&) const;
		void get(int, int, float&) const;
		void get(int, int, double&) const;
		void get(int, int, vec3&) const;
		void get(int, int, string&) const;

		// built-in checksum calculation
		void setChecksum(ulint);
		ulint getChecksum() const;
		ulint checksum() const;

		void append(cContainer&);
		cContainer& operator=(const cContainer&);
		cContainer operator+(cContainer&);
		cContainer& operator+=(cContainer&);
		// packing
		int packedSize() const;
		Byte* pack(int&) const;		// allocates memory and returns *ptr and size of dump
		int pack(Byte*) const;		// packs right into preallocated memory, returns Bytes written(cPtr)
		int unpack(Byte*);			// returns unpacked Bytes
	protected:
		// ---------- class data -----------
		string _signature;
		vector<cList> data;	// current attributes is: start, end, visibility
		cList nullRec;		// empty object to return if at() fails
		ulint _checksum;
};

#endif
