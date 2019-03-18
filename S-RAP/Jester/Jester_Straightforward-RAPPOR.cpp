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
//�������� 

const int voter_max = 15000 ;						//����û��� 					
const int pai_num = 3600 ;				    		//��Ŀ��Ϊn��ƽ����1=>1, 2=>4, 3=>9, 4=>16, 5=>25, 6=>36, 7=>49, 8=>64, 9=>81, 10=>100 
const int top_num = 60 ;								//ǰ��������� 
const int jingdu = 100 ;							//�ٷֱȾ��ȣ�100������λ��1000������λ���Դ����� 
const double epsilon = 1 ;							//�ŵ�ֵ��ʼ�� 
const double xsh = 1.000000 ;						//С���� 

int a ;												//��ʱ������� 
int x , y ;											//�Ƚ� 
int item[top_num][top_num] ;						//��Ŀ��ά���� 
int final_data[top_num][top_num] ;					//�ռ��ߵõ�������	
int jgl_num[top_num][top_num] ;						//�Ӹ����Ժ�δУ��5000������120�����е�ͳ�Ƹ��� 
int xz_num[top_num][top_num] ;						//У������У���������
int borda_count[top_num] ;							//��������� 
int xu_budong[top_num] ;							//�����ﲻ������ 
int xu_budong_mxz[top_num] ;						//�����ﲻ������ 
int i = 0, j = 0 ;
int bili ;											//���� 
int xh = 0;											//ѭ������ 
int pj_num[top_num][top_num] ;						//���ѭ�����ƽ�����洢 

double p ;											//����	
double a_kendall ;									//ƽ���ϵ¶����� 
double a_kendall_mxz ;

FILE*ss;											//"sushi3a.5000.5-result.txt","r"
FILE*fd;											//�Ӹ��ʺ��������������û�н���У��
FILE*jgl;											//�Ӹ��ʺ��ÿ�еĸ���ͳ�ƽ�� 
FILE*bordaxu;										//��������Ĳ����� 

//===========================================================================
//borda�����ṹ��

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
//���������ʼ�� 

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
		borda_xg[i].borda_xu = i ; 					//borda���б�ų�ʼ�� 
		borda_xg_mxz[i].borda_xu = i ;	 
	}
	
	srand((unsigned long)time(NULL));				//RR part ��������� 
	p = ( exp(epsilon / (2.0 * top_num))) / (1 + exp(epsilon / (2.0 * top_num))) ; 
	bili = p * jingdu ;						//��ʵ�ش�ĸ��� 
	printf("p = %.16f     bili = %d  \n" , p,bili);
	
	fd = fopen ("dataset\\output_RR_item-60.txt","w");
	jgl = fopen ("dataset\\output_statistics_item-60.txt","w");
	bordaxu = fopen ("dataset\\output_Final-Borda_item-60.txt","a");
}

//============================================================================
//�û�������ȫ������Ŀƥ��  ������ 

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
		for(i = 0 ; i < top_num ; i++)				//��Ŀ��ά�����ʼ�� 
		{					
			for(j = 0 ; j < top_num ; j++)
			{
				item[i][j] = 0 ;
			} 						 
		}
		
		item[a][0] = 1 ;							//���û�ÿһ���û������������ 
		for( i = 1 ; i < top_num ; i++)
		{
			fscanf(ss , "%d" , &a );
			item[a][i] = 1 ;
		}
		for(i = 0 ; i < top_num ; i++)					//���ʺ����� 
		{
			for(j = 0 ; j < top_num ; j++)	
			{
				fprintf(fd,"%d",item[i][j]);
			}
			fprintf(fd," ");
		}
		fprintf(fd,"\n");

		//================================================= 
		//RR�����Ӧ���� // rand() ����һ��int���͵������.

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
			
		/*for(i = 0 ; i < top_num ; i++)					//���ʺ����� 
		{
			for(j = 0 ; j < top_num ; j++)	
			{
				fprintf(fd,"%d",final_data[i][j]);
			}
			fprintf(fd," ");
		}
		fprintf(fd,"\n");*/
		
		//================================================= 
		//�Ӹ���δУ��ͳ�Ƹ���
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
	rewind(ss);											//ָ�뷵�ص��ļ���ʼ��λ�� 
	
	for( i = 0 ; i < top_num ; i++ )					//�Ӹ���δУ��ͳ�Ƹ������ 
	{
		for(j = 0 ; j < top_num ; j++)
		{
			fprintf(jgl,"%d ",jgl_num[i][j]);
		}
		fprintf(jgl,"\n");
	}
	fprintf(jgl,"\n");
	for( i = 0 ; i < top_num ; i++ )					//�Ӹ���δУ��ͳ�Ƹ������ 
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
//У�����ͳ�Ƹ���
 
void XiaoZheng()
{
	for(i = 0 ; i < top_num ; i++ )
	{
		int jgl_num_ls ;
		for(j = 0 ; j < top_num ; j++)
		{
			jgl_num_ls = jgl_num[i][j] ;
			xz_num[i][j] =  ((((jgl_num_ls / (voter_max * xsh)) + p - 1) / (2 * p - 1)))* voter_max;
			/*if(xz_num[i][j] < 0)						//�п��ܳ��� xz_num[j]�Ǹ����Ŀ����ԣ����ǲ����õ����Խ�������Ϊ0 
			{
				xz_num[i][j] = 0 ;
			}*/
		}
	}
}

//==========================================================================
//����������� 

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
//�Ƚ��жϣ��������ɴ�С���򣬷�����ͬ�����С������� 
 
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
//sort���� 

 void PaiXu()
 {
 	sort(borda_xg , borda_xg + top_num , cmp ) ;
 	sort(borda_xg_mxz , borda_xg_mxz + top_num , cmp ) ;
 }

//==========================================================================
//�̶����������������� 

void budong()
{
	for(int i = 0 ; i < top_num ; i ++)
	{
		xu_budong[borda_xg[i].borda_xu] = i ;			// ����ÿ����ζ��Ӧ������
		xu_budong_mxz[borda_xg_mxz[i].borda_xu] = i ;
	}
}

//==========================================================================
//�ϵ¶��������
 
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
//������򼰵÷֣�ͬʱ���ƽ���ϵ¶����� 

void Print()
{
    for(int i = 0 ; i < top_num ; i ++)
    {
        fprintf(bordaxu ,"Item��%d   Goal��%d\n",borda_xg[i].borda_xu,borda_xg[i].borda_goal);
    }
    fprintf(bordaxu ,"A kendall = %f    Epsilon = %f\n\n" ,a_kendall,epsilon) ;
    
    fprintf(bordaxu ,"NO Correction \n") ;
    for(int i = 0 ; i < top_num ; i ++)
    {
        fprintf(bordaxu ,"Item��%d   Goal��%d\n",borda_xg_mxz[i].borda_xu,borda_xg_mxz[i].borda_goal);
    }
    fprintf(bordaxu ,"A kendall_mxz = %f    Epsilon = %f\n=====================================================================\n" ,a_kendall_mxz,epsilon) ;
}

//==========================================================================
//�ر����д򿪵��ļ� 

void close_file()
{
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
