#include "pageRank.h"
#include<fstream>
#include<istream>
#include<iostream>
#include<math.h>
#include<algorithm>
using namespace std;

//构造函数
PageRank::PageRank(int _minItemNum, int _maxItemNum, int _maxIteration, long double _beta, int _blockNum,
    long double _convergence, int _showNum) : maxItemNum(_maxItemNum), minItemNum(_minItemNum), maxIteration(_maxIteration),
    beta(_beta), blockNum(_blockNum), convergence(_convergence), showNum(_showNum) {
    //清空matrix, nodeToIndex, indexToNode
    this->matrix.clear();
    this->nodeToIndex.clear();
    this->indexToNode.clear();
    this->totalEdges = 0;
    this->totalNum = 0;
}

long double PageRank::getConvergence() const{
    return this->convergence;
};
void PageRank::setConvergence(long double convergence) {
    this->convergence = convergence;
};
long double PageRank::getBeta() const {
    return this->beta;
};
void PageRank::setBeta(long double beta) {
    this->beta = beta;
};
int PageRank::getMaxIteration() const {
    return this->maxIteration;
};
void PageRank::setMaxIteration(int maxIteration) {
    this->maxIteration = maxIteration;
};
int PageRank::getMaxItemNum() const {
    return this->maxItemNum;
};
void PageRank::setMaxItemNum(int maxItemNum) {
    this->maxItemNum = maxItemNum;
};
int PageRank::getMinItemNum() const {
    return this->minItemNum;
};
void PageRank::setMinItemNum(int maxItemNum) {
    this->minItemNum = minItemNum;
};
int PageRank::getBlockNum() const {
    return this->blockNum;
};
void PageRank::setBlockNum(int blockNum) {
    this->blockNum = blockNum;
};
int PageRank::getShowNum() const {
    return this->showNum;
};
void PageRank::setShowNum(int showNum) {
    this->showNum = showNum;
};

//读取文件得到最大值和最小值
void PageRank::getItemRange(string dataFile) {
    ifstream data;
    data.open(dataFile.c_str(), ios::in);
    if (!data.is_open())
    {
        cerr << "读取文件失败" << endl;
        exit(0);
    }
    int a = INT_MAX, b = -1;
    int minItem = INT_MAX, maxItem = -1;
    while (data >> a >> b) {
        minItem = min(minItem, min(a, b));
        maxItem = max(maxItem, max(a, b));
    }
    this->minItemNum = minItem;
    this->maxItemNum = maxItem;
    data.close();
}

//将Item加入matrix中
void PageRank::insertItem(int a, int b) {
    //先检查a,b的值是不是合法
    if (minItemNum == -1 || maxItemNum == -1) {
        cerr << "Your minItemNum or maxItemNum must have something wrong!" << endl;
        exit(0);
    }
    else if (a > maxItemNum || a < minItemNum) {
        cerr << "a form error!" << endl;
        exit(0);
    }
    else if (b > maxItemNum || b < minItemNum) {
        cerr << "b form error!" << endl;
        exit(0);
    }
    //检查是否存在
    if (matrix.find(a) != matrix.end()) {
        matrix[a].insert(b);
    }
    else {
        matrix.insert({ a, set<int>() });
        matrix[a].insert(b);
    }
}

//读取文件
void PageRank::readFile(string dataFile) {
    //如果没有成功得到min和max，或者max<min的话需要先得到min和max
    if (minItemNum == -1 || maxItemNum == -1 || minItemNum > maxItemNum) {
        cerr << "Your minItemNum or maxItemNum must have something wrong!" << endl;
        exit(0);
    }
    
    //进行矩阵的读取
    ifstream data;
    data.open(dataFile.c_str(), ios::in);
    int a, b;
    //用usedItem数组记录使用过的节点
    bool* usedItem = new bool[maxItemNum + 2](); //这里使用指针方便释放
    while (data >> a >> b) {
        insertItem(a, b);
        usedItem[a] = true;
        usedItem[b] = true;
        totalEdges++;
    }

    data.close();
    totalNum = 0;
    //得到节点数，并将映射离散化，即将离散化的节点数分别用1,2,3...表示，这样有利于节省空间
    for (int i = minItemNum; i < maxItemNum; i++) {
        if (usedItem[i]) {
            nodeToIndex[i] = totalNum; //离散->紧凑
            indexToNode[totalNum] = i; //紧凑->离散
            totalNum++;
        }
    }
    delete[] usedItem;

    //进行dead points的判断以及出度数的获得
    //dead points的判断是根据usedItem为true但是matrix中并没有(因为matrix中只记录有出度的)
    for (int i = 0; i < totalNum; i++) {
        if (matrix.find(indexToNode[i]) == matrix.end()) {
            degree.push_back(0);
        }
        else {
            degree.push_back((int)matrix[indexToNode[i]].size());
        }
    }

    //进行数据分块(其实就是matrix矩阵的分块，这样可以优化计算)
    char fileName[100][300];
    ofstream blockFile[100]; //设置最多一百个分块
    for (int i = 0; i < blockNum; i++) {
        sprintf_s(fileName[i], "block-%d.txt", i);
        blockFile[i].open(fileName[i]);
    }
    
    /***
    以下是分块示例:
      1   2   3   4
    | 1/4 1/2 0   0 |          
    | 1/4 0   1/3 0 |   ===>  | 1/4 1/2 0   0 |  + | 1/4 1/2 0   0 |
    | 1/4 1/2 0   0 |   ===>  | 1/4 0   1/3 0 |    | 1/4 0   1/3 1 |
    | 1/4 0   1/3 1 |         
    因此在分块的时候需要记住节点i的所有出度数，才能得到节点i->节点j的系数，因此系数等于1/出度数
    ***/

    //依次遍历
    int tmpBlock = 0;
    for (auto i : matrix) {
        for (auto j : i.second) {
            //先将离散化转为紧凑化
            int index_j = nodeToIndex[j];
            int index_i = nodeToIndex[i.first];
            //将index_j化分在对应的块数
            tmpBlock = index_j / (totalNum / blockNum + 1);
            blockFile[tmpBlock] << index_i << " " << index_j << endl;
        }
    }
    for (int i = 0; i < blockNum; i++) {
        blockFile[i].close();
    }
    //将node<-->index的映射表打印出来
    ofstream nodeIndexFile;
    nodeIndexFile.open("nodeToIndex.txt");
    for (auto i : nodeToIndex) {
        nodeIndexFile << i.first << "->" << i.second << endl;
    }
    nodeIndexFile.close();
    cout << "Read Over!" << endl;

};

