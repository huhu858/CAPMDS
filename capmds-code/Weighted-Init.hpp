#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <float.h>
#include <assert.h>
#include <memory.h>
#include <unistd.h>
#include <math.h>
#include <set>
#include <algorithm>
#include <queue>
#include <utility>
#include <time.h>
#include <limits.h>
//#include<sys/times.h>
#include "set.hpp"
#define MAXVer 560000

using namespace std;

//tms start, finish;
clock_t start,finish;

typedef struct Vertex_information{
	int score;
	int cu;
	int time_stamp;
	int config;
	int is_in_c;
	int cap;
    int rcap;
	int num_in_c;
	int cover_neighbour[151];
}Vertex_information;

double parameter_thre=5000;
double parameter_decrese=0.3;
double  alpha=0.9;

double best_time;
Vertex_information cs[MAXVer];//����
Vertex_information bs[MAXVer];
Vertex_information temp_cs[MAXVer];

int TabuTenue[MAXVer];

int retime=0;

double time_limit;
int seed;
int step;
int total_step;
int best_array[MAXVer];
int best_value;
set<int> tabu_list;

int uncover_num;
int vertex_num;//�������
int e_num;
int capacity;  //Uniform
int  **vertex;//���㼯��
int *vertex_neighbourNum;
int *best_sol;//���Ž�
int *vertex_weight;
double p_greedy;

void init();
inline int compare(int s1, int c1, int s2, int c2);
void init_best();
void add(int c);
void remove(int c);
int in_tabu(int i);
int find_best_in_c(int allowTabu);
void uncov_r_weight_inc();
void uncov_r_weight_inc_init();
void localsearch(int maxStep);
void localsearch_init(int maxStep);
int cost_C();
void update_best_sol();
int  check();
void update_redundancy();
void AddVertexToCurrentSol(int addv);
void init_sol();

void free_all(){
	free(vertex);
	free(vertex_neighbourNum);
	free(best_sol);
	free(vertex_weight);
}

bool build_instance_uniform(char *file){

    char line[1024];
    FILE *fp;
    FILE *tempfp;
    char *tempstr;
    int  temp;
    int i,j,h,t;


    fp     = fopen(file, "r");
    tempfp = fopen(file,"r");
    printf("11111");
    if(!fp){
        printf("open %s fail \n",file);
        return false;
    }

    tempstr=fgets(line, 100, fp);
    sscanf(line, "%d%d",&vertex_num,&capacity);
    e_num=0;

    char templine[1000];
    fgets(templine,100,tempfp);


    vertex = new int *[vertex_num+2];
    vertex_neighbourNum = new int[vertex_num];
    best_sol = new int[vertex_num]; //���Ž�
    vertex_weight = new int[vertex_num]; //��̬Ȩ��


    for(int i=0;i<vertex_num;i++){

        vertex_neighbourNum[i] = 0;
        vertex_weight[i] = 1;

    }

    int v1,v2;
    temp=fscanf(tempfp,"%d%d",&v1,&v2);
    while(temp>0){
        vertex_neighbourNum[v1]++;
        vertex_neighbourNum[v2]++;
        temp = fscanf(tempfp,"%d%d",&v1,&v2);
    }

    for(int i=0;i<vertex_num;i++){

        //vertex_neighbourNum[i] = 0;
        vertex_weight[i] = 1;
        vertex[i] = new int[vertex_neighbourNum[i]+2];
        vertex_neighbourNum[i] = 0;
    }

     fclose(tempfp);
     //printf("-----------------\n");


    temp=fscanf(fp,"%d%d",&v1,&v2);
    while(temp>=0){
        //printf("%d V1=%d V2=%d -----------------+++++++++++++=\n",e_num,v1,v2);
        e_num++;
        bool f1=false,f2=false;

        if(v1<0||v2<0||v1>vertex_num||v2>vertex_num){
            printf("the vertex is wrong\n");
            return false;
        }

        if(v1 == v2){
            temp=fscanf(fp,"%d%d",&v1,&v2);
            continue;
        }
        for(int ii=0;ii<vertex_neighbourNum[v1];ii++){ //��ֹ�ھ��ظ�
            if(vertex[v1][ii]==v2){
                f1=true;
                break;
            }
        }

        if(!f1){
            vertex[v1][vertex_neighbourNum[v1]]=v2;
            vertex_neighbourNum[v1]++;
        }

        for(int jj=0;jj<vertex_neighbourNum[v2];jj++){
            if(vertex[v2][jj]==v1){
                f2=true;
                break;
            }
        }

        if(!f2){
            vertex[v2][vertex_neighbourNum[v2]]=v1;
            vertex_neighbourNum[v2]++;
        }


        temp=fscanf(fp,"%d%d",&v1,&v2);
    }

    //printf("-----------------+++++++++++++=\n");
    for(int i=0; i < vertex_num; i++){

        vertex_weight[i]=1;
        temp_cs[i].config=1;
        temp_cs[i].time_stamp=1;
        temp_cs[i].is_in_c=0;
        temp_cs[i].num_in_c=0;
        temp_cs[i].score=vertex_neighbourNum[i]+1;
        temp_cs[i].cu = vertex_neighbourNum[i]+1;

        cs[i].config=1;
        cs[i].time_stamp=1;
        cs[i].is_in_c=0;
        cs[i].num_in_c=0;
        cs[i].score=vertex_neighbourNum[i]+1;
        cs[i].cu = vertex_neighbourNum[i]+1;

        if(vertex_neighbourNum[i]<capacity){
            cs[i].cap=vertex_neighbourNum[i];
            cs[i].rcap=vertex_neighbourNum[i];
        }else{
            cs[i].cap=capacity;
            cs[i].rcap=capacity;
        }

        temp_cs[i].cap=cs[i].cap;
        temp_cs[i].rcap=cs[i].rcap;

        best_sol[i]=0;
    }

    e_num=e_num/2;
    p_greedy=(vertex_num*1.0)/(e_num*1.0);

    fclose(fp);

    printf("build_instance success\n");
    return true;
}

