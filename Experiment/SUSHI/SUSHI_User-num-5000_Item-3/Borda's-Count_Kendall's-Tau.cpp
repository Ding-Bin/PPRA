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
//���������� 

const int voter_max = 5005 ;							//����û��� 
const int item_max = 20 ;  								//�����Ŀ�� 

int voter_num ;											//ͶƱ���� 
int item_num ;											//��Ŀ���� 
int top_num ;											//top���� 
int i,j;												//ѭ������ 
int a,b;												//���ǰ����û���õ����� 
int xu_budong[100]; 									//�̶�������������� 
int kendall = 0;										//�ϵ¶�������� 
int n = 0;												//��ż��� 
int sum ;

double a_kendall = 0;									//ƽ���ϵ¶����� 

FILE*fp;												//"sushi3a.5000.10.txt","r"
FILE*p;													//"sushi3a.5000.10-Kendall's Tau-result.txt","w"

//=================================================================================
//��Ŀ�ṹ�� 

struct item												
{
    int item_xu ;										//��Ŀ��� 
    int goal ;											//��Ӧ��Ŀ��ŵ��ܵ÷� 
    item()												//��ʼ���ṹ���ڶ���ı��� 
    {
        item_xu = 0 ;
        goal = 0;
    }
}goal_item[item_max] ;					

//=================================================================================
//��������ʼ�� 
 
void Init()									
{
    voter_num = 5000 ;									//�����û��� 
    item_num = 3 ; 										//������Ŀ�� 
	top_num = 3 ;										//����top-k�������� 
    for(i = 0 ; i < item_num ; i ++)
    {
        goal_item[i].item_xu = i ;			
    }
	fp = fopen("dataset\\input_sushi3a.5000.3-result.txt","r");
	p = fopen ("dataset\\Output_Borda's-Count-Kendall's-Tau_User'num-5000_item-3.txt","w");			
}

//=================================================================================
//����
 
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
//�Ƚϣ��÷��ɴ�С�������ͬ�ķְ��������С�������
 
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
//����
 
void Sort_item()
{
    sort(goal_item , goal_item + 10 , cmp) ;
}

//=================================================================================
//�̶�����
 
void budong()
{
	for(int i = 0 ; i < item_num ; i++)
	{
		xu_budong[goal_item[i].item_xu] = i ;				//����ÿ����ζ��Ӧ������
	}
}

//=================================================================================
//��� 

void Print()
{
    for(int i = 0 ; i < item_num ; i++)
    {
        fprintf(p,"item��%d   goal��%d\n",goal_item[i].item_xu,goal_item[i].goal); 
    }
	fprintf(p,"Kendall's Tau=%f\n",a_kendall);
    fclose(p) ;
}

//=================================================================================
//�ϵ¶�������� 

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
//������ 

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





