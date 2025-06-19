#include "Weighted-Init.hpp"
#include "set.hpp"

int main(int argc, char *argv[]){

    if(argc<3){
        printf("./filename inputfile V(U)\n");
        exit;
    }

    FILE* sol_filename  = fopen("Var_bigdata_sol-uniformavg-all2-1030.txt","a");
    FILE* res_filename  = fopen("Var_bigdata_res-uniformavg-all2-1030.txt","a");
    char line[1000];

    double current_time;
    double limit_time = 3600;
   // while(fgets(line,1000,namelist)){

        int len = strlen(line);
       // line[len-1] = '\0';
        //printf("%s\n",line);
       // fprintf(temp,"%s--------------------------------------------\n",line);

        char filename[1000];
        strcpy(filename,argv[1]);
        //strcpy(filename,"Problem.dat_1000_10000_3");

        seed = time(0);
        srand(seed);

        bool isBuild;
        if(strcmp(argv[2],"U")==0){
             printf("will build");
             isBuild = build_instance_uniform(filename);
        }
        if(strcmp(argv[2],"V")==0){
            isBuild = build_instance_Var(filename);
        }
        if(!isBuild){
            printf("%s Build instances fail\n",filename);
            fprintf(res_filename,"%s Build instances fail\n",filename);
            //continue;
        }
        best_value = vertex_num;

        start = clock();

        for(int step = 0; step < 10; step++){
            init();
            init_best();
            localsearch_init(5000);

            finish = clock();
            current_time = (double)((finish-start)/CLOCKS_PER_SEC);
           //printf("step<10 current_time= %lf\n", current_time);
            if(current_time > 900) break;
            //printf("56 current_time= %d", current_time);
        }

        int best_value_last = best_value;
        int Impro = 0;
        int ls_step = 6000;

        for(int step = 0; step < 6000; step++){

            init();
            init_sol();

            int neighbor;
            for(int ver = 0; ver < vertex_num; ver++){
                cs[ver] = bs[ver];
                cs[ver].score  = 0;
            }

	    for(int ver = 0; ver < vertex_num; ver++){

               if(cs[ver].is_in_c == 0){
                    cs[ver].score = 0;
                }else {
                    for(int i = 0; i < cs[ver].cap; i++){
                        neighbor = cs[ver].cover_neighbour[i];
                        if(cs[neighbor].num_in_c == 1){
                            cs[neighbor].score--;
                        }
                    }
                }

	    }
            finish = clock();
            current_time = (double)((finish-start)/CLOCKS_PER_SEC);
            //printf("step<6000 current_time= %lf\n", current_time);
            if(current_time > 1800) break;
            //localsearch_simple(ls_step);
            localsearch(ls_step);
           
            /*if(best_value_last > best_value){
                Impro = 0;
                best_value_last = best_value;
                ls_step = 6000;
            }else{
                if(ls_step > 500){
                    ls_step = ls_step - 500;
                }
                best_value_last = best_value;
            }*/
            
        }
        finish = clock();
        current_time = (double)((finish-start)/CLOCKS_PER_SEC);
        printf("current_time=%lf\n", current_time);

        bool *fflag=new bool[vertex_num];
        for(int i=0;i<vertex_num;i++){
            fflag[i]=false;
        }
        for(int i=0;i<vertex_num;i++){
            if(bs[i].is_in_c!=1) continue;
            fflag[i]=true;
            for(int j=0;j<bs[i].cap-bs[i].rcap;j++){
                int temp=bs[i].cover_neighbour[j];
                fflag[temp]=true;
            }
        }

        for(int i=0;i<vertex_num;i++){
            if(!fflag[i]){
                printf("answer is wrong!\n");
                fprintf(res_filename,"%s answer is wrong\n",filename);
                exit(0);
            }
        }

        finish = clock();
        current_time = (double)((finish-start)/CLOCKS_PER_SEC);

        fprintf(res_filename,"%30s: %15d time:%20.6lf finishtime: %20.6lf seed: %20d \n",filename,best_value,best_time,current_time,seed);

        fprintf(sol_filename,"%30s: %15d time:%20.6lf finishtime: %20.6lf seed: %20d \n",filename,best_value,best_time,current_time,seed);

        for(int i = 0; i < vertex_num; i++){
            if(bs[i].is_in_c!=1) continue;
            fprintf(sol_filename,"%d: ",i);

            for(int j=0;j<bs[i].cap-bs[i].rcap;j++){
                int temp=bs[i].cover_neighbour[j];
                fprintf(sol_filename," %d",temp);
            }
            fprintf(sol_filename,"\n");
        }

        fflush(res_filename);
        fflush(sol_filename);

        delete [] fflag;
        free_all();
        //printf(" %s bestvalue = %d seed= %d ------------\n",filename, best_value,seed);
    //}
    fclose(sol_filename);
    fclose(res_filename);



    //printf(" %s bestvalue = %d seed= %d ------------\n",filename, best_value,seed);
   // fprintf(fp," %s bestvalue = %d seed= %d ------------\n",filename, best_value,seed);

    /*for(int step = 0; step < 1000; step++){

        init();
        init_best();
        int ls_step = 5000;
        localsearch(ls_step);

        printf("step=%d %d\n",step,best_value);
    }*/
  //  printf("Second %s bestvalue = %d seed= %d ------------\n",filename, best_value,seed);


    /*int ResFinal = 0;
    for(int ver = 0; ver < vertex_num; ver++){
        if(bs[ver].is_in_c == 1){
            printf("%d ",ver);
            fprintf(fp, "%d ", ver);
            ResFinal++;
        }
    }

    printf("\n");
    fprintf(fp,"\n");*/

    //printf("ResFinal %d \n",ResFinal);
   // fprintf("ResFinal %d\n",ResFinal);



    return 0;
}


