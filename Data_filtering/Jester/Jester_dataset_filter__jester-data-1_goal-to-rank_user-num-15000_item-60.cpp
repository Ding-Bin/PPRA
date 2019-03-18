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
struct item									//结构体 
{
    int xu ;								//项目序号 
    double goal ;							//对应项目序号的总得分 
    item()									//初始化结构体内定义的变量 
    {
        xu = 0 ;
        goal = 0.000;
    }
}item_xg[60] ;

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
 	sort(item_xg , item_xg + 60 , cmp ) ;
 }


int main()
{
	const int voter_max = 15000 ;				
	const int item_max = 60 ;  				
	
	int need_wei = 2 ;                              //在此设置需要的位数 (可设置) 
	int voter_num ;
	int item_num ;
	int top_num ;
	int i,j;
    int a;				
				
    FILE*fp;
	fp=fopen("dataset\\input_Top-60-user_num-15000.txt","r");
	FILE*p;
    p = fopen ("dataset\\output_XuHao_Top-2.txt","w");
	
	for(i = 0 ; i < 60 ; i++)
	{
		item_xg[i].xu = i;
	}
	
	
		 
    while (fscanf(fp,"%d",&a)!= EOF)
    {
    	for(i = 0 ; i < 60 ; i++)
		{
			item_xg[i].xu = i;
		}
   		float b ; 
        for(i = 0 ; i < 60 ; i ++)
        {
            fscanf(fp,"%f",&b);
            //printf("b= %f\n",b);
            item_xg[i].goal = b ;
            //printf("item_xg[%d].goal = %f\n",i,item_xg[i].goal);
        }
        PaiXu() ;
        
        for(i=0 ;i < 60 ; i++)
        {
        	//printf("%d ",item_xg[i].xu);
		}
		//printf("\n");
		
        for(i=0 ;i < 60 ; i++)
        {
        	if(item_xg[i].xu < need_wei )
        	{
        		fprintf(p,"%d ",item_xg[i].xu);	
			}
		}
        fprintf(p,"\n");
    }
    fclose(fp) ;
    fclose(p) ;
    return 0 ;
}



    
