#include "stdafx.h"
#include "FileUtils.h"
#include <ShlObj.h>
#include <iostream>
#include <fstream>
#include <io.h>

FileUtils::FileUtils()
{
}


FileUtils::~FileUtils()
{
}
FileUtils* FileUtils::_ins = nullptr;
FileUtils * FileUtils::getIns()
{
	if (nullptr == _ins)
	{
		_ins = new(std::nothrow) FileUtils();
	}
	return _ins;
}
//windwos api
long FileUtils::getFileSize(const char * path)
{
	long size = 0;
	WIN32_FILE_ATTRIBUTE_DATA attribute;
	if (!GetFileAttributesExA(path, GetFileExInfoStandard, &attribute)) {
		size = 0;
	}
	else
	{
		LARGE_INTEGER integer;
		integer.HighPart = attribute.nFileSizeHigh;
		integer.LowPart = attribute.nFileSizeLow;
		size = (long)integer.QuadPart;
	}
	return size;
}
//Դ����cocos2d-x
unsigned char * FileUtils::getFileData(const char * path, bool forString)
{
	size_t size = 0;
	size_t readsize;
	unsigned char* buffer = nullptr;
	do
	{
		FILE *fp;
		fopen_s(&fp,path, "rb");
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		if (forString)
		{
			buffer = (unsigned char*)malloc(sizeof(unsigned char) * (size + 1));
			buffer[size] = '\0';
		}
		else
		{
			buffer = (unsigned char*)malloc(sizeof(unsigned char) * size);
		}
		readsize = fread(buffer, sizeof(unsigned char), size, fp);
		fclose(fp);
		if (forString && readsize < size)
		{
			buffer[readsize] = '\0';
		}
	} while (0);
	return buffer;
}
//Դ�������磬�����Լ��Ĺ�
unsigned char * FileUtils::getFileData(const char * path)
{
	std::filebuf *pbuf;
	std::ifstream filestr;
	unsigned char* ret;
	std::streamoff size;
	// Ҫ���������ļ���������ö����ƴ� 
	filestr.open(path, std::ios::binary);
	// ��ȡfilestr��Ӧbuffer�����ָ�� 
	pbuf = filestr.rdbuf();
	// ����buffer���󷽷���ȡ�ļ���С
	size = pbuf->pubseekoff(0, std::ios::end, std::ios::in);
	pbuf->pubseekpos(0, std::ios::in);
	// �����ڴ�ռ�
	ret = (unsigned char*)malloc(sizeof(unsigned char) * size);
	// ��ȡ�ļ�����
	filestr.read((char*)ret, size);
	filestr.close();
	return ret;
}

void FileUtils::WritableData(const char * path, const unsigned char* data, unsigned int size)
{
	std::ofstream filestr(path, std::ios::out | std::ios::binary);
	if (filestr)
	{
		std::ifstream iffilestr(path, std::ios::binary | std::ios::in);
		filestr.write((char*)data, size);
		filestr.flush();
		filestr.close();
	}
}
