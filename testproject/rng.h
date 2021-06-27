#pragma once

#include <random>

class rng
{
public:
    static rng& get();

    rng(const rng& other) = delete;
    rng& operator=   (const rng& other) = delete;

    int intInRange(int low, int high);
    float floatInRange(float low, float high);

private:
    rng();

    std::mt19937 m_rng;
};