void init(){

    uncover_num = vertex_num;
    for(int ver = 0; ver < vertex_num; ver++){
        cs[ver] = temp_cs[ver];
        vertex_weight[ver]=1;
    }
    tabu_list.clear();
}

void init_sol(){

    int     addv = -1;
    int     maxCap = 0;
    int     cap;
    int     maxDegree = 0;
    int     fitness = 0;

    while(1){

        maxCap = 0;
        addv = -1;

        for(int ver = 0; ver < vertex_num; ver++){

            if( cs[ver].is_in_c !=1 ){

                if(cs[ver].cap==0 &&cs[ver].num_in_c==0){
                    addv = ver;
                }

                if( cs[ver].cu > cs[ver].cap ){
                    cap = cs[ver].cap;
                }else{
                    cap = cs[ver].cu;
                }

                if( maxCap < cap){
                    maxCap = cap;
                    addv = ver;
                }else if( maxCap == cap){
                    if( cs[ver].score < cs[addv].score){
                        maxCap = cap;
                        addv = ver;
                    }
                }
            }
        }

        if( maxCap == 0 && addv == -1) break;

        cs[addv].is_in_c = 1;
        cs[addv].num_in_c++;

        if(cs[addv].num_in_c == 1){
            uncover_num--;
            cs[addv].cu--;
        }

        AddVertexToCurrentSol(addv);
        fitness++;
    }

    if(fitness < best_value){
        best_value = fitness;
        for(int ver = 0; ver < vertex_num; ver++){
            bs[ver]=cs[ver];
        }
        finish = clock();
        best_time = (double)((finish-start)/CLOCKS_PER_SEC);
    }
}

