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

const int voter_max = 15000 ;						//最大用户量 										——可设置			
const int pai_num = 36 ;				    			//项目数为n的平方，1=>1, 2=>4, 3=>9, 4=>16, 5=>25, 6=>36, 7=>49, 8=>64, 9=>81, 10=>100 ——可设置
const int top_num = 6 ;								//前五个的排序 										——可设置
const int jingdu = 1000 ;							//百分比精度，100代表两位，1000代表三位，以此类推 	——可设置
const double epsilon = 1.0 ;						//ε的值初始化 										——可设置
const double xsh = 1.000000 ;						//小数化 

int a ;												//临时存放数据 
int x , y ;											//比较 
int item[top_num][top_num] ;						//项目二维数组 
int final_data[top_num][top_num] ;					//收集者得到的数据	
int jgl_num[top_num][top_num] ;						//加概率以后未校正5000个数据120个序列的统计个数 
int xz_num[top_num][top_num] ;						//校正，加校正后的数字
int xz_ql_num[top_num][top_num] ;					//校正负数清零 
int borda_count[top_num] ;							//博尔达计数 
int xu_budong[top_num] ;							//博尔达不动序列 
int xu_budong_clear[top_num] ;
int ystj_num[top_num][top_num] ;					//原始统计个数 

int i = 0, j = 0 ;
int pi ;
int xh = 0 ;
int xscs = 0 ; 

double p ;											//概率	
double a_kendall ;									//平均肯德尔距离 
double clear_a_kendall ;	
double max_a_kendall = 0 ;							//最大 
double min_a_kendall = 5 ;							//最小 
double all_a_kendall = 0 ;							//多次循环后平均肯德尔总和 
double all_average_kendall = 0 ;					//多次循环后总和平均 
double tongji_num[top_num][top_num] ;				//统计循环后的校正总数 
double clear_max_a_kendall = 0 ;					//最大 
double clear_min_a_kendall = 5 ;					//最小 
double clear_all_a_kendall = 0 ;					//多次循环后平均肯德尔总和 
double clear_all_average_kendall = 0 ;				//多次循环后总和平均 
double clear_tongji_num[top_num][top_num] ;			//统计循环后的校正总数 
double MAE_xz = 0;  									//校正后的MAE
double MAE_xz_clear = 0;								//校正后清零的MAE

FILE*ss;											//"sushi3a.5000.5-result.txt","r"
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
}borda_xg[top_num] , borda_xg_clear[top_num] ;								

//=======================================================================
//打开文件
 
void open()
{
	ss = fopen("dataset\\input_Jester-1-user_num-15000_item-6.txt","r");
	bordaxu = fopen ("dataset\\Output_Straightforward-RAPPOR_User'num-15000_item-6_Epsilon-1.0_for-1000.txt","a");
}
	
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
			ystj_num[i][j] = 0 ;
		}
		borda_xg[i].borda_xu = i ; 					//borda序列编号初始化 
		borda_xg[i].borda_goal = 0 ;	
		borda_xg_clear[i].borda_xu = i ; 					//borda序列编号初始化 
		borda_xg_clear[i].borda_goal = 0 ; 
	}
	
	p = ( exp(epsilon / (2.0 * top_num))) / (1 + exp(epsilon / (2.0 * top_num))) ; 
	pi = p * jingdu ;						//真实回答的概率 
}

//============================================================================
//用户排序与全排列项目匹配  并进行 

