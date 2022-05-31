/*******************************************/
/*** DO NOT CHANGE ANYTHING IN THIS FILE ***/
/*******************************************/

#include "player.hxx"

Player
other_player(Player player)
{
    switch (player) {
    case Player::light:
        return Player::dark;
    case Player::dark:
        return Player::light;
    default:
        return player;
    }
}

void
set_king(Piece piece)
{
    switch (piece) {
    case Piece::standard:
        piece = Piece::king;
    case Piece::king:
        piece = Piece::king;
    default:
        piece = Piece::neither;
    }
}

bool
is_king(Piece piece)
{
    if (piece == Piece::king) {
        return true;
    }else{
        return false;
    }
}

std::ostream&
operator<<(std::ostream& os, Player p)
{
    switch (p) {
    case Player::light:
        return os << "L";
    case Player::dark:
        return os << "D";
    default:
        return os << "_";
    }
}

