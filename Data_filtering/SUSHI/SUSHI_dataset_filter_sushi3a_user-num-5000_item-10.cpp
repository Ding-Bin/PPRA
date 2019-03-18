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
int main()
{
	const int voter_max = 5005 ;				
	const int item_max = 20 ;  				
	
	int need_wei = 1;                              //在此设置需要的位数 （可设置） 
	int voter_num ;
	int item_num ;
	int top_num ;
	int i,j;
    int a,b;								
    FILE*fp;
	fp=fopen("dataset\\input_sushi3a.5000.10.txt","r");
	FILE*p;
    p = fopen ("dataset\\output_sushi3a.5000.1-result.txt","w");	 
    while (fscanf(fp,"%d",&a) != EOF)
    {
        fscanf(fp,"%d",&b);
        for(i = 0 ; i < 10 ; i ++)
        {
            int t ;
            fscanf(fp,"%d",&t);
            if(t < need_wei)
            {
            	fprintf(p,"%d ",t);
            }
        }
        fprintf(p,"\n");
    }
    fclose(fp) ;
    fclose(p) ;
    return 0 ;
}

