// ResourcesEncrypt.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <io.h>
#include <vector>
#include <fstream>
#include <direct.h>
#include "InputCheck.h"
#include "FileUtils.h"
int main()
{
	char buffer[260];
	_getcwd(buffer, 260);
	std::fstream configFile(strcat(buffer,"\\configFile.txt"),std::ios::in);
	std::vector<char*> configFiles;
	if (configFile.is_open()) {
		while (!configFile.eof())
		{
			char *line = (char*)malloc(sizeof(char) * 150);
			configFile.getline(line, 150);
			configFiles.push_back(line);
		}
	}
	std::cout << "�������ļ�Ŀ¼(����·��)���Ǽ��ܻ��ǽ��ܣ�1��0����" << std::endl;
	InputCheck inputCheck(configFiles);
	return 0;
}



