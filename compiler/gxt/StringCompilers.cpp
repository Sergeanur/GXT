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