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

const int voter_max = 5000 ;					//����û��� 									����������				
const int pai_num = 2 ;				    		//5��ȫ���и���  5!=120; 6!=720; 7!=5040; 8!=40320; 9!=362880; 10!=3628800	����������
const int top_num = 2 ;							//ǰ���������									����������
const int jingdu = 1000 ;						//���ȣ�100Ϊ2λ��1000Ϊ3λ���Դ����� 			����������
const double epsilon = 1.0 ;					// ��											����������
const double xsh = 1.000000 ;					//С���� 

int a ;
int x , y ;										//�Ƚ� 
int sample_match[top_num] ;						//����ƥ�� 
int final_match[top_num] ;						//ƥ���� 
int final_data[pai_num] ;						//�ռ��ߵõ�������	
int jgl_num[pai_num] ;							//�Ӹ����Ժ�δУ��5000������120�����е�ͳ�Ƹ��� 
int xz_num[pai_num] ;							//У������У���������
int xz_ql_num[pai_num] ;
int borda_count[top_num] ;						//��������� 
int xu_budong[top_num] ;						//�����ﲻ������ 
int xu_budong_clear[top_num] ;
int ystj_num[pai_num] ;							//ԭʼͳ�Ƹ��� 
int ystj_zh_num[top_num][top_num] ; 			//ԭʼ�����ϳ�N����ͳ�Ƹ��� 
int xz_zh_num[top_num][top_num] ;
int xz_zh_clear_num[top_num][top_num] ;

int i = 0, j = 0 ;
int bi ;
int pi ;
int xh = 0 ;
int xscs = 0 ; 
				 
double p ;										//����
double a_kendall ;								//ƽ���ϵ¶����� 
double clear_a_kendall ;	
double max_a_kendall = 0 ;						//��� 
double min_a_kendall = 5 ;						//��С 
double all_a_kendall = 0 ;						//���ѭ����ƽ���ϵ¶��ܺ� 
double all_average_kendall = 0 ;				//���ѭ�����ܺ�ƽ�� 
double tongji_num[pai_num] ;					//ͳ��ѭ�����У������ 
double clear_max_a_kendall = 0 ;				//��� 
double clear_min_a_kendall = 5 ;				//��С 
double clear_all_a_kendall = 0 ;				//���ѭ����ƽ���ϵ¶��ܺ� 
double clear_all_average_kendall = 0 ;			//���ѭ�����ܺ�ƽ�� 
double clear_tongji_num[pai_num] ;				//ͳ��ѭ�����У������ 
double MAE_xz = 0;  							//У�����MAE
double MAE_xz_clear = 0;						//У���������MAE

FILE*ar;										//"all rank of 5.txt","r"
FILE*ss;										//"sushi3a.5000.5-result.txt","r"
FILE*bordaxu;									//��������Ĳ����� 

//===========================================================================
//�����ṹ�壺����������ṹ�壬ȫ���нṹ�壬�û���ʵbitλ�ṹ�� 

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
//��������ʼ�� 

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
		v_item[i].v_bit = 0 ;					//V��120����ÿһλ�ĳ�ʼ�� 
		jgl_num[i] = 0 ; 	 					//�Ӹ����Ժ�5000������120�����е�ͳ�Ƹ�����ʼ�� 
		final_data[i] = 0 ;
		ystj_num[i] = 0 ; 
	}
	i=0;
	
	for(i = 0 ; i < top_num ; i++)				//���ϳ�ʼ�� 
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
	pi = p * jingdu ;							//pi = ( 1 - p / 2 ) * 100 ;���ߴ���֤ 
	
	for(i = 0 ; i < top_num ; i ++)				//borda���б�ų�ʼ�� 
    {
        borda_xg[i].borda_xu = i ; 					//borda���б�ų�ʼ�� 
		borda_xg[i].borda_goal = 0 ;	
		borda_xg_clear[i].borda_xu = i ; 					//borda���б�ų�ʼ�� 
		borda_xg_clear[i].borda_goal = 0 ; 		
    }
}

//============================================================================
//�û�������ȫ������Ŀƥ�䣬�����������Ӧ���� 

