#include "view.hxx"

// Convenient type aliases:
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

//static color
static Color const light_square_color(212, 187, 126);
static Color const dark_square_color(155, 103, 60);
static Color const black_piece_color(0, 0, 0);
static Color const red_piece_color(100, 0, 0);
static Color const highlighting_color(0, 100, 0);
static Color const game_over_color{220,220,220};
// You can change this or even determine it some other way:
static int const grid_size = 84;

View::View(Model const& model)
        : model_(model),
          //builder_(sans20),
          model_2(model),
        // You may want to add sprite initialization here
          light_square({84,84}, light_square_color),
          dark_square({84,84}, dark_square_color),
          black_piece(40, black_piece_color),
          red_piece(40, red_piece_color),
          highlighting_sprite({84, 84,}, highlighting_color),
          black_king({60,60}, black_piece_color),
          red_king({60,60}, red_piece_color),
          game_over_sprite({84, 84}, game_over_color)
{
   // builder_.color(builder_color_);
}

void View::draw(Sprite_set& set, Position mp) {
    auto bp = screen_to_board(mp);
    Move const *move = model_.find_move(bp);
    Position_set pset;
    if (move) {
        pset = move->second;
    } else {
        pset = {};
    }
    if (model_.highlighted().empty()) {
        for (auto p: model_.board()) {
            //if (!pset.empty()) {
            if (pset[p]) {
                set.add_sprite(highlighting_sprite,
                               {p.x * (dark_square.dimensions()
                                                  .width * 1.0),
                                p.y * (dark_square.dimensions()
                                                  .height * 1.0)},
                               10);
            }
        }
    } else {
        for (auto p: model_.highlighted()) {
            set.add_sprite(highlighting_sprite,
                           {p.x * (dark_square.dimensions()
                                              .width * 1.0),
                            p.y * (dark_square.dimensions()
                                              .height * 1.0)},
                           10);
        }
    }
    for (int i = 0; i < model_.board().dimensions().width; i += 2) {
        for (int j = 0; j < model_.board().dimensions().height; j += 2) {

            set.add_sprite(dark_square,
                           {i * (dark_square.dimensions().width * 1.0),
                            j * (dark_square.dimensions().height * 1.0)},
                           0);

            add_player_sprite_(set,
                               {i * dark_square.dimensions().width,
                                j * dark_square.dimensions().height});
        }
    }
    for (int i = 1; i < model_.board().dimensions().width; i += 2) {
        for (int j = 1; j < model_.board().dimensions().height; j += 2) {

            set.add_sprite(dark_square,
                           {i * (dark_square.dimensions().width * 1.0),
                            j * (dark_square.dimensions().height * 1.0)},
                           0);

            add_player_sprite_(set,
                               {i * dark_square.dimensions().width,
                                j * dark_square.dimensions().height});

        }
    }

    for (int i = 0; i < model_.board().dimensions().width; i += 2) {
        for (int j = 1; j < model_.board().dimensions().height; j += 2) {

            set.add_sprite(light_square,
                           {i * (light_square.dimensions().width * 1.0),
                            j * (light_square.dimensions().height * 1.0)},
                           0);

        }

    }
    for (int i = 1; i < model_.board().dimensions().width; i += 2) {
        for (int j = 0; j < model_.board().dimensions().height; j += 2) {

            set.add_sprite(light_square,
                           {i * (light_square.dimensions().width * 1.0),
                            j * (light_square.dimensions().height * 1.0)},
                           0);

        }
    }
    ge211::Text_sprite::Builder name_builder(sans20);
    name_builder.color(ge211::Color::black()) << "Black score:";
    dark_eaten.reconfigure(name_builder);

    ge211::Text_sprite::Builder dark(sans20);
    dark.color(ge211::Color::black()) << std::to_string(model_.get_dark_score());
    dark_score.reconfigure(dark);

    ge211::Text_sprite::Builder name_builder_2(sans20);
    name_builder_2.color(ge211::Color(red_piece_color)) << "Red score:";
    light_eaten.reconfigure(name_builder_2);

    ge211::Text_sprite::Builder light(sans20);
    light.color(ge211::Color(red_piece_color)) << std::to_string(model_.get_light_score());
    light_score.reconfigure(light);

    set.add_sprite(dark_eaten, {589, 18}, 40);
    set.add_sprite(light_eaten, {6, 606}, 40);
    set.add_sprite(dark_score, {623, 38}, 40);
    set.add_sprite(light_score, {35, 626}, 40);

    if (model_.is_game_over()) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                double myx = i * (84 * 1.0);
                double myy = j * (84 * 1.0);
                if (model_.winner() == Player::neither) {
                    if (model_[{i, j}] != Player::neither) {
                        set.add_sprite(game_over_sprite,
                                       {myx, myy},
                                       20);
                    }
                } else {
                    if (model_[{i, j}] == model_.winner()) {
                        set.add_sprite(game_over_sprite,
                                       {myx, myy},
                                       20);
                    }

                }
            }

        }
    }
}

View::Dimensions
View::initial_window_dimensions() const
{
    // You can change this if you want:
    return grid_size * model_.board().dimensions();
}

std::string
View::initial_window_title() const
{
    // You can change this if you want:
    return "Reversi";
}

View::Position
View::board_to_screen(Model::Position pos) const
{
    return {grid_size * pos.x, grid_size * pos.y};
}

Model::Position
View::screen_to_board(View::Position pos) const
{
    return {pos.x / grid_size, pos.y / grid_size};
}

void
View::add_player_sprite_(
        Sprite_set& set,
        Position pos) const
{
    //remember about kings
    Position board_pos = screen_to_board(pos);
    if (model_[board_pos] == Player::dark) {
        if (model_2(board_pos) == Piece::standard) {
            set.add_sprite(black_piece,
                           {1.0 * pos.x, 1.0 * pos.y},
                           100);
        }else{
            set.add_sprite(black_piece,
                           {1.0 * pos.x, 1.0 * pos.y},
                           100);
            set.add_sprite(crown,
                           {1.0 * pos.x + 7.0, 1.0 * pos.y + 13.0},
                           110,
                           ge211::Transform::scale(0.3));
        }
    } else if(model_[board_pos] == Player::light) {
        if (model_2(board_pos) == Piece::standard) {
            set.add_sprite(red_piece,
                           {1.0 * pos.x, 1.0 * pos.y},
                           100);
        }else {
            set.add_sprite(red_piece,
                           {1.0 * pos.x, 1.0 * pos.y},
                           100);
            set.add_sprite(crown,
                           {1.0 * pos.x + 7.0, 1.0 * pos.y + 13.0},
                           110,
                           ge211::Transform::scale(0.3));
        }
    }
}
