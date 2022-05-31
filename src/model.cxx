#include "model.hxx"

using namespace ge211;

Model::Model(int size)
        : Model(size, size)
{ }

Model::Model(int width, int height)
        : board_({width, height}),
          board_2({width, height})
{
    for (int i = 0; i < 8; i += 2) {
        for (int j = 0; j < 3; j += 2) {
            board_[{i,j}] = Player::dark;
            board_2({i, j}) = Piece::standard;
        }
    }
    for (int i = 1; i < 8; i += 2) {
        board_[{i,1}] = Player::dark;
        board_2({i, 1}) = Piece::standard;
    }
    for (int i = 0; i < 8; i += 2) {
        board_[{i,6}] = Player::light;
        board_2({i, 6}) = Piece::standard;
    }
    for (int i = 1; i < 8; i += 2) {
        for (int j = 5; j < 8; j += 2) {
            board_[{i,j}] = Player::light;
            board_2({i, j}) = Piece::standard;
        }
    }
    /*
    for (int i = 1; i < 8; i += 2){
        board_[{i,3}] = Player::neither;
        board_2[{i, 3}] = Piece::neither;
    }
    for (int i = 0; i < 8; i += 2){
        board_[{i,4}] = Player::neither;
        board_2[{i, 4}] = Piece::neither;
    }
*/
    compute_next_moves_();
}

Model::Rectangle
Model::board() const
{
    return board_.all_positions();
}

int Model::get_dark_score() const{
    return dark_score_;
}
int Model::get_light_score() const{
    return light_score_;
}

Player
Model::operator[](Position pos) const
{
    return board_[pos];
}

Piece
Model::operator()(Position pos) const
{
    return board_2(pos);
}

const Move*
Model::find_move(Position pos) const
{
    auto i = next_moves_.find(pos);

    if (i == next_moves_.end()) {
        // Nothing was found, so return NULL (nullptr in C++)
        return nullptr;
    } else {
        // Dereferences the iterator to get the value then returns a pointer
        // to that value. This is safe as the pointer is to a value living
        // within the `next_moves_` structure.
        return &(*i);
    }
}


void
Model::play_move(Position start, Position end)
{
    if (is_game_over()) {
        throw Client_logic_error("Model::play_move: game over");
    }

    const Move* movep = find_move(start);
    if (movep == nullptr) { // check if there was no such move
        throw Client_logic_error("Model::play_move: no such move");
    }

    // TODO: actually execute the move, advance the turn, refill
    // `next_moves_`, etc.
    really_play_move_(start, end);
}

//
// BELOW ARE HELPER FUNCTIONS
// Our tests will run on the public functions, not the helper functions
// However, you are required to implement the following functions to help the
// public functions
//

void
Model::eat_checker()
{
    counter_ = 0;
    jump_ = true;
    if (turn_ == Player::light) {
        light_score_ += 1;
    }
    if (turn_ == Player::dark) {
        dark_score_ += 1;
    }
}

Position_set
Model::find_moves_(Position current, Dimensions dir) const
{
    Position_set pset = {};
    current += dir;



    if (!board_.good_position(current)){
        return {};
    }
    if(board_[current] == Player::neither){
        pset[current] = true;
    }
    if(board_[current] == other_player(turn())){
        if (board_.good_position(current + dir) and board_[current + dir] ==
                                                    Player::neither){
            pset[current + dir] = true;


        //eat checker might be modified later
        //for now resetting count
        }
    }


    /*while(board_[current] == other_player(turn()) and board_[current + dir]
    == Player::neither){
        pset[current + dir] = true;
        current = current + dir+dir;
        //eat checker might be modified later
        //for now resetting count
    }*/

    return pset;

}

Position_set
Model::evaluate_position_(Position pos, Player player, Piece piece) const
{
    Position_set final_set = {};
    Dimensions d;
    int size = 4;
    //if the player is at the position
    if(board_[pos] == turn()) {
        for (int i = 0; i < size; ++i) {
            if (piece == Piece::king){
                d = board_.all_directions()[i];
            } else if (piece == Piece::standard){
                if (player == Player::light){
                    d = board_.red_directions()[i];
                }else if (player == Player::dark){
                    d = board_.black_directions()[i];
                }
            }
            Position_set p_set = find_moves_(pos, d);
            Position outstanding = pos + d + d;
            if (board_.good_position(outstanding) and p_set[outstanding]) {
                final_set.clear();
                final_set |= (p_set);
                return final_set;
            }
            final_set |= (p_set);
        }
    }
    return final_set;
}