void match()
{	
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
		
		for(i = 0 ; i < top_num ; i++)
		{
			for( j = 0 ; j < top_num ; j++ )									
			{
				if( item[i][j] == 1 )
				{
					ystj_num[i][j] = ystj_num[i][j] + 1 ;
				}
			}	
		} 

		//================================================= 
		//RR随机响应处理 // rand() 生成一个int类型的随机数.

		for(i = 0 ; i < top_num ; i++)
		{
			for(j = 0 ; j < top_num ; j++)
			{
				if(item[i][j] == 0)
				{
					if(rand() % jingdu < pi) 
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
					if(rand() % jingdu < pi) 
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
	}
	rewind(ss);											//指针返回的文件开始的位置 
	
	fprintf(bordaxu,"第%d次\n",++xscs);
	fprintf(bordaxu,"Bef-correction-Statistics：");
	for( i = 0 ; i < top_num ; i++ )					//原始统计个数 
	{
		for(j = 0 ; j < top_num ; j++)
		{
			fprintf(bordaxu,"%8d ",jgl_num[i][j]);
		}
	}
	fprintf(bordaxu,"\n");
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
			xz_num[i][j] =  ((((jgl_num_ls / (voter_max * xsh)) + p - 1) / ((2 * p) - 1)))* voter_max;
			xz_ql_num[i][j] = xz_num[i][j] ;
			if(xz_ql_num[i][j] < 0)						//有可能出现 xz_num[j]是负数的可能性，这是不可用的所以将他设置为0 
			{
				xz_ql_num[i][j] = 0 ;
			}
		}
	}
	
	for( i = 0 ; i < top_num ; i++ )					//校正统计个数 
	{
		for(j = 0 ; j < top_num ; j++)
		{
			tongji_num[i][j] = tongji_num[i][j] + xz_num[i][j]/(1000*xsh) ;
		}
	}
	
	for( i = 0 ; i < top_num ; i++ )					//校正统计个数 
	{
		for(j = 0 ; j < top_num ; j++)
		{
			clear_tongji_num[i][j] = clear_tongji_num[i][j] + xz_ql_num[i][j]/(1000*xsh) ;
		}
	}
	
	fprintf(bordaxu,"Aft-correction-Statistics：");
	for( i = 0 ; i < top_num ; i++ )					//校正统计个数 
	{
		for(j = 0 ; j < top_num ; j++)
		{
			fprintf(bordaxu,"%8d ",xz_num[i][j]);
		}
	}
	fprintf(bordaxu,"\n");
	
	fprintf(bordaxu,"Cl0-correction-Statistics：");
	for( i = 0 ; i < top_num ; i++ )					//校正统计个数 
	{
		for(j = 0 ; j < top_num ; j++)
		{
			fprintf(bordaxu,"%8d ",xz_ql_num[i][j]);
		}
	}
	fprintf(bordaxu,"\n");
	
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
			borda_xg_clear[i].borda_goal = borda_xg_clear[i].borda_goal + (xz_ql_num[i][j] * (top_num - j)) ;
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
 	sort(borda_xg_clear , borda_xg_clear + top_num , cmp ) ;
 }

//==========================================================================
//固定博尔达计数输出排序 

void budong()
{
	for(int i = 0 ; i < top_num ; i ++)
	{
		xu_budong[borda_xg[i].borda_xu] = i ;			// 最终每个口味对应的排名
		xu_budong_clear[borda_xg_clear[i].borda_xu] = i ;
	}
}

//==========================================================================
//肯德尔距离计算
 
void Kendall()
{
	double cs = top_num * (top_num - 1) / 2.000000 ;
	int kendall = 0 ;
	int kendall_clear = 0 ;
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
				
				if((xu_budong_clear[borda_xg_clear[i].borda_xu] - xu_budong_clear[borda_xg_clear[j].borda_xu]) * (xu[borda_xg_clear[i].borda_xu] - xu[borda_xg_clear[j].borda_xu]) < 0)
				{
					kendall_clear++;	
				}
			}
		}
        n++;
        a_kendall = kendall / (n * cs) ;
        clear_a_kendall = kendall_clear / (n * cs) ;
    }
    rewind(ss) ;
    
    all_a_kendall = all_a_kendall + a_kendall ;
    xh++;
    if(a_kendall > max_a_kendall)
    {
    	max_a_kendall = a_kendall ;
	}
	if(a_kendall < min_a_kendall)
	{
		min_a_kendall = a_kendall ; 
	}
	
	clear_all_a_kendall = clear_all_a_kendall + clear_a_kendall ;
    if(clear_a_kendall > clear_max_a_kendall)
    {
    	clear_max_a_kendall = clear_a_kendall ;
	}
	if(clear_a_kendall < clear_min_a_kendall)
	{
		clear_min_a_kendall = clear_a_kendall ; 
	}
    
}

//==========================================================================
//MAE（Mean Absolute Error）平均绝对差值

