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
	 
	int need_wei = 100;                              //�ڴ�������Ҫ��λ�� 
	int voter_num ;
	int item_num ;
	int top_num ;
	int i,j;
    int a;	
	int n = 0 ;			
				
    FILE*fp;
	fp=fopen("dataset\\input_jester-data-1.txt","r");
	FILE*p;
    p = fopen ("dataset\\output_jester-data-1-result_Number-of-occurrences.txt","w");
	
	for(i=0;i<100;i++)
	{
		item_xg[i].xu = i;
		printf("item_xg[%d].xu = %d\n",i,item_xg[i].xu) ;
	}
	
    while (fscanf(fp,"%d",&a) != EOF)
    {
   		float b ;
        for(i = 0 ; i < 100 ; i ++)
        {
            fscanf(fp,"%f",&b);
            if(b != 99)
            {
            	item_xg[i].goal = item_xg[i].goal + 1 ;
			}
        }
        n++ ; 
    }
    PaiXu() ;
		
    for(i=0 ;i < 100 ; i++)
    {
    	fprintf(p,"����%d = %d     ������%f\n",i+1,item_xg[i].xu+1,item_xg[i].goal) ;	
	}
    
    fclose(fp) ;
    fclose(p) ;
    return 0 ;
}



    
