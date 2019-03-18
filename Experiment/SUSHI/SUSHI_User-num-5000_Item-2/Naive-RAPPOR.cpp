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

const int voter_max = 5000 ;					//最大用户量 									——可设置				
const int pai_num = 2 ;				    		//5的全排列个数  5!=120; 6!=720; 7!=5040; 8!=40320; 9!=362880; 10!=3628800	——可设置
const int top_num = 2 ;							//前五个的排序									——可设置
const int jingdu = 1000 ;						//精度，100为2位，1000为3位，以此类推 			——可设置
const double epsilon = 1.0 ;					// ε											——可设置
const double xsh = 1.000000 ;					//小数化 

int a ;
int x , y ;										//比较 
int sample_match[top_num] ;						//阳历匹配 
int final_match[top_num] ;						//匹配结果 
int final_data[pai_num] ;						//收集者得到的数据	
int jgl_num[pai_num] ;							//加概率以后未校正5000个数据120个序列的统计个数 
int xz_num[pai_num] ;							//校正，加校正后的数字
int xz_ql_num[pai_num] ;
int borda_count[top_num] ;						//博尔达计数 
int xu_budong[top_num] ;						//博尔达不动序列 
int xu_budong_clear[top_num] ;
int ystj_num[pai_num] ;							//原始统计个数 
int ystj_zh_num[top_num][top_num] ; 			//原始的整合成N方的统计个数 
int xz_zh_num[top_num][top_num] ;
int xz_zh_clear_num[top_num][top_num] ;

int i = 0, j = 0 ;
int bi ;
int pi ;
int xh = 0 ;
int xscs = 0 ; 
				 
double p ;										//概率
double a_kendall ;								//平均肯德尔距离 
double clear_a_kendall ;	
double max_a_kendall = 0 ;						//最大 
double min_a_kendall = 5 ;						//最小 
double all_a_kendall = 0 ;						//多次循环后平均肯德尔总和 
double all_average_kendall = 0 ;				//多次循环后总和平均 
double tongji_num[pai_num] ;					//统计循环后的校正总数 
double clear_max_a_kendall = 0 ;				//最大 
double clear_min_a_kendall = 5 ;				//最小 
double clear_all_a_kendall = 0 ;				//多次循环后平均肯德尔总和 
double clear_all_average_kendall = 0 ;			//多次循环后总和平均 
double clear_tongji_num[pai_num] ;				//统计循环后的校正总数 
double MAE_xz = 0;  							//校正后的MAE
double MAE_xz_clear = 0;						//校正后清零的MAE

FILE*ar;										//"all rank of 5.txt","r"
FILE*ss;										//"sushi3a.5000.5-result.txt","r"
FILE*bordaxu;									//最终输出的博尔达 

//===========================================================================
//三个结构体：博尔达计数结构体，全排列结构体，用户真实bit位结构体 

struct borda_item								
{
    int borda_xu ;								
    int borda_goal ;							
    borda_item()								
    {
        borda_xu = 0 ;
        borda_goal = 0;
    }
}borda_xg[top_num] , borda_xg_clear[top_num];								

struct all_rank										 
{
    int v_bit ;									 
    int v_pai[pai_num] ;								
    all_rank()										
    {
        v_bit = 0 ;
        for(i = 0 ; i < pai_num ; i++)
        {
        	v_pai[i] = 0;
		}       
    }
}v_item[pai_num] ;

struct user										
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
//各参数初始化 

void open()
{
	ar = fopen("dataset\\input_all-rank-of-2.txt","r");
	ss = fopen("dataset\\input_sushi3a.5000.2-result.txt","r");
	bordaxu = fopen ("dataset\\Output_Naive-RAPPOR_User'num-5000_item-2_Epsilon-1.0_for-1000_1.txt","a");	
}

