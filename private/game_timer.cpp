//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// game_timer.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// BombActor class implementation
//--------------------------------------------------------------------------------------------------------

#include "game_timer.h"

GameTimer::GameTimer() : m_timeSinceLastPause(0), m_timeRemaining(0)
{
    
}

/**
 * \brief Stops the timer.
 *
 * This function records the time remaining on the timer and the current time in milliseconds before
 * stopping the timer.
 */
void GameTimer::Pause()
{
    m_timeRemaining = GetTimeLeft();
    m_timeSinceLastPause = GetCurrentTimeMs();
    
    // stop the timer
    Stop();
}

/**
 * \brief Get the time since the game was paused.
 * \return time_paused the difference of the current time in milliseconds and the time since the pause
 * occured.
 */
uint64 GameTimer::GetTimeSinceLastPause() const
{
    uint64 time_paused = GetCurrentTimeMs() - m_timeSinceLastPause;
    
    return time_paused;
}

/**
 * \brief Resume the timer.
 *
 * The timer will resume from the time that was remaining when it was paused.
 */
void GameTimer::Resume()
{
    // record the time since last pause
    m_timeSinceLastPause = GetTimeSinceLastPause();
    
    // continue the duration
    setDuration(m_timeRemaining);
}
