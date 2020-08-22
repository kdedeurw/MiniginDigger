#pragma once
#include <chrono>

using namespace std::chrono;

template <typename clock>
class Timer final
{
public:
	explicit Timer() : m_Start{} , m_IsHighResClock{ typeid(clock) == typeid(high_resolution_clock) ? true : false } { ResetStartTime(); };
	~Timer() = default;
	Timer(const Timer& other) = delete;
	Timer(Timer&& other) = delete;
	Timer& operator=(const Timer& other) = delete;
	Timer& operator=(Timer&& other) = delete;

	void ResetStartTime();
	const time_point<clock> Now() const;

	template <typename variable, typename measure>
	const variable GetElapsedTime() const;

private:
	template <typename measure>
	time_point<clock> GetElapsedTimePoint() const;

	time_point<clock> m_Start;
	const bool m_IsHighResClock;
};

template <typename clock>
void Timer<clock>::ResetStartTime()
{
	m_Start = Now();
}

template <typename clock>
const time_point<clock> Timer<clock>::Now() const
{
	time_point<clock> now{};
	if (m_IsHighResClock) now = high_resolution_clock::now();
	else now = steady_clock::now();
	return now;
}

template<typename clock>
template <typename measure>
time_point<clock> Timer<clock>::GetElapsedTimePoint() const
{
	return std::chrono::duration_cast<measure>(Now() - m_Start);
}

template <typename clock>
template <typename variable, typename measure>
const variable Timer<clock>::GetElapsedTime() const
{
	time_point<clock> now = Now();
	auto deltaTimePoint = std::chrono::duration<variable>(now - m_Start);

	return (variable)std::chrono::duration_cast<measure>(deltaTimePoint).count();
}