#include "genetic_train.cpp"
#include "glukose_pred.cpp"
#include "printing.cpp"
#include <stdalloc.h>
#include <stdfile.h>
#include <stdstring.h>

#define CHROMO_NUM 100
#define MAX_GEN 1000
#define MIN_COST 1e-6
#define CHROMO_KEEP 5
#define CHROMO_BREED 50

constexpr unsigned RAND_MAX = (unsigned)(-1);
constexpr float rand_min = 0, rand_max=1;
constexpr float rand_range = rand_max - rand_min;
constexpr float rand_perc(float perc)
{
    return rand_min + perc * rand_range;
}

int trng_fd = -1;
float randfloat(void)
{
    unsigned rand;
    read(trng_fd, reinterpret_cast<char *>(&rand), sizeof(unsigned));
    return rand_min + (rand/(float)RAND_MAX) * (rand_max - rand_min);
}
extern int uart_fd;

extern int t_delta, t_pred, t_offset;
extern float lambda;
char bfr[100];
int gen=0;

/**
 * @brief Computation step
 * Recieve user input, update the population, and return the prediction and parameters with the lowest cost
 * user input: glucose level
 * periodically poll the user for input -> if "stop" is entered, stop computation
 * 
 * @return int 0 if computation is stopped, 1 otherwise 
 */
int comp_step(chromo *mem, float *costs, params *p, float *result)
{
    float datum;
    printf("input glucose level");
    sscanf(bfr,"%lf", &datum);
    if(!add_data(datum))
        return 0;
    // update the population
    popul_init(mem, CHROMO_NUM);
    do{
        popul_eval(mem, costs, CHROMO_NUM);
        popul_select(mem, costs, CHROMO_NUM, CHROMO_KEEP, CHROMO_BREED);
        popul_mutate(mem, CHROMO_NUM, rand_perc(0.1));
        gen++;

        scanf("%s", bfr);
        if(strcmp(bfr, "stop") == 0)
            return 0;
    }while(!popul_stop(mem, costs, CHROMO_NUM,gen,MAX_GEN,MIN_COST)); // stop when the population is converged

    // get the best prediction
    *p = mem[popul_best(mem, costs, CHROMO_NUM)];
    *result = pred(t_curr, p);
    return 1;
}

int main()
{
    chromo *mem = (chromo *)malloc(sizeof(chromo)*CHROMO_NUM);
    float *costs = (float *)malloc(sizeof(float)*CHROMO_NUM);
    params p; float result;

    printf("input t_delta and t_pred");
    scanf("%d %d", &t_delta, &t_pred);
    if(t_delta < 1 || t_pred < 1 || t_pred % t_delta != 0)
    {
        printf("invalid input\n");
        return 1;
    }
    t_offset = t_pred/t_delta;
    printf("input lambda");
    scanf("%lf", &lambda);


    for (int i=0; i<GLUCOSE_N_DATA; ++i)
    {
        while(1)
        {
            bscanf(bfr);
            if(strncmp(bfr, "parameters", 10) == 0)
            { // print parameters
                print_params(&p);
            }
            else break;
        }
        if(!comp_step(mem, costs, &p, &result))
            continue;

        printf("prediction: %lf\n", result);
    }
}