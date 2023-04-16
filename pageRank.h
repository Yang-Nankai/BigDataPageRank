#ifndef PAGERANK_PAGERANK_H
#define PAGERANK_PAGERANK_H

#include<map>;
#include<vector>
#include<set>
#include<string>
using namespace std;

//����PageRank��
class PageRank {
private:
	map<int, set<int>> matrix;		//�洢ϡ�����ʹ��map+set�ķ�ʽ���ܼ��ٴ洢�ռ䣬�Լ����������
	int maxIteration;				//����������
	long double beta;				//�������ϵ��
	long double convergence;		//�����޽�
	map<int, int> nodeToIndex;		//��ʼ�������ɢ�����ŵ�ӳ�䣬������ɢ����Ϊ��ȥ��һЩ�������ڵĽڵ�(��û�����Ҳû�г���)�����پ���Ĵ洢�ռ�
	map<int, int> indexToNode;		//��ɢ����ı�����ʼ��ŵ�ӳ��
	vector<long double> pr;			//PageRank����
	vector<int> degree;				//��ɢ��ÿһ����ĳ�����
	int totalNum;					//��ɢ����ı������
	int showNum;
	int blockNum;					//�ֿ����
	int totalEdges;					//�����ܹ��ı���
	int minItemNum, maxItemNum;		//��С��ź������
	vector<pair<long double, int>> output;	//�������������

public:
	long double getConvergence() const;
	void setConvergence(long double convergence);
	long double getBeta() const;
	void setBeta(long double beta);
	int getMaxIteration() const;
	void setMaxIteration(int maxIteration);
	int getMaxItemNum() const;
	void setMaxItemNum(int maxItemNum);
	int getMinItemNum() const;
	void setMinItemNum(int maxItemNum);
	int getBlockNum() const;
	void setBlockNum(int blockNum);
	int getShowNum() const;
	void setShowNum(int showNum);

	//��ȡ�ļ�
	void readFile(string dataFile);
	//PageRank���캯��
	PageRank(int _minItemNum = -1, int _maxItemNum = -1, int _maxIteration = -1, long double _beta = 0,
		int block_nums = 1, long double convergence = 1e-5, int _showNum = 0);
	//Ĭ����������
	~PageRank() = default;
	//����ֵ��matrix��
	void insertItem(int a, int b);
	//pageRank�㷨
	void pageRank();
	//��ȡ�ļ��õ����ֵ����Сֵ
	void getItemRange(string dataFile);
	//��ӡ�����Ϣ
	void print();
};





#endif