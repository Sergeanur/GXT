#include "common.h"
#include "StringCompiler.h"

std::string cStringCompiler::PrepareSource(const char* _source)
{
	return ConvertStringCodePage(_source, CP_UTF8, 1252);
}