//pageRank算法
void PageRank::pageRank() {
    long double alpha = 1 - beta;
    vector<long double> oldPr; //上一次计算的PageRank
    oldPr.resize((size_t)totalNum);
    pr.resize((size_t)totalNum);
    //初始化pageRank向量
    for (auto& i : pr) {
        i = 1.0 / totalNum;
    }

    long double diff = 1e9; //差值
    int iterations = 0; //迭代次数
    int blockSize = totalNum / blockNum + 1;
    while (diff > convergence && iterations < maxIteration) {
        long double sumPr = 0.0; //PageRank所有之和
       // long double sumDead = 0.0; //dead ends的pageRank之和
        for (int i = 0; i < totalNum; i++) {
            //如果是dead ends
            //if (degree[i] == 0) {
            //    sumDead += pr[i];
            //}
            sumPr += pr[i];
        }

        for (int i = 0; i < totalNum; i++) {
            oldPr[i] = pr[i] / sumPr;
        }

      //  long double addDead = beta * sumDead / totalNum / sumPr;
        long double addSpider = alpha * 1.0 / totalNum;
        sumPr = 0.0;
        diff = 0.0;
        

        fstream blockFile;
        for (int block = 0; block < blockNum; block++) {
            char fileName[300];
            sprintf_s(fileName, "block-%d.txt", block);
            blockFile.open(fileName);
            vector<long double>tmpPr; //部分的PageRank
            tmpPr.resize((size_t)min(blockSize * (block + 1), totalNum) - blockSize * block + 1);
            for (auto& i : tmpPr) {
                i = 0.0;
            }
            int minIndex = blockSize * block;
            int maxIndex = min(blockSize * (block + 1), totalNum);

            int a, b; //index_i, degree, index_j

            while (blockFile >> a >> b) {
                tmpPr[b - minIndex] += beta * 1.0 / degree[a] * oldPr[a];
            }
            //对Dead ends修正
            for (int i = 0; i < totalNum; i++) {
                //如果是dead ends
                if (degree[i] == 0) {
                    for (int j = minIndex; j < maxIndex; j++) {
                        tmpPr[j - minIndex] += beta * 1.0 / totalNum * oldPr[i];
                    }
                }
            }

            //加上dead ends 和 spider ends 的修正
            for (int i = minIndex; i < maxIndex; i++) {
                tmpPr[i - minIndex] += addSpider;
            }

            for (int i = minIndex; i < maxIndex; i++) {
                pr[i] = tmpPr[i - minIndex];
                sumPr += pr[i];
            }
            blockFile.close();
        }

        //long double testSum = 0.0;

        //进行归一化处理(但是这里有问题)
        for (int i = 0; i < pr.size(); i++) {
            pr[i] /= sumPr;
            //testSum += pr[i];
        }
        //cout << "Test Sum is = " << testSum << "testSum is " << testSum<< endl;

        for (int i = 0; i < totalNum; i++) {
            diff += fabs(pr[i] - oldPr[i]);
        }
        cout << diff << endl;
        iterations++;
    }

    output.resize(totalNum);
    for (int i = 0; i < totalNum; i++) {
        output[i] = { -pr[i], i };
    }
    sort(output.begin(), output.end());
    ofstream result;
    result.open("result.txt");
    if (showNum < 0 ) {
        for (int i = 0; i < totalNum; i++) {
            result << indexToNode[output[i].second] << " " << pr[output[i].second] << endl;
            cout << indexToNode[output[i].second] << " " << pr[output[i].second] << endl;
        }
    }
    else {
        for (int i = 0; i < totalNum; i++) {
            result << indexToNode[output[i].second] << " " << pr[output[i].second] << endl;
        }
    }
    
    result.close();
    cout << iterations << endl;
}

void PageRank::print() {
    cout << totalNum << " " << totalEdges << " " << minItemNum << " " << maxIteration << " " << maxItemNum << endl;
}