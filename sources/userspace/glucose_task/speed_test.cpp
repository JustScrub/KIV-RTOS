#include "genetic_train.cpp"
//#include "glukose_pred.cpp" -- included in genetic_train.cpp
#include "printing.cpp"
#include <stdalloc.h>
#include <stdfile.h>
#include <stdstring.h>
#define ever (;;)
#define DEBUG 1
constexpr uint32_t TEST_ITERS  = 100;

#define MAX_GEN 1000
#define MIN_COST 1e-4f
constexpr uint32_t CHROMO_NUM = 2000;//mem::PageSize / sizeof(chromo);
constexpr uint32_t CHROMO_KEEP = CHROMO_NUM/32;

extern int t_delta, t_pred, t_offset;
extern int t_curr;
extern float lambda;
extern float *Y;
float Ys[] = {
8.9569f, 8.3048f, 11.763f, 9.9706f, 13.245f, 8.5567f, 9.8713f, 11.771f, 9.4677f, 15.086f, 9.2391f, 11.025f, 9.6909f, 13.201f, 9.2697f, 9.2124f, 10.753f, 10.107f, 11.498f, 10.078f, 12.274f, 7.1522f, 10.429f, 8.7274f, 9.1992f, 8.7919f, 10.464f, 9.9598f, 11.665f, 5.8849f, 12.840f, 9.6997f, 11.104f, 11.785f, 8.7883f, 12.351f, 9.8235f, 10.871f, 9.5999f, 11.704f, 11.051f, 8.4210f, 9.0107f, 11.009f, 9.3609f, 8.7576f, 11.071f, 10.658f, 13.268f, 8.6185f, 8.8134f, 9.6365f, 11.537f, 9.5032f, 9.2192f, 8.8702f, 11.630f, 8.1559f, 9.9779f, 7.9868f, 7.6655f, 10.053f, 11.109f, 7.8456f, 13.921f, 12.460f, 9.4236f, 8.4529f, 8.6909f, 16.826f, 11.052f, 13.669f, 9.9032f, 10.228f, 10.999f, 9.2767f, 5.4001f, 8.2857f, 12.185f, 11.130f, 11.438f, 11.952f, 10.844f, 10.097f, 6.9498f, 12.048f, 11.472f, 10.930f, 8.4243f, 12.592f, 6.5191f, 12.370f, 6.9199f, 9.0239f, 9.3307f, 9.7417f, 11.412f, 12.586f, 13.585f, 9.8308f, 11.604f, 13.685f, 8.1696f, 10.668f, 9.7203f, 9.1930f, 10.887f, 13.683f, 11.053f, 13.908f, 9.3142f, 11.275f, 9.2123f, 8.2000f, 13.539f, 7.5890f, 10.151f, 12.522f, 11.055f, 10.473f, 7.1832f, 10.651f, 5.8166f, 8.3125f, 12.272f, 12.255f, 8.8082f, 10.999f, 10.244f, 5.2701f, 10.856f, 11.764f, 7.4101f, 8.6594f, 8.4270f, 10.825f, 10.269f, 14.541f, 10.921f, 7.8204f, 11.238f, 7.8463f, 9.4747f, 9.3427f, 10.313f, 7.1969f, 6.7406f, 9.3586f, 7.9546f, 10.299f, 14.050f, 12.633f, 9.6571f, 12.582f, 9.0978f, 6.9823f, 12.066f, 6.8105f, 12.326f, 12.487f, 6.3016f, 14.116f, 7.7732f, 16.477f, 9.9670f, 10.883f, 10.476f, 9.0716f, 9.4670f, 13.325f, 10.262f, 8.1227f, 9.0686f, 7.2387f, 10.614f, 7.9201f, 11.043f, 5.8079f, 10.165f, 7.8762f, 11.936f, 9.5933f, 9.2124f, 9.8396f, 9.4412f, 10.072f, 9.6968f, 9.1778f, 9.3942f, 11.127f, 8.8901f, 7.0970f, 8.7319f, 10.353f, 10.488f, 8.3152f, 6.6994f, 8.7205f, 10.253f, 9.6793f, 15.368f, 8.0535f, 10.281f, 8.0707f, 8.6853f, 8.5363f, 11.919f, 11.342f, 9.0068f, 7.5443f, 10.821f, 12.541f, 11.031f, 7.8605f, 12.791f, 8.7686f, 7.9848f, 13.691f, 8.9566f, 8.7919f, 10.068f, 9.2086f, 8.8092f, 8.3913f, 8.8923f, 14.220f, 12.115f, 10.630f, 10.488f, 10.519f, 9.3813f, 7.4219f, 10.565f, 11.977f, 14.176f, 9.4642f, 8.3298f, 11.154f, 12.506f, 9.1165f, 11.199f, 10.934f, 9.6750f, 11.456f, 6.7951f, 8.7749f, 11.287f, 7.2568f, 10.153f, 8.3474f, 12.344f, 10.463f, 8.3441f, 9.8047f, 11.451f, 11.710f, 11.682f, 8.5969f, 9.0514f, 10.727f, 9.3729f, 13.540f, 10.291f, 8.7279f, 9.4865f, 4.1419f, 13.111f, 14.635f, 9.7516f, 9.6268f, 10.395f, 11.322f, 8.9743f, 9.0327f, 7.3663f, 8.1395f, 11.661f, 8.9102f, 14.249f, 9.5697f, 10.525f, 9.6718f, 13.971f, 7.7098f, 7.1380f, 10.368f, 9.6967f, 9.5659f, 7.2053f, 9.4701f, 12.170f, 13.148f, 10.639f, 8.9358f, 7.9664f, 10.908f, 10.480f, 14.455f, 12.195f, 12.719f, 13.692f, 6.7620f, 15.763f, 12.558f, 10.325f, 9.4346f, 11.600f, 8.9936f, 8.8803f, 13.431f, 9.6545f, 9.6054f, 12.211f, 8.6563f, 11.642f, 10.499f, 12.848f, 12.027f, 8.3485f, 9.7438f, 8.9819f, 9.3020f, 10.144f, 10.254f, 8.5628f, 9.6197f, 8.1076f, 9.9549f, 10.223f, 9.9696f, 12.033f, 8.3635f, 10.371f, 15.101f, 11.800f, 12.341f, 6.7877f, 9.6349f, 13.634f, 10.040f, 11.647f, 13.101f, 9.3243f, 10.530f, 11.376f, 9.9448f, 9.5843f, 10.118f, 10.529f, 10.796f, 6.7750f, 13.682f, 12.828f, 10.363f, 8.3781f, 10.666f, 14.148f, 10.561f, 10.036f, 11.330f, 9.4385f, 8.4705f, 12.262f, 12.979f, 9.7895f, 11.279f, 14.823f, 8.0471f, 11.202f, 9.4018f, 8.6873f, 11.061f, 11.283f, 9.7112f, 8.1462f, 9.6345f, 8.8994f, 10.023f, 9.2282f, 6.8526f, 11.962f, 12.919f, 8.6191f, 8.3079f, 12.826f, 10.483f, 9.3929f, 8.5504f, 10.635f, 7.7656f, 12.783f, 11.467f, 11.777f, 7.1086f, 10.484f, 8.9412f, 12.244f, 9.3977f, 8.1941f, 8.7761f, 14.298f, 12.054f, 7.5657f, 10.933f, 12.189f, 9.5645f, 11.743f, 7.0416f, 12.671f, 7.0751f, 8.4278f, 10.640f, 8.7348f, 8.2241f, 9.9473f, 9.7247f, 11.073f, 9.9450f, 10.632f, 10.296f, 9.7839f, 8.3901f, 10.896f, 11.589f, 8.8188f, 8.1753f, 11.948f, 9.9885f, 10.713f, 8.2030f, 9.0910f, 9.9213f, 9.3901f, 12.094f, 10.393f, 9.2470f, 14.127f, 6.0942f, 7.9495f, 10.580f, 8.7666f, 4.8940f, 12.228f, 7.3625f, 11.139f, 8.8996f, 6.1654f, 9.2941f, 11.476f, 11.924f, 7.0365f, 6.5780f, 8.0459f, 9.1643f, 10.114f, 7.9761f, 4.5263f, 11.960f, 8.8883f, 7.8717f, 8.9036f, 9.3568f, 11.277f, 9.7090f, 7.3676f, 8.9656f, 12.114f, 12.450f, 7.2790f, 8.4735f, 15.157f, 10.308f, 9.8660f, 11.702f, 9.3572f, 6.3807f, 12.059f, 7.1549f, 9.7746f, 10.078f, 13.184f, 9.2497f, 8.0776f, 8.7116f, 10.715f, 11.905f, 10.329f, 12.178f, 11.128f, 8.7588f, 10.951f, 9.2441f, 7.8307f, 10.465f, 10.627f, 9.8974f, 9.2482f, 10.080f, 10.335f, 10.148f, 9.5404f, 10.662f, 9.8806f, 7.7062f, 12.387f, 10.659f, 7.8479f, 10.537f, 13.100f, 12.312f, 8.6303f, 8.2893f, 11.880f, 11.626f, 9.7631f, 11.186f, 10.884f, 8.6674f, 7.5742f, 8.5889f, 8.7131f, 5.2103f, 6.4183f, 11.529f, 10.254f, 9.1300f, 9.8593f, 8.3685f, 10.742f, 11.739f, 9.1254f, 10.163f, 7.1707f, 9.5021f, 9.7688f, 7.7027f, 9.5924f, 9.0318f, 8.1525f, 11.308f, 10.791f, 13.831f, 11.325f, 9.8178f, 8.9321f, 8.1108f, 11.373f, 8.1766f, 9.8705f, 13.191f, 9.1112f, 12.785f, 8.4705f, 13.124f, 9.3621f, 10.310f, 11.097f, 7.9856f, 11.930f, 8.6522f, 12.460f, 8.9097f, 8.1709f, 9.7421f, 12.170f, 7.5073f, 8.0688f, 9.2353f, 8.4010f, 10.149f, 8.8147f, 10.088f, 10.475f, 10.245f, 11.768f, 8.4693f, 10.535f, 9.0864f, 10.637f, 10.764f, 9.6371f, 11.455f, 14.966f, 9.3474f, 11.292f, 11.791f, 10.478f, 7.0769f, 9.9415f, 9.2643f, 12.259f, 7.5694f, 11.722f, 7.7578f, 7.3004f, 14.006f, 10.885f, 8.0173f, 6.2209f, 10.563f, 8.8699f, 10.234f, 8.6251f, 10.576f, 9.9727f, 9.7737f, 8.5839f, 13.244f, 10.509f, 9.2003f, 13.047f, 10.151f, 10.014f, 10.031f, 9.9537f, 11.385f, 6.7653f, 7.1854f, 7.4735f, 10.332f, 8.3775f, 9.8816f, 4.6115f, 9.8165f, 8.9007f, 9.6086f, 10.502f, 9.9672f, 9.1972f, 12.180f, 10.981f, 8.2066f, 10.668f, 10.470f, 13.208f, 8.7433f, 9.3112f, 10.645f, 10.707f, 11.244f, 13.151f, 13.951f, 10.839f, 10.918f, 8.3151f, 8.4460f, 12.269f, 9.2614f, 12.417f, 9.8233f, 12.807f, 9.3697f, 8.4632f, 8.4817f, 12.251f, 9.4166f, 8.5810f, 10.536f, 8.0803f, 8.9591f, 8.6797f, 13.242f, 6.7784f, 11.066f, 13.063f, 13.122f, 10.685f, 12.930f, 11.981f, 10.627f, 12.836f, 13.282f, 10.190f, 8.6397f, 6.1483f, 6.4607f, 8.5430f, 13.802f, 9.1859f, 11.087f, 10.692f, 11.393f, 11.592f, 12.446f, 9.7076f, 8.4370f, 10.732f, 11.683f, 8.2220f, 9.5873f, 7.5879f, 12.992f, 14.819f, 8.5586f, 12.582f, 8.3121f, 11.816f, 11.635f, 9.6856f, 10.893f, 11.335f, 10.912f, 10.212f, 4.2001f, 5.5977f, 10.739f, 11.138f, 8.1491f, 8.8191f, 11.466f, 11.471f, 10.506f, 8.7344f, 9.1892f, 9.1303f, 9.6940f, 6.6028f, 13.614f, 7.5758f, 11.944f, 7.2957f, 11.225f, 5.8798f, 10.653f, 12.919f, 10.019f, 8.6430f, 12.103f, 5.2986f, 9.1012f, 11.016f, 10.015f, 8.0086f, 8.1735f, 6.7063f, 9.5509f, 10.015f, 10.013f, 12.852f, 10.829f, 8.1880f, 10.594f, 15.284f, 11.710f, 10.062f, 10.415f, 8.8485f, 10.161f, 9.9685f, 8.7955f, 8.2317f, 11.392f, 11.158f, 11.409f, 8.0274f, 9.4102f, 13.382f, 12.158f, 6.5248f, 8.4456f, 9.0763f, 12.618f, 8.3425f, 12.056f, 8.3397f, 7.3701f, 7.4887f, 9.4380f, 10.064f, 10.499f, 13.411f, 8.7670f, 6.3177f, 9.9828f, 7.1457f, 13.057f, 10.690f, 9.4084f, 9.1651f, 9.4771f, 13.513f, 10.420f, 8.4974f, 14.917f, 10.708f, 14.558f, 9.0852f, 11.969f, 8.8606f, 8.3607f, 6.8992f, 10.846f, 9.8353f, 12.725f, 6.0356f, 12.121f, 8.0419f, 8.7038f, 11.113f, 11.152f, 7.2195f, 10.103f, 6.7460f, 12.339f, 9.4256f, 9.2715f, 5.8306f, 5.1848f, 8.9060f, 11.962f, 7.8492f, 11.020f, 11.035f, 9.4671f, 10.284f, 7.8122f, 9.7327f, 12.385f, 10.123f, 13.714f, 6.4762f, 12.686f, 10.736f, 14.399f, 7.4436f, 10.847f, 8.2593f, 6.6030f, 6.7153f, 10.221f, 12.297f, 10.660f, 8.4195f, 10.186f, 11.585f, 9.0108f, 10.470f, 12.230f, 9.9506f, 14.351f, 8.4394f, 12.156f, 11.577f, 9.7746f, 10.121f, 7.1914f, 4.2089f, 9.1031f, 7.8739f, 7.9331f, 11.673f, 9.0251f, 8.8917f, 11.052f, 11.816f, 10.926f, 9.9653f, 10.112f, 6.8831f, 8.7436f, 11.774f, 11.443f, 11.219f, 9.6936f, 10.183f, 10.183f, 10.262f, 10.471f, 14.374f, 8.8276f, 6.5745f, 10.642f, 10.865f, 8.8012f, 8.1779f, 9.9741f, 13.100f, 11.854f, 10.063f, 9.9294f, 15.156f, 7.7791f, 8.9058f, 9.7533f, 7.4206f, 8.5027f, 10.399f, 6.9458f, 8.2239f, 11.297f, 12.799f, 6.0502f, 12.870f, 7.6358f, 9.7812f, 8.8790f, 11.002f, 9.4171f, 11.006f, 11.633f, 7.6324f, 10.519f, 9.0532f, 14.745f, 9.8672f, 11.006f, 10.585f, 13.352f, 8.8633f, 10.966f, 10.341f, 11.925f, 13.004f, 12.876f, 9.7016f, 10.732f, 10.715f, 13.026f, 8.2827f, 8.9040f, 12.880f, 8.4631f, 12.785f, 10.118f, 8.2967f, 4.7727f, 10.041f, 10.041f, 9.0278f, 10.918f, 6.7052f, 12.061f, 10.785f, 9.1070f, 12.895f, 9.6585f, 10.538f, 7.0440f, 12.314f, 9.9439f, 11.647f, 9.6309f, 7.7415f, 6.7372f, 9.1734f, 9.9194f, 9.1378f, 9.1349f, 11.056f, 7.8995f, 8.9176f, 9.1922f, 8.8095f, 9.5004f, 12.195f, 8.5214f, 10.319f, 9.8797f, 9.2633f, 8.2210f, 9.2452f, 10.302f, 9.8052f, 9.4282f, 13.037f, 10.002f, 10.622f, 9.1864f, 12.852f, 11.403f, 10.132f, 10.365f, 8.8544f, 9.3389f, 9.8879f, 9.4719f, 9.5600f, 12.682f, 10.806f, 13.772f, 13.542f, 8.5266f, 11.774f, 7.1235f, 8.4705f, 8.8255f, 8.1706f, 10.238f, 11.257f, 12.169f, 12.374f, 9.3747f, 13.916f, 16.093f, 8.9073f, 11.891f, 9.6248f, 9.4347f, 12.499f, 8.9193f
};
unsigned mutate_rate; float mutate_magnitude;
int gen=0;
char bfr[100];

