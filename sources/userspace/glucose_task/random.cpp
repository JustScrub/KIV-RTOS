#include <stdfile.h>
#include <stdalloc.h>

constexpr int N_RANDS_LOG2 = 18; // +2
unsigned *rands = nullptr;
int rands_idx = 0;
constexpr int N_RANDS = 1<<N_RANDS_LOG2;
constexpr int modN_RANDS = N_RANDS-1;

void rand_init()
{
    rands = (unsigned *)malloc(N_RANDS*sizeof(unsigned));
    int trng_fd = open("DEV:trng", NFile_Open_Mode::Read_Only);
    read(trng_fd, reinterpret_cast<char *>(rands), N_RANDS*sizeof(unsigned));
    close(trng_fd);
    rands_idx = 0;
}

inline unsigned urand()
{
    unsigned ret = rands[rands_idx];
    rands_idx = (rands_idx+1) & modN_RANDS;
    return ret;
}

inline float frand(float min, float max)
{
    return min + (float)urand()*2.328306434370292e-10f * (max - min);
                                 // 2^-32
}