void match()
{	
	int c = 0;									 //�û���� 
	while (fscanf(ss , "%d" , &a ) != EOF)
	{
		sample_match[0] = a ;					 //���û�ÿһ���û������������ 
		for( i = 1 ; i < top_num ; i++)
		{
			fscanf(ss , "%d" , &a );
			sample_match[i] = a ;
		}
		
		for(i = 0 ; i < top_num ; i++)			//����ͳ�� 
		{
			ystj_zh_num[sample_match[i]][i] = ystj_zh_num[sample_match[i]][i] + 1 ;
		} 
		
		/*for(i = 0 ; i < top_num ; i++)				//���ϳ�ʼ�� 
		{
			for(j = 0 ; j < top_num ; j++)
			{
				printf("%d ",ystj_zh_num[i][j]) ;
			}
		}
		printf("\n");*/

		for(i = 0 ; i < pai_num ; i++ )			 //����ƥ�� 
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
				user_num[c].user_bit[i] = 1 ;	  //ƥ������Ժ󣬵õ�ƥ�����ţ������û��ĸ������Ŀ����Ϊ1 
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
		//RR�����Ӧ���� // rand() ����һ��int���͵������.

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
		//�Ӹ���δУ��ͳ�Ƹ��� 
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
	
	fprintf(bordaxu,"��%d��\n",++xscs);
	
	fprintf(bordaxu,"Bef-correction-Statistics��");
	for( j = 0 ; j < pai_num ; j++ )			  //�Ӹ���δУ��ͳ�Ƹ������ 
	{
		fprintf(bordaxu,"%6d ",jgl_num[j]);
	}
	fprintf(bordaxu,"\n");
}

//==========================================================================
//У����ÿһ�е�ͳ�Ƹ���
 
void XiaoZheng()
{
	for(j = 0 ; j < pai_num ; j++ )
	{
		int jgl_num_ls ;
		jgl_num_ls = jgl_num[j] ;
		xz_num[j] =   (((jgl_num_ls / (voter_max * xsh)) + p - 1) / (2 * p -1)) * voter_max;
		xz_ql_num[j] = xz_num[j] ;
		if(xz_ql_num[j]<0)				  			 //�п��ܳ��� xz_num[j]�Ǹ����Ŀ����ԣ����ǲ����õ����Խ�������Ϊ0 
		{
			xz_ql_num[j] = 0 ;
		}
	}
	
	for( i = 0 ; i < pai_num ; i++ )					//У��ͳ�Ƹ��� 
	{
		tongji_num[i] = tongji_num[i] + xz_num[i]/(1000*xsh) ;
	}
	
	for( i = 0 ; i < pai_num ; i++ )					//У��ͳ�Ƹ��� 
	{
		clear_tongji_num[i] = clear_tongji_num[i] + xz_ql_num[i]/(1000*xsh) ;
	}
	
	
	fprintf(bordaxu,"Aft-correction-Statistics��");
	for( i = 0 ; i < pai_num ; i++ )					//У��ͳ�Ƹ��� 
	{
		fprintf(bordaxu,"%6d ",xz_num[i]);
	}
	fprintf(bordaxu,"\n");
	
	fprintf(bordaxu,"Cl0-correction-Statistics��");
	for( i = 0 ; i < pai_num ; i++ )					//У��ͳ�Ƹ��� 
	{
		fprintf(bordaxu,"%6d ",xz_ql_num[i]);
	}
	fprintf(bordaxu,"\n");
}

//==========================================================================
//��������� 

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
//�Ƚϣ����յ÷��ɴ�С��˳��������÷���ͬ���С������� 

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
//����
 
 void PaiXu()
 {
 	sort(borda_xg , borda_xg + top_num , cmp ) ;
 	sort(borda_xg_clear , borda_xg_clear + top_num , cmp ) ;
 }
 
//==========================================================================
//�̶��������������Ϊ�˼���ϵ¶�����
 
void budong()
{
	for(int i = 0 ; i < top_num ; i ++)
	{
		xu_budong[borda_xg[i].borda_xu]=i;// ����ÿ����ζ��Ӧ������
		xu_budong_clear[borda_xg_clear[i].borda_xu]=i;
	}
}

//==========================================================================
//ƽ���ϵ¶�������� 

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
//MAE��Mean Absolute Error��ƽ�����Բ�ֵxz_zh_clear_num
 
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
//���
 
void Print()
{
    for(int i = 0 ; i < top_num ; i ++)
    {
        fprintf(bordaxu ,"item��%d   goal��%d               ",borda_xg[i].borda_xu,borda_xg[i].borda_goal);
        fprintf(bordaxu ,"Item��%d   Goal��%d\n",borda_xg_clear[i].borda_xu,borda_xg_clear[i].borda_goal);
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
	
	fprintf(bordaxu,"\nYS_TJ_GS    ��             ");
	for( i = 0 ; i < pai_num ; i++ )					//ԭʼͳ�Ƹ��� 
	{
		fprintf(bordaxu,"%8d ",ystj_num[i]);
	}
	fprintf(bordaxu,"\n");
	
	//=============================================================================================== 
	
	fprintf(bordaxu ,"average_tongji_zh =") ;
	for( i = 0 ; i < pai_num ; i++ )				//У������xz_zh_clear_num								
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
	for( i = 0 ; i < pai_num ; i++ )				//У������xz_zh_clear_num								
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
	
	fprintf(bordaxu ,"\nYS_TJ_GS_zh    ��             ") ;
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
//�ر����д򿪵��ĵ�
 
void close_file()
{
	fclose(ar) ;
	fclose(ss) ;
	fclose(bordaxu) ;
}

//===========================================================================
//������ 

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
