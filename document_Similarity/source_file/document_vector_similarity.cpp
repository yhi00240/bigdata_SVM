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

string dir_searh_tmp = "C:\\bigdata\\morpheme_train\\output"; // 형태소분석한 파일을 이용한다.
string dir_search = "C:\\bigdata\\morpheme_train\\*";
string file_search = "";

string output_search = "C:\\bigdata\\similarity\\similarity";

int dict_cnt = 0;
int file_cnt = 0;
int dictIndex = 1;

/*
각 범주별로, 임의의 두 문서의 유사도를 분석한다.
먼저 set으로 한 범주의 term개수 다 넣어주고
알아야할것, 각각 문서에서 전체 set에서 몇개 겹치는 지 확인.
그리고 임의의 두 문서가 몇개 겹치는지
--> d1먼저 set에 넣고, d2에 있으면 확인
*/

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
		///////////////////////// 한 범주당, 임의의 두 문서를 뽑기 위해 1~21(임의로 조정가능)에서 두 숫자를 뽑는다.
		///////////////////////// 일단 문서 output3, output7 이라고 가정(문서 번호도 조정가능)
		
		///////////////////////// 한 범주를 반복해서 열기(형태소 파일 output3, output7 열기)
		string file1 = dir_searh_tmp + to_string(dictIndex) + "\\output3.txt";
		string file2 = dir_searh_tmp + to_string(dictIndex) + "\\output7.txt";

		//우선 두 문서의 term들을 set에 저장한다.

		set<string> file1_allTerm;
		set<string> file2_allTerm;
		string str;

		ifstream txtFile(file1.c_str());
		while (txtFile >> str)
		{
			file1_allTerm.insert(str);
		}
		//txtFile.close();

		ifstream txtFile2(file2.c_str());
		while (txtFile2 >> str)
		{
			file2_allTerm.insert(str);
		}
		//txtFile2.close();

		double a = 0, b = 0, c = 0; 
		double dice=0, jacc=0, cos=0;

		/*
		T={computer, retrieval, archiving, hypertext, hypermedia, indexing}
		d1={					archiving,			  hypermedia		  }
		d2={		 retrieval,									  indexing}
		
		DICE유사도 2a/b+c			
		JACC유사도 a/b+c-a
		COS 유사도 a/((b^2*b^2)^(1/2))
		a=d1와 d2에서 같은 term개수
		b=d1에서의 term 개수, c=d2에서의 term개수
		*/
		
		//d1의 term들이 저장되어 있는 set을 살펴보며 d2에 같은 term이 있는지 확인하며 갯수를 카운트함

		for (set<string>::iterator sit = file1_allTerm.begin(); sit != file1_allTerm.end(); sit++)
		{
			if (file2_allTerm.find(*sit) != file2_allTerm.end() ) //d1의 term이 d2에도 존재하면
			{
				a++;
			}
		}
		b = file1_allTerm.size();
		c = file2_allTerm.size();

		dice = 2 * a / (b + c);
		jacc = a / (b + c - a);
		cos = a / (sqrt(b*b)*sqrt(c*c));

		string output_search_tmp = output_search + to_string(dictIndex)+".txt";
		ofstream outFile(output_search_tmp.c_str());

		outFile << "문서" << dictIndex << endl;
		outFile << "output3과 output7의 유사도 분석" << endl;
		outFile << "DICE similarity : " << dice << endl;
		outFile << "JACC similarity : " << jacc << endl;
		outFile << "COS similarity : " << cos << endl;
		dictIndex++;
		//outFile.close();
	}

	_findclose(dirHandle);   //handle 종료
	return 0;
}