bool build_instance_Var(char *file){

    char line[1024];
    FILE *fp;
    FILE *tempfp;
    char *tempstr;
    char templine[1000];
    int  temp;
    int i,j,h,t;
    e_num=0;


    fp = fopen(file, "r");
    tempfp = fopen(file,"r");
    if(!fp){
        printf("open %s fail \n",file);
        return false;
    }
    printf("22222");
    tempstr = fgets(line, 100, fp);
    fgets(templine,100,tempfp);

    sscanf(line, "%d",&vertex_num);



    vertex=new int *[vertex_num+2];
    vertex_neighbourNum = new int[vertex_num+2];//�����ھӸ���
    best_sol = new int[vertex_num+2];//���Ž�
    vertex_weight = new int[vertex_num+2];//��̬Ȩ��
    int *vercap=new int[vertex_num+2];


    for(int i=0;i<vertex_num;i++){
        vertex_neighbourNum[i] = 0;
        vertex_weight[i] = 1;
        //vertex[i]=new int[vertex_num];
    }

    int ver,vertexcap;

    for(int i=0;i<vertex_num;i++){
        temp=fscanf(fp,"%d%d",&ver,&vertexcap);
        vercap[ver]=vertexcap;

        fgets(templine,100,tempfp);
    }

    int v1,v2;

    temp = fscanf(tempfp,"%d%d",&v1,&v2);
    while(temp >= 0){
        vertex_neighbourNum[v1]++;
        vertex_neighbourNum[v2]++;
        temp = fscanf(tempfp,"%d%d",&v1,&v2);
    }
    fclose(tempfp);

    for(int i=0;i<vertex_num;i++){

        vertex_weight[i] = 1;
        vertex[i]=new int[vertex_neighbourNum[i]+2];
        vertex_neighbourNum[i] = 0;
    }



    temp=fscanf(fp,"%d%d",&v1,&v2);

   // printf("step=\n");

    while(temp>=0){
        e_num++;

        //if(v1)
        //printf("e_num  = %d\n",e_num);

        bool f1=false,f2=false;
        if(v1>vertex_num||v2>vertex_num||v1<0||v2<0){
            printf("The vertex is error %d %d %d\n",e_num,v1,v2);
            return false;
        }

        if(v1 == v2){
            temp=fscanf(fp,"%d%d",&v1,&v2);
            continue;
        }
        for(int ii=0;ii<vertex_neighbourNum[v1];ii++){

            if(vertex[v1][ii]==v2){
                f1=true;
                break;
            }
        }

        if(!f1){
            vertex[v1][vertex_neighbourNum[v1]]=v2;
            vertex_neighbourNum[v1]++;
        }

        for(int ii=0;ii<vertex_neighbourNum[v2];ii++){
            if(vertex[v2][ii]==v1){
                f2=true;
                break;
            }
        }

        if(!f2){
            vertex[v2][vertex_neighbourNum[v2]]=v1;
            vertex_neighbourNum[v2]++;
        }


        temp=fscanf(fp,"%d%d",&v1,&v2);
    }



    for(int i=0; i < vertex_num; i++){

        vertex_weight[i]=1;
        temp_cs[i].config=1;
        temp_cs[i].time_stamp=1;
        temp_cs[i].is_in_c=0;
        temp_cs[i].num_in_c=0;
        temp_cs[i].score=vertex_neighbourNum[i]+1;
        temp_cs[i].cu = vertex_neighbourNum[i]+1;

        cs[i].config=1;
        cs[i].time_stamp=1;
        cs[i].is_in_c=0;
        cs[i].num_in_c=0;
        cs[i].score=vertex_neighbourNum[i]+1;
        cs[i].cu = vertex_neighbourNum[i]+1;
        if(vertex_neighbourNum[i]<vercap[i]){
            cs[i].cap=vertex_neighbourNum[i];
            cs[i].rcap=vertex_neighbourNum[i];
        }else{
            cs[i].cap=vercap[i];
            cs[i].rcap=vercap[i];
        }

        temp_cs[i].cap=cs[i].cap;
        temp_cs[i].rcap=cs[i].rcap;
        best_sol[i]=0;
    }

	e_num=e_num/2;
	p_greedy=(vertex_num*1.0)/(e_num*1.0);
    fclose(fp);

    free(vercap);
    printf("build instances success\n");
    return true;
}


