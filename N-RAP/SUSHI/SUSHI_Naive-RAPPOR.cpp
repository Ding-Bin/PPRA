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

const int voter_max = 5000 ;				//����û��� 					
const int pai_num = 5040 ;				        //ȫ���и���  5!=120; 6!=720; 7!=5040; 8!=40320; 9!=362880; 10!=3628800(������)
const int top_num = 7 ;						//ǰ��������� (������)  
const double epsilon = 1.0 ;				//�ŵ�ֵ��ʼ�� �������ã� 

int a ;
int x , y ;									//�Ƚ� 
int sample_match[top_num] ;
int final_match[top_num] ;
int final_data[pai_num] ;					//�ռ��ߵõ�������	
int jgl_num[pai_num] ;						//�Ӹ����Ժ�δУ��5000������120�����е�ͳ�Ƹ��� 
int xz_num[pai_num] ;						//У������У���������
int borda_count[top_num] ;					//��������� 
int xu_budong[top_num] ;					//�����ﲻ������ 

int i = 0, j = 0 ;
int bi ;
int pi ;
 
double p ;									//����
double cp ;									//��������	
double a_kendall ;		

FILE*ar;									//"all rank of 5.txt","r"
FILE*ss;									//"sushi3a.5000.5-result.txt","r"
FILE*fd;									//�Ӹ��ʺ��������������û�н���У��
FILE*jgl;									//�Ӹ��ʺ��ÿ�еĸ���ͳ�ƽ�� 
FILE*bordaxu;								//��������Ĳ����� 

//===========================================================================
//�����ṹ�壺ȫ���нṹ�壬�û��ṹ�� 

struct borda_item									//�ṹ��Borda 
{
    int borda_xu ;									//��Ŀ��� 
    int borda_goal ;								//��Ӧ��Ŀ��ŵ��ܵ÷� 
    borda_item()									//��ʼ���ṹ���ڶ���ı��� 
    {
        borda_xu = 0 ;
        borda_goal = 0;
    }
}borda_xg[top_num] ;								

struct all_rank										//�ṹ����Ŀ 
{
    int v_bit ;										//��Ŀ��� 
    int v_pai[pai_num] ;								
    all_rank()										//��ʼ���ṹ���ڶ���ı��� 
    {
        v_bit = 0 ;
        for(i = 0 ; i < pai_num ; i++)
        {
        	v_pai[i] = 0;
		}       
    }
}v_item[pai_num] ;

struct user											//�û���ʵbitλ�ṹ�� 
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
//��ʼ�� 

void Init()
{
	for(i = 0 ; i < pai_num ; i++)
	{
		v_item[i].v_bit = 0 ;//V��120����ÿһλ�ĳ�ʼ�� 
		jgl_num[i] = 0 ; 	 //�Ӹ����Ժ�5000������120�����е�ͳ�Ƹ�����ʼ�� 
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
	
	for(i = 0 ; i < top_num ; i ++)								//borda���б�ų�ʼ�� 
    {
        borda_xg[i].borda_xu = i ;			
    }
	
	fd = fopen ("dataset\\output_RR_item-7.txt","w");
	jgl = fopen ("dataset\\output_statistics_item-7.txt","w");
	bordaxu = fopen ("dataset\\output_Final-Borda_item-7.txt","a");
	
}

//============================================================================
//�û�������ȫ������Ŀƥ��  ������ 

void match()
{	
	int c = 0;														//�û���� 
	ss = fopen("dataset\\input_sushi3a.5000.7-result.txt","r");
	while (fscanf(ss , "%d" , &a ) != EOF)
	{
		sample_match[0] = a ;										//���û�ÿһ���û������������ 
		for( i = 1 ; i < top_num ; i++)
		{
			fscanf(ss , "%d" , &a );
			sample_match[i] = a ;
		}

		for(i = 0 ; i < pai_num ; i++ )								//����ƥ�� 
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
				user_num[c].user_bit[i] = 1 ;						//ƥ������Ժ󣬵õ�ƥ�����ţ������û��ĸ������Ŀ����Ϊ1 
				break;
			}
			else
			{	
				continue;
			}
		}
		//================================================= 
		//RR�����Ӧ���� // rand() ����һ��int���͵������.

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
			
		for(j = 0 ; j < pai_num ; j++)													//���ʺ����� 
		{
			fprintf(fd,"%d",final_data[j]);
		}
		fprintf(fd,"\n");
		
		//================================================= 
		//�Ӹ���δУ��ͳ�Ƹ��� 
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
	
	for( j = 0 ; j < pai_num ; j++ )				//�Ӹ���δУ��ͳ�Ƹ������ 
	{
		fprintf(jgl,"%d ",jgl_num[ j ]);
	}
}

//==========================================================================
//У�����ͳ�Ƹ���
 
void XiaoZheng()
{
	for(j = 0 ; j < pai_num ; j++ )
	{
		int jgl_num_ls ;
		jgl_num_ls = jgl_num[ j ] ;
		xz_num[j] =   (((jgl_num_ls/5000.000) + p - 1) / (2*p -1))* 5000;
		if(xz_num[j]<0)										//�п��ܳ��� xz_num[j]�Ǹ����Ŀ����ԣ����ǲ����õ����Խ�������Ϊ0 
		{
			xz_num[j] = 0 ;
		}
	}
}

//==========================================================================
//������

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
        fprintf(bordaxu ,"��ţ�%d   �÷֣�%d\n",borda_xg[i].borda_xu,borda_xg[i].borda_goal);
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
		xu_budong[borda_xg[i].borda_xu]=i;// ����ÿ����ζ��Ӧ������
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
//������ 

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
