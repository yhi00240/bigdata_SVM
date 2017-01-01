#pragma warning(disable:4996)
#include<iostream>
#include<string>
#include<io.h>
#include <stdlib.h>

using namespace std;

string dir_searh_tmp = "C:\\bigdata\\TestingData\\";
string dir_search = "C:\\bigdata\\TestingData\\*";
string file_search = "";

string exe_search = "C:\\bigdata\\KLT2010-TestVersion-2015\\EXE\\.\\index -scw"; //형태소 분석기 
string exe_cmd_str = ""; // 형태소 분석기 명령어
string exe_output_search = "C:\\bigdata\\output";

int dict_cnt = 0;
int file_cnt = 0;

int main()
{
	long dirHandle;  //찾기 위한 핸들
	_finddata_t dirInfo;   //폴더 정보 구조체
	dirHandle = _findfirst(dir_search.c_str(), &dirInfo);  //처음 폴더 찾기
	int file_tmp = 1;

	if (-1 == dirHandle) return -1;
	_findnext(dirHandle, &dirInfo);

	while (!_findnext(dirHandle, &dirInfo))  // 80개 폴더가 저장된 상위폴더, 폴더가 80개라면 80개 끝까지 탐색
	{
		long fileHandle;
		_finddata_t fileInfo;
		file_search = dir_searh_tmp + dirInfo.name + "\\*";
		fileHandle = _findfirst(file_search.c_str(), &fileInfo);
		if (-1 == fileHandle) return -1;
		_findnext(fileHandle, &fileInfo);
		dict_cnt++;
		while (!_findnext(fileHandle, &fileInfo))  // 20개 txt데이터가 저장된 상위 폴더, 폴더내 처음 txt파일 찾기
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

	_findclose(dirHandle);   //handle 종료
	return 0;
}
