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

int x, y;
struct item									//�ṹ�� 
{
    int xu ;								//��Ŀ��� 
    double goal ;							//��Ӧ��Ŀ��ŵ��ܵ÷� 
    item()									//��ʼ���ṹ���ڶ���ı��� 
    {
        xu = 0 ;
        goal = 0.000;
    }
}item_xg[100] ;

bool cmp(item x , item y)
{
    if(x.goal == y.goal)
    {
        return x.xu < y.xu ;
    }
    else
    {
        return x.goal > y.goal ;
    }
}

//==========================================================================


 void PaiXu()
 {
 	sort(item_xg , item_xg + 100 , cmp ) ;
 }


int main()
{
	const int voter_max = 30000 ;				
	const int item_max = 100 ;  
	const int guding_num = 100 ;					 //��Ҫ�ĸ��� 
	 
	int need_wei = 100;                              //�ڴ�������Ҫ��λ�� 
	int voter_num ;
	int item_num ;
	int top_num ;
	int i,j;
    int a;	
	int n = 0 ;	
	double shuzu[100];		
				
    FILE*fp;
	fp=fopen("dataset\\input_jester-data-1_Top-10.txt","r");
	FILE*p;
    p = fopen ("dataset\\output_jester-data-1_Top-10_no-99.txt","w");
	
    while (fscanf(fp,"%d",&a) != EOF)
    {
   		float b ;
        for(i = 0 ; i < guding_num ; i ++)
        {
            fscanf(fp,"%f",&b);
			shuzu[i] = b ; 
        }
        for(i=0;i<guding_num;i++)
        {
        	if(shuzu[i] == 99)
        	{
        		break;
			}
			else
			{
				n = n + 1 ;
			}
		}
		if(n == guding_num)
		{
			fprintf(p,"%5d ",a) ;	
			for(i=0;i<guding_num;i++)
			{
				fprintf(p,"%5.2f ",shuzu[i]) ;	
			}
			fprintf(p,"\n") ;					//�������ߣ�  ��ȷ�ķŵ������if����ȥ 
		}	
		
        n = 0 ;
    }
    
    fclose(fp) ;
    fclose(p) ;
    return 0 ;
}



    
