#include "cStatus.h"
using namespace std;

// constructor/destructor
cStatus::cStatus()
{
	set(cStatus::ok);
}

cStatus::cStatus(int code)
{
	set(code);
}

cStatus::~cStatus() {}


// member funcs
bool cStatus::isError() const
{
	if(status != cStatus::ok) return true;
	return false;
}

int cStatus::get() const
{
	return status;
}

void cStatus::set(int code)
{
	status = code;
}

string cStatus::getMsg(const int code) const
{
	string head = "status: ";
	string msg;
	switch(code)
	{
		case cStatus::ok: {msg = "ok"; break;}
		case cStatus::error: {msg = "generic error"; break;}
		case cStatus::mem_error: {msg = "general memory error"; break;}
		case cStatus::sig_error: {msg = "wrong signature"; break;}
		case cStatus::nodata: {msg = "no data in dump"; break;}

		case cStatus::f_mode_undef: {msg = "undefined file mode"; break;}
		case cStatus::f_mode_wrong: {msg = "wrong file mode"; break;}
		case cStatus::f_open_read_error: {msg = "error opening file for reading"; break;}
		case cStatus::f_open_write_error: {msg = "error opening file for writing"; break;}
		case cStatus::f_read_error: {msg = "error reading from file"; break;}
		case cStatus::f_write_error: {msg = "error writing to file"; break;}
		case cStatus::f_seek_error: {msg = "error seeking in file"; break;}

		case cStatus::f_wrong_signature: {msg = "wrong signature"; break;}
		case cStatus::f_wrong_version: {msg = "wrong version"; break;}
		case cStatus::f_wrong_fat: {msg = "wrong FAT"; break;}

		default: {stringstream s; s << code; msg = "unknown error code" + s.str(); break;}
	}
	return(head+msg);
}


string cStatus::getMsg() const
{
	return (getMsg(status));
}

void cStatus::errMsg() const
{
	if( isError() ) msg();
}

void cStatus::msg() const
{
	cout << getMsg() << endl;
}

void cStatus::msg(const string str) const
{
	cout << getMsg() << ": " << str << endl;
}

void cStatus::msg(const int code) const
{
	cout << getMsg(code) << endl;
}

ostream& operator<<(std::ostream& ostr, const cStatus& b)
{
	OSTR_SET
	ostr << b.getMsg() << ": " << endl;
	return ostr;
}

cStatus& cStatus::operator=(const cStatus &b)
{
	if(this == &b){return(*this);}	// check for self-assignment
	status = b.status;
	return(*this);
}

cStatus& cStatus::operator=(const int b)
{
	status = b;
	return(*this);
}

bool cStatus::operator==(const cStatus &b) const
{
	if(status == b.status) return true; else return false;
}

bool cStatus::operator!=(const cStatus &b) const
{
	if(status != b.status) return true; else return false;
}

bool cStatus::operator!=(const int b) const
{
	if(status != b) return true; else return false;
}