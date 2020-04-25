#include "common.h"
#include "utils.h"
#include "TableData.h"
#include "StringCompilers.h"
#include <fstream>
#include <streambuf>
#include <assert.h>

void RemoveReturns(std::string& s)
{
	size_t p = s.find("\r\n");
	while (p != std::string::npos)
	{
		s.replace(p, 2, " ");
		p = s.find("\r\n");
	}

	p = s.find('\r');
	while (p != std::string::npos)
	{
		s[p] = ' ';
		p = s.find('\r');
	}

	p = s.find('\n');
	while (p != std::string::npos)
	{
		s[p] = ' ';
		p = s.find('\n');
	}

	p = s.find('\t');
	while (p != std::string::npos)
	{
		s[p] = ' ';
		p = s.find('\t');
	}

}

std::vector<std::string> inputFiles;
std::string outputFile;
bool isRussian = false; // todo more
bool isJapanese = false; // todo

int main(int argc, char* argv[])
{
	/* args:
	-g [III, VC, SA, LCS, VCS, IV]: game
	-r: is russian
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
			"-i [input files]: list of txt files which you want to compile into GXT (can be one or multiple files)\n"
			"-o [output_file]: name of a GXT file that will be generated\n"
			"\n"
			"Optional parameters:\n"
			"-r: set this is you're compiling a russian text\n"
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
		case 'r':
			isRussian = true;
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
		else
			stringCompiler = new cStringCompilerIII;
	}
	else if (game == "VC")
		stringCompiler = new cStringCompilerVC;
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

	std::string str_new = "";
	for (auto inputFile : inputFiles)
	{
		std::string str0;
		std::ifstream t(inputFile);
		if (!t.is_open())
		{
			printf("ERROR: File %s could not be opened.\n", inputFile.c_str());
			delete stringCompiler;
			return 0;
		}

		t.seekg(0, std::ios::end);
		str0.reserve(t.tellg());
		t.seekg(0, std::ios::beg);

		int utf8BomCheck = 0;
		t.read((char*)&utf8BomCheck, 3);
		if (utf8BomCheck != 0xBFBBEF)
		{
			printf("ERROR: File %s is not encoded with UTF-8 BOM.\n", inputFile.c_str());
			delete stringCompiler;
			return 0;
		}

		// read all stream to string
		str0.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

		t.close();

		std::string str = stringCompiler->PrepareSource(str0.c_str());

		// remove some start-end block
		if (str.find("start") == 0)
		{
			auto endPos = str.find("end");
			str.erase(0, endPos + 3);
		}

		RemoveReturns(str);

		// remove comments
		for (auto p = str.find_first_of('{'); p != std::string::npos; p = str.find_first_of('{'))
		{
			auto p2 = str.find_first_of('}', p);
			if (p == std::string::npos)
			{
				printf("ERROR: Closing comment bracket was not found in file %s.\n", inputFile.c_str());
				return 0;
			}
			str.erase(p, p2 - p + 1);
		}

		str_new += str;
	}

	auto skob_pos = str_new.find('[');
	while (skob_pos != std::string::npos)
	{
		auto skob_close_pos = str_new.find(']', skob_pos);
		if (skob_close_pos == std::string::npos)
		{
			char nearStr[10];
			strcpy_s(nearStr, str_new.c_str() + skob_pos);
			printf("ERROR: Didn't find closing key bracket near %s\n", nearStr);
			delete stringCompiler;
			return 0;
		}

		std::string tableName = "MAIN";

		auto keyName = str_new.substr(skob_pos + 1, skob_close_pos - skob_pos - 1);

		// looking for the table name in the key
		{
			int tableNamePos = keyName.find(':');
			if (tableNamePos != std::string::npos)
			{
				tableName = keyName.substr(tableNamePos + 1);
				keyName.resize(tableNamePos);
			}
		}

		// turn key to uppercase
		for (auto& c : keyName)
			c = toupper(c);


		skob_pos = str_new.find('[', skob_close_pos);
		if (skob_pos == std::string::npos)
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