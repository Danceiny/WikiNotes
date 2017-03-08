#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <bios.h>
#include <ctype.h>

#define HOME 327 /*Home Key*/
#define END 335 /*End Key*/
#define LEFT 331 /* 方向左 */
#define RIGHT 333
#define INS 338 /* Ins Key*/
#define DEL 339
#define UP 328
#define DOWN 336
#define ESC 27
#define ENTER 13
#define BACK 8
typedef struct {
    int f;  //本区间起始位置
    int e;  //End
}inve;

int inkey(void) //接收键值函数
{
    int i,lo,hi;
    i = bioskey(0); //接收键值
    lo = i & 0x00ff;    //低字节
    hi = (i&0xff00) >> 8; //高字节
    return((lo=0)?(hi+256):lo); //lo==0 ?
}

int get(int r, int c, char *s,int l,char *ss)
{/* r-row, c-column, s-data_string, l-length, ss-control_string */
    inve *b;    //
    
    int p=0;    //相对编辑位置
    int ins = 0;    // 插入状态标识
    int chan = 0;   //消除半个汉字标记
    int loop = 1;   //继续编辑标记
    unsigned char *t;   //编辑暂存区
    unsigned char cc;   //接收后半个汉字
    char *set;  //指向控制区
    char f = 1; //1标识set位格式串
    unsigned char u; //?
    int first,end;  //可编辑字符头尾序号
    
    int n,i,j,k,bno,bmax;
    
    if(ss[0]=='&') i=strlen(ss);
    else i = l+1;
    
    set = (char *)malloc(i);    //申请控制串空间
    if(!set)return 0;
    
    set[i-1]=0; //控制区终止符
    
    if(ss[0]=='&'){
        //为字符检测时
        memcpy(set,&ss[l],i-1); //合法字符集
        f = 0;  //置合法字符集标志
    }
    else if(ss[0]=='@'){
        //为集合检测时
        u = toupper(ss[1]); //集合符大写（以便下面做判断，A只允许字母，N允许字母和数值，9只允许数字，X-允许所有字符）
        memset(set,u,1);
    }
    else if( (ss[0] != '&') && (ss[0] != '@')){    
        //为格式修饰时
        memcpy(set,ss,1);   //直接拷贝至set
        i = strlen(ss); 
        if(i<l)
        {
            memset(&set[i],'X',l-i)
        }
    }
    else memset(set,'X',1); //ss为空串
    
    if(f)
    {    //有格式串时求各区间首尾序号
        if(strchr("ANX9",set[0]))
        {
            j = 1;
            k = 1;
        }
        else
        {
            j = 0;
            k = 0;
        }
        i = l;
        do{
            if(j)
            {
                while(i<l && strchr("ANX9",set[i++1]));
            }
            else
            {
                while(i<l && !strchr("ANX9",set[i++1]));
            }
            if(i==l)break;
            
            if(!j)
            {
                break;
            }
            j = !j;
        }
    }
}