void MAE()
{
	int linshi ;
	for(i = 0 ; i < top_num ; i++)
	{
		for(j = 0 ; j < top_num ; j++)
		{
			linshi = (tongji_num[i][j]/xh)*1000 ;
			MAE_xz = MAE_xz + abs(linshi - ystj_num[i][j]) ;
		}
	}
	MAE_xz = MAE_xz / (top_num * top_num) ;
	
	for(i = 0 ; i < top_num ; i++)
	{
		for(j = 0 ; j < top_num ; j++)
		{
			linshi = (clear_tongji_num[i][j]/xh)*1000 ;
			MAE_xz_clear = MAE_xz_clear + abs(linshi - ystj_num[i][j]) ;
		}
	}
	MAE_xz_clear = MAE_xz_clear / (top_num * top_num) ;	
}

//==========================================================================
//输出排序及得分，同时输出平均肯德尔距离 

void Print()
{
    for(int i = 0 ; i < top_num ; i ++)
    {
        fprintf(bordaxu ,"Item：%d   Goal；%d                    ",borda_xg[i].borda_xu,borda_xg[i].borda_goal);
        fprintf(bordaxu ,"Item：%d   Goal；%d\n",borda_xg_clear[i].borda_xu,borda_xg_clear[i].borda_goal);
    }
    fprintf(bordaxu ,"A kendall = %f    Epsilon = %f\n" ,a_kendall,epsilon) ;
    fprintf(bordaxu ,"Clear A kendall = %f    Epsilon = %f\n\n" ,clear_a_kendall,epsilon) ;
}

void final_print()
{
	int linshi ;
	bordaxu = fopen ("dataset\\Output_Straightforward-RAPPOR_User'num-15000_item-6_Epsilon-1.0_for-1000.txt","a");
	fprintf(bordaxu ,"max_a_kendall = %f    min_a_kendall = %f    all_average_kendall = %f\n" ,max_a_kendall,min_a_kendall,all_a_kendall/xh) ;
	fprintf(bordaxu ,"clear_max_a_kendall = %f    clear_min_a_kendall = %f    clear_all_average_kendall = %f\naverage_tongji =           " ,clear_max_a_kendall,clear_min_a_kendall,clear_all_a_kendall/xh) ;
	for(i = 0 ; i < top_num ; i++)
	{
		for(j = 0 ; j < top_num ; j++)
		{
			linshi = (tongji_num[i][j]/xh)*1000 ;
			fprintf(bordaxu ,"%8d " ,linshi) ;
		}
	}

	fprintf(bordaxu ,"\nclear_average_tongji =") ;
	for(i = 0 ; i < top_num ; i++)
	{
		for(j = 0 ; j < top_num ; j++)
		{
			linshi = (clear_tongji_num[i][j]/xh)*1000 ;
			fprintf(bordaxu ,"%8d " ,linshi) ;
		}
	}
	
	fprintf(bordaxu,"\nYS_TJ_GS    ：             ");
	for( i = 0 ; i < top_num ; i++ )					//原始统计个数 
	{
		for(j = 0 ; j < top_num ; j++)
		{
			fprintf(bordaxu,"%8d ",ystj_num[i][j]);
		}
	}
	fprintf(bordaxu,"\n");
	
	fprintf(bordaxu,"MAE_xz:%.16f\n",MAE_xz);
	fprintf(bordaxu,"MAE_xz_clear:%.16f\n",MAE_xz_clear);
	
	fclose(bordaxu) ;
}

//==========================================================================
//关闭所有打开的文件 

void close_file()
{
	fclose(ss) ;
	fclose(bordaxu) ;
}

//===========================================================================
//主函数 

int main()
{
	srand((unsigned long)time(NULL));
	for(i = 0 ; i < top_num ; i++)
	{
		for(j = 0 ; j < top_num ; j++)
		{
			tongji_num[i][j] = 0 ;
			clear_tongji_num[i][j] = 0 ;	
		}
	}
	i = 0 ;
	j = 0 ;
	int m ;
	for(m = 0 ; m < 1000 ; m++)
	{
		open() ; 
		Init() ;
		match() ;
		XiaoZheng() ;
		Borda() ;
		PaiXu() ;
		budong() ;
		Kendall() ;
		Print() ;
		close_file() ;
	}
	MAE() ;
	final_print() ; 
	return 0 ;
}	
