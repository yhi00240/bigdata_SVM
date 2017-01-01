#pragma warning(disable:4996)
#include<iostream>
#include<string>
#include<io.h>
#include<stdlib.h>
#include<set>
#include<map>
#include<fstream>
#include<cstdio>
#include<sstream>
#include<vector>
#include<cmath>

using namespace std;

string dir_searh_tmp = "C:\\bigdata\\wordCount(trainData)\\";
string dir_search = "C:\\bigdata\\wordCount(trainData)\\*";
string file_search = "";
string train_search = "C:\\bigdata\\trainData\\train";

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

	// vector<vector<pair<int, int> > > dv; // 각 학습문서 di에 대해 문서 벡터 저장 TID,가중치
	vector<pair<int, double> > dv[20000];

	while (!_findnext(dirHandle, &dirInfo))  // 80개 폴더가 저장된 상위폴더, 폴더가 80개라면 80개 끝까지 탐색
	{
		long fileHandle;
		_finddata_t fileInfo;
		file_search = dir_searh_tmp + dirInfo.name + "\\*";
		fileHandle = _findfirst(file_search.c_str(), &fileInfo);
		if (-1 == fileHandle) return -1;
		_findnext(fileHandle, &fileInfo);
		dict_cnt++;

		multiset<string> df; // 한 범주에서 특정한 단어가 몇개의 문서에 있는지 저장
		vector<int> maxTf; // 한 범주에서 각 문서당 maxTF를 vector를 사용하여 배열로 저장
		int N = 0; // 한 범주당 전체 문서 개수

		/////////////////////// df구하기
		while (!_findnext(fileHandle, &fileInfo))  // 20개 txt데이터가 저장된 상위 폴더, 폴더내 처음 txt파일 찾기
		{
			string tmp_fileName = dir_searh_tmp + dirInfo.name + "\\" + fileInfo.name;
			ifstream txtFile(tmp_fileName);
			int tmpCnt;
			int tmpMax = -1;
			while (!txtFile.eof())
			{
				string tmpStr, str;
				stringstream tmpStream;

				getline(txtFile, tmpStr);
				tmpStream.str(tmpStr);
				tmpCnt = 0;
				while (tmpStream >> str) 
				{
					if (tmpCnt == 0) // 여기서 maxTF를 계속하여 찾아냄(이렇게 하지 않고 wordcount를 돌려 옵션을 -6을 주고 첫번째것만 찾을 수도 있음)
					{
						if (tmpMax < atoi(str.c_str()))
						{
							tmpMax = atoi(str.c_str());
						}
						tmpCnt = 1;
						continue; 
					}
					df.insert(str);

				}
			}
			maxTf.push_back(tmpMax); // 각 문서당 maxTF가 저장됨.
			N++;
		}
		_findclose(fileHandle);
		file_cnt = 0;

		///////////////////////// 한 범주를 반복해서 열기 (위에서 maxTF(d), DF(t)를 알아냄)
		file_search = dir_searh_tmp + dirInfo.name + "\\*";
		fileHandle = _findfirst(file_search.c_str(), &fileInfo);
		if (-1 == fileHandle) return -1;
		_findnext(fileHandle, &fileInfo);

		int dvIndex = 0;

		while (!_findnext(fileHandle, &fileInfo))  // 20개 txt데이터가 저장된 상위 폴더, 폴더내 처음 txt파일 찾기
		{
			string tmp_fileName = dir_searh_tmp + dirInfo.name + "\\" + fileInfo.name;
			ifstream txtFile(tmp_fileName);
			int tmpCnt;
			int tidCnt = 1;
			while (!txtFile.eof())
			{
				string tmpStr, str;
				stringstream tmpStream;

				getline(txtFile, tmpStr);
				tmpStream.str(tmpStr);
				tmpCnt = 0;

				double dvFreq=0, dvMaxTf=0;
				double dvTf=0, dvW=0;
				float dvIdf = 0;

				dvMaxTf = maxTf[dvIndex];

				while (tmpStream >> str)
				{
					if (tmpCnt == 0)
					{
						dvFreq = atoi(str.c_str());
						tmpCnt = 1;
						continue;
					}
					int cnttt=df.count(str);
					dvIdf=log((float)20/df.count(str));
					dvTf = dvFreq / dvMaxTf;
					dvW = dvTf * dvIdf;
				}
				dv[dvIndex].push_back(make_pair(tidCnt, dvW)); // 문서 벡터 생성
				tidCnt++;
			}
			dvIndex++;
		}

		_findclose(fileHandle);
		file_cnt = 0;
	}
	_findclose(dirHandle);   //handle 종료

	////////////////////// 만들어진 문서 벡터 파일에 출력

	//string train_search = "C:\\bigdata\\trainData\\train";
	int trainIndex = 1;
	for (int i = 0; i < 2; i++) // 범주는 2개만 있다고 가정
	{
		string train_search_tmp = train_search + to_string(trainIndex) + ".dat";
		ofstream outFile(train_search_tmp.c_str());
		//outFile << " " << trainIndex << " ";
		for (int j = 1; j <= 20; j++)
		{
			outFile << " " << j << " ";
			for (auto vit = dv[j-1].begin(); vit != dv[j - 1].end(); vit++)
			{
				outFile << " " << vit->first << ":" << vit->second << " ";
			}
			outFile << endl;
		}
		outFile << endl;
		trainIndex++;
	}
	return 0;
}