void AddVertexToCurrentSol(int addv){

    int         i,neighbor;
    int         index;
    nodeSet     wNodes,gbNodes;

    if(cs[addv].rcap == 0){
        return ;
    }

    initNodeSet(&wNodes);
    initNodeSet(&gbNodes);

    for( i = 0; i < vertex_neighbourNum[addv]; i++){
        neighbor = vertex[addv][i];

        if(cs[addv].num_in_c == 1){
            cs[neighbor].cu--;
        }

        if(cs[neighbor].num_in_c == 0){
            addNodetoNodeSet(neighbor,&wNodes);
        }else{
            addNodetoNodeSet(neighbor,&gbNodes);
        }
    }

    int cnt = 0;
    int mindegree;
    int temp_neighbor;
    while(wNodes.size > 0){
        /*index  = 0;
        neighbor = wNodes.set[index];
        mindegree = vertex_neighbourNum[neighbor];

        for(int i = 1; i < wNodes.size; i++){
            temp_neighbor = wNodes.set[i];
            if(mindegree > vertex_neighbourNum[temp_neighbor]){
                mindegree = vertex_neighbourNum[temp_neighbor];
                index = i;
                neighbor = temp_neighbor;
            }
        }*/

        index = rand()%wNodes.size;
        neighbor = wNodes.set[index];

        uncover_num--;

        cs[neighbor].num_in_c++;
        delNodefromNodeSet(neighbor, &wNodes);
        cs[addv].rcap--;
        cs[addv].cover_neighbour[cnt++] = neighbor;

        if(cs[neighbor].num_in_c == 1){
            cs[neighbor].cu--;
        }

        for( i = 0; i < vertex_neighbourNum[neighbor]; i++){
            int neighbor_neighbor = vertex[neighbor][i];
            cs[neighbor_neighbor].cu--;
        }

        if(cs[addv].rcap == 0){
            break;
        }

    }

    if(cs[addv].rcap == 0) return;

    while(cs[addv].rcap != 0 && gbNodes.size != 0){
        index = rand() % gbNodes.size;
        neighbor = gbNodes.set[index];
        cs[addv].cover_neighbour[cnt++] = neighbor;
        delNodefromNodeSet(neighbor,&gbNodes);
        cs[neighbor].num_in_c++;
        cs[addv].rcap--;
    }

}

