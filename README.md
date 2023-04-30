# pagerank算法

> Author: YangXin(2011028)
>
> Date: 2023-4-27

## 作业要求

Dataset: `Data.txt`

The format of the lines in the file is as follow:  `FromNodeID   ToNodeID`

In this project, you need to report the `Top 100 NodeID` with their PageRank scores. You can choose different parameters, such as the teleport parameter, to compare different results. One result you must report is that when setting the teleport parameter to `0.85`.

In addition to the basic PageRank algorithm, you need to implement the Block-Stripe Update algorithm.

## 开发环境

- 语言：C++11
- Windows 可执行文件生成：Visual Studio 2022,  Windows 10 64 位专业版

## 实现功能


- [x] 实现基础的pagerank算法
- [x] 考虑了 dead ends 和 spider trap 节点的 PageRank 基础算法
- [x] 稀疏矩阵存储优化
- [x] 稀疏矩阵分块计算（Block-Stripe Update algorithm）
- [x] 各网页节点的最终 PageRank 值按照此格式输出：`NodeID   Score`
- [x] 考虑了并行优化问题

## 目录结构

```
pagerank
├─BlockPageRank   # 有稀疏矩阵优化和分块计算的PageRank程序
│  ├─block		#分块数据存放文件夹
│  ├─源代码	#源代码存放文件夹
│  │  │  main.cpp
│  │  │  mem.h
│  │  │  pageRank.cpp
│  │  │  pageRank.h
│  │  Data.txt	#数据集
│  │  nodeToIndex.txt	#映射表
│  │  PageRank.exe	#可执行文件
│  │  result.txt	#结果集
│  │
├─ThreadBlockPageRank   # 有稀疏矩阵优化和分块计算以及并行PageRank程序
│  ├─block		#分块数据存放文件夹
│  ├─源代码	#源代码存放文件夹
│  │  │  main.cpp
│  │  │  mem.h
│  │  │  pageRank.cpp
│  │  │  pageRank.h
│  │  Data.txt	#数据集
│  │  nodeToIndex.txt	#映射表
│  │  PageRankThread.exe	#可执行文件
│  │  result.txt	#结果集
│  │
└─实验报告   #存放实验报告
│  ├─实验报告.pdf
│	
├─ 实验结果
│  ├─result.txt	#所有结果文件
│  ├─Top100-result.txt	#前100结果文件
│
└─README.md
```

## 程序运行

输入-h可以查看程序运行的参数：

![image-20230430201907537](C:\Users\neverquit\AppData\Roaming\Typora\typora-user-images\image-20230430201907537.png)

下面是分块优化算法程序设置α游走系数为0.85，c阈值为1e-9，m最大迭代次数100，b分块个数20，s展示前100个结果，Data.txt数据集。

![image-20230430202144442](C:\Users\neverquit\AppData\Roaming\Typora\typora-user-images\image-20230430202144442.png)

最后得到结果Top_100.txt为前100个结果，result为全部的结果，注意设置-s参数才会输出前100个结果，否则只会输出全部的result的结果。

![image-20230430202204921](C:\Users\neverquit\AppData\Roaming\Typora\typora-user-images\image-20230430202204921.png)

## 参考资料

https://youtu.be/ytjf6zYDd4s
