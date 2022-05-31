/*******************************************/
/*** DO NOT CHANGE ANYTHING IN THIS FILE ***/
/*******************************************/

#pragma once

#include <iostream>

// A player or lack thereof.
enum class Player
{
    dark,
    light,
    neither,
};

enum class Piece
{
    standard,
    king,
    neither,
};
bool is_king(Piece);

// Returns the other player.
Player other_player(Player);
void set_king(Piece piece);
// Prints a player in a manner suitable for debugging.
std::ostream& operator<<(std::ostream&, Player);