bool
Model::check_for_j(Piece piece, Position pos)
{
    Position_set pset = {};
    if (board_[pos] == turn()) {
        if (piece == Piece::king) {
            for (Dimensions d: board_.all_directions()) {
                if (board_.good_position(pos + d) and
                    board_.good_position(pos + d + d)) {
                    if (board_[pos + d] == other_player(turn()) and
                        board_[pos + d + d] == Player::neither) {
                        pset[pos + d + d] = true;
                    }
                }
            }
        } else if (turn() == Player::dark) {
            for (Dimensions d: board_.black_directions()) {
                if (board_.good_position(pos + d) and
                    board_.good_position(pos + d + d)) {
                    if (board_[pos + d] == other_player(turn()) and
                        board_[pos + d + d] == Player::neither) {
                        pset[pos + d + d] = true;
                    }
                }
            }
        } else if (turn() == Player::light) {
            for (Dimensions d: board_.red_directions()) {
                if (board_.good_position(pos + d) and
                    board_.good_position(pos + d + d)) {
                    if (board_[pos + d] == other_player(turn()) and
                        board_[pos + d + d] == Player::neither) {
                        pset[pos + d + d] = true;
                    }
                }
            }
        }
    }
    if (!pset.empty()) {
        return true;
    }else{
        return false;
    }
}


void
Model::compute_next_moves_()
{
    next_moves_.clear();
    Position_set a_pset;
    for (auto a_pos : board_.all_positions()) {
        if (check_for_j(board_2(a_pos), a_pos)) {
            a_pset = evaluate_position_(a_pos,
                                        turn(),
                                        board_2(a_pos));
            next_moves_[a_pos] = a_pset;
        }
    }
    if (!next_moves_.empty()){
        return;
    }
    for (auto a_pos : board_.all_positions()){
        a_pset = evaluate_position_(a_pos,
                                    turn(),
                                    board_2(a_pos));
        if (!a_pset.empty()){
            next_moves_[a_pos] = a_pset;
        }
    }
}

bool
Model::advance_turn_(Position start, Position end)
{
    if(board_.good_position(start) and board_.good_position(end)){
        board_[start] = Player::neither;
        board_2(start) = Piece::neither;
        board_[end] = turn();
        board_2(end) = turn();
        highlighted_pieces.clear();
        turn_ = other_player(turn());
        compute_next_moves_();
    } else {
        throw Client_logic_error("Model::play_move: heeee over");

    }

    return !(next_moves_.empty());
}

void
Model::set_game_over_()
{
    //counter > 25
    //a player is eliminated
    //a player cannot move
    next_moves_.clear();
    highlighted_pieces.clear();
    turn_ = Player::neither;


}

void
Model::set_king(Piece p)
{
    p = Piece::king;
}

bool
Model::check_if_eob(Position p)
{
    if (turn() == Player::light){
        if (p.y == 0){
            return true;
        }
    }else{
        if (p.y == 7){
            return true;
        }
    }
    return false;
}

bool
Model::check_for_dj(Piece piece, Position pos)
{
    Position_set pset = {};
    if (piece == Piece::king){
        for (Dimensions d : board_.all_directions()){
            if (board_.good_position(pos+d) and board_.good_position(pos+d+d)){
                if (board_[pos + d] == other_player(turn()) and
                    board_[pos + d + d] == Player::neither) {
                    pset[pos + d + d] = true;
                }
            }
        }
    }else if (turn() == Player::dark){
        for (Dimensions d : board_.black_directions()) {
            if (board_.good_position(pos+d) and board_.good_position(pos+d+d)){
                if (board_[pos + d] == other_player(turn()) and
                    board_[pos + d + d] == Player::neither) {
                    pset[pos + d + d] = true;
                }
            }
        }
    }else if (turn() == Player::light){
        for (Dimensions d : board_.red_directions()) {
            if (board_.good_position(pos+d) and board_.good_position(pos+d+d)){
                if (board_[pos + d] == other_player(turn()) and
                    board_[pos + d + d] == Player::neither) {
                    pset[pos + d + d] = true;
                }
            }
        }
    }
    if (!pset.empty()) {
        next_moves_.clear();
        next_moves_[pos] = pset;
        //select_move_(pos);
        return true;
    }else{
        return false;
    }
}


void
Model::select_move_(Position pos)
{
    if (is_game_over()) {
        throw Client_logic_error("Model::select_move_: game over");
    }
    const Move* movep = find_move(pos);
    if (movep == nullptr) { // check if there was no such move
        throw Client_logic_error("Model::play_move: no such move");
    }
    selected_ = true;
    selected_piece_ = pos;
    dont_really_play_move_(*movep);
}

void
Model::unselect_move_()
{
    if (is_game_over()) {
        throw Client_logic_error("Model::select_move_: game over");
    }
    selected_ = false;
    highlighted_pieces.clear();
}

bool
Model::is_turns_piece(Player player, Position position){
    return (board_[position] == player);
}


