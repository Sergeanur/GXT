#include "common.h"
#include "StringCompilers.h"

void cStringCompilerIII::ConvertString(const std::string& s, std::vector<tCharType>& buf)
{
	const char euro_chars[] = "ÀÁÂÃÆÇÈÉÊËÌÍÎÏÒÓÔÖÙÚÛÜßàáâãæçèéêëìíîïòóôöùúûüÑñ¿\xA1\xB4";
	for (auto i = s.begin(); i != s.end(); i++)
	{
		unsigned char c = (unsigned char)*i;
		if (c >= 0x80)
		{
			bool found = false;
			for (int a = 0; a < ARRAYSIZE(euro_chars); a++)
			{
				if (euro_chars[a] == *i) {
					found = true;
					buf.push_back(a + 0x80);
					break;
				}
			}
			if (!found)
				buf.push_back(c);
		}
		else
			buf.push_back(c);

	}
	buf.push_back(0);
}

void cStringCompilerVC::ConvertString(const std::string& s, std::vector<tCharType>& buf)
{
	const char euro_chars[] = "ÀÁÂÃÆÇÈÉÊËÌÍÎÏÒÓÔÖÙÚÛÜßàáâãæçèéêëìíîïòóôöùúûüÑñ¿\xA1\xB4";
	for (auto i = s.begin(); i != s.end(); i++)
	{
		unsigned char c = (unsigned char)*i;
		if (c >= 0x80)
		{
			if (c == 0xA1) // upside down exclamation mark
				buf.push_back(0x5e);
			else
			{
				bool found = false;
				for (int a = 0; a < ARRAYSIZE(euro_chars); a++)
				{
					if (euro_chars[a] == *i) {
						found = true;
						buf.push_back(a + 0x80);
						break;
					}
				}
				if (!found)
					buf.push_back(c);
			}
		}
		else
			buf.push_back(c);
	}
	buf.push_back(0);
}


void cStringCompilerSA::ConvertString(const std::string& s, std::vector<tCharType>& buf)
{
	const char euro_chars[] = "ÀÁÂÃÆÇÈÉÊËÌÍÎÏÒÓÔÖÙÚÛÜßàáâãæçèéêëìíîïòóôöùúûüÑñ¿\xA1\xB4";
	for (auto i = s.begin(); i != s.end(); i++)
	{
		unsigned char c = (unsigned char)*i;
		if (c >= 0x80)
		{
			if (c == 0xA1) // upside down exclamation mark
				buf.push_back(0x5e);
			else
			{
				bool found = false;
				for (int a = 0; a < ARRAYSIZE(euro_chars); a++)
				{
					if (euro_chars[a] == *i) {
						found = true;
						buf.push_back(a + 0x80);
						break;
					}
				}
				if (!found)
					buf.push_back(c);
			}
		}
		else
			buf.push_back(c);

	}
	buf.push_back(0);
}

void cStringCompilerLCS::ConvertString(const std::string& s, std::vector<tCharType>& buf)
{
	wchar LcsEfigsChars[] = { 128, 130, 132, 136, 139, 140, 145, 146, 147, 148, 152, 153, 155, 156, 159, 161,
						 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 174, 176, 185, 187, 191, 127 };
	for (auto i = s.begin(); i != s.end(); i++)
	{
		wchar c = (unsigned char)*i;
		if (c >= 0xC0)
			buf.push_back(c - 0x40);
		else if (c >= 0x80)
		{
			bool found = false;
			for (int a = 0; a < ARRAYSIZE(LcsEfigsChars); a++)
			{
				if (LcsEfigsChars[a] == c)
				{
					found = true;
					buf.push_back(a + 0xC0);
					break;
				}
			}
			if (!found)
				buf.push_back(c);
		}
		else
			buf.push_back(c);

	}
	buf.push_back(0);
}

void cStringCompilerVCS::ConvertString(const std::string& s, std::vector<tCharType>& buf)
{
	const std::string &str = s;
	for (auto i = str.begin(); i != str.end(); i++)
	{
		wchar c = (unsigned char)*i;
		buf.push_back(c);
	}
	buf.push_back(0);
}

bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }

void cStringCompilerIV::ConvertString(const std::string& s, std::vector<tCharType>& buf)
{
	std::string str = s;
	// remove double spaces, TODO: could be needed for all games, not just IV?
	std::string::iterator new_end = std::unique(str.begin(), str.end(), BothAreSpaces);
	str.erase(new_end, str.end());

	for (auto i = str.begin(); i != str.end(); i++)
	{
		wchar c = (unsigned char)*i;
		buf.push_back(c);
	}
	buf.push_back(0);
}


std::string cStringCompilerIII_Rus::PrepareSource(const char* _source)
{
	return ConvertStringCodePage(_source, CP_UTF8, 1251);
}

void cStringCompilerIII_Rus::ConvertString(const std::string& s, std::vector<tCharType>& buf)
{
	const std::string& str = s;
	for (auto i = str.begin(); i != str.end(); i++)
	{
		wchar c = (unsigned char)*i;
		if (c >= 0xC0)
			c -= 0x40;
		buf.push_back(c);
	}
	buf.push_back(0);
}

std::string cStringCompilerIII_PL::PrepareSource(const char* _source)
{
	return ConvertStringCodePage(_source, CP_UTF8, 1250);
}

void cStringCompilerIII_PL::ConvertString(const std::string& s, std::vector<tCharType>& buf)
{
	struct { int c, gxt; } pl_chars[] =
	{
		{ 0x8C, 0x0080 }, // S
		{ 0x8F, 0x0081 }, // Z
		{ 0xA3, 0x0082 }, // L
		{ 0xA5, 0x0083 }, // A
		{ 0xC6, 0x0085 }, // C
		{ 0xCA, 0x0086 }, // E
		{ 0xD1, 0x0087 }, // N
		{ 0xD3, 0x0088 }, // O
		{ 0xAF, 0x0089 }, // Z
		{ 0x9C, 0x0097 }, // s
		{ 0x9F, 0x0098 }, // z
		{ 0xB3, 0x0099 }, // l
		{ 0xB9, 0x009A }, // a
		{ 0xE6, 0x009C }, // c
		{ 0xEA, 0x009D }, // e
		{ 0xF1, 0x009E }, // n
		{ 0xF3, 0x009F }, // o
		{ 0xBF, 0x00A0 }  // z
	};

	for (auto i = s.begin(); i != s.end(); i++)
	{
		unsigned char c = (unsigned char)*i;
		if (c >= 0x80)
		{
			bool found = false;
			for (int a = 0; a < ARRAYSIZE(pl_chars); a++)
			{
				if (pl_chars[a].c == c) {
					found = true;
					buf.push_back(pl_chars[a].gxt);
					break;
				}
			}
			if (!found)
				buf.push_back(c);
		}
		else
			buf.push_back(c);

	}
	buf.push_back(0);
}