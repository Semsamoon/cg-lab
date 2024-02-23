#include "TimeAccumulator.h"

void Lib::TimeAccumulator::Update()
{
    const auto current = std::chrono::system_clock::now();
    delta_ = std::chrono::duration<double>(current - current_).count();
    current_ = current;
    accumulated_ += delta_;
}