void Init()
{
	for(i = 0 ; i < pai_num ; i++)
	{
		v_item[i].v_bit = 0 ;					//V中120列中每一位的初始化 
		jgl_num[i] = 0 ; 	 					//加概率以后5000个数据120个序列的统计个数初始化 
		final_data[i] = 0 ;
		ystj_num[i] = 0 ; 
	}
	i=0;
	
	for(i = 0 ; i < top_num ; i++)				//整合初始化 
	{
		for(j = 0 ; j < top_num ; j++)
		{
			ystj_zh_num[i][j] = 0 ;
			xz_zh_num[i][j] = 0 ;
			xz_zh_clear_num[i][j] = 0 ;
		}
	}
	
	for(i = 0 ; i < pai_num ; i++)
    {
    	v_item[i].v_bit = 0;
    	for(j=0;j<pai_num;j++)
    	{
    		v_item[i].v_pai[j] = 0 ;
		}
    	
	} 
	
	for(i=0;i<voter_max;i++)
	{
		for(j=0;j<pai_num;j++)
		{
			user_num[i].user_bit[j] = 0 ;
		}
	}
	
	i = 0 ;
	j = 0 ;
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
	rewind(ar);
	
	p = exp(epsilon / 2.0) / (1 + exp(epsilon / 2.0)) ;
	pi = p * jingdu ;							//pi = ( 1 - p / 2 ) * 100 ;二者待验证 
	
	for(i = 0 ; i < top_num ; i ++)				//borda序列编号初始化 
    {
        borda_xg[i].borda_xu = i ; 					//borda序列编号初始化 
		borda_xg[i].borda_goal = 0 ;	
		borda_xg_clear[i].borda_xu = i ; 					//borda序列编号初始化 
		borda_xg_clear[i].borda_goal = 0 ; 		
    }
}

//============================================================================
//用户排序与全排列项目匹配，并进行随机响应处理 

void match()
{	
	int c = 0;									 //用户编号 
	while (fscanf(ss , "%d" , &a ) != EOF)
	{
		sample_match[0] = a ;					 //把用户每一个用户的排序读进来 
		for( i = 1 ; i < top_num ; i++)
		{
			fscanf(ss , "%d" , &a );
			sample_match[i] = a ;
		}
		
		for(i = 0 ; i < top_num ; i++)			//整合统计 
		{
			ystj_zh_num[sample_match[i]][i] = ystj_zh_num[sample_match[i]][i] + 1 ;
		} 
		
		/*for(i = 0 ; i < top_num ; i++)				//整合初始化 
		{
			for(j = 0 ; j < top_num ; j++)
			{
				printf("%d ",ystj_zh_num[i][j]) ;
			}
		}
		printf("\n");*/

		for(i = 0 ; i < pai_num ; i++ )			 //进行匹配 
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
				user_num[c].user_bit[i] = 1 ;	  //匹配完成以后，得到匹配的序号，并将用户的该序号项目设置为1 
				break;
			}
			else
			{	
				continue;
			}
		}
		
		for(i = 0 ; i < pai_num ; i++)
		{
			if( user_num[c].user_bit[i] == 1 )
			{
				ystj_num[i] = ystj_num[i] + 1 ;
			}
		}
		
		//================================================= 
		//RR随机响应处理 // rand() 生成一个int类型的随机数.

		for(i = 0 ; i < pai_num ; i++)
		{
			if(user_num[c].user_bit[i] == 0)
			{
				if(rand() % jingdu < pi) 
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
				if(rand() % jingdu < pi) 
				{ 
			        final_data[i] = 1;
			    } 
				else 
				{
			        final_data[i] = 0;
			    }
			}
		}
		
		//================================================= 
		//加概率未校正统计个数 
		for( j = 0 ; j < pai_num ; j++ )									
		{
			if( final_data[j] == 1 )
			{
				jgl_num[j] = jgl_num[j] + 1 ;
			}
		}
			
		c++;
	}
	rewind(ss);
	
	fprintf(bordaxu,"第%d次\n",++xscs);
	
	fprintf(bordaxu,"Bef-correction-Statistics：");
	for( j = 0 ; j < pai_num ; j++ )			  //加概率未校正统计个数输出 
	{
		fprintf(bordaxu,"%6d ",jgl_num[j]);
	}
	fprintf(bordaxu,"\n");
}

//==========================================================================
//校正后每一列的统计个数
 
