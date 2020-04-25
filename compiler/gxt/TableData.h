#pragma once

template<class T>
class TableEntry
{
public:
	uint32 offset;
	T key;

	TableEntry(T _key, uint32 _offset) { key = _key; offset = _offset; }
	TableEntry<T>& operator =(const TableEntry<T>& t)
	{
		key = t.key; offset = t.offset;
		return *this;
	}
};

template<class T>
bool operator<(const TableEntry<T>& l, const TableEntry<T>& r)
{
	return l.key < r.key;
}