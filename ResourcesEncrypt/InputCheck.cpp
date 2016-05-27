#include "stdafx.h"
#include "InputCheck.h"
#include "xxtea.h"
#include "FileUtils.h"
/**
*/

InputCheck::InputCheck()
	:_isFileDirectory(false),
	_directory(""),
	_fileType(""),
	_isEncrypt(-1)
{
	inputDirectory(true);
}

InputCheck::InputCheck(std::vector<char*> parm)
{
	if (parm.size() > 2)
	{
		_directory = parm.at(0);
		_isEncrypt = (size_t)parm.at(1);
		_fileType = parm.at(2);
		inputDirectory(false);
	}
	else
	{
		inputDirectory(true);
	}
	
}

InputCheck::~InputCheck()
{
}

void InputCheck::inputDirectory(const bool isCin)
{
	if (isCin)
	{
		std::cin >> _directory;
		std::cin >> _isEncrypt;
	}
	isFileDirectory(_directory);
	if (!_isFileDirectory)
	{
		std::cout << "��������ļ�Ŀ¼������,����������:" << std::endl;
		inputDirectory(true);
	}
	else
	{
		std::cout << "������Ҫ���ܵ��ļ���׺�磺.png,.jsonʹ��Ӣ�Ķ��ŷָ�" << std::endl;
		inputFileType(isCin);
	}
}

void InputCheck::inputFileType(const bool isCin)
{
	if (isCin)
	{
		std::cin >> _fileType;
	}
	if (_fileType.empty())
	{
		std::cout << "����Ϊ�����������룺" << std::endl;
		inputFileType(true);
		return;
	}
	std::string::size_type findIndex = _fileType.find(".", 0);
	if (findIndex != 0 || findIndex == std::string::npos)
	{
		std::cout << "���Ͳ��� ����������:" << std::endl;
		_spilts.clear();
		inputFileType(true);
		return;
	}
	std::string pattern = ",";
	std::string::size_type pos;
	size_t size = _fileType.size();
	std::cout << "�Ѿ��ָ������ַ���" << std::endl;
	for (size_t i = 0; i < size; i++)
	{
		pos = _fileType.find(pattern, i);
		pos = pos == std::string::npos ? size : pos;
		if (pos <= size)
		{
			std::string s = _fileType.substr(i, pos - i);
			_spilts.push_back(s);
			i = pos + pattern.size() - 1;
			std::cout << s << std::endl;
		}
	}
	checkFils(_directory);
}

bool InputCheck::isFileDirectory(const std::string path)
{
	_isFileDirectory = false;
	std::string str;
	struct _finddata_t fileinfo;
	size_t hFile = _findfirst(str.assign(path).append("\\*").c_str(), &fileinfo);
	_isFileDirectory = hFile != -1;
	return _isFileDirectory;
}

void InputCheck::checkFils(const std::string path)
{
	size_t hFile = 0l;
	struct _finddata_t fileinfo;
	std::string str;
	if ((hFile = _findfirst(str.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//// �����Ŀ¼,����֮�������,�����б�
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				/// .����������һ���ļ�..
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					std::cout << "�Ѿ���⵽�������ļ���" << fileinfo.name << std::endl;
					checkFils(str.assign(path).append("\\").append(fileinfo.name));
				}
			}
			else
			{
				for (size_t i = 0; i < _spilts.size(); i++)
				{
					auto str1 = strstr(fileinfo.name, _spilts.at(i).c_str());
					if (str1 != nullptr)
					{
						_files.push_back(str.assign(path).append("\\").append(fileinfo.name));
						printf("name = %s    index = %s\n", fileinfo.name, str1);
						break;
					}
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	startEncrypt();
}

void InputCheck::startEncrypt()
{
	auto length = _files.size();
	unsigned char* data = nullptr;
	unsigned char key[6] = "jerry";
	unsigned char* ret_data;
	FileUtils* file = FileUtils::getIns();
	xxtea_long rect_size;
	xxtea_long size;
	const char* path;
	for (size_t i = 0; i < length; i++)
	{
		path = _files.at(i).c_str();
		data = file->getFileData(path);
		size = file->getFileSize(path);
		if (_isEncrypt > 0)
		{
			ret_data = xxtea_encrypt(data, size, key, 6, &rect_size);
		}
		else
		{
			ret_data = xxtea_decrypt(data, size, key, 6, &rect_size);
		}
		if (nullptr != ret_data)
		{
			file->WritableData(path, ret_data, rect_size);
		}
	}
	std::cout << "�������" << std::endl;
}
