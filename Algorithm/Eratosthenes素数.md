# 原理
偶数不是素数，先筛去偶数；
筛去3的倍数；
筛去5的倍数；
……

# Example

```C
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
