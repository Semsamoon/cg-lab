#include "Stopwatch.h"

Core::Stopwatch Core::Stopwatch::New()
{
	return Stopwatch(std::chrono::system_clock::now());
}

void Core::Stopwatch::Update()
{
	auto current = std::chrono::system_clock::now();
	auto deltaTime = std::chrono::duration<double>(current - current_).count();
	current_ = current;
	lag_ += deltaTime;
}

bool Core::Stopwatch::CanDecreaseLag(double deltaTime)
{
	if (lag_ < deltaTime) return false;
	lag_ -= deltaTime;
	return true;
}