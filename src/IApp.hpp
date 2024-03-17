#ifndef __IAPP_HPP__
#define __IAPP_HPP__

#include <utility>

class IApp
{
public:
    struct program_t
    {
        std::pair<float,int> ttConfig[5];
        int rep_cnt = 0;
    };

    struct status_t
    {
        bool  isRunning;
        const char* state;
        int   currentTargetN;
        float currentTargetT;
        float currentActualT;
        int   currentRemaining;
        int   pwm;
        int   totalRemaining;
    };

    struct sample_t
    {
        short target_temp;
        short actual_temp;
    };

    IApp() = default;
    virtual ~IApp() = default;

    virtual bool start(const program_t&) = 0;
    virtual bool stop() = 0;
    virtual std::pair<uint32_t, std::vector<sample_t>> get_chart(uint32_t offset, uint32_t size, uint32_t resample) = 0;
    virtual status_t status() = 0;
};

#endif // __IAPP_HPP__