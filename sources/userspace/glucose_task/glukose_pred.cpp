#pragma once
#define GLPRED_REGULATION 0
#define GLPRED_LINREG 0
constexpr unsigned GLUCOSE_N_DATA = 0x100000 / sizeof(float); // 1MB


typedef struct {
    float A, B, C ,D ,E;
} params;
// training and regularization parameters

float *Y;
int t_curr = 0; // how many user inputs have we received so far
int t_pred, t_delta, t_offset; // prediction time, time delta, offset= t_pred/t_delta
#if GLPRED_REGULATION
float lambda=0.f;
#endif

bool add_data(float datum)
{
    if(t_curr >= GLUCOSE_N_DATA)
        return false;
    Y[t_curr] = datum;
    t_curr++;
    return true;
}

#define abs(x) ((x) > 0.f ? (x) : -(x))

#if !GLPRED_LINREG
    inline float pred(int t, params p) {
        float y = Y[t];
        float b = (p.D *(y - Y[t-1]) + y)*p.E;  // this should be /p.E !! but since it's a parameter, it will adapt
        return (p.A + p.B * (b - y))*b + p.C;
    }
#else
    inline float pred(int t, params p)
    {
        float y= Y[t];
        float y2= y*y;
        float y3= y2*y;
        float y4= y3*y;
        return (p.A*y + p.B*y2 + p.C + p.D*y3 + p.E*y4);
    }
#endif

inline float cost(params *p){
    // MAE -- not divided by number of train samples!
    float sum = 0.f, tmp;
    params pd = *p;
    for(int t = t_offset+1;t<t_curr;++t)  //t_curr points after the last element
    {
        tmp = pred(t-t_offset, pd) - Y[t];
        sum += abs(tmp);
    }
    #if GLPRED_REGULATION
        // the regularization term (do not regularize C - absolute term)
        sum += lambda * (abs(pd.A) + abs(pd.B) + abs(pd.D) + abs(pd.E));
    #endif
    return sum;
}