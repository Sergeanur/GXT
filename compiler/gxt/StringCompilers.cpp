#include "common.h"
#include "StringCompilers.h"

const wchar_t euro_chars[] = { 192, 193, 194, 195, 198, 199, 200, 201, 202, 203, 204,
							205, 206, 207, 210, 211, 212, 214, 217, 218, 219, 220,
							223, 224, 225, 226, 227, 230, 231, 232, 233, 234, 235,
							236, 237, 238, 239, 242, 243, 244, 246, 249, 250, 251,
							252, 209, 241, 191, 161, 180, };

wchar_t StandartFIGSConvertion(wchar_t wc)
{
	if ((wc >= 0x80) && (wc < 0x100))
	{
		for (int a = 0; a < ARRAYSIZE(euro_chars); a++)
		{
			if (euro_chars[a] == wc)
				return a + 0x80;
		}
	}
	return wc;
}

wchar_t cStringCompilerIII::ConvertChar(wchar_t c)
{
	return StandartFIGSConvertion(c);
}

wchar_t cStringCompilerVC::ConvertChar(wchar_t c)
{
	if (c == 0xA1)  // upside down exclamation mark
		return 0x5e;
	return cStringCompilerIII::ConvertChar(c);
}

wchar_t cStringCompilerVC_Mobile::ConvertChar(wchar_t c)
{
	if (c == '<') return '[';
	if (c == '>') return ']';
	return cStringCompilerVC::ConvertChar(c);
}

wchar_t cStringCompilerSA::ConvertChar(wchar_t c)
{
	if (c == 0xA1)  // upside down exclamation mark
		return 0x5e;
	return StandartFIGSConvertion(c);
}

wchar_t cStringCompilerLCS::ConvertChar(wchar_t c)
{
	//const wchar LcsEfigsChars[] = { 128, 130, 132, 136, 139, 140, 145, 146, 147, 148, 152, 153, 155, 156, 159, 161,
	//					 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 174, 176, 185, 187, 191, 127 };
	const wchar_t LcsEfigsChars[] = { 0x20AC, 0x201A, 0x201E, 0x02C6, 0x2039, 0x0152, 0x2018, 0x2019, 0x201C, 0x201D, 0x02DC, 0x2122, 0x203A, 0x0153, 0x0178, 161,
						 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 174, 176, 185, 187, 0xBF, 127 };
	if ((c >= 0xC0) && (c < 0x100))
		return c - 0x40;
	else if (c >= 0x80)
	{
		for (int a = 0; a < ARRAYSIZE(LcsEfigsChars); a++)
			if (LcsEfigsChars[a] == c)
				return a + 0xc0;
	}
	return c;
}

wchar_t cStringCompilerVCS::ConvertChar(wchar_t c)
{
	switch (c)
	{
	case 0x20AC: return 128;

	case 0x201A: return 130;
	case 0x0192: return 131;
	case 0x201E: return 132;
	case 0x2026: return 133;
	case 0x2020: return 134;
	case 0x2021: return 135;
	case 0x02C6: return 136;
	case 0x2030: return 137;
	case 0x0160: return 138;
	case 0x2039: return 139;
	case 0x0152: return 140;

	case 0x017D: return 142;

	case 0x2018: return 145;
	case 0x2019: return 146;
	case 0x201C: return 147;
	case 0x201D: return 148;
	case 0x2022: return 149;
	case 0x2013: return 150;
	case 0x2014: return 151;
	case 0x02DC: return 152;
	case 0x2122: return 153;
	case 0x0161: return 154;
	case 0x203A: return 155;
	case 0x0153: return 156;

	case 0x017E: return 158;
	case 0x0178: return 159;
	}
	return c;
}


wchar_t cStringCompilerIV::ConvertChar(wchar_t c)
{
	switch (c)
	{
	case 0x20AC: return 128;

	case 0x201A: return 130;
	case 0x0192: return 131;
	case 0x201E: return 132;
	case 0x2026: return 133;
	case 0x2020: return 134;
	case 0x2021: return 135;
	case 0x02C6: return 136;
	case 0x2030: return 137;
	case 0x0160: return 138;
	case 0x2039: return 139;
	case 0x0152: return 140;

	case 0x017D: return 142;

	case 0x2018: return 145;
	case 0x2019: return 146;
	case 0x201C: return 147;
	case 0x201D: return 148;
	case 0x2022: return 149;
	case 0x2013: return 150;
	case 0x2014: return 151;
	case 0x02DC: return 152;
	case 0x2122: return 153;
	case 0x0161: return 154;
	case 0x203A: return 155;
	case 0x0153: return 156;

	case 0x017E: return 158;
	case 0x0178: return 159;
	}
	return c;
}

wchar_t cStringCompilerIII_Rus::ConvertChar(wchar_t c)
{
	if ((c >= 0x0410) && (c < 0x0450))
		return c - 0x390;
	return c;
}

void cStringCompilerIII_Rus::ConvertString(const std::wstring& key, const std::wstring& s, std::vector<wchar_t>& buf)
{
	for (auto i = s.begin(); i != s.end(); i++)
	{
		if (key.find(L"WIN_") == 0)
			buf.push_back(*i);
		else
			buf.push_back(ConvertChar(*i));
	}
	buf.push_back(0);
}

wchar_t cStringCompilerIII_Pl::ConvertChar(wchar_t c)
{
	switch (c)
	{
	case 0x15A: return 128;
	case 0x179: return 129;
	case 0x141: return 130;
	case 0x104: return 131;

	case 0x106: return 133;
	case 0x118: return 134;
	case 0x143: return 135;
	case 0xD3: return 136;
	case 0x17B: return 137;

	case 0x15B: return 151;
	case 0x17A: return 152;
	case 0x142: return 153;
	case 0x105: return 154;

	case 0x107: return 156;
	case 0x119: return 157;
	case 0x144: return 158;
	case 0xF3: return 159;
	case 0x17C: return 160;
	}
	return c;
}