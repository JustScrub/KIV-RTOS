#pragma once
#include "glukose_pred.cpp"
#include <stdstring.h>

typedef params chromo;

/**
 * @brief function to return a random float between 1 and 0
 */
extern float randfloat();
float randfloat(float min, float max)
{
    return min + randfloat() * (max - min);
}

void popul_init(chromo *mem, int n)
{
    while(n-->0)
    {
        mem[n] = (chromo){
            .A = randfloat(),
            .B = randfloat(),
            .C = randfloat(),
            .D = randfloat(),
            .E = randfloat(),
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

void swap(void *a, void *b, int sizeof_elm)
{
    char tmp[sizeof_elm];
    memcpy(a, tmp, sizeof_elm);
    memcpy(b, a, sizeof_elm);
    memcpy(tmp, b, sizeof_elm);
}

void popul_heapify(chromo *mem, float *costs, int n, int root)
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
        swap(costs+root, costs+largest, sizeof(float));
        swap(mem+root, mem+largest, sizeof(chromo));
        root = largest;
    }
}

void popul_heapsort(chromo *mem, float *costs, int n)
{
    //build heap
    for(int i=n/2-1;i>=0;--i)
    {
        popul_heapify(mem, costs, n, i);
    }
    //sort
    for(int i=n-1;i>=0;--i)
    {
        swap(costs, costs+i, sizeof(float));
        swap(mem, mem+i, sizeof(chromo));
        popul_heapify(mem, costs, i, 0);
    }
}

/**
 * @brief selects chromosomes for new population and crossbreeds them
 * 
 * @param mem all chromosomes
 * @param costs the costs of the chromosomes
 * @param n number of chromosomes
 * @param keep number of best chromosomes to keep unchanged (elite)
 * @param breed number of chromosomes to breed (crossover)
 */
void popul_select(chromo *mem, float *costs, int n, int keep, int breed)
{
    // sort the chromosomes by cost
    popul_heapsort(mem, costs, n);
    // breed the best chromosomes 
    for(int i=n-1; i>=keep; --i)
    {
        unsigned a = (unsigned)randfloat(0.0f, 4294967295.0f) % (breed); // range of unsigned
        mem[i] = (chromo){
            .A = (mem[a].A + mem[i].A)/2.0f,
            .B = (mem[a].B + mem[i].B)/2.0f,
            .C = (mem[a].C + mem[i].C)/2.0f,
            .D = (mem[a].D + mem[i].D)/2.0f,
            .E = (mem[a].E + mem[i].E)/2.0f,
        };
    }
}

/**
 * @brief mutates chromosomes
 * 
 * @param mem all chromosomes
 * @param n number of chromosomes
 * @param rate mutation rate
 */
void popul_mutate(chromo *mem, int n, float rate, float perc_change=0.1f)
{
    for(int i=0;i<n;++i)
    {
        if(randfloat() < rate)
        {
            mem[i].A *= 1.0f + randfloat(-perc_change, perc_change);
        }
        if(randfloat() < rate)
        {
            mem[i].B *= 1.0f + randfloat(-perc_change, perc_change);
        }
        if(randfloat() < rate)
        {
            mem[i].C *= 1.0f + randfloat(-perc_change, perc_change);
        }
        if(randfloat() < rate)
        {
            mem[i].D *= 1.0f + randfloat(-perc_change, perc_change);
        }
        if(randfloat() < rate)
        {
            mem[i].E *= 1.0f + randfloat(-perc_change, perc_change);
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

