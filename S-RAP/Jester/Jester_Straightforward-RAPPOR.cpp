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
//参数定义 

const int voter_max = 15000 ;						//最大用户量 					
const int pai_num = 3600 ;				    		//项目数为n的平方，1=>1, 2=>4, 3=>9, 4=>16, 5=>25, 6=>36, 7=>49, 8=>64, 9=>81, 10=>100 
const int top_num = 60 ;								//前五个的排序 
const int jingdu = 100 ;							//百分比精度，100代表两位，1000代表三位，以此类推 
const double epsilon = 1 ;							//ε的值初始化 
const double xsh = 1.000000 ;						//小数化 

int a ;												//临时存放数据 
int x , y ;											//比较 
int item[top_num][top_num] ;						//项目二维数组 
int final_data[top_num][top_num] ;					//收集者得到的数据	
int jgl_num[top_num][top_num] ;						//加概率以后未校正5000个数据120个序列的统计个数 
int xz_num[top_num][top_num] ;						//校正，加校正后的数字
int borda_count[top_num] ;							//博尔达计数 
int xu_budong[top_num] ;							//博尔达不动序列 
int xu_budong_mxz[top_num] ;						//博尔达不动序列 
int i = 0, j = 0 ;
int bili ;											//比例 
int xh = 0;											//循环次数 
int pj_num[top_num][top_num] ;						//多次循环后的平均数存储 

double p ;											//概率	
double a_kendall ;									//平均肯德尔距离 
double a_kendall_mxz ;

FILE*ss;											//"sushi3a.5000.5-result.txt","r"
FILE*fd;											//加概率后的输出结果，但是没有进行校正
FILE*jgl;											//加概率后的每列的个数统计结果 
FILE*bordaxu;										//最终输出的博尔达 

//===========================================================================
//borda计数结构体

struct borda_item									
{
    int borda_xu ;									
    int borda_goal ;								
    borda_item()									
    {
        borda_xu = 0 ;
        borda_goal = 0;
    }
}borda_xg[top_num],borda_xg_mxz[top_num] ;								

//=======================================================================
//各类参数初始化 

void Init()
{
	for(i = 0 ; i < top_num ; i++)					
	{					
		for(j = 0 ; j < top_num ; j++)
		{
			item[i][j] = 0 ;						
			jgl_num[i][j] = 0 ;						
			final_data[i][j] = 0 ;
			pj_num[i][j] = 0 ;
		}
		borda_xg[i].borda_xu = i ; 					//borda序列编号初始化 
		borda_xg_mxz[i].borda_xu = i ;	 
	}
	
	srand((unsigned long)time(NULL));				//RR part 随机数生成 
	p = ( exp(epsilon / (2.0 * top_num))) / (1 + exp(epsilon / (2.0 * top_num))) ; 
	bili = p * jingdu ;						//真实回答的概率 
	printf("p = %.16f     bili = %d  \n" , p,bili);
	
	fd = fopen ("dataset\\output_RR_item-60.txt","w");
	jgl = fopen ("dataset\\output_statistics_item-60.txt","w");
	bordaxu = fopen ("dataset\\output_Final-Borda_item-60.txt","a");
}

//============================================================================
//用户排序与全排列项目匹配  并进行 

void match()
{	
	int linshi_num[top_num][top_num] ;
	for( i = 0 ; i < top_num ; i++ )					
	{
		for(j = 0 ; j < top_num ; j++)
		{
			linshi_num[i][j] = 0 ;
		}
	}
	ss = fopen("dataset\\input_XuHao_Top-60-user_num-15000.txt","r");
	while (fscanf(ss , "%d" , &a ) != EOF)
	{
		for(i = 0 ; i < top_num ; i++)				//项目二维数组初始化 
		{					
			for(j = 0 ; j < top_num ; j++)
			{
				item[i][j] = 0 ;
			} 						 
		}
		
		item[a][0] = 1 ;							//把用户每一个用户的排序读进来 
		for( i = 1 ; i < top_num ; i++)
		{
			fscanf(ss , "%d" , &a );
			item[a][i] = 1 ;
		}
		for(i = 0 ; i < top_num ; i++)					//概率后的输出 
		{
			for(j = 0 ; j < top_num ; j++)	
			{
				fprintf(fd,"%d",item[i][j]);
			}
			fprintf(fd," ");
		}
		fprintf(fd,"\n");

		//================================================= 
		//RR随机响应处理 // rand() 生成一个int类型的随机数.

		for(i = 0 ; i < top_num ; i++)
		{
			for(j = 0 ; j < top_num ; j++)
			{
				if(item[i][j] == 0)
				{
					if((rand() % jingdu) < bili) 
					{ 
				        final_data[i][j] = 0;
				    } 
					else 
					{
				        final_data[i][j] = 1;
				    }
				}
				else
				{
					if((rand() % jingdu) < bili) 
					{ 
				        final_data[i][j] = 1;
				    } 
					else 
					{
				        final_data[i][j] = 0;
				    }
				}
			}		
		}
			
		/*for(i = 0 ; i < top_num ; i++)					//概率后的输出 
		{
			for(j = 0 ; j < top_num ; j++)	
			{
				fprintf(fd,"%d",final_data[i][j]);
			}
			fprintf(fd," ");
		}
		fprintf(fd,"\n");*/
		
		//================================================= 
		//加概率未校正统计个数
		for(i = 0 ; i < top_num ; i++)
		{
			for( j = 0 ; j < top_num ; j++ )									
			{
				if( final_data[i][j] == 1 )
				{
					jgl_num[i][j] = jgl_num[i][j] + 1 ;
				}
			}	
		} 
		for(i = 0 ; i < top_num ; i++)
		{
			for( j = 0 ; j < top_num ; j++ )									
			{
				if( item[i][j] == 1 )
				{
					linshi_num[i][j] = linshi_num[i][j] + 1 ;
				}
			}	
		} 	
	}
	rewind(ss);											//指针返回的文件开始的位置 
	
	for( i = 0 ; i < top_num ; i++ )					//加概率未校正统计个数输出 
	{
		for(j = 0 ; j < top_num ; j++)
		{
			fprintf(jgl,"%d ",jgl_num[i][j]);
		}
		fprintf(jgl,"\n");
	}
	fprintf(jgl,"\n");
	for( i = 0 ; i < top_num ; i++ )					//加概率未校正统计个数输出 
	{
		for(j = 0 ; j < top_num ; j++)
		{
			fprintf(jgl,"%d ",linshi_num[i][j]);
		}
		fprintf(jgl,"\n");
	}
	
	xh++ ;
	for(i = 0 ; i < top_num ; i++)
	{
		for(j = 0 ; j < top_num ; j++)
		{
			pj_num[i][j] =  (pj_num[i][j] + jgl_num[i][j]) / xh ;
		}
	}
}

