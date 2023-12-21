#include "genetic_train.cpp"
//#include "glukose_pred.cpp" -- included in genetic_train.cpp
#include "printing.cpp"
#include <stdalloc.h>
#include <stdfile.h>
#include <stdstring.h>
#define ever (;;)
#define DEBUG 0

#define MAX_GEN 150
#define MIN_COST 1e-4f
constexpr uint32_t CHROMO_NUM = 2000;//mem::PageSize / sizeof(chromo);
constexpr uint32_t CHROMO_KEEP = CHROMO_NUM/20;

extern int t_delta, t_pred, t_offset;
extern int t_curr;
#if GLPRED_REGULATION
    extern float lambda;
#endif
extern float *Y;
unsigned mutate_rate;
float mutate_magnitude;
int gen=0;
char bfr[100];

extern int uart_fd;
bool get_init_values()
{
    printf("input t_delta");
    get_numeric_input(bfr,100);
    t_delta = atoi(bfr);
    if(t_delta <= 0)
        return false;
    write(uart_fd, "t_delta=", 8);
    printint(t_delta);

    printf("input t_pred");
    get_numeric_input(bfr,100);
    t_pred = atoi(bfr);
    if(t_pred <= 0)
        return false;
    write(uart_fd, "t_pred=", 7);
    printint(t_pred);

    if (t_pred % t_delta != 0)
        return false;

    #if GLPRED_REGULATION
        printf("input lambda (parameter regulation term)");
        get_numeric_input(bfr,100);
        lambda = atof(bfr);
        if(lambda < 0.f)
            return false;
        write(uart_fd, "lambda=", 7);
        printfloat(lambda);
    #endif

    printf("input mutation rate");
    get_numeric_input(bfr,100);
    mutate_magnitude = atof(bfr); // pre-cycle unused float
    if(mutate_magnitude < 0.f || mutate_magnitude > 1.f)
        return false;
    write(uart_fd, "mutation rate=", 14);
    printfloat(mutate_magnitude);
    mutate_rate = (unsigned)(mutate_magnitude * 0xffffffff);

    printf("input mutation magnitude");
    get_numeric_input(bfr,100);
    mutate_magnitude = atof(bfr);
    if(mutate_magnitude < 0.f)
        return false;
    write(uart_fd, "mutation magnitude=", 19);
    printfloat(mutate_magnitude);

    return true;
}

/**
 * @brief Computation step
 * Recieve user input, update the population, and return the prediction and parameters with the lowest cost
 * user input: glucose level
 * periodically poll the user for input -> if "stop" is entered, stop computation
 * 
 * @return int 0 if computation is stopped, 1 otherwise 
 */
bool comp_step(chromo *mem, float *costs, params *p, float *result)
{
    float datum = atof(bfr);

    write(uart_fd, "received: ", 10);
    printfloat(datum);

    if(!add_data(datum))
        return false;
    if(t_curr - t_offset <= 1)   // 1 because of backward difference
        return false;
    gen = 0; // reset the generation counter
    printf("Training...");
    
    // update the population
    popul_init(mem, CHROMO_NUM);
    popul_eval(mem, costs, CHROMO_NUM);
    do{
        #if DEBUG
            write(uart_fd, "generation: ", 12);
            printint(gen);
        #endif
        popul_select(mem, costs, CHROMO_NUM, CHROMO_KEEP);
        popul_mutate(mem, CHROMO_NUM, mutate_rate, mutate_magnitude);
        popul_eval(mem, costs, CHROMO_NUM);

        if(scanf(bfr) > 3) // &&
        if(strncmp(bfr, "stop", 4) == 0)
        {
            *result = pred(t_curr, *p);
            return true;
        }
    }while(!popul_stop(mem, costs, CHROMO_NUM,++gen,MAX_GEN,MIN_COST)); // stop when the population is converged

    // get the best prediction
    *p = mem[popul_best(mem, costs, CHROMO_NUM)];
    *result = pred(t_curr, *p);
    return true;
}

int main()
{
    chromo *mem = (chromo *)malloc(sizeof(chromo)*CHROMO_NUM);
    float *costs = (float *)malloc(sizeof(float)*CHROMO_NUM);
    Y = (float *)malloc(sizeof(float)*GLUCOSE_N_DATA);
    params p = {0}; float result;

    for (size_t i = 0; i < 50; i++)
    {
        Y[i] = i*10.f;
    }
    t_curr = 50;

    open_uart();
    printf("Glucose Evolution 1.0");
    printf("Author: Jakub Mlady (A23N0014P)");

    while(!get_init_values())
    {
        printf("invalid input:");
        printf(" - t_pred must be a multiple of t_delta and both must be positive");
        #if GLPRED_REGULATION
            printf(" - lambda must be positive");
        #endif
        printf(" - mutation rate must be between 0 and 1 and mutation magnitude must be positive");
    }
    t_offset = t_pred/t_delta; 

    printf("Inputs valid. Preparing computational environment...");
    // Sike! actually generating bunch of random numbers so that I don't have to wait for the TRNG SWI
    rand_init(); // takes some time, since TRNG needs to wait for entropy :'( -- but still faster than TRNG reads
    printf("Done.");

    for ever
    {
        for ever
        {
            printf("input glucose level or 'parameters' or 'inputs'");
            bscanf(bfr);
            if(strncmp(bfr, "parameters", 10) == 0)
            { // print parameters
                print_params(&p);
            }
            else if(strncmp(bfr, "inputs", 6)==0)
            {
                for(int i=0;i<t_curr;++i)
                {
                    printfloat(Y[i]);
                }
            }
            else if(!is_numeric(bfr))
            {
                printf("invalid input");
            }
            else
                break;
        }

        if(!comp_step(mem, costs, &p, &result))
        { // all data points have been used or not enough data points (t_curr < t_offset)
            printf("NaN");
            continue;
        }

        bzero(bfr, 100);
        ftoa(result, bfr, 4);
        write(uart_fd, "prediction: ", 12);
        printf(bfr);  // number ends in "ers"
    }
}