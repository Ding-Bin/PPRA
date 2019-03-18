#include<iostream>
#include<string>
#include<cstring>
#include<algorithm>
#include<cmath>
#include<queue>
#include<vector>
#include<map>
#include<set>
#include<stdio.h>
#include<stdlib.h> 
#include<algorithm> 
#include<fstream>
#define M 20  
using namespace std ;

//=============================================================== 

int n;  
int a[M];  
int cnt = 0;									// 记录全排列个数  
  
void swap(int *a, int *b)						//交换a,b  
{  
    char t;  
    t = *a;  
    *a = *b;  
    *b = t;  
}  

//===============================================================
  
void dfs(int cur)  
{  
    int i;  
	  
    if(cur == n)								// 找到 输出全排列  
    {  
        ++cnt;
		FILE*p; 
    	p = fopen ("dataset\\all-rank-of-1.txt","a");
        for(i=0; i<n; i++)  
        {
        	fprintf(p,"%d ",a[i]);
        }   
        fprintf(p,"\n");  
        fclose(p) ; 
    }  
    else  
    {  
        // 将集合中的所有元素分别与第一个交换，这样就总是在  
        // 处理剩下元素的全排列(即用递归)  
        for(i=cur; i<n; i++)  
        {  
            swap(&a[cur], &a[i]);  
            dfs(cur+1);  
            swap(&a[cur], &a[i]);//回溯  
        }  
    } 

}  

//===============================================================
  
int main()  
{  
	
    while(scanf("%d", &n) != EOF)  
    {  
        for(int i=0; i<n; i++)  
            a[i] = i;							// 假设集合S为:1 2 3 ... n  
        cnt = 0;  
        dfs(0);  
        printf("count:%d\n", cnt);  
    }  
    return 0;  
}  

 
 


