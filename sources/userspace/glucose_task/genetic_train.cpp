#pragma once
#include "glukose_pred.cpp"
#include "random.cpp"
#include <stdstring.h>
#define GEN_CROSS_PROPORTIONAL 0
constexpr float GEN_INIT_RANGE = 250.f;

typedef params chromo;

void popul_init(chromo *mem, int n, float range=GEN_INIT_RANGE)
{
    while(n-->0)
    {
        mem[n] = (chromo){
            .A = frand(-range, range),
            .B = frand(-range, range),
            .C = frand(-range, range),
            .D = frand(-range, range),
            .E = frand(-range, range),
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
        unsigned a = urand() % (n); // range of unsigned
        #if GEN_CROSS_PROPORTIONAL
            float ca = costs[a];
            float ci = costs[i];
            float c = 1.f/(ci + ca);
            mem[i] = (chromo){
                .A = (ci*mem[a].A + ca*mem[i].A)*c,
                .B = (ci*mem[a].B + ca*mem[i].B)*c,
                .C = (ci*mem[a].C + ca*mem[i].C)*c,
                .D = (ci*mem[a].D + ca*mem[i].D)*c,
                .E = (ci*mem[a].E + ca*mem[i].E)*c,
            };
        #else
            mem[i] = (chromo){
                .A = (mem[a].A + mem[i].A)*0.5f,
                .B = (mem[a].B + mem[i].B)*0.5f,
                .C = (mem[a].C + mem[i].C)*0.5f,
                .D = (mem[a].D + mem[i].D)*0.5f,
                .E = (mem[a].E + mem[i].E)*0.5f,
            };
        #endif
    }
}

#include "printing.cpp"
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
        float ch = 1.0f + frand(-perc_change, perc_change);
        if(urand() < rate)
        {
            mem[i].A *= ch;
        }
        if(urand() < rate)
        {
            mem[i].B *= ch;
        }
        if(urand() < rate)
        {
            mem[i].C *= ch;
        }
        if(urand() < rate)
        {
            mem[i].D *= ch;
        }
        if(urand() < rate)
        {
            mem[i].E *= ch;
        }
    }
}

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

/**
 * @brief stop condition - returns 1 if the best chromosome is good enough or if the number of generations is too high
 * 
 * @param mem all chromosomes
 * @param costs the costs of the chromosomes
 * @param n number of chromosomes
 * @param gen current generation
 * @param max_gen maximum number of generations
 * @param min_cost minimum cost
 * @return int 1 if the best chromosome is good enough or if the number of generations is too high
 */
bool popul_stop(chromo *mem, float *costs, int n, int gen, int max_gen, float min_cost)
{
    return (costs[popul_best(mem, costs, n)] < min_cost) || (gen >= max_gen);
}

