#include "common.h"

std::string ConvertStringCodePage(const std::string& s, int code_from, int code_to)
{
	int requiredSize = MultiByteToWideChar(code_from, 0, s.c_str(), s.size(), 0, 0);

	std::wstring wstr;
	wstr.resize(requiredSize);

	MultiByteToWideChar(code_from, 0, s.c_str(), s.size(), (LPWSTR)wstr.data(), wstr.size());

	requiredSize = WideCharToMultiByte(code_to, 0, wstr.c_str(), wstr.size(), 0, 0, 0, 0);

	std::string str;
	str.resize(requiredSize);

	int retval = WideCharToMultiByte(code_to, 0, wstr.c_str(), wstr.size(), (LPSTR)str.data(), str.size(), 0, 0);

	return str;
}