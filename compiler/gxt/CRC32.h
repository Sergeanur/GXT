#pragma once

struct CRC32
{
	uint32 value;
public:
	CRC32() {}
	CRC32(const char* str);
	CRC32(const wchar_t* str);
	CRC32(uint32 val) : value(val) {}
};

bool operator<(const CRC32& l, const CRC32& r);