void
Model::dont_really_play_move_(Move move)
{
    highlighted_pieces |= move.second;
}


void
Model::really_play_move_(Position start, Position end)
{
    Piece piece = board_2(start);
    Player player = board_[start];
    counter_ += 1;
    if (turn() == Player::light) {
        if (piece == Piece::standard) {
            //jump
            if ((start.y - 1) != end.y) {
                eat_checker();
                if (start.x < end.x) {
                    if (board_.good_position({start.x + 1, start.y - 1})) {
                        board_[{start.x + 1, start.y - 1}] = Player::neither;
                        board_2({start.x + 1, start.y - 1}) = Piece::neither;
                    }
                } else {
                    if (board_.good_position({start.x - 1, start.y - 1})) {
                        board_[{start.x - 1, start.y - 1}] = Player::neither;
                        board_2({start.x - 1, start.y - 1}) = Piece::neither;
                    }
                }
            }
        }
        if(piece == Piece::king) {
            if ((start.y - 1) > end.y) {
                eat_checker();
                if (start.x < end.x) {
                    if (board_.good_position({start.x + 1, start.y - 1})) {
                        board_[{start.x + 1, start.y - 1}] = Player::neither;
                        board_2({start.x + 1, start.y - 1}) = Piece::neither;
                    }
                } else {
                    if (board_.good_position({start.x - 1, start.y - 1})) {
                        board_[{start.x - 1, start.y - 1}] = Player::neither;
                        board_2({start.x - 1, start.y - 1}) = Piece::neither;
                    }
                }
            }
            if ((start.y + 1) < end.y) {
                eat_checker();
                if (start.x < end.x) {
                    if (board_.good_position({start.x + 1, start.y + 1})) {
                        board_[{start.x + 1, start.y + 1}] = Player::neither;
                        board_2({start.x + 1, start.y + 1}) = Piece::neither;
                    }
                } else {
                    if (board_.good_position({start.x - 1, start.y + 1})) {
                        board_[{start.x - 1, start.y + 1}] = Player::neither;
                        board_2({start.x - 1, start.y + 1}) = Piece::neither;
                    }
                }
            }
        }
    }
    if (turn() == Player::dark) {
        if (piece == Piece::standard) {
            if ((start.y + 1) != end.y) {
                eat_checker();
                if (start.x < end.x) {
                    if (board_.good_position({start.x + 1, start.y + 1})) {
                        board_[{start.x + 1, start.y + 1}] = Player::neither;
                        board_2({start.x + 1, start.y + 1}) = Piece::neither;
                    }
                } else {
                    if (board_.good_position({start.x - 1, start.y + 1})) {
                        board_[{start.x - 1, start.y + 1}] = Player::neither;
                        board_2({start.x - 1, start.y + 1}) = Piece::neither;
                    }
                }
            }
        }
        if(piece == Piece::king) {
            if ((start.y - 1) > end.y) {
                eat_checker();
                if (start.x < end.x) {
                    if (board_.good_position({start.x + 1, start.y - 1})) {
                        board_[{start.x + 1, start.y - 1}] = Player::neither;
                        board_2({start.x + 1, start.y - 1}) = Piece::neither;
                    }
                } else {
                    if (board_.good_position({start.x - 1, start.y - 1})) {
                        board_[{start.x - 1, start.y - 1}] = Player::neither;
                        board_2({start.x - 1, start.y - 1}) = Piece::neither;
                    }
                }
            }
            if ((start.y + 1) < end.y) {
                eat_checker();
                if (start.x < end.x) {
                    if (board_.good_position({start.x + 1, start.y + 1})) {
                        board_[{start.x + 1, start.y + 1}] = Player::neither;
                        board_2({start.x + 1, start.y + 1}) = Piece::neither;
                    }
                } else {
                    if (board_.good_position({start.x - 1, start.y + 1})) {
                        board_[{start.x - 1, start.y + 1}] = Player::neither;
                        board_2({start.x - 1, start.y + 1}) = Piece::neither;
                    }
                }
            }
        }
    }
    board_[start] = Player::neither;
    board_2(start) = Piece::neither;
    board_[end] = player;
    board_2(end) = piece;
    if (check_if_eob(end)){
        board_2(end) = Piece::king;
    }
    highlighted_pieces.clear();
    if (jump_){
        if (check_for_dj(piece, end)){
            return;
        } else {
            jump_ = false;
        }
    }
    if (!advance_turn_(start, end)) {
        winner_ = other_player(turn());
        set_game_over_();
    }
    if (board_.count_player(turn()) == 0) {
        winner_ = other_player(turn());
        set_game_over_();
    }

    if (counter_ > 25) {
        winner_ = Player::neither;
        set_game_over_();
    }
}