void XiaoZheng()
{
	for(j = 0 ; j < pai_num ; j++ )
	{
		int jgl_num_ls ;
		jgl_num_ls = jgl_num[j] ;
		xz_num[j] =   (((jgl_num_ls / (voter_max * xsh)) + p - 1) / (2 * p -1)) * voter_max;
		xz_ql_num[j] = xz_num[j] ;
		if(xz_ql_num[j]<0)				  			 //有可能出现 xz_num[j]是负数的可能性，这是不可用的所以将他设置为0 
		{
			xz_ql_num[j] = 0 ;
		}
	}
	
	for( i = 0 ; i < pai_num ; i++ )					//校正统计个数 
	{
		tongji_num[i] = tongji_num[i] + xz_num[i]/(1000*xsh) ;
	}
	
	for( i = 0 ; i < pai_num ; i++ )					//校正统计个数 
	{
		clear_tongji_num[i] = clear_tongji_num[i] + xz_ql_num[i]/(1000*xsh) ;
	}
	
	
	fprintf(bordaxu,"Aft-correction-Statistics：");
	for( i = 0 ; i < pai_num ; i++ )					//校正统计个数 
	{
		fprintf(bordaxu,"%6d ",xz_num[i]);
	}
	fprintf(bordaxu,"\n");
	
	fprintf(bordaxu,"Cl0-correction-Statistics：");
	for( i = 0 ; i < pai_num ; i++ )					//校正统计个数 
	{
		fprintf(bordaxu,"%6d ",xz_ql_num[i]);
	}
	fprintf(bordaxu,"\n");
}

//==========================================================================
//博尔达计数 

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
					borda_xg_clear[num].borda_goal =  borda_xg_clear[num].borda_goal + (top_num - j)*xz_ql_num[i] ;
				}
			}
		}
	}	
}  

//==========================================================================
//比较，按照得分由大到小的顺序输出，得分相同序号小的先输出 

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
//排序
 
 void PaiXu()
 {
 	sort(borda_xg , borda_xg + top_num , cmp ) ;
 	sort(borda_xg_clear , borda_xg_clear + top_num , cmp ) ;
 }
 
//==========================================================================
//固定最终输出的排序，为了计算肯德尔距离
 
void budong()
{
	for(int i = 0 ; i < top_num ; i ++)
	{
		xu_budong[borda_xg[i].borda_xu]=i;// 最终每个口味对应的排名
		xu_budong_clear[borda_xg_clear[i].borda_xu]=i;
	}
}

//==========================================================================
//平均肯德尔距离计算 

void Kendall()
{
	double cs = top_num * (top_num - 1) / 2.000000 ;
	int kendall = 0 ;
	int kendall_clear = 0 ;
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
				
				if((xu_budong_clear[borda_xg_clear[i].borda_xu] - xu_budong_clear[borda_xg_clear[j].borda_xu])*(xu[borda_xg_clear[i].borda_xu] - xu[borda_xg_clear[j].borda_xu]) < 0)
				{
					kendall_clear++;	
				}
			}
		}
        n++;
        a_kendall = kendall / (n * cs) ;
        clear_a_kendall = kendall_clear / (n * cs) ;
    }
    rewind(ss);
    
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
//MAE（Mean Absolute Error）平均绝对差值xz_zh_clear_num
 
void MAE()
{
	for(i = 0 ; i < top_num ; i++)
	{
		for(j = 0 ; j < top_num ; j++)
		{
			MAE_xz = MAE_xz + (abs(xz_zh_num[i][j] - ystj_zh_num[i][j])) / (1000 * xsh);
		}
	}
	MAE_xz = (MAE_xz * 1000) / (top_num * top_num) ;
	
	for(i = 0 ; i < top_num ; i++)
	{
		for(j = 0 ; j < top_num ; j++)
		{
			MAE_xz_clear = MAE_xz_clear + (abs(xz_zh_clear_num[i][j] - ystj_zh_num[i][j])) /(1000 * xsh) ;
		}
	}
	MAE_xz_clear = (MAE_xz_clear * 1000) / (top_num * top_num) ;
	
	fprintf(bordaxu,"MAE_xz:%.16f\n",MAE_xz);
	fprintf(bordaxu,"MAE_xz_clear:%.16f\n",MAE_xz_clear);	
} 

//==========================================================================
//输出
 
void Print()
{
    for(int i = 0 ; i < top_num ; i ++)
    {
        fprintf(bordaxu ,"item：%d   goal；%d               ",borda_xg[i].borda_xu,borda_xg[i].borda_goal);
        fprintf(bordaxu ,"Item：%d   Goal；%d\n",borda_xg_clear[i].borda_xu,borda_xg_clear[i].borda_goal);
    }
    fprintf(bordaxu ,"a_kendall = %f    epsilon = %f\n" ,a_kendall,epsilon) ;
    fprintf(bordaxu ,"Clear A kendall = %f    Epsilon = %f\n\n" ,clear_a_kendall,epsilon) ;
}

