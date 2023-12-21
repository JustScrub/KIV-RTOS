#pragma once
constexpr unsigned GLUCOSE_N_DATA = 0x100000 / sizeof(float); // 1MB


typedef struct {
    float A, B, C ,D ,E;
} params;
// training and regularization parameters

float *Y;
int t_curr = 0; // how many user inputs have we received so far
int t_pred, t_delta, t_offset; // prediction time, time delta, offset=t_pred/t_delta
float lambda=0.f;

bool add_data(float datum)
{
    if(t_curr >= GLUCOSE_N_DATA)
        return false;
    Y[t_curr] = datum;
    t_curr++;
    return true;
}

float b(int t, params *p) {
    return p->D/p->E * 1440.f*(Y[t] - Y[t-1])/t_delta + 1/p->E * Y[t];
}

float pred(int t, params *p) {
    return p->A * b(t,p) + p->B * b(t,p) * (b(t,p) - Y[t]) + p->C;
}

float cost(params *p){
    // the least squares cost of pred(t) and Y[t]
    float sum = 0.f, tmp;
    // the least square sum
    for(int t = t_offset;t<t_curr;++t)  //t_curr points after the last element
    {
         tmp = (pred(t-t_offset,p) - Y[t]);
         sum += tmp*tmp;
    }
    // the regularization term (do not regularize C - absolute term)
    sum += lambda * (p->A * p->A + p->B * p->B + p->D * p->D + p->E * p->E);
    sum /= (2.f*(t_curr-t_offset));
    return sum;
}