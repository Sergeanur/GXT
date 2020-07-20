#pragma once

#include "StringCompiler.h"
#include "CRC32.h"
#include "JenkinsHash.h"
#include <functional>


class cStringCompilerIII : public cStringCompilerTmpl<wchar_t, KeyChar8>
{
public:
	virtual wchar_t ConvertChar(wchar_t c);
	virtual bool IsSortedTableList() { return true; };
};


class cStringCompilerVC : public cStringCompilerIII
{
public:
	virtual wchar_t ConvertChar(wchar_t c);
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
	virtual wchar_t ConvertChar(wchar_t c);
	virtual bool IsSortedTableList() { return true; };
};

class cStringCompilerLCS : public cStringCompilerTmpl<wchar_t, KeyChar8>
{
public:
	virtual wchar_t ConvertChar(wchar_t c);
	virtual bool IsSortedTableList() { return false; };
};

class cStringCompilerVCS : public cStringCompilerTmpl<wchar_t, KeyChar8>
{
public:
	virtual wchar_t ConvertChar(wchar_t c);
	virtual bool IsSortedTableList() { return false; };
};

class cStringCompilerIV : public cStringCompilerTmpl<wchar_t, JenkinsHash>
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
	virtual wchar_t ConvertChar(wchar_t c);
	virtual bool IsSortedTableList() { return true; };
};

class cStringCompilerIII_Rus : public cStringCompilerIII
{
public:
	virtual void ConvertString(const std::wstring& key, const std::wstring& s, std::vector<wchar_t>& buf);
	virtual wchar_t ConvertChar(wchar_t c);
};

class cStringCompilerIII_Pl : public cStringCompilerIII
{
public:
	virtual wchar_t ConvertChar(wchar_t c);
};

class cStringCompilerIII_Mobile_Jap : public cStringCompilerIII
{
public:
	virtual wchar_t ConvertChar(wchar_t c);
};

class cStringCompilerIII_Mobile_Kor : public cStringCompilerIII
{
public:
	virtual wchar_t ConvertChar(wchar_t c);
};

class cStringCompilerVC_Mobile : public cStringCompilerVC
{
public:
	virtual wchar_t ConvertChar(wchar_t c);
};

class cStringCompilerVC_Mobile_Jap : public cStringCompilerVC
{
public:
	virtual wchar_t ConvertChar(wchar_t c);
};

class cStringCompilerVC_Mobile_Kor : public cStringCompilerVC
{
public:
	virtual wchar_t ConvertChar(wchar_t c);
};

class cStringCompilerVC_Mobile_Rus : public cStringCompilerVC
{
public:
	virtual wchar_t ConvertChar(wchar_t c);
};