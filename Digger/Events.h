#pragma once

enum class Event
{
	event_play_game,
	event_player_die,
	event_level_complete,

	event_item_pickup_Gem,
	event_item_pickup_Gold,

	event_enemy_spawned_Nobbin,
	event_enemy_spawned_Hobbin,
	event_enemy_killed_Nobbin,
	event_enemy_killed_Hobbin,
};