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

//*********************************************************

const int max_space = 9999999 ;
const int top_space = 100 ; 

int voter_num ;
int item_num ;
int top_num ;
int i,j;
int k = 0;
int y;
int kendall = 0;
int n = 0;
int min_xu[top_space]; 
double a_kendall[max_space];
double cs;

FILE*fp;
FILE*qpl;
FILE*p;

//*********************************************************

void Init()									//结构体数组内项目序号初始化过程 
{
    voter_num = 5000 ;						//用户量——即投票人数 
    item_num = 5 ; 							//（可设置） 
	top_num = 5 ;							//项目数量 （可设置） 
	cs = top_num * ( top_num - 1 ) / 2.000000 ;

	fp = fopen("dataset\\input_sushi3a.5000.5-result.txt" , "r") ;
	qpl = fopen("dataset\\input_all rank of 5.txt" , "r") ;
	p = fopen ("dataset\\output_all rank of 5-Best Kendall's Tau-result.txt" , "a") ;
}

void Close()
{
	fclose(fp) ;
	fclose(qpl) ;
	fclose(p) ;
}

void Kendall()
{
	int a,b;
	int m = 0 ;
	int t[top_space],xu[top_space] ;					
	int f_xu[top_space] ;
	double min = 5.000000 ;
	
	
	while(fscanf(qpl,"%d",&y) != EOF)
	{
		
		kendall = n = m = 0 ; 
		f_xu[0] = y ;
		t[f_xu[0]] = 0 ;
		
		for(i = 1 ; i < top_num ; i++)
        {
            fscanf(qpl,"%d",&f_xu[i]) ;
            t[f_xu[i]] = i ;
        }
        	
		while(fscanf(fp,"%d",&y) != EOF)
	    {
	    	xu[y] = 0 ;
	        for(i = 1 ; i < top_num  ; i++)
	        {
	            int x ;
	            fscanf(fp,"%d",&x) ;
	            xu[x] = i ;        
	        }
	        
	        for ( i = 0 ; i < top_num - 1 ; i++)
			{
				for(j = i + 1 ; j < top_num ; j++)
				{
					if(( t[f_xu[i]] - t[f_xu[j]] ) * ( xu[f_xu[i]] - xu[f_xu[j]] ) < 0 )
					{
						kendall ++ ;	
					}
					m ++ ;
				}
			}
	        n ++ ;
	        a_kendall[k] = kendall / (n * cs) ;
	    }
	    rewind(fp);//指针返回文件开头 
	
	   	for(i = 0 ; i < top_num ; i ++)
	   	{
	   		fprintf(p,"%d ",f_xu[i]) ;
	   	}
	    fprintf(p,"        a_kendall[%d]=%.16lf\n",k,a_kendall[k]) ;
	    
	    if(a_kendall[k] < min)
	    {
	    	min = a_kendall[k] ;
	    	for(i = 0 ; i < top_num ; i++)
	    	{
	    		min_xu[i] = f_xu[i] ;
	    	}	
	    }  
	}

	fprintf(p,"Best Rank:") ;
	for(i = 0 ; i < top_num ; i++)
	{
		fprintf(p,"%d ",min_xu[i]) ;
	}
	fprintf(p,"            min-a_kendall = %.16lf\n ",min) ;
}

//*********************************************************

int main()
{
    Init() ;
	Kendall();
	Close();
    return 0 ;
}

