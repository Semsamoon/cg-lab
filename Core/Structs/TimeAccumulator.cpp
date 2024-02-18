#include "TimeAccumulator.h"

void Core::Structs::TimeAccumulator::Update()
{
	auto current = std::chrono::system_clock::now();
	delta_ = std::chrono::duration<double>(current - current_).count();
	current_ = current;
	accumulated_ += delta_;
}