int in_tabu(int i){
  return tabu_list.find(i)!=tabu_list.end();
}

int state=0;

int find_best_in_c(int allowTabu){//ѡ��ɾ���Ķ���

	int i,maxc,k;
	int sr=INT_MIN, ct=0;
  	state=0;
  	bool isF=true;
  	for(i=0;i<vertex_num;i++){
    	if(cs[i].is_in_c!=1) continue;
    	if(TabuTenue[i]>0) continue;
    	//if(allowTabu&&in_tabu(i)) continue;
    	state=1;

		/*if(isF){
			isF=false;
			sr=cs[i].score;
			ct=cs[i].cap;
			maxc=i;
			continue;
		}

		if(cs[i].cap==0){
			sr=cs[i].score;
			ct=cs[i].cap;
			maxc=i;
			continue;
		}

		if(ct==0){
			continue;
		}

		double tt=(sr*1.0)/(ct*1.0),tt1=(cs[i].score*1.0/(cs[i].cap*1.0));*/
  		 // k=compare(cs[i].cap,sr, ct, cs[i].score);
    	k=compare(sr,cs[i].cap,cs[i].score,ct);//ѡ��scoreֵ��cap����С�Ķ���ɾ��
		//k=compare(tt,0,tt1,0);
		if(k<0||sr==INT_MIN){
      		sr=cs[i].score;
      		ct=cs[i].cap;
      		maxc=i;
   		} else if(k==0){
      		if(cs[maxc].time_stamp<cs[i].time_stamp){
            	maxc=i;
      		}
   		}
  	}
  return maxc;

}

