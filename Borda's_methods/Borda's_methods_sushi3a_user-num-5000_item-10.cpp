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

using namespace std ;

//========================================================================= 
//各参数定义
 
const int voter_max = 5005 ;						//最大用户量 
const int item_max = 20 ;  							//最大项目量 

int voter_num ;
int item_num ;
int top_num ;
int i,j;

//========================================================================= 
//项目结构体 

struct item											
{
    int item_xu ;									//项目序号 
    int goal ;										//对应项目序号的总得分 
    item()											//初始化结构体内定义的变量 
    {
        item_xu = 0 ;
        goal = 0;
    }
}goal_item[item_max] ;								//数组结构体 

//========================================================================= 
//各参数初始化 
void Init()											
{
    voter_num=5000 ;								
    item_num=10 ; 
	top_num=10 	;									
    for(i = 0 ; i < item_num ; i ++)
    {
        goal_item[i].item_xu = i ;					
    }
}
//========================================================================= 
//输入
 
void Input()										
{
	int a,b;										
    FILE*fp;
	fp=fopen("dataset\\input_sushi3a_user-num-5000_item-10.txt","r");			
    while (fscanf(fp,"%d",&a)!= EOF)
    {
        fscanf(fp,"%d",&b);
        for(i = 0 ; i < top_num ; i ++)
        {
            int t ;
            fscanf(fp,"%d",&t);
            int  sum = top_num - i ;
            goal_item[t].goal=goal_item[t].goal+sum ;
        }
    }
    fclose(fp) ;
}

//========================================================================= 
//比较 

bool cmp(item a , item b)
{
    if(a.goal == b.goal)
    {
        return a.item_xu < b.item_xu ;
    }
    else
    {
        return a.goal > b.goal ;
    }
}

//========================================================================= 
//排序
 
void Sort_item()
{
    sort(goal_item , goal_item + 10 , cmp ) ;
}

//========================================================================= 
//输出
 
void Print()
{
	FILE*p;
    p = fopen ("dataset\\output_sushi3a_user-num-5000_item-10_Borda's_methods.txt","w");
    for(int i = 0 ; i < item_num ; i ++)
    {
        fprintf(p,"编号：%d   得分；%d\n\n",goal_item[i].item_xu,goal_item[i].goal);
    }
    fclose(p) ;
}

//========================================================================= 
//主函数
 
int main()
{
    Init() ;
    Input() ;
    Sort_item() ;
    Print() ;
    return 0 ;
}

