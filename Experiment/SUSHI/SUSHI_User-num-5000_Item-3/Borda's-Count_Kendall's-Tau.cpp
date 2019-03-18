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
#include<time.h>

using namespace std ;

//=================================================================================
//各参数定义 

const int voter_max = 5005 ;							//最大用户量 
const int item_max = 20 ;  								//最大项目量 

int voter_num ;											//投票人数 
int item_num ;											//项目个数 
int top_num ;											//top数量 
int i,j;												//循环参数 
int a,b;												//存放前两个没有用的数据 
int xu_budong[100]; 									//固定最终排序的数组 
int kendall = 0;										//肯德尔距离计数 
int n = 0;												//序号计数 
int sum ;

double a_kendall = 0;									//平均肯德尔距离 

FILE*fp;												//"sushi3a.5000.10.txt","r"
FILE*p;													//"sushi3a.5000.10-Kendall's Tau-result.txt","w"

//=================================================================================
//项目结构体 

struct item												
{
    int item_xu ;										//项目序号 
    int goal ;											//对应项目序号的总得分 
    item()												//初始化结构体内定义的变量 
    {
        item_xu = 0 ;
        goal = 0;
    }
}goal_item[item_max] ;					

//=================================================================================
//各参数初始化 
 
void Init()									
{
    voter_num = 5000 ;									//设置用户量 
    item_num = 3 ; 										//设置项目量 
	top_num = 3 ;										//设置top-k个排序量 
    for(i = 0 ; i < item_num ; i ++)
    {
        goal_item[i].item_xu = i ;			
    }
	fp = fopen("dataset\\input_sushi3a.5000.3-result.txt","r");
	p = fopen ("dataset\\Output_Borda's-Count-Kendall's-Tau_User'num-5000_item-3.txt","w");			
}

//=================================================================================
//输入
 
void Input()								
{
    while(fscanf(fp,"%d",&a) != EOF)
    {
        sum = top_num - 0 ;
        goal_item[a].goal = goal_item[a].goal + sum ;
        for(i = 1 ; i < top_num ; i++)
        {
            fscanf(fp,"%d",&a);
            sum = top_num - i ;
            goal_item[a].goal = goal_item[a].goal + sum ;
        }
    }
    fclose(fp) ;
}

//=================================================================================
//比较，得分由大到小输出，相同的分按照序号由小到大输出
 
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

//=================================================================================
//排序
 
void Sort_item()
{
    sort(goal_item , goal_item + 10 , cmp) ;
}

//=================================================================================
//固定排序
 
void budong()
{
	for(int i = 0 ; i < item_num ; i++)
	{
		xu_budong[goal_item[i].item_xu] = i ;				//最终每个口味对应的排名
	}
}

//=================================================================================
//输出 

void Print()
{
    for(int i = 0 ; i < item_num ; i++)
    {
        fprintf(p,"item：%d   goal；%d\n",goal_item[i].item_xu,goal_item[i].goal); 
    }
	fprintf(p,"Kendall's Tau=%f\n",a_kendall);
    fclose(p) ;
}

//=================================================================================
//肯德尔距离计算 

void Kendall()
{
	int t[10],xu[100];										
	double cs = top_num * (top_num - 1) / 2.000000 ;
	fp = fopen("dataset\\input_sushi3a.5000.3-result.txt","r") ;				
    while(fscanf(fp,"%d",&a) != EOF)
    {
        xu[a] = 0 ; 
        for(i = 1 ; i < top_num ; i++)
        {
            fscanf(fp,"%d",&a) ;
            xu[a] = i ; 
        }
        
        for(i = 0 ; i < top_num - 1 ; i++)
		{
			for(j = i + 1 ; j < top_num ; j++)
			{
				if((xu_budong[goal_item[i].item_xu] - xu_budong[goal_item[j].item_xu]) * (xu[goal_item[i].item_xu] - xu[goal_item[j].item_xu]) < 0)
				{
					kendall++;	
				}
			}
		}
        n++;
        
        a_kendall = kendall / (n * cs) ;
    }
    fclose(fp) ;
}

//=================================================================================
//主函数 

int main()
{
    Init() ;
    Input() ;
    Sort_item() ;
    budong() ;
	Kendall() ;
    Print() ;
    return 0 ;
}





