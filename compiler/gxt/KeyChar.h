#pragma once

template<size_t count>
struct KeyChar
{
	char value[count];
public:
	KeyChar() { memset(value, 0, count); }
	KeyChar(char val[count])
	{
		memset(value, 0, count);
		strcpy_s(value, val);
	}
	KeyChar(const char* val)
	{
		memset(value, 0, count);
		strcpy_s(value, val);
	}
	KeyChar(const wchar_t* val)
	{
		memset(value, 0, count);
		//strcpy_s(value, val);
		for (int i = 0; i < count-1; i++)
		{
			if (!val[i]) break;
			value[i] = val[i];
		}
	}
};

template<size_t count>
bool operator<(const KeyChar<count>& l, const KeyChar<count>& r) { return strcmp(l.value, r.value) < 0; }

typedef KeyChar<8> KeyChar8;
typedef KeyChar<16> KeyChar16;
