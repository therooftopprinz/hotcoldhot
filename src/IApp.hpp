#ifndef __IAPP_HPP__
#define __IAPP_HPP__

#include <utility>
#include <optional>

class IApp
{
public:
    struct program_t
    {
        std::pair<float,int> tt_config[5];
        int rep_cnt = 0;
    };

    struct status_t
    {
        const char*             state;
        std::optional<int>      currentTargetN;
        std::optional<unsigned> repn;
        std::optional<float>    currentTargetT;
        float                   currentActualT;
        std::optional<int>      currentRemaining;
        int                     pwm;
        std::optional<int>      totalRemaining;
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
    virtual std::tuple<short*, size_t, size_t> getChartTarget() = 0;
    virtual std::tuple<short*, size_t, size_t> getChartActual() = 0;
    virtual status_t status() = 0;
};

#endif // __IAPP_HPP__