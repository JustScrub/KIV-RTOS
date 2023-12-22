#pragma once
#include "glukose_pred.cpp"
#include "random.cpp"
#include <stdstring.h>
#define GEN_CROSS_PROPORTIONAL 0
#define GEN_MUT_LINEAR 0
constexpr float GEN_INIT_RANGE = 20.f;

typedef params chromo;

void popul_init(chromo *mem, int n)
{
    while(n-->0)
    {
        mem[n] = (chromo){
            .A = frand(-GEN_INIT_RANGE, GEN_INIT_RANGE),
            .B = frand(-GEN_INIT_RANGE, GEN_INIT_RANGE),
            .C = frand(-GEN_INIT_RANGE, GEN_INIT_RANGE),
            .D = frand(-GEN_INIT_RANGE, GEN_INIT_RANGE),
            .E = frand(-GEN_INIT_RANGE, GEN_INIT_RANGE),
        };
    }
}

void popul_eval(chromo *mem, float *costs, int n)
{
    while(n-->0)
    {
        costs[n] = cost(mem+n);
    }
}

template <typename T>
void swap(T *a, T *b)
{
    T tmp = *a;
    *a = *b;
    *b = tmp;
}
/*
inline void popul_heapify(chromo *mem, float *costs, int n, int root)
{
    //non-recursively heapify
    while(1)
    {
        int left = 2*root+1;
        int right = 2*root+2;
        int largest = root;
        if(left < n && costs[left] > costs[largest])
        {
            largest = left;
        }
        if(right < n && costs[right] > costs[largest])
        {
            largest = right;
        }
        if(largest == root)
        {
            break;
        }
        swap(costs+root, costs+largest);
        swap(mem+root, mem+largest);
        root = largest;
    }
}

inline void popul_heapsort(chromo *mem, float *costs, int n)
{
    //build heap
    for(int i=n/2-1;i>=0;--i)
    {
        popul_heapify(mem, costs, n, i);
    }
    //sort
    for(int i=n-1;i>=0;--i)
    {
        swap(costs, costs+i);
        swap(mem, mem+i);
        popul_heapify(mem, costs, i, 0);
    }
}
*/

int popul_qsel_partition(chromo *mem, float *costs, int l, int r)
{
    int pivot = l;
    for(int i=l+1;i<r;++i)
    {
        if(costs[i] < costs[l])
        {
            swap(costs+i, costs+pivot+1);
            swap(mem+i, mem+pivot+1);
            ++pivot;
        }
    }
    swap(costs+l, costs+pivot);
    swap(mem+l, mem+pivot);
    return pivot;
}

void popul_qsel(chromo *mem, float *costs, int l, int r, int k)
{ // quickselect non-recursively
    while(1)
    {
        int pivot = popul_qsel_partition(mem, costs, l, r);
        if(pivot == k)
        {
            return;
        }
        else if(pivot > k)
        {
            r = pivot;
        }
        else
        {
            l = pivot+1;
        }
    }
}

#if GEN_CROSS_PROPORTIONAL
    #define gen_cross(P) \
        .P = (ci*mem[a].P + ca*mem[i].P)*c,
#else
    #define gen_cross(P) \
        .P = (mem[a].P + mem[i].P)*0.5f,
#endif

/**
 * @brief selects chromosomes for new population and crossbreeds them
 * 
 * @param mem all chromosomes
 * @param costs the costs of the chromosomes
 * @param n number of chromosomes
 * @param keep number of best chromosomes to keep unchanged (elite)
 */
void popul_select(chromo *mem, float *costs, int n, int keep)
{
    //popul_heapsort(mem, costs, n);
    popul_qsel(mem, costs, 0, n, keep);
    // breed the best chromosomes 
    for(int i=n-1; i>=keep; --i)
    {
        unsigned a = urand() % (keep); // range of unsigned
        #if GEN_CROSS_PROPORTIONAL
            float ca = costs[a];
            float ci = costs[i];
            float c = 1.f/(ci + ca);
        #endif
        mem[i] = (chromo){
            gen_cross(A)
            gen_cross(B)
            gen_cross(C)
            gen_cross(D)
            gen_cross(E)
        };
    }
}
#undef gen_cross

#include "printing.cpp"
#if GEN_MUT_LINEAR
    #define gen_delta(P) \
        if(urand() < rate) \
            {mem[i].P += q*(mem[i].P + c)}
#else
    #define gen_delta(P) \
        if(urand() < rate) \
            {mem[i].P += frand(-perc_change, perc_change);}
#endif
/**
 * @brief mutates chromosomes
 * 
 * @param mem all chromosomes
 * @param n number of chromosomes
 * @param rate mutation rate
 */
void popul_mutate(chromo *mem, int n, unsigned rate, float perc_change=0.1f)
{
    for(int i=0;i<n;++i)
    {
        #if GEN_MUT_LINEAR
            float q = frand(-perc_change, perc_change);
            float c = frand(-GEN_INIT_RANGE, GEN_INIT_RANGE);
        #endif
        gen_delta(A)
        gen_delta(B)
        gen_delta(C)
        gen_delta(D)
        gen_delta(E)
    }
}
#undef gen_delta

/**
 * @brief returns index of the best chromosome
 * 
 * @param mem all chromosomes
 * @param costs the costs of the chromosomes
 * @param n number of chromosomes
 * @return index of the best chromosome
 */
int popul_best(chromo *mem, float *costs, int n)
{
    int best = 0;
    for(int i=1;i<n;++i)
    {
        if(costs[i] < costs[best])
        {
            best = i;
        }
    }
    return best;
}

