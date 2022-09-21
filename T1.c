#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <unistd.h> 
#include <time.h>

#define BUDGET 100000
#define LEN_ACTIONS 10

#define UP_LIMIT_MC 0.009
#define LO_LIMIT_MC 0.002

#define UP_LIMIT_WIN_PROB 1.0
#define LO_LIMIT_WIN_PROB 0.0

#define N_THREADS 4
typedef struct{
    char id;
    float win_prob;
    int n;
    int reward;
}node;

int playout(double win_prob){
    double value = ((UP_LIMIT_MC-LO_LIMIT_MC)*rand())/RAND_MAX + LO_LIMIT_MC;
    double running_simul, limit_simul; 
    running_simul = omp_get_wtime();
    limit_simul = running_simul + value;
    
    while(running_simul < limit_simul){
        int x = 1*1000*20218;
        int y = 1881*198812;
        running_simul = omp_get_wtime();
    }



    return (rand()/(double)RAND_MAX) <= win_prob?1:0;
}

void simulate(node *n, int budget){
    //* PODE PARALELIZAR AQUI *//
    // PARALELISMO DE SIMULAÇÃO
    #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < budget; i++){
        n->reward = n->reward + playout(n->win_prob);
        n->n = n-> n + 1;
    }
}
//gcc T1.c -lm -fopenmp -o run
 int main (int argc, char *argv[]) {
   int th_id, nthreads;
    double start_run, end_run; 
    start_run = omp_get_wtime();

    omp_set_num_threads(2); 
   
    node *children = malloc(sizeof(node) * LEN_ACTIONS);
    int id = (int)'a';
    for(int i=0; i < LEN_ACTIONS;i++){

        children[i].id = (char)('a'+i);
        children[i].win_prob = ((UP_LIMIT_WIN_PROB-LO_LIMIT_WIN_PROB)*rand())/RAND_MAX + LO_LIMIT_WIN_PROB;
        children[i].n = 0;
        children[i].reward = 0;
    }

    int layer_plays = LEN_ACTIONS;
    while(layer_plays > 1){
        int layer_budget = (int)floor(BUDGET/(layer_plays*ceil(log2(LEN_ACTIONS))));
        
        //monte-carlo
        // PARALELISMO DE AÇÃO
        //* PODE PARALELIZAR AQUI *//
        //#pragma omp parallel for schedule(dynamic)
        for(int i = 0; i < layer_plays;i++){
            simulate(&children[i], layer_budget);
        }
        
        //sort
        for (int counter = 0 ; counter < layer_plays - 1; counter++)
        {
            for (int counter1 = 0 ; counter1 < layer_plays - counter - 1; counter1++)
            {
                double r1 = (float)children[counter1].reward/children[counter1].n; 
                double r2 = (float)children[counter1+1].reward/children[counter1+1].n; 
                if (r1 < r2) {
                    node swap_var     = children[counter1];
                    children[counter1]   = children[counter1+1];
                    children[counter1+1] = swap_var;
                }
            }
        }

        //halve 
        layer_plays = (int)layer_plays/2;
    }

    for(int i=0;i<LEN_ACTIONS;i++){
        printf("%c, %d, %d\n", children[i].id, children[i].n, children[i].reward);
    }


    end_run = omp_get_wtime();

	printf("Tempo de execucao: %3.2f segundos\n", end_run-start_run);
    return EXIT_SUCCESS;
 }


