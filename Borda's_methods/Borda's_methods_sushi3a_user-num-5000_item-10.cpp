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
//����������
 
const int voter_max = 5005 ;						//����û��� 
const int item_max = 20 ;  							//�����Ŀ�� 

int voter_num ;
int item_num ;
int top_num ;
int i,j;

//========================================================================= 
//��Ŀ�ṹ�� 

struct item											
{
    int item_xu ;									//��Ŀ��� 
    int goal ;										//��Ӧ��Ŀ��ŵ��ܵ÷� 
    item()											//��ʼ���ṹ���ڶ���ı��� 
    {
        item_xu = 0 ;
        goal = 0;
    }
}goal_item[item_max] ;								//����ṹ�� 

//========================================================================= 
//��������ʼ�� 
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
//����
 
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
//�Ƚ� 

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
//����
 
void Sort_item()
{
    sort(goal_item , goal_item + 10 , cmp ) ;
}

//========================================================================= 
//���
 
void Print()
{
	FILE*p;
    p = fopen ("dataset\\output_sushi3a_user-num-5000_item-10_Borda's_methods.txt","w");
    for(int i = 0 ; i < item_num ; i ++)
    {
        fprintf(p,"��ţ�%d   �÷֣�%d\n\n",goal_item[i].item_xu,goal_item[i].goal);
    }
    fclose(p) ;
}

//========================================================================= 
//������
 
int main()
{
    Init() ;
    Input() ;
    Sort_item() ;
    Print() ;
    return 0 ;
}

