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
	long dirHandle;  //ã�� ���� �ڵ�
	_finddata_t dirInfo;   //���� ���� ����ü
	dirHandle = _findfirst(dir_search.c_str(), &dirInfo);  //ó�� ���� ã��
	int file_tmp = 1;

	if (-1 == dirHandle) return -1;
	_findnext(dirHandle, &dirInfo);

	// vector<vector<pair<int, int> > > dv; // �� �н����� di�� ���� ���� ���� ���� TID,����ġ
	vector<pair<int, double> > dv[20000];

	while (!_findnext(dirHandle, &dirInfo))  // 80�� ������ ����� ��������, ������ 80����� 80�� ������ Ž��
	{
		long fileHandle;
		_finddata_t fileInfo;
		file_search = dir_searh_tmp + dirInfo.name + "\\*";
		fileHandle = _findfirst(file_search.c_str(), &fileInfo);
		if (-1 == fileHandle) return -1;
		_findnext(fileHandle, &fileInfo);
		dict_cnt++;

		multiset<string> df; // �� ���ֿ��� Ư���� �ܾ ��� ������ �ִ��� ����
		vector<int> maxTf; // �� ���ֿ��� �� ������ maxTF�� vector�� ����Ͽ� �迭�� ����
		int N = 0; // �� ���ִ� ��ü ���� ����

		/////////////////////// df���ϱ�
		while (!_findnext(fileHandle, &fileInfo))  // 20�� txt�����Ͱ� ����� ���� ����, ������ ó�� txt���� ã��
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
					if (tmpCnt == 0) // ���⼭ maxTF�� ����Ͽ� ã�Ƴ�(�̷��� ���� �ʰ� wordcount�� ���� �ɼ��� -6�� �ְ� ù��°�͸� ã�� ���� ����)
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
			maxTf.push_back(tmpMax); // �� ������ maxTF�� �����.
			N++;
		}
		_findclose(fileHandle);
		file_cnt = 0;

		///////////////////////// �� ���ָ� �ݺ��ؼ� ���� (������ maxTF(d), DF(t)�� �˾Ƴ�)
		file_search = dir_searh_tmp + dirInfo.name + "\\*";
		fileHandle = _findfirst(file_search.c_str(), &fileInfo);
		if (-1 == fileHandle) return -1;
		_findnext(fileHandle, &fileInfo);

		int dvIndex = 0;

		while (!_findnext(fileHandle, &fileInfo))  // 20�� txt�����Ͱ� ����� ���� ����, ������ ó�� txt���� ã��
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
				dv[dvIndex].push_back(make_pair(tidCnt, dvW)); // ���� ���� ����
				tidCnt++;
			}
			dvIndex++;
		}

		_findclose(fileHandle);
		file_cnt = 0;
	}
	_findclose(dirHandle);   //handle ����

	////////////////////// ������� ���� ���� ���Ͽ� ���

	//string train_search = "C:\\bigdata\\trainData\\train";
	int trainIndex = 1;
	for (int i = 0; i < 2; i++) // ���ִ� 2���� �ִٰ� ����
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
