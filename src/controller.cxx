#include "controller.hxx"

Controller::Controller(int size)
        : Controller(size, size)
{ }

Controller::Controller(int width, int height)
        : model_(width, height),
          view_(model_)
{ }

void
Controller::on_mouse_move(ge211::Posn<int> position)
{
    mouse_pos = position;
}

void
Controller::on_mouse_down(ge211::Mouse_button mb, ge211::Posn<int> mp)
{
    ge211::Posn<int> bp = view_.screen_to_board(mp);
    try {
        if (!model_.selected()) {
            model_.select_move_(bp);
        }else if (!model_.is_turns_piece(model_.turn(), bp) and
                    !model_.is_highlighted(bp)){
            try {
                model_.unselect_move_();
            }
            catch(ge211::Client_logic_error) {}
        }else if (model_.is_turns_piece(model_.turn(), bp) and
                    !model_.is_selected_piece(bp)){
            model_.unselect_move_();
            try {
                model_.select_move_(bp);
            }
            catch(ge211::Client_logic_error) {}
        } else if (model_.is_highlighted(bp)){
            model_.play_move(model_.selected_piece(),bp);
            model_.unselect_move_();
        }
    }
    catch(ge211::Client_logic_error) {}

}

void
Controller::draw(ge211::Sprite_set& sprites)
{
    try {
        view_.draw(sprites, mouse_pos);
    }
    catch(ge211::Client_logic_error) {}
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}