extern int uart_fd;

#define test(type, fn_call) \
    printf(#type " test:"); \
    ticks = get_tick_count(); \
    for(i=0;i<TEST_ITERS;i++) \
        fn_call; \
    ticks = (get_tick_count() - ticks); \
    printint(ticks/TEST_ITERS); \
    printf("------------------");

int main()
{
    open_uart();
    Y = Ys;
    t_delta = 1;
    t_pred = 1;
    t_offset = 1;
    lambda = 0.1f;
    mutate_magnitude = 0.1f;
    mutate_rate = (4294967296U)/10U;
    t_curr = sizeof(Ys)/sizeof(float);
    uint32_t ticks; int i;
    set_task_deadline(1000);
    chromo *mem = (chromo *)malloc(sizeof(chromo)*CHROMO_NUM);
    float *costs = (float *)malloc(sizeof(float)*CHROMO_NUM);

    printf("rand_init test:");
    ticks = get_tick_count();
    rand_init();
    ticks = (get_tick_count() - ticks);
    printint(ticks);

    for(int i=0; i< TEST_ITERS; i++)
        {
            printint(urand());
            printfloat(frand(-3.f, 3.f));
        }

    t_curr = 25;
    for ever{
        popul_init(mem, 1);
        test(cost, cost(mem));
        test(popul_init, popul_init(mem, CHROMO_NUM));
        test(popul_eval, popul_eval(mem, costs, CHROMO_NUM));
        test(popul_select, popul_select(mem, costs, CHROMO_NUM, CHROMO_KEEP));
        test(popul_mutate, popul_mutate(mem, CHROMO_NUM, mutate_rate, mutate_magnitude));
        popul_eval(mem, costs, CHROMO_NUM);
        test(popul_best, popul_best(mem, costs, CHROMO_NUM));
    }



}