
#define TEST_DATA 0
#if TEST_DATA
    extern bool add_data(float datum);

    #if TEST_DATA == 3
        #define N_TEST_DATA 10
        float pow(float x, int n)
        {
            float ret = 1.f;
            for(int i=0;i<n;++i)
                ret *= x;
            return ret;
        }
    #else
        #define N_TEST_DATA 50
    #endif
    
    void prepare_test_data()
    {
        for(int i=0;i<N_TEST_DATA;++i)
        {
            float y;
            #if TEST_DATA == 1
                y = 12.f;
            #elif TEST_DATA == 2
                y = 12.f + 0.1f*i;
            #elif TEST_DATA == 3
                y = pow(2, i);
            #endif
            add_data(y);
        }
    }
#else
    #define prepare_test_data()
#endif