void final_print()
{
	int linshi ;
	int tongji_num_zh_linshi[pai_num] ;
	int clear_tongji_num_zh_linshi[pai_num] ;
	bordaxu = fopen ("dataset\\Output_Naive-RAPPOR_User'num-5000_item-2_Epsilon-1.0_for-1000_1.txt","a");
	fprintf(bordaxu ,"max_a_kendall = %f    min_a_kendall = %f    all_average_kendall = %f\n" ,max_a_kendall,min_a_kendall,all_a_kendall/xh) ;
	fprintf(bordaxu ,"clear_max_a_kendall = %f    clear_min_a_kendall = %f    clear_all_average_kendall = %f\naverage_tongji =           " ,clear_max_a_kendall,clear_min_a_kendall,clear_all_a_kendall/xh) ;
	for(i = 0 ; i < pai_num ; i++)
	{
		linshi = (tongji_num[i]/xh)*1000 ;
		tongji_num_zh_linshi[i] = linshi ;
		fprintf(bordaxu ,"%8d " ,linshi) ;
	}

	fprintf(bordaxu ,"\nclear_average_tongji =") ;
	for(i = 0 ; i < pai_num ; i++)
	{
		linshi = (clear_tongji_num[i]/xh)*1000 ;
		clear_tongji_num_zh_linshi[i] = linshi ;
		fprintf(bordaxu ,"%8d " ,linshi) ;
	}
	
	fprintf(bordaxu,"\nYS_TJ_GS    ：             ");
	for( i = 0 ; i < pai_num ; i++ )					//原始统计个数 
	{
		fprintf(bordaxu,"%8d ",ystj_num[i]);
	}
	fprintf(bordaxu,"\n");
	
	//=============================================================================================== 
	
	fprintf(bordaxu ,"average_tongji_zh =") ;
	for( i = 0 ; i < pai_num ; i++ )				//校正整合xz_zh_clear_num								
	{
		for(j = 0 ; j < top_num ; j++)
		{
			a = v_item[i].v_pai[j] ;
			xz_zh_num[a][j] = xz_zh_num[a][j] + tongji_num_zh_linshi[i] ;
		}
	}
	for(i = 0 ; i < top_num ; i++)
	{
		for(j = 0 ; j < top_num ; j++)
		{
			fprintf(bordaxu ,"%8d " ,xz_zh_num[i][j]) ;
		}
	}
	
	fprintf(bordaxu ,"\nclear_average_tongji_zh =") ;
	for( i = 0 ; i < pai_num ; i++ )				//校正整合xz_zh_clear_num								
	{
		for(j = 0 ; j < top_num ; j++)
		{
			a = v_item[i].v_pai[j] ;
			xz_zh_clear_num[a][j] = xz_zh_clear_num[a][j] + clear_tongji_num_zh_linshi[i] ;
		}
	}
	for(i = 0 ; i < top_num ; i++)
	{
		for(j = 0 ; j < top_num ; j++)
		{
			fprintf(bordaxu ,"%8d " ,xz_zh_clear_num[i][j]) ;
		}
	}
	
	fprintf(bordaxu ,"\nYS_TJ_GS_zh    ：             ") ;
	for(i = 0 ; i < top_num ; i++)
	{
		for(j = 0 ; j < top_num ; j++)
		{
			fprintf(bordaxu ,"%8d " ,ystj_zh_num[i][j]) ;
		}
	}
	fprintf(bordaxu ,"\n") ;
	
}

//==========================================================================
//关闭所有打开的文档
 
void close_file()
{
	fclose(ar) ;
	fclose(ss) ;
	fclose(bordaxu) ;
}

//===========================================================================
//主函数 

int main()
{
	srand((unsigned long)time(NULL));
	for(i = 0 ; i < pai_num ; i++)
	{
		tongji_num[i] = 0 ;
		clear_tongji_num[i] = 0 ;
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
	final_print() ;
	MAE() ;
	fclose(bordaxu) ;
	return 0 ;
}	
