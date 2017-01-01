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
#include<queue>

using namespace std;

string dir_searh_tmp = "C:\\bigdata\\morpheme_train_knn\\"; // ���¼Һм��� ������ �̿��Ѵ�.
string dir_search = "C:\\bigdata\\morpheme_train_knn\\*";
string file_search = "";

string test_search = "C:\\bigdata\\knn_test\\output8.txt"; 
string output_search = "C:\\bigdata\\knn_similarity\\output.txt";

int dict_cnt = 0;
int file_cnt = 0;
int dictIndex = 1;

/*

** kNN����� �Է� ������ ���� ���絵�� ���� ���� k���� �н� ����
- k���� Ȧ��(�� ������Ʈ������ ���� 3���� ������)
- cosine ���絵�� �̿���

** �����鿡 �̹� �Ҵ�� ���� ������ �̿��Ͽ� �Է� ������ ���ָ� �����ϴ� ����
- ���ִ� 2���� ������
- ����� k���� ���� ���� �� ����ġ ���� ���� ���� ���� ��Ÿ���� ���ָ� �̿���

** test�� ������ ���� <2.���ͳ�>�� ���� <6.�����ͺ��̽�> �� ���� <2.���ͳ�>�� output8�� ������ -> �� test������ ���� <2.���ͳ�>�� �����ϴٴ� ��� ���.

*/

int main()
{
	long dirHandle;  //ã�� ���� �ڵ�
	_finddata_t dirInfo;   //���� ���� ����ü
	dirHandle = _findfirst(dir_search.c_str(), &dirInfo);  //ó�� ���� ã��
	int file_tmp = 1;
	int dict_index = 0;
	int setIndex = 0;

	if (-1 == dirHandle) return -1;
	_findnext(dirHandle, &dirInfo);
	
	double result[2] = { 0 }; // 2�� ���ָ� ���Ͽ� ������� �־���
	priority_queue<double> pq[2]; // �� ���ִ� ���絵�� ���� ����3���� ������ �̴´�.

    ///////////////////////// test�� ���� ����(���¼�����)
	//�켱 test�� ������ term���� set�� �����Ѵ�.

	set<string> test_allTerm;
	string str;

	ifstream txtFile(test_search.c_str());
	while (txtFile >> str)
	{
		test_allTerm.insert(str);
	}
	txtFile.close();

	while (!_findnext(dirHandle, &dirInfo))  // 2�� ������ ����� ��������, ������ 2����� 2�� ������ Ž��
	{
		long fileHandle;
		_finddata_t fileInfo;
		file_search = dir_searh_tmp + dirInfo.name + "\\*";
		fileHandle = _findfirst(file_search.c_str(), &fileInfo);
		if (-1 == fileHandle) return -1;
		_findnext(fileHandle, &fileInfo);
		dict_cnt++;

		set<string> s[10];	// 10�� txt�������� term���� �ߺ����� �־���.

		while (!_findnext(fileHandle, &fileInfo))  // 7�� txt�����Ͱ� ����� ���� ����, ������ ó�� txt���� ã��
		{
			string tmp_fileName = dir_searh_tmp + dirInfo.name + "\\" + fileInfo.name;
			ifstream txtFile(tmp_fileName);
			string str;

			while (txtFile >> str)
			{
				s[setIndex].insert(str);
			}
			setIndex++;
		}
		_findclose(fileHandle);
		file_cnt = 0;

		/*
		T={computer, retrieval, archiving, hypertext, hypermedia, indexing}
		d1={					archiving,			  hypermedia		  }
		d2={		 retrieval,									  indexing}

		COS ���絵 a/((b^2*b^2)^(1/2))
		a=d1�� d2���� ���� term����
		b=d1������ term ����, c=d2������ term����
		*/

		//d1�� term���� ����Ǿ� �ִ� set�� ���캸�� d2�� ���� term�� �ִ��� Ȯ���ϸ� ������ ī��Ʈ��

		for (int i = 0; i < 10; i++)
		{
			double a = 0, b = 0, c = 0;
			double cos = 0;
			double tmpCos = 0;

			for (set<string>::iterator sit = test_allTerm.begin(); sit != test_allTerm.end(); sit++)
			{
				if (s[i].find(*sit) != s[i].end()) //test�� term�� s[i]���� �����ϸ�
				{
					a++;
				}
			}
			b = test_allTerm.size();
			c = s[i].size();
			cos = a / (sqrt(b*b)*sqrt(c*c));

			pq[dict_index].push(cos);
		}
		//dictIndex++;
		dict_index++;
		setIndex = 0;
		//outFile.close();
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			result[j] += pq[j].top();
			pq[j].pop();
		}
	}
	int tmp = 1;
	if (result[0] < result[1]) { tmp = 2; }

	ofstream outFile(output_search.c_str());

	outFile << "test���ϰ� ����1�� ���� 3���� ���絵 ���� " << result[0] << "�̰�" << endl;
	outFile << "test���ϰ� ����2�� ���� 3���� ���絵 ���� " << result[1] << "�̰�" << endl;
	outFile << "�� test������ ���ִ� " << tmp << "��° ������ �������ϴ�" << endl;

	_findclose(dirHandle);   //handle ����
	return 0;
}

