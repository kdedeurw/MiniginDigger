#include "MiniginPCH.h"
#include "DiggerGameObserver.h"
#include "Events.h"

DiggerGameObserver::DiggerGameObserver()
    : m_Score{}
    , m_EnemiesAlive{}
    , m_HobbinsKilled{}
    , m_NobbinsKilled{}
    , m_Lives{ 3 }
    , m_Gems{}
{}

void DiggerGameObserver::OnNotify(GameObject* pGameObject, int event)
{
    pGameObject;
	switch ((Event)event)
	{
    case Event::event_play_game:
        break;
    case Event::event_player_die:
        --m_Lives;
        break;
    case Event::event_level_complete:
        //progress to next level
        break;
    case Event::event_item_pickup_Gem:
        //play sound
        m_Score += 25;
        --m_Gems;
        break;
    case Event::event_item_pickup_Gold:
        //play sound
        m_Score += 500;
        break;
    case Event::event_enemy_killed_Nobbin:
        m_Score += 250;
        break;
    case Event::event_enemy_killed_Hobbin:
        m_Score += 250;
        break;
	}
}

unsigned int DiggerGameObserver::GetScore() const
{
    return m_Score;
}

unsigned int DiggerGameObserver::GetLives() const
{
    return m_Lives;
}

unsigned int DiggerGameObserver::GetGems() const
{
    return m_Gems;
}