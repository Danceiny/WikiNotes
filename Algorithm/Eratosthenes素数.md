# 原理
偶数不是素数，先筛去偶数；
筛去3的倍数；
筛去5的倍数；
……

# Example

```c
#include<stdio.h>
#define MAXNUM 1000 // 求1000以内素数

int main(int argc, char const *argv[]) {
    int i,j;
    int c = 0;
    int prime[MAXNUM+1];
    for(i=2; i<MAXNUM; i++)
    {
        prime[i] = 1;//标识为1，表示对应的数默认为素数
    }

    for (i = 2; i*i <= MAXNUM ; i++) {
        if (prime[i]==1) {  //是素数（默认）则开始筛选
            for (j = 2*i; j <= MAXNUM; j++) {
                if(!prime[i])continue;  //是合数，跳过
                if(j%i==0)  //j能被整除，说明不是素数
                    prime[j] = 0;   //清除素数标识
            }
        }
    }

    for (i = 2; i < MAXNUM; i++) {
        if(prime[i]==1)
        {
            printf("%4d ",i );
            c++;
            if(c%10 == 0)
                printf("\n");
        }
    }
    return 0;
}
```


结构化的实现： 
```java
boolean[] sieveOfEratorthenes(int max){
    boolean[] flags = new boolean[max+1];
    int count = 0;
    
    init(flags);    //将flags中0,1意外的的元素设为默认的true
    int prime = 2;

    while(prime <= max){
        crossOff(flags,prime);  //划掉余下为prime倍数的数字
        prime = getNextPrime(flags,prime);
        if(prime>=flags.length)break;
    }
    return flags;
}

void crossOff(boolean[] flags, int prime){
    //从prime*prime开始，因为如果k*prime且k<prime，这个值早就在之前的迭代里被划掉了。

    for(int i=prime*prime; i<flags.length; i+=prime)
        flags[i] = false;
}

int getNextPrime(boolean[] flags; int prime){
    int next = prime + 1;
    while(next<flags.length && !flags[next]){
        next++;
    }
    return next;
}
```