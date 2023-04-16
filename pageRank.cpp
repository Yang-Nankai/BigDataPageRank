#include "pageRank.h"
#include<fstream>
#include<istream>
#include<iostream>
#include<math.h>
#include<algorithm>
using namespace std;

//���캯��
PageRank::PageRank(int _minItemNum, int _maxItemNum, int _maxIteration, long double _beta, int _blockNum,
    long double _convergence, int _showNum) : maxItemNum(_maxItemNum), minItemNum(_minItemNum), maxIteration(_maxIteration),
    beta(_beta), blockNum(_blockNum), convergence(_convergence), showNum(_showNum) {
    //���matrix, nodeToIndex, indexToNode
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

//��ȡ�ļ��õ����ֵ����Сֵ
void PageRank::getItemRange(string dataFile) {
    ifstream data;
    data.open(dataFile.c_str(), ios::in);
    if (!data.is_open())
    {
        cerr << "��ȡ�ļ�ʧ��" << endl;
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

//��Item����matrix��
void PageRank::insertItem(int a, int b) {
    //�ȼ��a,b��ֵ�ǲ��ǺϷ�
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
    //����Ƿ����
    if (matrix.find(a) != matrix.end()) {
        matrix[a].insert(b);
    }
    else {
        matrix.insert({ a, set<int>() });
        matrix[a].insert(b);
    }
}

//��ȡ�ļ�
void PageRank::readFile(string dataFile) {
    //���û�гɹ��õ�min��max������max<min�Ļ���Ҫ�ȵõ�min��max
    if (minItemNum == -1 || maxItemNum == -1 || minItemNum > maxItemNum) {
        cerr << "Your minItemNum or maxItemNum must have something wrong!" << endl;
        exit(0);
    }
    
    //���о���Ķ�ȡ
    ifstream data;
    data.open(dataFile.c_str(), ios::in);
    int a, b;
    //��usedItem�����¼ʹ�ù��Ľڵ�
    bool* usedItem = new bool[maxItemNum + 2](); //����ʹ��ָ�뷽���ͷ�
    while (data >> a >> b) {
        insertItem(a, b);
        usedItem[a] = true;
        usedItem[b] = true;
        totalEdges++;
    }

    data.close();
    totalNum = 0;
    //�õ��ڵ���������ӳ����ɢ����������ɢ���Ľڵ����ֱ���1,2,3...��ʾ�����������ڽ�ʡ�ռ�
    for (int i = minItemNum; i < maxItemNum; i++) {
        if (usedItem[i]) {
            nodeToIndex[i] = totalNum; //��ɢ->����
            indexToNode[totalNum] = i; //����->��ɢ
            totalNum++;
        }
    }
    delete[] usedItem;

    //����dead points���ж��Լ��������Ļ��
    //dead points���ж��Ǹ���usedItemΪtrue����matrix�в�û��(��Ϊmatrix��ֻ��¼�г��ȵ�)
    for (int i = 0; i < totalNum; i++) {
        if (matrix.find(indexToNode[i]) == matrix.end()) {
            degree.push_back(0);
        }
        else {
            degree.push_back((int)matrix[indexToNode[i]].size());
        }
    }

    //�������ݷֿ�(��ʵ����matrix����ķֿ飬���������Ż�����)
    char fileName[100][300];
    ofstream blockFile[100]; //�������һ�ٸ��ֿ�
    for (int i = 0; i < blockNum; i++) {
        sprintf_s(fileName[i], "block-%d.txt", i);
        blockFile[i].open(fileName[i]);
    }
    
    /***
    �����Ƿֿ�ʾ��:
      1   2   3   4
    | 1/4 1/2 0   0 |          
    | 1/4 0   1/3 0 |   ===>  | 1/4 1/2 0   0 |  + | 1/4 1/2 0   0 |
    | 1/4 1/2 0   0 |   ===>  | 1/4 0   1/3 0 |    | 1/4 0   1/3 1 |
    | 1/4 0   1/3 1 |         
    ����ڷֿ��ʱ����Ҫ��ס�ڵ�i�����г����������ܵõ��ڵ�i->�ڵ�j��ϵ�������ϵ������1/������
    ***/

    //���α���
    int tmpBlock = 0;
    for (auto i : matrix) {
        for (auto j : i.second) {
            //�Ƚ���ɢ��תΪ���ջ�
            int index_j = nodeToIndex[j];
            int index_i = nodeToIndex[i.first];
            //��index_j�����ڶ�Ӧ�Ŀ���
            tmpBlock = index_j / (totalNum / blockNum + 1);
            blockFile[tmpBlock] << index_i << " " << index_j << endl;
        }
    }
    for (int i = 0; i < blockNum; i++) {
        blockFile[i].close();
    }
    //��node<-->index��ӳ����ӡ����
    ofstream nodeIndexFile;
    nodeIndexFile.open("nodeToIndex.txt");
    for (auto i : nodeToIndex) {
        nodeIndexFile << i.first << "->" << i.second << endl;
    }
    nodeIndexFile.close();
    cout << "Read Over!" << endl;

};

//pageRank�㷨
void PageRank::pageRank() {
    long double alpha = 1 - beta;
    vector<long double> oldPr; //��һ�μ����PageRank
    oldPr.resize((size_t)totalNum);
    pr.resize((size_t)totalNum);
    //��ʼ��pageRank����
    for (auto& i : pr) {
        i = 1.0 / totalNum;
    }

    long double diff = 1e9; //��ֵ
    int iterations = 0; //��������
    int blockSize = totalNum / blockNum + 1;
    while (diff > convergence && iterations < maxIteration) {
        long double sumPr = 0.0; //PageRank����֮��
       // long double sumDead = 0.0; //dead ends��pageRank֮��
        for (int i = 0; i < totalNum; i++) {
            //�����dead ends
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
            vector<long double>tmpPr; //���ֵ�PageRank
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
            //��Dead ends����
            for (int i = 0; i < totalNum; i++) {
                //�����dead ends
                if (degree[i] == 0) {
                    for (int j = minIndex; j < maxIndex; j++) {
                        tmpPr[j - minIndex] += beta * 1.0 / totalNum * oldPr[i];
                    }
                }
            }

            //����dead ends �� spider ends ������
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

        //���й�һ������(��������������)
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