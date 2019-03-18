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
#include <time.h>
#include<math.h>

using namespace std ;

//===========================================================================

const int voter_max = 5000 ;				//最大用户量 					
const int pai_num = 5040 ;				        //全排列个数  5!=120; 6!=720; 7!=5040; 8!=40320; 9!=362880; 10!=3628800(可设置)
const int top_num = 7 ;						//前五个的排序 (可设置)  
const double epsilon = 1.0 ;				//ε的值初始化 （可设置） 

int a ;
int x , y ;									//比较 
int sample_match[top_num] ;
int final_match[top_num] ;
int final_data[pai_num] ;					//收集者得到的数据	
int jgl_num[pai_num] ;						//加概率以后未校正5000个数据120个序列的统计个数 
int xz_num[pai_num] ;						//校正，加校正后的数字
int borda_count[top_num] ;					//博尔达计数 
int xu_budong[top_num] ;					//博尔达不动序列 

int i = 0, j = 0 ;
int bi ;
int pi ;
 
double p ;									//概率
double cp ;									//修正概率	
double a_kendall ;		

FILE*ar;									//"all rank of 5.txt","r"
FILE*ss;									//"sushi3a.5000.5-result.txt","r"
FILE*fd;									//加概率后的输出结果，但是没有进行校正
FILE*jgl;									//加概率后的每列的个数统计结果 
FILE*bordaxu;								//最终输出的博尔达 

//===========================================================================
//两个结构体：全排列结构体，用户结构体 

struct borda_item									//结构体Borda 
{
    int borda_xu ;									//项目序号 
    int borda_goal ;								//对应项目序号的总得分 
    borda_item()									//初始化结构体内定义的变量 
    {
        borda_xu = 0 ;
        borda_goal = 0;
    }
}borda_xg[top_num] ;								

struct all_rank										//结构体项目 
{
    int v_bit ;										//项目序号 
    int v_pai[pai_num] ;								
    all_rank()										//初始化结构体内定义的变量 
    {
        v_bit = 0 ;
        for(i = 0 ; i < pai_num ; i++)
        {
        	v_pai[i] = 0;
		}       
    }
}v_item[pai_num] ;

struct user											//用户真实bit位结构体 
{								 
    int user_bit[pai_num] ;								
    user()									 
    {
        for(i = 0 ; i < pai_num ; i++)
        {
        	user_bit[i] = 0;
		}       
    }
}user_num[voter_max] ;

//=======================================================================
//初始化 

void Init()
{
	for(i = 0 ; i < pai_num ; i++)
	{
		v_item[i].v_bit = 0 ;//V中120列中每一位的初始化 
		jgl_num[i] = 0 ; 	 //加概率以后5000个数据120个序列的统计个数初始化 
	}
	i=0;
	
	ar = fopen("dateset\\input_all-rank-of-7.txt","r");
	while (fscanf(ar , "%d" , &a ) != EOF)
	{
		v_item[i].v_pai[0] = a ;
		for( j = 1 ; j < top_num ; j++ )
		{
			fscanf(ar,"%d",&a); 
			v_item[i].v_pai[j] = a ; 
		}
		i++;
	}
	
	srand((unsigned long)time(NULL));								//RR part
	p = exp(epsilon/2.0)/(1+exp(epsilon/2.0));
	bi = ( 1 - p / 2 ) * 100 ;
	pi = p * 100 ;
	printf("exp() = %.16f      pi = %d  \n" , p,pi);
	
	for(i = 0 ; i < pai_num ; i++)
	{
		final_data[i] = 0 ;
	}
	
	for(i = 0 ; i < top_num ; i ++)								//borda序列编号初始化 
    {
        borda_xg[i].borda_xu = i ;			
    }
	
	fd = fopen ("dataset\\output_RR_item-7.txt","w");
	jgl = fopen ("dataset\\output_statistics_item-7.txt","w");
	bordaxu = fopen ("dataset\\output_Final-Borda_item-7.txt","a");
	
}

//============================================================================
//用户排序与全排列项目匹配  并进行 

