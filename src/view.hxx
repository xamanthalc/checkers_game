#pragma once

#include "model.hxx"

#include <string>

class View
{
public:
    /// View dimensions will use `int` coordinates.
    using Dimensions = ge211::Dims<int>;

    /// View positions will use `int` coordinates.
    using Position = ge211::Posn<int>;

    /// View rectangles will use `int` coordinates.
    using Rectangle = ge211::Rect<int>;

    /// Constructs a view that knows about the given model.
    explicit View(Model const&);

    // TODO: your code here
    // You will probably want to add arguments here so that the
    // controller can communicate UI state (such as a mouse or
    // cursor position):
    void draw(ge211::Sprite_set& set, Position mp);

    Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;

    // Converts an abstract board position to a concrete screen
    // position.
    Position
    board_to_screen(Model::Position board_pos) const;

    // Converts a concrete screen (pixel) position to an abstract board
    // position.
    Model::Position
    screen_to_board(Position screen_pos) const;

    // TODO: Add any public member functions you need.

private:
    Model const& model_;
    Model const& model_2;

    // TODO: Add any additional private members you need, such as helper
    // functions or sprites.
    ge211::Image_sprite crown {"crown.png"};
    ge211::Rectangle_sprite const light_square;
    ge211::Rectangle_sprite const dark_square;
    ge211::Circle_sprite const black_piece;
    ge211::Circle_sprite const red_piece;
    ge211::Rectangle_sprite const highlighting_sprite;
    ge211::Rectangle_sprite const black_king;
    ge211::Rectangle_sprite const red_king;
    ge211::Rectangle_sprite const game_over_sprite;
    //ge211::Text_sprite::Builder builder_;
    //ge211::Color builder_color_ = {0, 0, 0};
    ge211::Font sans20 {"sans.ttf", 15};
    //ge211::Text_sprite dark_eaten = ge211::Text_sprite("Dark score:", sans20);
    ge211::Text_sprite dark_eaten;
    ge211::Text_sprite light_eaten;
    ge211::Text_sprite dark_score;
    ge211::Text_sprite light_score;
    void
    add_player_sprite_(ge211::Sprite_set&, Position)

    const;
};
