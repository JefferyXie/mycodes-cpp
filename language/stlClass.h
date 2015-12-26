#ifndef STLCLASS_H
#define STLCLASS_H

#include "../header.h"
#include "../utility.h"

// this class test basic operations on STL containers
class stlClass
{
private:
//	char m_ch[5];
//	short m_sh;
	string m_name;
	int m_value;

public:
	stlClass()
	{
		cout << "stlClass()" << endl;
	}
    stlClass(string name, int value) : m_name(name), m_value(value)
	{
		cout << "stlClass(string, int) > " << Dump() << endl;
	}
	stlClass(const stlClass& obj) : m_name(obj.m_name), m_value(obj.m_value)
	{
        cout << "stlClass(const stlClass&) > " << Dump() <<endl;
	}
	virtual ~stlClass()
	{
		cout << "~stlClass() > " << Dump() << endl;
	}
	string Dump()
	{
	    return "m_name: " + m_name + "  m_value: " + ToString(m_value);
	}
	bool operator< (const stlClass& obj) const
	{
		return m_value < obj.m_value;
	}
	bool operator> (const stlClass& obj) const
	{
		return obj < *this;
	}
	bool operator== (const stlClass& obj)
	{
		return !(*this < obj || *this > obj);
	}

    friend ostream& operator<< (ostream& os, const stlClass& obj)
    {
        os << "Name: " << obj.m_name << "  Value: " << obj.m_value << endl;
        return os;
    }
};
/*
ostream& operator<< (ostream& os, const stlClass& obj)
{
    os << "Name: " << obj.m_name << "  Value: " << obj.m_value << endl;
    return os;
}
*/
#endif
