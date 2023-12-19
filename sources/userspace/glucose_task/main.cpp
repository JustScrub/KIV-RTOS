#include "genetic_train.cpp"
//#include "glukose_pred.cpp" -- included in genetic_train.cpp
#include "printing.cpp"
#include <stdalloc.h>
#include <stdfile.h>
#include <stdstring.h>
#define ever (;;)
#define DEBUG 1

#define MAX_GEN 1000
#define MIN_COST 1e-6f
constexpr uint32_t CHROMO_NUM = 600;//mem::PageSize / sizeof(chromo);
constexpr uint32_t CHROMO_BREED = CHROMO_NUM/4;
constexpr uint32_t CHROMO_KEEP = CHROMO_BREED/8;

constexpr float fTRNG_MAX = (float)((unsigned)(-1));
int trng_fd = -1;
float randfloat()
{
    unsigned rand;
    read(trng_fd, reinterpret_cast<char *>(&rand), sizeof(unsigned));
    return ((float)rand/fTRNG_MAX);
}

extern int t_delta, t_pred, t_offset;
extern int t_curr;
extern float lambda;
extern float *Y;
float mutate_rate, mutate_magnitude;
int gen=0;
char bfr[100];

extern int uart_fd;
bool get_init_values()
{
    printf("input t_delta");
    get_numeric_input(bfr,100);
    t_delta = atoi(bfr);

    printf("input t_pred");
    get_numeric_input(bfr,100);
    t_pred = atoi(bfr);

    if (t_pred <= 0 || t_delta <= 0 || t_pred % t_delta != 0)
        return false;

    printf("input lambda (parameter regualtion term)");
    get_numeric_input(bfr,100);
    lambda = atof(bfr);

    if(lambda < 0.f)
        return false;

    printf("input mutation rate");
    get_numeric_input(bfr,100);
    mutate_rate = atof(bfr);

    printf("input mutation magnitude");
    get_numeric_input(bfr,100);
    mutate_magnitude = atof(bfr);

    return (mutate_rate >= 0.f && mutate_rate <= 1.f && mutate_magnitude >= .0f);
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

    #if DEBUG
        write(uart_fd, "received: ", 10);
        printfloat(datum);
    #endif

    if(!add_data(datum))
        return false;
    if(t_curr < t_offset)
        return false;
    
    // update the population
    popul_init(mem, CHROMO_NUM);
    popul_eval(mem, costs, CHROMO_NUM);
    do{
        popul_select(mem, costs, CHROMO_NUM, CHROMO_KEEP, CHROMO_BREED);
        popul_mutate(mem, CHROMO_NUM, mutate_rate, mutate_magnitude);
        popul_eval(mem, costs, CHROMO_NUM);

        if(scanf(bfr) > 3) // &&
        if(strncmp(bfr, "stop", 4) == 0)
        {
            *result = pred(t_curr, p);
            return true;
        }
    }while(!popul_stop(mem, costs, CHROMO_NUM,++gen,MAX_GEN,MIN_COST)); // stop when the population is converged

    // get the best prediction
    *p = mem[popul_best(mem, costs, CHROMO_NUM)];
    *result = pred(t_curr, p);
    return true;
}

int main()
{
    chromo *mem = (chromo *)malloc(sizeof(chromo)*CHROMO_NUM);
    float *costs = (float *)malloc(sizeof(float)*CHROMO_NUM);
    Y = (float *)malloc(sizeof(float)*GLUCOSE_N_DATA);
    params p; float result;

    trng_fd = open("DEV:trng", NFile_Open_Mode::Read_Only);
    open_uart();

    while(!get_init_values())
    {
        printf("invalid input:");
        printf(" t_pred must be a multiple of t_delta and both must be positive");
        printf(" lambda must be positive");
        printf(" mutation rate must be between 0 and 1 and mutation magnitude must be positive");
    }
    t_offset = t_pred/t_delta;

    for ever
    {
        for ever
        {
            bscanf(bfr);
            if(strncmp(bfr, "parameters", 10) == 0)
            { // print parameters
                print_params(&p);
            }
            else if(!is_numeric(bfr))
            {
                printf("invalid input");
                printf("input glucose level or 'parameters'");
            }
            else
                break;
        }

        if(!comp_step(mem, costs, &p, &result))
        { // all data points have been used or not enough data points (t_curr < t_offset)
            printf("NaN");
            continue;
        }

        ftoa(result, bfr, 4);
        write(uart_fd, "prediction: ", 12);
        printf(bfr);
    }
}