void removes(int c){

  //printf("removeC=%d\n",c);
  cs[c].is_in_c=0;
  cs[c].score=-cs[c].score;
  cs[c].config=0;

  if( cs[c].num_in_c==1) uncover_num++;
  int i,j,k,t,cnt,s,h,l;

  int tempnum=0;

  for(h=0; h<vertex_neighbourNum[c]; h++){ //C���ھ�

    i=vertex[c][h];

    bool fflag2=false;
    if(cs[i].is_in_c==1){   //�ھ�i�Ƿ񸲸�c
        for(int jj=0;jj<cs[i].cap;jj++){
            if(c==cs[i].cover_neighbour[jj]){
                fflag2=true;
                break;
            }
        }
    }

	if(cs[c].num_in_c==2&&cs[i].is_in_c==1){ //����scoreֵ��
        if(fflag2) cs[i].score-=vertex_weight[c];
		else cs[i].score+=vertex_weight[c];
    }else if(cs[c].num_in_c==1&&cs[i].is_in_c!=1){
        cs[i].score+=vertex_weight[c];
	}else if(cs[c].num_in_c==1&&cs[i].is_in_c==1){
		cs[i].score-=vertex_weight[c];
	}

	cs[i].config=1;
	//cs[i].config++;//????
	for(int ii=0;ii<vertex_neighbourNum[i];ii++){

           j=vertex[i][ii];
           cs[j].config=2;
		  //cs[j].config++;
      }

    bool fflag=false;             //�ھ�i�Ƿ�C����

    for(int ii=0;ii<cs[c].cap;ii++){
        if(i==cs[c].cover_neighbour[ii]){
            fflag=true;
            break;
        }
    }



    if(cs[i].num_in_c==1&&fflag){
        uncover_num++;
		tempnum++;

        cs[i].score+=vertex_weight[i];

        for(int ii=0;ii<vertex_neighbourNum[i];ii++){

            j=vertex[i][ii];
            if(j==c) continue;
            if(cs[j].is_in_c!=1) cs[j].score+=vertex_weight[i];
			else if(cs[j].is_in_c==1) cs[j].score-=vertex_weight[i];

        }
        cs[i].num_in_c--;
    }else if(cs[i].num_in_c==2&&fflag){
        bool ss=false;
        for(int ii=0;ii<vertex_neighbourNum[i];ii++){
            j=vertex[i][ii];
            if(j==c) continue;
            if(cs[j].is_in_c==1){
                for(int ll=0;ll<cs[j].cap;ll++){
                    if(cs[j].cover_neighbour[ll]==i){
                        ss=true;break;
                    }
                }
            }
            if(ss) break;
        }

        if(ss) cs[j].score-=vertex_weight[i];
        cs[i].num_in_c--;
        continue;
    }else if(fflag){
        cs[i].num_in_c--;
    }

  }


    for(int ii=0;ii<cs[c].cap;ii++){
        cs[c].cover_neighbour[ii]=-2;
    }
    cs[c].rcap=cs[c].cap;
    cs[c].num_in_c--;

	//printf("+++++++++++++++ tempnum= %d ++++++++++++++++\n",tempnum);
}

void uncov_r_weight_inc(){

  int i,j,h;
  double sum_weight=0;
  for(i=0;i<vertex_num;i++){
    if(cs[i].num_in_c==0){
      vertex_weight[i]+=1;
      cs[i].score+=1;
      for(h=0;h<vertex_neighbourNum[i];h++){
        j=vertex[i][h];
		//cs[j].score+=1;
       if(cs[j].is_in_c!=1){
            cs[j].score+=1;
        }else cs[j].score-=1;
      }
    }

	sum_weight+=vertex_weight[i];
  }

  if(sum_weight> 3*vertex_num){
	for(i=0;i<vertex_num;i++){
	   parameter_decrese = 0.35;
       vertex_weight[i]=(int)((double) vertex_weight[i]*parameter_decrese);
    }
  }

}

void uncov_r_weight_inc_init(){

  int i,j,h;
  double sum_weight=0;
  for(i=0;i<vertex_num;i++){
    if(cs[i].num_in_c==0){
      vertex_weight[i]+=1;
      cs[i].score+=1;
      for(h=0;h<vertex_neighbourNum[i];h++){
        j=vertex[i][h];
		//cs[j].score+=1;
       if(cs[j].is_in_c!=1){
            cs[j].score+=1;
        }else cs[j].score-=1;
      }
    }

	sum_weight+=vertex_weight[i];
  }
}


int compare(int s1, int c1, int s2, int c2){

    if(s1>s2) return 1;
    if(s1<s2) return -1;

    if(s1==s2){
        if(c1>c2) return 1;
        else if(c1==c2) return 0;
        else return -1;
    }

}


