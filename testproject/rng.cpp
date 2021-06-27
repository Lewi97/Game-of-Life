#include "rng.h"

#include <ctime>

rng& rng::get()
{
    static rng r;
    return r;
}

rng::rng()
{
    m_rng.seed((unsigned)std::time(nullptr));
}

int rng::intInRange(int low, int high)
{
    std::uniform_int_distribution<int> dist(low, high);
    return dist(m_rng);
}

float rng::floatInRange(float low, float high)
{
    std::uniform_real_distribution<float> dist(low, high);
    return dist(m_rng);
}