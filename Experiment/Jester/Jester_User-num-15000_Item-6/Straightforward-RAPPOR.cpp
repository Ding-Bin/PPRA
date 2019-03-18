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

const int voter_max = 15000 ;						//����û��� 										����������			
const int pai_num = 36 ;				    			//��Ŀ��Ϊn��ƽ����1=>1, 2=>4, 3=>9, 4=>16, 5=>25, 6=>36, 7=>49, 8=>64, 9=>81, 10=>100 ����������
const int top_num = 6 ;								//ǰ��������� 										����������
const int jingdu = 1000 ;							//�ٷֱȾ��ȣ�100������λ��1000������λ���Դ����� 	����������
const double epsilon = 1.0 ;						//�ŵ�ֵ��ʼ�� 										����������
const double xsh = 1.000000 ;						//С���� 

int a ;												//��ʱ������� 
int x , y ;											//�Ƚ� 
int item[top_num][top_num] ;						//��Ŀ��ά���� 
int final_data[top_num][top_num] ;					//�ռ��ߵõ�������	
int jgl_num[top_num][top_num] ;						//�Ӹ����Ժ�δУ��5000������120�����е�ͳ�Ƹ��� 
int xz_num[top_num][top_num] ;						//У������У���������
int xz_ql_num[top_num][top_num] ;					//У���������� 
int borda_count[top_num] ;							//��������� 
int xu_budong[top_num] ;							//�����ﲻ������ 
int xu_budong_clear[top_num] ;
int ystj_num[top_num][top_num] ;					//ԭʼͳ�Ƹ��� 

int i = 0, j = 0 ;
int pi ;
int xh = 0 ;
int xscs = 0 ; 

double p ;											//����	
double a_kendall ;									//ƽ���ϵ¶����� 
double clear_a_kendall ;	
double max_a_kendall = 0 ;							//��� 
double min_a_kendall = 5 ;							//��С 
double all_a_kendall = 0 ;							//���ѭ����ƽ���ϵ¶��ܺ� 
double all_average_kendall = 0 ;					//���ѭ�����ܺ�ƽ�� 
double tongji_num[top_num][top_num] ;				//ͳ��ѭ�����У������ 
double clear_max_a_kendall = 0 ;					//��� 
double clear_min_a_kendall = 5 ;					//��С 
double clear_all_a_kendall = 0 ;					//���ѭ����ƽ���ϵ¶��ܺ� 
double clear_all_average_kendall = 0 ;				//���ѭ�����ܺ�ƽ�� 
double clear_tongji_num[top_num][top_num] ;			//ͳ��ѭ�����У������ 
double MAE_xz = 0;  									//У�����MAE
double MAE_xz_clear = 0;								//У���������MAE

FILE*ss;											//"sushi3a.5000.5-result.txt","r"
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
}borda_xg[top_num] , borda_xg_clear[top_num] ;								

//=======================================================================
//���ļ�
 
void open()
{
	ss = fopen("dataset\\input_Jester-1-user_num-15000_item-6.txt","r");
	bordaxu = fopen ("dataset\\Output_Straightforward-RAPPOR_User'num-15000_item-6_Epsilon-1.0_for-1000.txt","a");
}
	
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
			ystj_num[i][j] = 0 ;
		}
		borda_xg[i].borda_xu = i ; 					//borda���б�ų�ʼ�� 
		borda_xg[i].borda_goal = 0 ;	
		borda_xg_clear[i].borda_xu = i ; 					//borda���б�ų�ʼ�� 
		borda_xg_clear[i].borda_goal = 0 ; 
	}
	
	p = ( exp(epsilon / (2.0 * top_num))) / (1 + exp(epsilon / (2.0 * top_num))) ; 
	pi = p * jingdu ;						//��ʵ�ش�ĸ��� 
}

//============================================================================
//�û�������ȫ������Ŀƥ��  ������ 