//==========================================================================
//校正后的统计个数
 
void XiaoZheng()
{
	for(i = 0 ; i < top_num ; i++ )
	{
		int jgl_num_ls ;
		for(j = 0 ; j < top_num ; j++)
		{
			jgl_num_ls = jgl_num[i][j] ;
			xz_num[i][j] =  ((((jgl_num_ls / (voter_max * xsh)) + p - 1) / (2 * p - 1)))* voter_max;
			/*if(xz_num[i][j] < 0)						//有可能出现 xz_num[j]是负数的可能性，这是不可用的所以将他设置为0 
			{
				xz_num[i][j] = 0 ;
			}*/
		}
	}
}

//==========================================================================
//博尔达计数法 

void Borda()
{
	for(i = 0 ; i < top_num ; i++ )
	{
		for(j = 0 ; j < top_num ; j++)
		{
			borda_xg[i].borda_goal = borda_xg[i].borda_goal + (xz_num[i][j] * (top_num - j)) ;
			borda_xg_mxz[i].borda_goal = borda_xg_mxz[i].borda_goal + (jgl_num[i][j] * (top_num - j)) ;
		}
	}	
} 

//==========================================================================
//比较判断，按分数由大到小排序，分数相同的序号小的先输出 
 
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
//sort排序 

 void PaiXu()
 {
 	sort(borda_xg , borda_xg + top_num , cmp ) ;
 	sort(borda_xg_mxz , borda_xg_mxz + top_num , cmp ) ;
 }

//==========================================================================
//固定博尔达计数输出排序 

void budong()
{
	for(int i = 0 ; i < top_num ; i ++)
	{
		xu_budong[borda_xg[i].borda_xu] = i ;			// 最终每个口味对应的排名
		xu_budong_mxz[borda_xg_mxz[i].borda_xu] = i ;
	}
}

//==========================================================================
//肯德尔距离计算
 
void Kendall()
{
	double cs = top_num * (top_num - 1) / 2.000000 ;
	int kendall = 0 ;
	int kendall_mxz = 0 ;
	int n = 0 ;
	int xu[ top_num ] ;			
    while (fscanf(ss,"%d",&a) != EOF)
    {
    	xu[a] = 0 ;
        for(i = 1 ; i < top_num ; i ++)
        {
            fscanf(ss,"%d",&a);
            xu[a]=i;
        }
        for(i = 0 ; i < (top_num - 1) ; i++)
		{
			for(j = i + 1 ; j < top_num ; j++)
			{
				if((xu_budong[borda_xg[i].borda_xu] - xu_budong[borda_xg[j].borda_xu]) * (xu[borda_xg[i].borda_xu] - xu[borda_xg[j].borda_xu]) < 0)
				{
					kendall++;	
				}
				if((xu_budong_mxz[borda_xg_mxz[i].borda_xu] - xu_budong_mxz[borda_xg_mxz[j].borda_xu]) * (xu[borda_xg_mxz[i].borda_xu] - xu[borda_xg_mxz[j].borda_xu]) < 0)
				{
					kendall_mxz++;	
				}
			}
		}
        n++;
        a_kendall = kendall / (n * cs) ;
        a_kendall_mxz = kendall_mxz / (n * cs) ;
    }
    rewind(ss) ; 
}

//==========================================================================
//输出排序及得分，同时输出平均肯德尔距离 

void Print()
{
    for(int i = 0 ; i < top_num ; i ++)
    {
        fprintf(bordaxu ,"Item：%d   Goal；%d\n",borda_xg[i].borda_xu,borda_xg[i].borda_goal);
    }
    fprintf(bordaxu ,"A kendall = %f    Epsilon = %f\n\n" ,a_kendall,epsilon) ;
    
    fprintf(bordaxu ,"NO Correction \n") ;
    for(int i = 0 ; i < top_num ; i ++)
    {
        fprintf(bordaxu ,"Item：%d   Goal；%d\n",borda_xg_mxz[i].borda_xu,borda_xg_mxz[i].borda_goal);
    }
    fprintf(bordaxu ,"A kendall_mxz = %f    Epsilon = %f\n=====================================================================\n" ,a_kendall_mxz,epsilon) ;
}

//==========================================================================
//关闭所有打开的文件 

void close_file()
{
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
