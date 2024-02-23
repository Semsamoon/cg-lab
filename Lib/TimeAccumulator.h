#pragma once

#include <chrono>

namespace Lib
{
    class TimeAccumulator final
    {
    public:
        TimeAccumulator() : current_(std::chrono::system_clock::now()), accumulated_(0), delta_(0)
        {
        }

        void Update();

        double Accumulated() const { return accumulated_; }
        void Accumulated(double value) { accumulated_ = value; }
        double Delta() const { return delta_; }

    private:
        std::chrono::system_clock::time_point current_;
        double accumulated_;
        double delta_;
    };
}
