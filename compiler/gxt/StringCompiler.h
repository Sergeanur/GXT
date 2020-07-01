#pragma once

#include "KeyChar.h"
#include "TableData.h"

template<class t1, class t2>
class TablePair : public std::pair<t1, t2>
{
public:
	bool operator<(const TablePair<t1, t2>& r) { return this->first < r.first; }
};

template<class t1, class t2>
bool operator<(const TablePair<t1, t2>& l, const TablePair<t1, t2>& r) { return l.first < r.first; }

template<class t1, class t2>
class umap : public std::vector<TablePair<t1, t2>>
{
public:
	t2& operator[](const t1& k)
	{
		for (auto elem = this->begin(); elem != this->end(); elem++)
		{
			if (elem->first == k)
				return elem->second;
		}
		t2 l;
		std::pair<t1, t2> p = (std::make_pair(k, l));
		this->push_back(*(TablePair<t1, t2>*)&p);

		return (this->end() - 1)->second;
	}

	t2& operator[] (t1&& k)
	{
		for (auto elem = this->begin(); elem != this->end(); elem++)
		{
			if (elem->first == k)
				return elem->second;
		}
		t2 l;
		std::pair<t1, t2> p(k, l);
		this->push_back(p);

		return (this->end()-1)->second;
	}
};

class cStringCompiler
{
protected:
	virtual void WriteHeaderStuff(std::ostream& s) {};
	virtual bool IsMainTableNeedSorting() { return true; };
public:
	virtual void AddTextLine(const std::wstring& table, const std::wstring& key, const std::wstring& text) = 0;
	virtual void OutputIntoStream(std::ostream& s) = 0;
	virtual bool IsSortedTableList() = 0;
};

template<class char_type, class key_type>
class cStringCompilerTmpl : public cStringCompiler
{
public:
	typedef char_type tCharType;
	typedef key_type tKeyType;

	struct sTableData
	{
		std::vector<TableEntry<key_type>> keys;
		std::vector<char_type> data;

		friend std::ostream& operator<<(std::ostream& os, const sTableData& td)
		{
			size_t n;

			os.write("TKEY", 4);
			n = td.keys.size() * sizeof(TableEntry<key_type>);
			os.write((const char*)&n, 4);


			for (auto key : td.keys) {
				os.write((const char*)&key.offset, 4);
				os.write((const char*)&key.key, sizeof(key_type));
			}

			os.write("TDAT", 4);
			n = td.data.size() * sizeof(char_type);
			os.write((const char*)&n, 4);

			os.write((const char*)td.data.data(), n);
			return os;
		}
	};

	sTableData mainTable;
	umap<std::wstring, sTableData> missionTables;

public:
	void AddTextLine(const std::wstring& table, const std::wstring& key, const std::wstring& text)
	{
		key_type _key = key.c_str();

		sTableData* pTable = &mainTable;

		if (table.compare(L"MAIN"))
			pTable = &missionTables[table];

		uint32 offset = pTable->data.size() * sizeof(char_type);
		pTable->keys.push_back(TableEntry<key_type>(_key, offset));
		ConvertString(key, text, pTable->data);
	}

	virtual wchar_t ConvertChar(wchar_t c) = 0;

	virtual void ConvertString(const std::wstring& key, const std::wstring& s, std::vector<char_type>& buf)
	{
		for (auto i = s.begin(); i != s.end(); i++)
			buf.push_back(ConvertChar(*i));
		buf.push_back(0);
	}

	void OutputIntoStream(std::ostream& s)
	{
		WriteHeaderStuff(s);
		if (missionTables.size() > 0)
		{
			if (IsSortedTableList())
				std::sort(missionTables.begin(), missionTables.end());

			size_t thisPos = size_t(s.tellp());
			s.write("TABL", 4);
			KeyChar8 tablName;
			size_t n = (missionTables.size() + 1) * (sizeof(tablName) + sizeof(size_t));
			s.write((const char*)&n, sizeof(size_t));

			size_t virtPos = thisPos + n + 8;

			tablName = "MAIN";

			s.write((char*)&tablName, sizeof(tablName));
			s.write((char*)&virtPos, sizeof(size_t));

			virtPos += mainTable.data.size() * sizeof(char_type) + 4 + 4 + mainTable.keys.size() * sizeof(TableEntry<key_type>) + 4 + 4;
			while (virtPos & 3) virtPos++;

			for (auto missionTable : missionTables)
			{
				tablName = missionTable.first.c_str();
				s.write((char*)&tablName, sizeof(tablName));
				s.write((char*)&virtPos, sizeof(size_t));

				virtPos += missionTable.second.data.size() * sizeof(char_type) + 4 + 4 + missionTable.second.keys.size() * sizeof(TableEntry<key_type>) + 4 + 4 + 8;
				while (virtPos & 3) virtPos++;
			}

			if (IsMainTableNeedSorting())
				std::sort(mainTable.keys.begin(), mainTable.keys.end());
			printf("Table MAIN has %i keys\n", mainTable.keys.size());
			s << mainTable;

			for (auto missionTable : missionTables)
			{
				std::sort(missionTable.second.keys.begin(), missionTable.second.keys.end());
				char z = 0;
				while (s.tellp() & 3) s.write(&z, 1);
				tablName = missionTable.first.c_str();
				s.write((char*)&tablName, sizeof(tablName));
				printf("Table %s has %i keys\n", &tablName, missionTable.second.keys.size());
				s << missionTable.second;
			}

		}
		else
		{
			if (IsMainTableNeedSorting())
				std::sort(mainTable.keys.begin(), mainTable.keys.end());
			printf("Table MAIN has %i keys\n", mainTable.keys.size());
			s << mainTable;
		}
	}
};