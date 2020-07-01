#include "common.h"
#include "utils.h"
#include "TableData.h"
#include "StringCompilers.h"
#include <fstream>
#include <streambuf>
#include <assert.h>

void RemoveReturns(std::wstring& s)
{
	size_t p = s.find(L"\r\n");
	while (p != std::wstring::npos)
	{
		s.replace(p, 2, L" ");
		p = s.find(L"\r\n");
	}

	p = s.find('\r');
	while (p != std::wstring::npos)
	{
		s[p] = ' ';
		p = s.find('\r');
	}

	p = s.find('\n');
	while (p != std::wstring::npos)
	{
		s[p] = ' ';
		p = s.find('\n');
	}

	p = s.find('\t');
	while (p != std::wstring::npos)
	{
		s[p] = ' ';
		p = s.find('\t');
	}

}

bool ReadFileToWString(const std::string &filename, int suggestedEncoding, std::wstring& outstr)
{
	std::wstring str0;
	std::ifstream t(filename);
	if (!t.is_open())
	{
		printf("ERROR: File %s could not be opened.\n", filename.c_str());
		return false;
	}

	t.seekg(0, std::ios::end);
	size_t filesize = t.tellg();
	t.seekg(0, std::ios::beg);

	int BomCheck = 0;
	t.read((char*)&BomCheck, 3);
	if ((BomCheck & 0xFFFF) == 0xFEFF)
	{
		t.seekg(2);
		str0.reserve(filesize/2);
		t.read((char*)str0.c_str(), filesize - 2);
	}
	else if ((BomCheck & 0xFFFF) == 0xFFFE)
	{
		t.close();
		return false;
	}
	else if (BomCheck == 0xBFBBEF)
	{
		str0.reserve(filesize-2);

		// read all stream to string
		std::string str_char;
		str_char.reserve(filesize - 2);
		str0.assign(filesize + 1, 0);
		str_char.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

		int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str_char.c_str(), str_char.size(), (LPWSTR)str0.data(), str0.capacity());
		str_char[requiredSize] = 0;
	}
	else if (suggestedEncoding != -1)
	{
		t.seekg(0);
		str0.reserve(filesize+1);
		str0.assign(filesize + 1, 0);
		std::string str_char;
		str_char.reserve(filesize + 1);
		str_char.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		int requiredSize = MultiByteToWideChar(suggestedEncoding, 0, str_char.c_str(), str_char.size(), (LPWSTR)str0.data(), str0.capacity());
		str_char[requiredSize] = 0;
	}
	else
	{
		t.close();
		return false;
	}

	t.close();

	outstr.assign(str0.c_str());

	// remove some start-end block
	if (outstr.find(L"start") == 0)
	{
		auto endPos = outstr.find(L"end");
		outstr.erase(0, endPos + 3);
	}

	RemoveReturns(outstr);

	// remove comments
	for (auto p = outstr.find_first_of('{'); p != std::wstring::npos; p = outstr.find_first_of('{'))
	{
		auto p2 = outstr.find_first_of('}', p);
		if (p == std::wstring::npos)
		{
			printf("ERROR: Closing comment bracket was not found in file %s.\n", filename.c_str());
			return false;
		}
		outstr.erase(p, p2 - p + 1);
	}
	return true;
}

std::vector<std::string> inputFiles;
std::string outputFile;
bool isRussian = false; // todo more
bool isJapanese = false; // todo
bool isPolish = false;
bool isMobile = false; // todo more

