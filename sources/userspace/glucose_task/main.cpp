#include "genetic_train.cpp"
//#include "glukose_pred.cpp"
#include "printing.cpp"
#include <stdalloc.h>
#include <stdfile.h>
#include <stdstring.h>
#define ever (;;)

#define MAX_GEN 1000
#define MIN_COST 1e-6
#define CHROMO_KEEP 5
constexpr uint32_t CHROMO_NUM = mem::PageSize / sizeof(chromo);
constexpr uint32_t CHROMO_BREED = CHROMO_NUM/4;

constexpr float fTRNG_MAX = (float)((unsigned)(-1));
constexpr float RAND_MIN = 0, RAND_MAX=1;
constexpr float RAND_RANGE = RAND_MAX - RAND_MIN;
constexpr float rand_perc(float perc)
{
    return RAND_MIN + perc * RAND_RANGE;
}

int trng_fd = -1;
float rand_min=RAND_MIN, rand_max=RAND_MAX;
float randfloat(float min, float max)
{
    unsigned rand;
    read(trng_fd, reinterpret_cast<char *>(&rand), sizeof(unsigned));
    return min + ((float)rand/fTRNG_MAX) * (max - min);
}

extern int t_delta, t_pred, t_offset;
extern int t_curr;
extern float lambda;
extern float DIFF_STEP;
extern float *Y;
int gen=0;
char bfr[100];

extern int uart_fd;
void get_init_values()
{
    printf("input t_delta");
    get_numeric_input(bfr,100);
    t_delta = atoi(bfr);

    printf("input t_pred");
    get_numeric_input(bfr,100);
    t_pred = atoi(bfr);

    printf("input lambda (parameter regualtion term)");
    get_numeric_input(bfr,100);
    lambda = atof(bfr);
    
    t_offset = t_pred / t_delta;
    DIFF_STEP = t_delta / 24.0 * 60.0;
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
    if(!add_data(datum))
        return false;
    if(t_curr < t_offset)
        return false;
    
    // update the population
    popul_init(mem, CHROMO_NUM);
    do{
        popul_eval(mem, costs, CHROMO_NUM);
        popul_select(mem, costs, CHROMO_NUM, CHROMO_KEEP, CHROMO_BREED);
        popul_mutate(mem, CHROMO_NUM, rand_perc(0.1));
        gen++;

        if(scanf(bfr) > 0)
        if(strncmp(bfr, "stop", 4) == 0)
        {
            *result = pred(t_curr, p);
            return true;
        }
    }while(!popul_stop(mem, costs, CHROMO_NUM,gen,MAX_GEN,MIN_COST)); // stop when the population is converged

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

    get_init_values();
    
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