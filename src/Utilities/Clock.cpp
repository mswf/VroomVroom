#include "Clock.hpp"

float Clock::cyclesPerSecond = 0.0;

inline uint64 Clock::SecondsToCycles( float timeSeconds )
{
    return (uint64)(timeSeconds * cyclesPerSecond);
}

inline float Clock::CyclesToSeconds( uint64 timeCycles )
{
    return (float)timeCycles / cyclesPerSecond;
}

void Clock::Init()
{
    cyclesPerSecond = (float)SDL_GetTicks();
}

Clock::Clock( float startTimeSeconds ) :
    timeCycles( SecondsToCycles( startTimeSeconds ) ),
    timeScale( 1.0f ),
    isPaused( false )
{
}

uint64 Clock::GetTimeCycles() const
{
    return timeCycles;
}

float Clock::CalculateDeltaSeconds(const Clock &other)
{
    uint64 delta = timeCycles - other.timeCycles;
    return CyclesToSeconds(delta);
}

void Clock::Update(float deltaRealSeconds )
{
    if (!isPaused)
    {
        uint64 deltaScaleCycles = SecondsToCycles( deltaRealSeconds * timeScale );
        timeCycles += deltaScaleCycles;
    }
}

void Clock::SetPause(bool pause)
{
    isPaused = pause;
}

bool Clock::IsPaused() const
{
    return isPaused;
}

void Clock::SetTimeScale(float scale)
{
    timeScale = scale;
}

float Clock::GetTimeScale() const
{
    return timeScale;
}

void Clock::SingleStep()
{
    const float FRAMERATE = 60.0f;
    if (isPaused)
    {
        // Add one ideal frame interval.
        uint64 deltaScaledCycles = SecondsToCycles( (1.0f / FRAMERATE) * timeScale );
        timeCycles += deltaScaledCycles;
    }
}
