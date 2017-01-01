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

string dir_searh_tmp = "C:\\bigdata\\morpheme_train_knn\\"; // 형태소분석한 파일을 이용한다.
string dir_search = "C:\\bigdata\\morpheme_train_knn\\*";
string file_search = "";

string test_search = "C:\\bigdata\\knn_test\\output8.txt"; 
string output_search = "C:\\bigdata\\knn_similarity\\output.txt";

int dict_cnt = 0;
int file_cnt = 0;
int dictIndex = 1;

/*

** kNN방식은 입력 문서와 가장 유사도가 높은 상위 k개의 학습 문서
- k값은 홀수(이 프로젝트에서는 상위 3개로 설정함)
- cosine 유사도를 이용함

** 문서들에 이미 할당된 범주 정보를 이용하여 입력 문서의 범주를 결정하는 것임
- 범주는 2개로 설정함
- 추출된 k개의 유사 문서 중 가중치 값의 합이 가장 높게 나타내는 범주를 이용함

** test할 문서는 범주 <2.인터넷>과 범주 <6.데이터베이스> 중 범주 <2.인터넷>의 output8로 가정함 -> 즉 test문서는 범주 <2.인터넷>과 유사하다는 결과 기대.

*/

int main()
{
	long dirHandle;  //찾기 위한 핸들
	_finddata_t dirInfo;   //폴더 정보 구조체
	dirHandle = _findfirst(dir_search.c_str(), &dirInfo);  //처음 폴더 찾기
	int file_tmp = 1;
	int dict_index = 0;
	int setIndex = 0;

	if (-1 == dirHandle) return -1;
	_findnext(dirHandle, &dirInfo);
	
	double result[2] = { 0 }; // 2개 범주를 비교하여 결과값을 넣어줌
	priority_queue<double> pq[2]; // 각 범주당 유사도가 높은 상위3개의 문서를 뽑는다.

    ///////////////////////// test할 파일 열기(형태소파일)
	//우선 test할 파일의 term들을 set에 저장한다.

	set<string> test_allTerm;
	string str;

	ifstream txtFile(test_search.c_str());
	while (txtFile >> str)
	{
		test_allTerm.insert(str);
	}
	txtFile.close();

	while (!_findnext(dirHandle, &dirInfo))  // 2개 폴더가 저장된 상위폴더, 폴더가 2개라면 2개 끝까지 탐색
	{
		long fileHandle;
		_finddata_t fileInfo;
		file_search = dir_searh_tmp + dirInfo.name + "\\*";
		fileHandle = _findfirst(file_search.c_str(), &fileInfo);
		if (-1 == fileHandle) return -1;
		_findnext(fileHandle, &fileInfo);
		dict_cnt++;

		set<string> s[10];	// 10개 txt데이터의 term들을 중복없이 넣어줌.

		while (!_findnext(fileHandle, &fileInfo))  // 7개 txt데이터가 저장된 상위 폴더, 폴더내 처음 txt파일 찾기
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

		COS 유사도 a/((b^2*b^2)^(1/2))
		a=d1와 d2에서 같은 term개수
		b=d1에서의 term 개수, c=d2에서의 term개수
		*/

		//d1의 term들이 저장되어 있는 set을 살펴보며 d2에 같은 term이 있는지 확인하며 갯수를 카운트함

		for (int i = 0; i < 10; i++)
		{
			double a = 0, b = 0, c = 0;
			double cos = 0;
			double tmpCos = 0;

			for (set<string>::iterator sit = test_allTerm.begin(); sit != test_allTerm.end(); sit++)
			{
				if (s[i].find(*sit) != s[i].end()) //test의 term이 s[i]에도 존재하면
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

	outFile << "test파일과 범주1의 상위 3개의 유사도 합은 " << result[0] << "이고" << endl;
	outFile << "test파일과 범주2의 상위 3개의 유사도 합은 " << result[1] << "이고" << endl;
	outFile << "즉 test파일의 범주는 " << tmp << "번째 문서에 가깝습니다" << endl;

	_findclose(dirHandle);   //handle 종료
	return 0;
}

