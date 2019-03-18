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
	const int guding_num = 9 ;						 //需要的个数 可设置为 2 3 4 5 6 7 8 9 10 20 30 40 50 60（可设置） 
	 
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
    p = fopen ("dataset\\output_Top-9-user_num-15000.txt","w");		//输出文件名输出不同top项目时要进行对应的修改 
	
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
		fprintf(p,"\n") ;//检测版放外边，  正确的放到上面的if里面去 	
		
        n = 0 ;
    }
    
    fclose(fp) ;
    fclose(p) ;
    return 0 ;
}



    
