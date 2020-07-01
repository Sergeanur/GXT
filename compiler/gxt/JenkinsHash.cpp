#include "common.h"
#include "JenkinsHash.h"

template <class T>
uint32 jenkins_one_at_a_time_hash(const T* key)
{
	uint32 hash, i;
	for (hash = i = 0; key[i]; ++i)
	{
		hash += tolower(key[i]);
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

bool operator<(const JenkinsHash& l, const JenkinsHash& r) { return l.value < r.value; }

JenkinsHash::JenkinsHash(const char* str)
{
	value = jenkins_one_at_a_time_hash(str);
}

JenkinsHash::JenkinsHash(const wchar_t* str)
{
	value = jenkins_one_at_a_time_hash(str);
}