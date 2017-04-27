# 题目

把n个骰子仍在地上，所有骰子朝上一面的点数之和为s，输入n，打印出s的所有可能值出现的概率。


## 解法1：基于递归，时间效率不高

把骰子分为两堆：第一堆只有一个，另一堆有n-1个。

定义一个6n-n+1的数组，点数和为s的出现的次数保存到该数组第s-n个元素中。

```cpp

int g_maxValue = 6;

void PrintProbability(int number)
{
    if(number < 1)return ;

    int maxSum = number * g_maxValue;
    int *pProbs = new int[maxSum - number + 1];
    for(int i=number; i<=maxSum; ++i)
        pProbs[i-number] = 0;
    
    Probability(number,pProbs);

    int total = pow((double)g_maxValue,number);

    for(int i=number; i<=maxSum; ++i)
    {
        double ratio = (double)pProbs[i-number]/total;
        printf("%d: %e\n",i,ratio);
    }
    delete[] pProbs;
}

void Probability(int number, int* pProbs)
{
    
}

```