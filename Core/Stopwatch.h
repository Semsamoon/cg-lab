#pragma once

#include <chrono>

namespace Core
{
	class Stopwatch final
	{
	public:
		static Stopwatch New();

		void Update();
		bool CanDecreaseLag(double deltaTime);

	private:
		Stopwatch(std::chrono::system_clock::time_point start) : current_(start), lag_(0) {}

		std::chrono::system_clock::time_point current_;
		double lag_;
	};
}