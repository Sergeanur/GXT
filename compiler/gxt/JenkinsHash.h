#pragma once

struct JenkinsHash
{
    uint32 value;
public:
    JenkinsHash() {}
    JenkinsHash(const char* str);
    JenkinsHash(uint32 val) : value(val) {}
};

bool operator<(const JenkinsHash& l, const JenkinsHash& r);