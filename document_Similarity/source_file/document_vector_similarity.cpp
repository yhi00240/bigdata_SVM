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

string dir_searh_tmp = "C:\\bigdata\\morpheme_train\\output"; // ���¼Һм��� ������ �̿��Ѵ�.
string dir_search = "C:\\bigdata\\morpheme_train\\*";
string file_search = "";

string output_search = "C:\\bigdata\\similarity\\similarity";

int dict_cnt = 0;
int file_cnt = 0;
int dictIndex = 1;

/*
�� ���ֺ���, ������ �� ������ ���絵�� �м��Ѵ�.
���� set���� �� ������ term���� �� �־��ְ�
�˾ƾ��Ұ�, ���� �������� ��ü set���� � ��ġ�� �� Ȯ��.
�׸��� ������ �� ������ � ��ġ����
--> d1���� set�� �ְ�, d2�� ������ Ȯ��
*/

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
		///////////////////////// �� ���ִ�, ������ �� ������ �̱� ���� 1~21(���Ƿ� ��������)���� �� ���ڸ� �̴´�.
		///////////////////////// �ϴ� ���� output3, output7 �̶�� ����(���� ��ȣ�� ��������)
		
		///////////////////////// �� ���ָ� �ݺ��ؼ� ����(���¼� ���� output3, output7 ����)
		string file1 = dir_searh_tmp + to_string(dictIndex) + "\\output3.txt";
		string file2 = dir_searh_tmp + to_string(dictIndex) + "\\output7.txt";

		//�켱 �� ������ term���� set�� �����Ѵ�.

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
		
		DICE���絵 2a/b+c			
		JACC���絵 a/b+c-a
		COS ���絵 a/((b^2*b^2)^(1/2))
		a=d1�� d2���� ���� term����
		b=d1������ term ����, c=d2������ term����
		*/
		
		//d1�� term���� ����Ǿ� �ִ� set�� ���캸�� d2�� ���� term�� �ִ��� Ȯ���ϸ� ������ ī��Ʈ��

		for (set<string>::iterator sit = file1_allTerm.begin(); sit != file1_allTerm.end(); sit++)
		{
			if (file2_allTerm.find(*sit) != file2_allTerm.end() ) //d1�� term�� d2���� �����ϸ�
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

		outFile << "����" << dictIndex << endl;
		outFile << "output3�� output7�� ���絵 �м�" << endl;
		outFile << "DICE similarity : " << dice << endl;
		outFile << "JACC similarity : " << jacc << endl;
		outFile << "COS similarity : " << cos << endl;
		dictIndex++;
		//outFile.close();
	}

	_findclose(dirHandle);   //handle ����
	return 0;
}

