#pragma warning(disable:4996)
#include<iostream>
#include<string>
#include<io.h>
#include <stdlib.h>

using namespace std;

string dir_searh_tmp = "C:\\bigdata\\TestingData\\";
string dir_search = "C:\\bigdata\\TestingData\\*";
string file_search = "";

string exe_search = "C:\\bigdata\\KLT2010-TestVersion-2015\\EXE\\.\\index -scw"; //���¼� �м��� 
string exe_cmd_str = ""; // ���¼� �м��� ��ɾ�
string exe_output_search = "C:\\bigdata\\output";

int dict_cnt = 0;
int file_cnt = 0;

int main()
{
	long dirHandle;  //ã�� ���� �ڵ�
	_finddata_t dirInfo;   //���� ���� ����ü
	dirHandle = _findfirst(dir_search.c_str(), &dirInfo);  //ó�� ���� ã��
	int file_tmp = 1;

	if (-1 == dirHandle) return -1;
	_findnext(dirHandle, &dirInfo);

	while (!_findnext(dirHandle, &dirInfo))  // 80�� ������ ����� ��������, ������ 80����� 80�� ������ Ž��
	{
		long fileHandle;
		_finddata_t fileInfo;
		file_search = dir_searh_tmp + dirInfo.name + "\\*";
		fileHandle = _findfirst(file_search.c_str(), &fileInfo);
		if (-1 == fileHandle) return -1;
		_findnext(fileHandle, &fileInfo);
		dict_cnt++;
		while (!_findnext(fileHandle, &fileInfo))  // 20�� txt�����Ͱ� ����� ���� ����, ������ ó�� txt���� ã��
		{
			string tmp_fileName = dir_searh_tmp + dirInfo.name + "\\" + fileInfo.name;
			exe_cmd_str = exe_search + " " + tmp_fileName + " " + exe_output_search + to_string(dict_cnt)+"\\output"+ to_string(++file_cnt) + ".txt";
			char *exe_cmd = new char[exe_cmd_str.length() + 1];
			strcpy(exe_cmd, exe_cmd_str.c_str());
			system(exe_cmd);
			delete[] exe_cmd;
		}
		_findclose(fileHandle);
		file_cnt = 0;
	}

	_findclose(dirHandle);   //handle ����
	return 0;
}
