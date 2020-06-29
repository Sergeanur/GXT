#pragma once

#include "StringCompiler.h"
#include "CRC32.h"
#include "JenkinsHash.h"
#include <functional>


class cStringCompilerIII : public cStringCompilerTmpl<wchar, KeyChar8>
{
public:
	virtual void ConvertString(const std::string& s, std::vector<tCharType>& buf);
	virtual bool IsSortedTableList() { return true; };
};


class cStringCompilerVC : public cStringCompilerIII
{
public:
	virtual void ConvertString(const std::string& s, std::vector<tCharType>& buf);
};


class cStringCompilerSA : public cStringCompilerTmpl<char, CRC32>
{
public:
	virtual void WriteHeaderStuff(std::ostream& s)
	{
		unsigned short n = sizeof(CRC32);
		s.write((char*)&n, 2);
		n = sizeof(tCharType) * 8;
		s.write((char*)&n, 2);
	};
	virtual void ConvertString(const std::string& s, std::vector<tCharType>& buf);
	virtual bool IsSortedTableList() { return true; };
};

class cStringCompilerLCS : public cStringCompilerTmpl<wchar, KeyChar8>
{
public:
	virtual void ConvertString(const std::string& s, std::vector<tCharType>& buf);
	virtual bool IsSortedTableList() { return false; };
};

class cStringCompilerVCS : public cStringCompilerTmpl<wchar, KeyChar8>
{
public:
	virtual void ConvertString(const std::string& s, std::vector<tCharType>& buf);
	virtual bool IsSortedTableList() { return false; };
};

class cStringCompilerIV : public cStringCompilerTmpl<wchar, JenkinsHash>
{
public:
	virtual bool IsMainTableNeedSorting() { return false; };
	virtual void WriteHeaderStuff(std::ostream& s)
	{
		unsigned short n = sizeof(JenkinsHash);
		s.write((char*)&n, 2);
		n = sizeof(tCharType) * 8;
		s.write((char*)&n, 2);
	};
	virtual void ConvertString(const std::string& s, std::vector<tCharType>& buf);
	virtual bool IsSortedTableList() { return true; };
};

class cStringCompilerIII_Rus : public cStringCompilerIII
{
public:
	virtual std::string PrepareSource(const char* _source);
	virtual void ConvertString(const std::string& s, std::vector<tCharType>& buf);
};

class cStringCompilerIII_PL : public cStringCompilerIII
{
public:
	virtual std::string PrepareSource(const char* _source);
	virtual void ConvertString(const std::string& s, std::vector<tCharType>& buf);
};