void add(int c){
    //printf("addC=%d\n",c);
    int i,j,k,t,cnt,s, ii,jj, ix,h, l;
    cs[c].is_in_c=1;
    cs[c].score=-cs[c].score;
    cs[c].config=0;

    if( cs[c].num_in_c==0){uncover_num--;}

    cnt=0;
    int *neighbour_cover;
    int *neighbour_covered;
    neighbour_covered=new int[vertex_num];
    neighbour_cover=new int[vertex_num];
    int coverednum=0;

    for(h=0;h<vertex_neighbourNum[c]; h++){//C������ÿһ������h����δ���Ǽ���

        i=vertex[c][h];
	    cs[i].config=1;
		for(int ii=0;ii<vertex_neighbourNum[i];ii++){
            j=vertex[i][ii];
            cs[j].config=2;
        }

        if(cs[c].num_in_c==0&&cs[i].is_in_c!=1){
            cs[i].score-=vertex_weight[c];
		}else  if(cs[c].num_in_c==0&&cs[i].is_in_c==1){
			cs[i].score+=vertex_weight[c];
		}else if(cs[c].num_in_c==1&&cs[i].is_in_c==1){//c��i���ǣ�i�ķ�����Ȩ��

            for(int kk=0;kk<cs[i].cap;kk++){
                if(cs[i].cover_neighbour[kk]==c){
                    cs[i].score+=vertex_weight[c];
                    break;
                }
            }

        }

        if(cs[i].num_in_c>0){ neighbour_covered[coverednum++]=i;continue;}

        if(cs[i].num_in_c==0){
            neighbour_cover[cnt]=i;
            cnt++;
        }
    }

   if(cnt<=cs[c].rcap){//ÿ��δ���ǵ��ھӶ����Ը���

        for(k=0;k<cnt;k++){

            i=neighbour_cover[k];
            cs[i].num_in_c++;
            uncover_num--;
            cs[i].score-=vertex_weight[i];
            for(l=0;l<vertex_neighbourNum[i];l++){
                j=vertex[i][l];
                if(j==c) continue;
                if(cs[j].is_in_c!=1) cs[j].score-=vertex_weight[i];
				if(cs[j].is_in_c==1) cs[j].score+=vertex_weight[i];
            }
            cs[c].cover_neighbour[k]=i;
        }
        int index=k;

        cs[c].rcap-=cnt;

        for(k=0;k<cs[c].rcap;k++){ //ʣ��ĸ����Ѿ������ǹ��Ķ���
			int kk;
            while(1){
                kk=rand()%coverednum;
                if(neighbour_covered[kk]==-1) continue;
              	 else break;
            }

	           /* bool isfirst=true;
	            int bestscore,bestcap;
	            for(int ind=0;ind<coverednum;ind++){
					if(neighbour_covered[ind]==-1) continue;
					int tt=neighbour_covered[ind];
					if(isfirst){
						isfirst=false;
						bestscore=cs[tt].score;
						bestcap=cs[tt].cap;
						kk=ind;
					}else{
						int ss;
						ss=compare(bestscore,bestcap,cs[tt].score,cs[tt].cap);
						if(ss>0){
							bestscore=cs[tt].score;
							bestcap=cs[tt].cap;
							kk=ind;
						}
					}

		    	}*/

            i=neighbour_covered[kk];
            neighbour_covered[kk]=-1;

            if(cs[i].num_in_c==1){
                bool fflag=false;
                for(l=0;l<vertex_neighbourNum[i];l++){
                    j=vertex[i][l];
                    if(j==c) continue;
                    if(cs[j].is_in_c!=1) continue;

                    for(int ii=0;ii<cs[j].cap;ii++){
                        if(cs[j].cover_neighbour[ii]==i) {fflag=true;break;}
                    }
                    if(fflag) break;
                }
                if(fflag) cs[j].score+=vertex_weight[i];
            }


            cs[i].num_in_c++;


            cs[c].cover_neighbour[index++]=i;
        }

        cs[c].rcap=0;


    }else{

        for(k=0;k<cs[c].rcap;k++){ //���ѡ��δ���ǵ��ھӽ��и���
            int kk;
            int pp=rand()%1000;
			double ppp=(pp*1.0)/1000.0;
			//p_greedy=1;
			//p_greedy=(cs[c].rcap*0.1)/cnt;
			if(ppp<=p_greedy){
				while(1){
					kk=rand()%cnt;
					if(neighbour_cover[kk]==-1) continue;
					else break;
				}
			}else{

				bool isfirst=true;
				int bestscore,bestcap;
				for(int ind=0;ind<cnt;ind++){
					if(neighbour_cover[ind]==-1) continue;
					int tt=neighbour_cover[ind];
					if(isfirst){
						isfirst=false;
						bestscore=cs[tt].score;
						bestcap=cs[tt].cap;
						kk=ind;
					}else{
						int ss;
						//double temp1=0.05*bestcap+0.95*bestscore;
						//double temp2=0.05*cs[tt].cap+0.95*cs[tt].score;
						int temp1=bestcap*bestscore;
						int temp2=cs[tt].cap*cs[tt].score;
						//ss=compare(temp1,0,temp2,0);
						ss=compare(bestscore,bestcap,cs[tt].score,cs[tt].cap);
						if(ss>0){
							bestscore=cs[tt].score;
							bestcap=cs[tt].cap;
							kk=ind;
						}
					}
				}
			}





            i=neighbour_cover[kk];//ѡ���Ҫ���ǵ��ھ�i
            neighbour_cover[kk]=-1;

            //printf("i=%d ",i);
            cs[i].num_in_c++;
            uncover_num--;

            cs[i].score-=vertex_weight[i];
            for(l=0;l<vertex_neighbourNum[i];l++){
                j=vertex[i][l];
                if(j==c) continue;
                if(cs[j].is_in_c!=1) cs[j].score-=vertex_weight[i];
				if(cs[j].is_in_c==1) cs[j].score+=vertex_weight[i];
            }

            cs[c].cover_neighbour[k]=i;

        }

        cs[c].rcap=0;
    }


    //for(int i=0)



    cs[c].num_in_c++;
    free(neighbour_cover);
    free(neighbour_covered);

    /*for(int ii=0;ii<vertex_num;ii++){
        printf("ii=%d %d ",ii,cs[ii].score);
    }
    printf("\n");*/
}

