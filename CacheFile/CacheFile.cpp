// CacheFile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ScopeTimePrompter.h"

bool cacheFile(const std::string& file)
{
	const auto bufSize = 1024 * 1024;
	std::string buf(bufSize, '\0');
	std::ifstream fin(file, std::ios::binary);

	while (fin)
	{
		fin.read(&buf[0], bufSize);
	}

	return true;
}
bool cacheFileEx(const std::string & filepath)
{
	auto hFile = CreateFileA(filepath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return false;
	LARGE_INTEGER size;
	GetFileSizeEx(hFile, &size);
	auto hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (NULL == hMapping)
		return false;

	auto addr = (const char*)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
	if (NULL == addr)
		return false;

	auto ptmp = new char(0);
	for (size_t i = 0; i < size.QuadPart; i += 4096)
	{
		*ptmp |= addr[i];	//ptmp的操作只是不让编译器优化掉读的代码
	}
	*ptmp |= addr[size.QuadPart - 1];
	delete ptmp;

	UnmapViewOfFile(addr);
	CloseHandle(hMapping);
	CloseHandle(hFile);

	return true;
}
void main(const int argc, const char *argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		std::string filename = argv[i];
		ScopeTimePrompter stp(filename);
		cacheFileEx(filename);
	}
}