void localsearch(int maxStep){

    step = 1;
    int i,j,k,h,l;
    int best_in_c;
    int maxc;

    int numnum=0;
    int num2=0,Noimpro=0;
    bool fflag=true;
	int tabu=0;

	for(int ii = 0; ii < vertex_num; ii++){
        TabuTenue[ii] = 0;
	}

    while(step<=maxStep){

        for(int ii = 0; ii < vertex_num; ii++){
            if(TabuTenue[ii] >  0) TabuTenue[ii]--;
        }

        if(uncover_num==0){
            update_best_sol();
            maxc = find_best_in_c(0);
            removes(maxc);
           // TabuTenue[maxc] = 0.05 * vertex_num;
            //Noimpro = 0;
            /*
            for(int ii=0;ii<vertex_num;ii++){
				if(TabuTenue[ii]>0) TabuTenue[ii]--;
			}
            TabuTenue[maxc]=(int)(0.1*vertex_num);
			Noimpro=0;
			*/
			//printf("update \n");++
            continue;
        }else Noimpro++;

        best_in_c=find_best_in_c(1);
        if(state==0) best_in_c=find_best_in_c(0);
	    if( step%10 == 0 )tabu_list.clear();

		if(Noimpro>30){

              //update_redundancy_version2();
			  Noimpro=0;
			  int allowTabu=1;
			  int * aa=new int[vertex_num];
			  int in=0;
			  for(i=0;i<vertex_num;i++){
				if(cs[i].is_in_c!=1) continue;
				//if(allowTabu&&in_tabu(i)) continue;
				aa[in++]=i;
			  }
			  if(in!=0){
				int r=rand()%in;
				best_in_c=aa[r];
			  }
			  free(aa);
		}


        removes(best_in_c);
        cs[best_in_c].time_stamp=step;

        //TabuTenue[best_in_c] = 0.05 * vertex_num;
        //printf("step=1\n");
		/*
            TabuTenue[best_in_c]=(int)(0.1*vertex_num);
            for(int ii=0;ii<vertex_num;ii++){
				if(TabuTenue[ii]>0) TabuTenue[ii]--;
			}
			TabuTenue[best_in_c]=(int)(0.1*vertex_num);
        */
        int ct=0;
        maxc=0;
        int sr;
        double temp,temp1;
        bool isf=true;
        for(j=0;j<vertex_num;j++){
            if(cs[j].is_in_c==1) continue;
            if(TabuTenue[j]>0) continue;
            //if(cs[j].config==0) continue;
           // if(cs[j].score<=0) continue;
           // k=compare(ct,sr,cs[j].cap,cs[j].score);

            //temp=alpha*cs[j].score+(1-alpha)*cs[j].cap;
            //temp1=alpha*sr+(1-alpha)*cs[j].score;
            // k=compare(sr,ct,temp,cs[j].config);

            //int tt1=sr*ct,tt2=cs[j].score*cs[j].cap;
            k=compare(sr,ct,cs[j].score,cs[j].cap);
            //  k=compare(temp1,0,temp,0);
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
        //printf("step=2 maxc=%d\n",);
        add(maxc);
        //TabuTenue[maxc] = 0.05*vertex_num;
        tabu_list.insert(maxc);
        cs[maxc].time_stamp=step;
        uncov_r_weight_inc();

 	update_redundancy();

        step++;

            finish = clock();
            double current_time = (double)((finish-start)/CLOCKS_PER_SEC);
            if(current_time > 1800) break;
    }


}


void localsearch_init(int maxStep){

    step = 1;
    int i,j,k,h,l;
    int best_in_c;
    int maxc;

    int numnum=0;
    int num2=0,Noimpro=0;
    bool fflag=true;
	int tabu=0;

    while(step<=maxStep){

        if(uncover_num==0){
            update_best_sol();
            maxc = find_best_in_c(0);
            removes(maxc);
           // TabuTenue[maxc] = 0.05 * vertex_num;
            //Noimpro = 0;
            /*
            for(int ii=0;ii<vertex_num;ii++){
				if(TabuTenue[ii]>0) TabuTenue[ii]--;
			}
            TabuTenue[maxc]=(int)(0.1*vertex_num);
			Noimpro=0;
			*/
			//printf("update \n");++
            continue;
        }else Noimpro++;

        best_in_c=find_best_in_c(1);
        if(state==0) best_in_c=find_best_in_c(0);
	    if( step%10 == 0 )tabu_list.clear();

		if(Noimpro>100){

              //update_redundancy_version2();
			  Noimpro=0;
			  int allowTabu=1;
			  int * aa=new int[vertex_num];
			  int in=0;
			  for(i=0;i<vertex_num;i++){
				if(cs[i].is_in_c!=1) continue;
				if(allowTabu&&in_tabu(i)) continue;
				aa[in++]=i;
			  }
			  if(in!=0){
				int r=rand()%in;
				best_in_c=aa[r];
			  }
			  free(aa);
		}


        removes(best_in_c);
        cs[best_in_c].time_stamp=step;

        //TabuTenue[best_in_c] = 0.05 * vertex_num;
        //printf("step=1\n");
		/*
            TabuTenue[best_in_c]=(int)(0.1*vertex_num);
            for(int ii=0;ii<vertex_num;ii++){
				if(TabuTenue[ii]>0) TabuTenue[ii]--;
			}
			TabuTenue[best_in_c]=(int)(0.1*vertex_num);
        */
        int ct=0;
        maxc=0;
        int sr;
        double temp,temp1;
        bool isf=true;
        for(j=0;j<vertex_num;j++){
            if(cs[j].is_in_c==1) continue;
            if(TabuTenue[j]>0) continue;
            //if(cs[j].config==0) continue;
           // if(cs[j].score<=0) continue;
           // k=compare(ct,sr,cs[j].cap,cs[j].score);

            //temp=alpha*cs[j].score+(1-alpha)*cs[j].cap;
            //temp1=alpha*sr+(1-alpha)*cs[j].score;
            // k=compare(sr,ct,temp,cs[j].config);

            //int tt1=sr*ct,tt2=cs[j].score*cs[j].cap;
            k=compare(sr,ct,cs[j].score,cs[j].cap);
            //  k=compare(temp1,0,temp,0);
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
        //printf("step=2 maxc=%d\n",);
        add(maxc);
        //TabuTenue[maxc] = 0.05*vertex_num;
        tabu_list.insert(maxc);
        cs[maxc].time_stamp=step;
        uncov_r_weight_inc_init();

 		update_redundancy();

        step++;

            finish = clock();
            double current_time = (double)((finish-start)/CLOCKS_PER_SEC);
            if(current_time > 900) break;
    }
}