int cost_C(){

    int s=0;
    for(int i=0;i<vertex_num;i++){

        if(cs[i].is_in_c==1){
            s++;
        }
    }
    return s;
}

void update_best_sol(){
  int i,j;
  int k=cost_C();
  bool flag=false;


  if(k<best_value){

	//printf("update+++++\n");

    best_value=k;
    for(j=0;j<vertex_num;j++){
        bs[j]=cs[j];
    }
    finish = clock();
    best_time = (double)((finish-start)/CLOCKS_PER_SEC);
    if(flag){
        printf("wrong\n");
        exit(0);
    }

   // times(&finish);
//	times(&finish);
//	best_time = double(finish.tms_utime - start.tms_utime + finish.tms_stime - start.tms_stime)/sysconf(_SC_CLK_TCK);
//	best_time = round(best_time * 1000)/1000.0;
  }
 // printf("****** %d ----\n",k);
}

int compare1(double sr1,double sr2){
	if(sr1<sr2) return -1;
	if(sr1>sr2) return 1;
	if(sr1==sr2) return 0;
}


void update_redundancy()
{
	int i,j,k,jj,kk,l,tmp1,tmp2;
	for(i=0; i<vertex_num; i++){
        bool flag=false;
		if(cs[i].num_in_c >= 2){
			//printf("i=%d ",i);
			//cs[i].config=1;        //i��ccֵ
			for(j=0; j<vertex_neighbourNum[i]; j++){
                flag=false;
				tmp1=vertex[i][j];
			//	cs[tmp1].config=1; //i�ھӵ�ccֵ
				if(cs[tmp1].is_in_c == 1){
					for(k=0; k<cs[tmp1].cap; k++){
						if(i == cs[tmp1].cover_neighbour[k]){   //i�Ƿ��ھ�j����
							for(kk=0; kk<vertex_neighbourNum[tmp1]; kk++){
								tmp2=vertex[tmp1][kk];
								//cs[tmp2].config=1; //i�ھӵ��ھ�ccֵ
								if(cs[tmp2].num_in_c == 0){
								    retime++;
                                    flag=true;
									cs[tmp1].cover_neighbour[k]=tmp2;
									cs[i].num_in_c--;
									cs[tmp2].num_in_c++;
									uncover_num--;

									//printf("Redundancy\n");

									//update cc value
									for(jj=0; jj<vertex_neighbourNum[tmp2]; jj++){
										cs[vertex[tmp2][jj]].config=2;
									}

									//update score
									cs[tmp1].score=cs[tmp1].score-vertex_weight[tmp2]+vertex_weight[i];
									cs[tmp2].score-=vertex_weight[tmp2];
									for(l=0;l<vertex_neighbourNum[tmp2];l++){
										j=vertex[tmp2][l];
										if(j==i) continue;
										if(cs[j].is_in_c!=1) cs[j].score-=vertex_weight[tmp2];
										if(cs[j].is_in_c==1) cs[j].score+=vertex_weight[tmp2];
									}

									if(cs[i].is_in_c==1&&cs[i].num_in_c==1){
                                        cs[i].score-=vertex_weight[i];
                                    }else if(cs[i].num_in_c == 1){
										for(l=0;l<vertex_neighbourNum[i];l++){
											j=vertex[i][l];
											if(j==tmp1) continue;
											if(cs[j].is_in_c!=1) continue;
                                            int ll;
											for( ll=0;ll<cs[j].cap;ll++){
                                                if(cs[j].cover_neighbour[ll]==i) break;
                                            }

                                            if(ll<cs[j].cap) break;
                                            //cs[j].score-=vertex_weight[i];
											//if(cs[j].is_in_c==1) cs[j].score+=vertex_weight[i];
										}
										cs[j].score-=vertex_weight[i];
									}
								    //printf("####bingo#####  %d\n",best_value);
									//if(cs[i].num_in_c == 1) break;
								}
								if(flag) break;
							}
						}
						if(flag) break;
					}
					if(cs[i].num_in_c == 1) break;
				}

			}
		}
	}

}