void match()
{	
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
		//RR�����Ӧ���� // rand() ����һ��int���͵������.

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
	}
	rewind(ss);											//ָ�뷵�ص��ļ���ʼ��λ�� 
	
	fprintf(bordaxu,"��%d��\n",++xscs);
	fprintf(bordaxu,"Bef-correction-Statistics��");
	for( i = 0 ; i < top_num ; i++ )					//ԭʼͳ�Ƹ��� 
	{
		for(j = 0 ; j < top_num ; j++)
		{
			fprintf(bordaxu,"%8d ",jgl_num[i][j]);
		}
	}
	fprintf(bordaxu,"\n");
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
			xz_num[i][j] =  ((((jgl_num_ls / (voter_max * xsh)) + p - 1) / ((2 * p) - 1)))* voter_max;
			xz_ql_num[i][j] = xz_num[i][j] ;
			if(xz_ql_num[i][j] < 0)						//�п��ܳ��� xz_num[j]�Ǹ����Ŀ����ԣ����ǲ����õ����Խ�������Ϊ0 
			{
				xz_ql_num[i][j] = 0 ;
			}
		}
	}
	
	for( i = 0 ; i < top_num ; i++ )					//У��ͳ�Ƹ��� 
	{
		for(j = 0 ; j < top_num ; j++)
		{
			tongji_num[i][j] = tongji_num[i][j] + xz_num[i][j]/(1000*xsh) ;
		}
	}
	
	for( i = 0 ; i < top_num ; i++ )					//У��ͳ�Ƹ��� 
	{
		for(j = 0 ; j < top_num ; j++)
		{
			clear_tongji_num[i][j] = clear_tongji_num[i][j] + xz_ql_num[i][j]/(1000*xsh) ;
		}
	}
	
	fprintf(bordaxu,"Aft-correction-Statistics��");
	for( i = 0 ; i < top_num ; i++ )					//У��ͳ�Ƹ��� 
	{
		for(j = 0 ; j < top_num ; j++)
		{
			fprintf(bordaxu,"%8d ",xz_num[i][j]);
		}
	}
	fprintf(bordaxu,"\n");
	
	fprintf(bordaxu,"Cl0-correction-Statistics��");
	for( i = 0 ; i < top_num ; i++ )					//У��ͳ�Ƹ��� 
	{
		for(j = 0 ; j < top_num ; j++)
		{
			fprintf(bordaxu,"%8d ",xz_ql_num[i][j]);
		}
	}
	fprintf(bordaxu,"\n");
	
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
			borda_xg_clear[i].borda_goal = borda_xg_clear[i].borda_goal + (xz_ql_num[i][j] * (top_num - j)) ;
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
 	sort(borda_xg_clear , borda_xg_clear + top_num , cmp ) ;
 }

//==========================================================================
//�̶����������������� 

void budong()
{
	for(int i = 0 ; i < top_num ; i ++)
	{
		xu_budong[borda_xg[i].borda_xu] = i ;			// ����ÿ����ζ��Ӧ������
		xu_budong_clear[borda_xg_clear[i].borda_xu] = i ;
	}
}

//==========================================================================
//�ϵ¶��������
 
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
//MAE��Mean Absolute Error��ƽ�����Բ�ֵ

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
//������򼰵÷֣�ͬʱ���ƽ���ϵ¶����� 

void Print()
{
    for(int i = 0 ; i < top_num ; i ++)
    {
        fprintf(bordaxu ,"Item��%d   Goal��%d                    ",borda_xg[i].borda_xu,borda_xg[i].borda_goal);
        fprintf(bordaxu ,"Item��%d   Goal��%d\n",borda_xg_clear[i].borda_xu,borda_xg_clear[i].borda_goal);
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
	
	fprintf(bordaxu,"\nYS_TJ_GS    ��             ");
	for( i = 0 ; i < top_num ; i++ )					//ԭʼͳ�Ƹ��� 
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
//�ر����д򿪵��ļ� 

void close_file()
{
	fclose(ss) ;
	fclose(bordaxu) ;
}

//===========================================================================
//������ 

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