int main(int argc, char* argv[])
{
	/* args:
	-g [III, VC, SA, LCS, VCS, IV]: game
	-m: is mobile
	-r: is russian
	-p: is polish
	-j: is japanese
	-i [a b .. z]: input txt files
	-o [file]: output gxt files
	*/


	printf("GXT Compiler by Sergeanur\n");

	if (argc < 2)
	{
		printf(
			"Usage:\n"
			"gxt -g [game] -i [input files] -o [output_file] [-r] [-j]\n"
			"\n"
			"Parameters:\n"
			"-g [III|VC|SA|LCS|VCS|IV]: game for which you're compiling a GXT file\n"
			"-m: set this is you're compiling for mobile GTA\n"
			"-i [input files]: list of txt files which you want to compile into GXT (can be one or multiple files)\n"
			"-o [output_file]: name of a GXT file that will be generated\n"
			"\n"
			"Optional parameters:\n"
			"-r: set this is you're compiling a russian text\n"
			"-p: set this is you're compiling a polish text\n"
			"-j: set this is you're compiling a japanese text\n");
		return 0;
	}

	std::string game = "";
	int cmdI = 1;
	while (cmdI < argc && argv[cmdI][0] == '-')
	{
		switch (argv[cmdI][1])
		{
		case 'g':
		{
			game = argv[++cmdI];
			// turn to uppercase
			for (auto& c : game)
				c = toupper(c);
			break;
		}
		case 'm':
			isMobile = true;
			break;
		case 'r':
			isRussian = true;
			break;
		case 'p':
			isPolish = true;
			break;
		case 'j': 
			isJapanese = true;
			break;
		case 'i':
			cmdI++;
			assert(cmdI < argc);
			while (cmdI < argc && argv[cmdI][0] != '-')
			{
				inputFiles.push_back(argv[cmdI++]);
			}
		case 'o':
			assert(cmdI+1 < argc);
			outputFile = argv[++cmdI];
		default:
			break;
		}
		cmdI++;
	}

	if (inputFiles.size() < 1)
	{
		printf("ERROR: No input file specified.\n");
		return 0;
	}

	if (outputFile.empty())
	{
		printf("ERROR: No output file specified.\n");
		return 0;
	}

	cStringCompiler* stringCompiler;

	if (game == "III")
	{
		if (isRussian)
			stringCompiler = new cStringCompilerIII_Rus;
		else if (isPolish)
			stringCompiler = new cStringCompilerIII_Pl;
		else
			stringCompiler = new cStringCompilerIII;
	}
	else if (game == "VC")
	{
		if (isMobile)
			stringCompiler = new cStringCompilerVC_Mobile;
		else
			stringCompiler = new cStringCompilerVC;
	}
	else if (game == "SA")
		stringCompiler = new cStringCompilerSA;
	else if (game == "LCS")
		stringCompiler = new cStringCompilerLCS;
	else if (game == "VCS")
		stringCompiler = new cStringCompilerVCS;
	else if (game == "IV")
		stringCompiler = new cStringCompilerIV;
	else
	{
		printf("Unknown game %s\n", game.c_str());
		return 0;
	}

	printf("Game - %s\n", game.c_str());

	std::wstring str_new = L"";
	for (auto inputFile : inputFiles)
	{
		std::wstring str;
		int suggestedEncoding = 1252;
		if (isRussian) suggestedEncoding = 1251;
		else if (isPolish) suggestedEncoding = 1250;
		else if (isJapanese) suggestedEncoding = -1;
		if (!ReadFileToWString(inputFile, suggestedEncoding, str))
		{
			delete stringCompiler;
			return 0;
		}

		str_new += str;
	}

	// remove double spaces
	std::wstring::iterator new_end = std::unique(str_new.begin(), str_new.end(), [](wchar_t lhs, wchar_t rhs) { return (lhs == rhs) && (lhs == ' '); });
	str_new.erase(new_end, str_new.end());

	auto skob_pos = str_new.find('[');
	while (skob_pos != std::wstring::npos)
	{
		auto skob_close_pos = str_new.find(']', skob_pos);
		if (skob_close_pos == std::wstring::npos)
		{
			wchar_t nearStr[10];
			wcscpy_s(nearStr, str_new.c_str() + skob_pos);
			printf("ERROR: Didn't find closing key bracket near %S\n", nearStr);
			delete stringCompiler;
			return 0;
		}

		std::wstring tableName = L"MAIN";

		auto keyName = str_new.substr(skob_pos + 1, skob_close_pos - skob_pos - 1);

		// looking for the table name in the key
		{
			int tableNamePos = keyName.find(':');
			if (tableNamePos != std::wstring::npos)
			{
				tableName = keyName.substr(tableNamePos + 1);
				keyName.resize(tableNamePos);
			}
		}

		// turn key to uppercase
		for (auto& c : keyName)
			c = toupper(c);


		skob_pos = str_new.find('[', skob_close_pos);
		if (skob_pos == std::wstring::npos)
		{
			// THAT DUMMY LABEL NEEDS TO BE THERE !!!
			break;
		}

		auto text = str_new.substr(skob_close_pos + 1, skob_pos - skob_close_pos - 1);
		trim(text);
		if (!text.empty())
			stringCompiler->AddTextLine(tableName, keyName, text);
	}

	// save gxt file
	std::ofstream t2(outputFile, std::ios::binary);

	if (!t2.is_open())
	{
		printf("ERROR: Output file %s could not be opened.\n", outputFile.c_str());
		delete stringCompiler;
		return 0;
	}

	stringCompiler->OutputIntoStream(t2);
	t2.close();

	delete stringCompiler;
	return 0;
}