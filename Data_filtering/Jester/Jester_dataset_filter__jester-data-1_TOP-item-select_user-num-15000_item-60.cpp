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
	const int guding_num = 9 ;						 //��Ҫ�ĸ��� ������Ϊ 2 3 4 5 6 7 8 9 10 20 30 40 50 60�������ã� 
	 
	int need_wei = 100;                              
	int voter_num ;
	int item_num ;
	int top_num ;
	int i,j;
    int a;	
	int n = 0 ;	
	double shuzu[100];		
				
    FILE*fp;
	fp=fopen("dataset\\input_Top-60-user_num-15000.txt","r");
	FILE*p;
    p = fopen ("dataset\\output_Top-9-user_num-15000.txt","w");		//����ļ��������ͬtop��ĿʱҪ���ж�Ӧ���޸� 
	
    while (fscanf(fp,"%d",&a) != EOF)
    {
   		float b ;
        for(i = 0 ; i < 60 ; i ++)
        {
            fscanf(fp,"%f",&b);
			shuzu[i] = b ; 
        }
		fprintf(p,"%5d ",a) ;	
		for(i = 0 ; i < guding_num ; i++)
		{
			fprintf(p,"%5.2f ",shuzu[i]) ;	
		}
		fprintf(p,"\n") ;//�������ߣ�  ��ȷ�ķŵ������if����ȥ 	
		
        n = 0 ;
    }
    
    fclose(fp) ;
    fclose(p) ;
    return 0 ;
}



    
