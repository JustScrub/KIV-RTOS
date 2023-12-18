#pragma once
constexpr int DIFF_STEP =  (24.0*60.0*60.0);
//#define DIFF_STEP (t_delta)  ?
constexpr unsigned GLUCOSE_N_DATA = 1000;


typedef struct {
    float A, B, C ,D ,E;
} params;
// training and regularization parameters
extern float lambda=0.0;

float Y[GLUCOSE_N_DATA];
int t_curr = 0; // how many user inputs have we received so far
extern int t_pred, t_delta, t_offset; // prediction time, time delta, offset=t_pred/t_delta

char add_data(float datum)
{
    if(t_curr >= GLUCOSE_N_DATA)
        return 0;
    Y[t_curr] = datum;
    t_curr++;
    return 1;
}

float b(int t, params *p) {
    return p->D/p->E * (Y[t] - Y[t-1])/DIFF_STEP + 1/p->E * Y[t];
}

float pred(int t, params *p) {
    return p->A * b(t,p) + p->B * b(t,p) * (b(t,p) - Y[t]) + p->C;
}

float cost(params *p){
    // the least squares cost of pred(t) and Y[t]
    int m = t_curr - t_offset;
    float sum = 0.0, tmp;
    // the least square sum
    for(int t = m;t<t_curr;++t)
    {
         tmp = (pred(t,p) - Y[t]);
         sum += tmp*tmp;
    }
    // the regularization term (do not regularize C - absolute term)
    sum += lambda * (p->A * p->A + p->B * p->B + p->D * p->D + p->E * p->E);
    sum *= 1.0/(2.0*m);
    return sum;
}