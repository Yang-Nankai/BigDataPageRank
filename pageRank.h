#ifndef PAGERANK_PAGERANK_H
#define PAGERANK_PAGERANK_H

#include<map>;
#include<vector>
#include<set>
#include<string>
using namespace std;

//定义PageRank类
class PageRank {
private:
	map<int, set<int>> matrix;		//存储稀疏矩阵，使用map+set的方式，能减少存储空间，以及更快的索引
	int maxIteration;				//最大迭代轮数
	long double beta;				//随机游走系数
	long double convergence;		//收敛限界
	map<int, int> nodeToIndex;		//初始编号与离散化后编号的映射，这里离散化是为了去除一些并不存在的节点(即没有入度也没有出度)，减少矩阵的存储空间
	map<int, int> indexToNode;		//离散化后的编号与初始编号的映射
	vector<long double> pr;			//PageRank数组
	vector<int> degree;				//离散后每一个点的出度数
	int totalNum;					//离散化后的编号总数
	int showNum;
	int blockNum;					//分块块数
	int totalEdges;					//数据总共的边数
	int minItemNum, maxItemNum;		//最小编号和最大编号
	vector<pair<long double, int>> output;	//保存最后排序结果

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

	//读取文件
	void readFile(string dataFile);
	//PageRank构造函数
	PageRank(int _minItemNum = -1, int _maxItemNum = -1, int _maxIteration = -1, long double _beta = 0,
		int block_nums = 1, long double convergence = 1e-5, int _showNum = 0);
	//默认析构函数
	~PageRank() = default;
	//插入值到matrix中
	void insertItem(int a, int b);
	//pageRank算法
	void pageRank();
	//读取文件得到最大值和最小值
	void getItemRange(string dataFile);
	//打印相关信息
	void print();
};





#endif