void init_best(){

    int cnt;
    int i,j,k,l,jj;
    int cr;
    int sr,temp;


    while(uncover_num>0){
        //printf("uncover_num = %d\n",uncover_num);
        cnt=0;
        sr=INT_MIN,cr=0;

        for(j=0;j<vertex_num;j++){
            if(cs[j].is_in_c==1) continue;
            if(cs[j].num_in_c>0&&vertex_neighbourNum[j]==0) continue;
            // best_array[cnt++]=j;
			//continue;
			if(cs[j].score<=0) continue;

            k=compare(cr,sr,cs[j].cap,cs[j].score);
            //k=compare(sr,cr,cs[j].score,cs[j].cap);
		/*	temp=alpha*cs[j].score+(1-alpha)*cs[j].cap;
			k=compare1(sr,temp);*/
			if(sr==INT_MIN||k<0){
				cr=cs[j].cap;
                sr=cs[j].score;
                best_array[0]=j;
                cnt=1;
            }else if(k==0){
                best_array[cnt++]=j;
            }

        }
        if(cnt>0){
            l=rand()%cnt;

            add(best_array[l]);
            int addv = best_array[l];

            //fprintf(midres,"uncover_num %d addv = %d Neigh=%d cap =%d  num_in_c %d best_in_c %d\n",uncover_num,best_array[l],vertex_neighbourNum[addv],cs[addv].cap,cs[addv].num_in_c,cs[addv].is_in_c);

            /*for(int ii = 0; ii < cs[addv].cap;ii++){

                    printf("cover %d num_in_c %d \n", cs[addv].cover_neighbour[ii],cs[cs[addv].cover_neighbour[ii]].num_in_c);
                //}
            }
            printf("\n");*/
        }

    }


    if(best_value > cost_C()){

	best_value = cost_C();
    	for(j=0;j<vertex_num;j++){
        	bs[j]=cs[j];
    	}
    	finish = clock();
   	 best_time = (double)((finish-start)/CLOCKS_PER_SEC);
   }
}


void localsearch_simple(int maxStep){

    step = 1;

    int rmv_num = 3;
    nodeSet inCNodes;
    initNodeSet(&inCNodes);

    for(int i = 0; i < vertex_num; i++){
        if(cs[i].is_in_c == 1){
            addNodetoNodeSet(i,&inCNodes);
        }
    }

    while(step < maxStep){
        //printf("Step = %d\n",step);

        /* remove vertices from current solution*/
        int remove_num = 0;
        while(inCNodes.size > 0 && remove_num < rmv_num){
            int index = rand()%inCNodes.size;
            int removeVertex =  inCNodes.set[index];
            delNodefromNodeSet(removeVertex, &inCNodes);
            removes(removeVertex);

            remove_num++;
        }

        while(uncover_num > 0){
            int ct=0;
            int maxc=0;
            int sr;
            double temp,temp1;
            bool isf=true;

            for(int j = 0; j < vertex_num; j++){
                if(cs[j].is_in_c == 1) continue;
                int k=compare(sr,ct,cs[j].score,cs[j].cap);
                if(isf||k<0){
                    sr=cs[j].score;
                    ct=cs[j].cap;
                    maxc=j;
                    isf=false;
                }/*else if(k==0&&cs[maxc].config==1&&cs[j].config==2){
                    maxc=j;
                }*/else if(k==0&&cs[maxc].time_stamp>=cs[j].time_stamp){
                    maxc=j;
                }
            }
            add(maxc);
            addNodetoNodeSet(maxc,&inCNodes);
        }
        //update_redundancy();
        update_best_sol();

        step++;
    }


}