void match()
{	
	int c = 0;														//用户编号 
	ss = fopen("dataset\\input_sushi3a.5000.7-result.txt","r");
	while (fscanf(ss , "%d" , &a ) != EOF)
	{
		sample_match[0] = a ;										//把用户每一个用户的排序读进来 
		for( i = 1 ; i < top_num ; i++)
		{
			fscanf(ss , "%d" , &a );
			sample_match[i] = a ;
		}

		for(i = 0 ; i < pai_num ; i++ )								//进行匹配 
		{
			int b = 0; 
			for( j = 0 ; j < top_num ; j++ )
			{
				if(v_item[i].v_pai[j]==sample_match[j])
				{
					final_match[j] = v_item[i].v_pai[j];
					b++;
				}
				else
				{
					continue;
				}
			}
			if(b == top_num )
			{
				user_num[c].user_bit[i] = 1 ;						//匹配完成以后，得到匹配的序号，并将用户的该序号项目设置为1 
				break;
			}
			else
			{	
				continue;
			}
		}
		//================================================= 
		//RR随机响应处理 // rand() 生成一个int类型的随机数.

		for(i = 0 ; i < pai_num ; i++)
		{
			
			if(user_num[c].user_bit[i] == 0)
			{
				if(rand()%100 < pi) 
				{ 
			        final_data[i] = 0;
			    } 
				else 
				{
			        final_data[i] = 1;
			    }
			}
			else
			{
				if(rand()%100 < pi) 
				{ 
			        final_data[i] = 1;
			    } 
				else 
				{
			        final_data[i] = 0;
			    }
			}
			
			//printf("i= %d \n",i);
			
			
		}
			
		for(j = 0 ; j < pai_num ; j++)													//概率后的输出 
		{
			fprintf(fd,"%d",final_data[j]);
		}
		fprintf(fd,"\n");
		
		//================================================= 
		//加概率未校正统计个数 
		for( j = 0 ; j < pai_num ; j++ )									
		{
			if( final_data[ j ] == 1 )
			{
				jgl_num[ j ] = jgl_num[ j ] + 1 ;
			}
		}
		c++;
	}
	rewind(ss);
	
	for( j = 0 ; j < pai_num ; j++ )				//加概率未校正统计个数输出 
	{
		fprintf(jgl,"%d ",jgl_num[ j ]);
	}
}

//==========================================================================
//校正后的统计个数
 
void XiaoZheng()
{
	for(j = 0 ; j < pai_num ; j++ )
	{
		int jgl_num_ls ;
		jgl_num_ls = jgl_num[ j ] ;
		xz_num[j] =   (((jgl_num_ls/5000.000) + p - 1) / (2*p -1))* 5000;
		if(xz_num[j]<0)										//有可能出现 xz_num[j]是负数的可能性，这是不可用的所以将他设置为0 
		{
			xz_num[j] = 0 ;
		}
	}
}

//==========================================================================
//博尔达

void Borda()
{ 
	int num = 0;
	for(i = 0 ; i < pai_num ; i++)
	{
		for(j = 0 ; j < top_num ; j++)
		{
			for(num = 0 ; num < top_num ; num++)
			{
				if(v_item[i].v_pai[j] == num)
				{
					borda_xg[num].borda_goal =  borda_xg[num].borda_goal + (top_num - j)*xz_num[i] ;
				}
			}	
		}
	}	
} 

//==========================================================================

bool cmp(borda_item x , borda_item y)
{
    if(x.borda_goal == y.borda_goal)
    {
        return x.borda_xu < y.borda_xu ;
    }
    else
    {
        return x.borda_goal > y.borda_goal ;
    }
}

//==========================================================================


 void PaiXu()
 {
 	sort(borda_xg , borda_xg + top_num , cmp ) ;
 }
 
//==========================================================================

void Print()
{
    for(int i = 0 ; i < top_num ; i ++)
    {
        fprintf(bordaxu ,"编号：%d   得分；%d\n",borda_xg[i].borda_xu,borda_xg[i].borda_goal);
    }
    fprintf(bordaxu ,"a_kendall = %f    epsilon = %f\n\n" ,a_kendall,epsilon) ;
}

//==========================================================================

void Kendall()
{
	double cs = top_num * (top_num - 1) / 2.000000 ;
	int kendall = 0 ;
	int n = 0 ;
	int xu[ top_num ] ;			
    while (fscanf(ss,"%d",&a)!=EOF)
    {
    	xu[a] = 0 ;
        for(i = 1 ; i < top_num ; i ++)
        {
            fscanf(ss,"%d",&a);
            xu[a]=i;
        }
        for(i=0;i<(top_num-1);i++)
		{
			for(j=i+1;j<top_num;j++)
			{
				if((xu_budong[borda_xg[i].borda_xu] - xu_budong[borda_xg[j].borda_xu])*(xu[borda_xg[i].borda_xu] - xu[borda_xg[j].borda_xu]) < 0)
				{
					kendall++;	
				}
			}
		}
        n++;
        a_kendall = kendall / (n * cs) ;
    }
}

//==========================================================================

void budong()
{
	for(int i = 0 ; i < top_num ; i ++)
	{
		xu_budong[borda_xg[i].borda_xu]=i;// 最终每个口味对应的排名
	}
}

//==========================================================================

void close_file()
{
	fclose(ar) ;
	fclose(ss) ;
	fclose(fd) ;
	fclose(jgl) ;
	fclose(bordaxu) ;
}

//===========================================================================
//主函数 

int main()
{
	Init() ;
	match() ;
	XiaoZheng() ;
	Borda() ;
	PaiXu() ;
	budong() ;
	Kendall() ;
	Print() ;
	close_file() ;
	return 0 ;
}
