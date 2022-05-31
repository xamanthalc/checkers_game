#include "model.hxx"
#include "board.hxx"
#include <catch.hxx>

using namespace ge211;

// These pass with the starter code and should continue
// to pass.


TEST_CASE("Passes with starter code")
{
    Model m(8, 8);
    CHECK(m.board() == Model::Rectangle {0, 0, 8, 8});
    CHECK_FALSE(m.is_game_over());
    CHECK(m.turn() == Player::dark);
    CHECK(m.winner() == Player::neither);
    CHECK(m[{0, 0}] == Player::dark);
    CHECK(m.find_move({0, 0}) == nullptr);
}

struct Test_access
{
    Model& model;
    // Constructs a `Test_access` with a reference to the Model under test.
    explicit Test_access(Model&);
    // Sets the player at `posn` to `player`.
    void set_player(Model::Position posn, Player player);
    // Gives direct access to `model.next_moves_` so our tests can modify it:
    Move_map& next_moves();
    //Position_set find_flips_(Model::Position current, Model::Dimensions dir);
};

Test_access::Test_access(Model& model)
        : model(model)
{ }

void
Test_access::set_player(Model::Position posn, Player player)
{
    model.board_[posn] = player;
}

Move_map&
Test_access::next_moves()
{
    return model.next_moves_;
}


TEST_CASE("Test 1")
{
    Model m(8, 8);
    Test_access access(m);
    CHECK(m.board() == Model::Rectangle {0, 0, 8, 8});
    CHECK_FALSE(m.is_game_over());
    CHECK(m.turn() == Player::dark);
    CHECK(m.winner() == Player::neither);
    CHECK(m[{0, 0}] == Player::dark);
    CHECK_FALSE(m.is_game_over());
    CHECK(m.turn() == Player::dark);
    m.play_move({6, 2}, {5,3}); //dark
    m.play_move({5, 5}, {6,4}); //light
    CHECK(m.find_move({5, 3}));
    CHECK(m.find_move({0, 0})== nullptr);
    const Move* mymove =  m.find_move({5, 3});
    CHECK(m[{6,4}] == Player::light);
    //std::cout << mymove->second;
    CHECK(mymove->second == Position_set{{4,4}});
    /*m.play_move({2, 3}); //blue light
    const Move* myothermove =  m.find_move({3, 3});
    CHECK(mymove->second == Position_set{{2,2}, {3,3}});
    m.play_move({3, 3});
    CHECK(m.winner() == Player::neither);
    CHECK_FALSE(m.is_game_over());
    CHECK(m.find_move({0, 3}) == nullptr);*/

}

TEST_CASE("Test 2")
{
    Model m(8, 8);
    Test_access access(m);
    CHECK(m.board() == Model::Rectangle {0, 0, 8, 8});
    CHECK_FALSE(m.is_game_over());
    CHECK(m.turn() == Player::dark);
    CHECK(m.winner() == Player::neither);
    CHECK(m[{0, 0}] == Player::dark);
    CHECK_FALSE(m.is_game_over());
    CHECK(m.turn() == Player::dark);
    m.play_move({0, 2}, {1,3}); //dark
    m.play_move({1, 5}, {0,4}); //light

    /*CHECK(m.find_move({1, 3}));
    CHECK(m.find_move({0, 0})== nullptr);
    const Move* mymove =  m.find_move({1, 3});
    CHECK(m[{2,4}] == Player::neither);
    //std::cout << mymove->second;
    CHECK(mymove->second == Position_set{{2,4}});*/
}

TEST_CASE("Draw: Count exceeds 25")
{
    Model m(8, 8);
    Test_access access(m);
    CHECK(m.board() == Model::Rectangle {0, 0, 8, 8});
    CHECK_FALSE(m.is_game_over());
    CHECK(m.turn() == Player::dark);
    CHECK(m.winner() == Player::neither);
    CHECK(m[{0, 0}] == Player::dark);
    CHECK_FALSE(m.is_game_over());
    CHECK(m.turn() == Player::dark);
    m.play_move({6, 2}, {5, 3}); //dark
    m.play_move({1, 5}, {2, 4}); //light
    m.play_move({5, 1}, {6, 2}); //dark
    m.play_move({2, 6}, {1, 5}); //light
    m.play_move({5, 3}, {4, 4}); //dark
    m.play_move({5, 5}, {3, 3}); //light
    m.play_move({3, 3}, {5, 1}); //light
    m.play_move({6, 0}, {4, 2}); //dark
    m.play_move({2, 4}, {3, 3}); //light
    m.play_move({2, 2}, {4, 4}); //dark
    m.play_move({4, 4}, {2, 6}); //dark
    m.play_move({1, 7}, {3, 5}); //light
    m.play_move({0, 2}, {1, 3}); //dark
    m.play_move({7, 5}, {6, 4}); //light
    m.play_move({1, 1}, {0, 2}); //dark
    m.play_move({6, 6}, {7, 5}); //light
    m.play_move({1, 3}, {0, 4}); //dark
    m.play_move({6, 4}, {7, 3}); //light
    m.play_move({0, 4}, {2, 6}); //dark
    m.play_move({3, 7}, {1, 5}); //light
    m.play_move({0, 2}, {1, 3}); //dark
    m.play_move({7, 3}, {5, 1}); //light
    m.play_move({4, 0}, {6, 2}); //dark
    m.play_move({7, 5}, {6, 4}); //light
    m.play_move({1, 3}, {0, 4}); //dark
    m.play_move({6, 4}, {7, 3}); //light
    //this line
    m.play_move({0, 4}, {2, 6}); //dark
    m.play_move({7, 3}, {5, 1}); //light
    m.play_move({2, 6}, {3, 7}); //dark
    m.play_move({5, 1}, {4, 0}); //light
    m.play_move({3, 7}, {5, 5}); //dark
    m.play_move({4, 0}, {2, 2}); //light
    //start count
    m.play_move({5, 5}, {6, 4}); //dark
    m.play_move({2, 2}, {1, 3}); //light
    m.play_move({6, 4}, {5, 5}); //dark
    m.play_move({1, 3}, {2, 2}); //light
    CHECK(m.turn() == Player::dark);
    m.play_move({5, 5}, {6, 4}); //dark
    m.play_move({2, 2}, {1, 3}); //light
    m.play_move({6, 4}, {5, 5}); //dark
    m.play_move({1, 3}, {2, 2}); //light
    m.play_move({5, 5}, {6, 4}); //dark
    CHECK(m.turn() == Player::light);
    m.play_move({2, 2}, {1, 3}); //light
    m.play_move({6, 4}, {5, 5}); //dark
    m.play_move({1, 3}, {2, 2}); //light
    m.play_move({5, 5}, {6, 4}); //dark
    m.play_move({2, 2}, {1, 3}); //light
    m.play_move({6, 4}, {5, 5}); //dark
    m.play_move({1, 3}, {2, 2}); //light
    m.play_move({5, 5}, {6, 4}); //dark
    CHECK(m.winner() == Player::neither);
    m.play_move({2, 2}, {1, 3}); //light
    m.play_move({6, 4}, {5, 5}); //dark
    m.play_move({1, 3}, {2, 2}); //light
    m.play_move({5, 5}, {6, 4}); //dark
    m.play_move({2, 2}, {1, 3}); //light
    m.play_move({6, 4}, {5, 5}); //dark
    m.play_move({1, 3}, {2, 2}); //light
    m.play_move({5, 5}, {6, 4}); //dark
    CHECK_FALSE(m.is_game_over());
    m.play_move({2, 2}, {1, 3}); //light 26th move
    CHECK(m.is_game_over());
    CHECK(m.winner() == Player::neither);
}

TEST_CASE("One player eliminates the other")
{
    Model m(8, 8);
    Test_access access(m);
    CHECK(m.board() == Model::Rectangle {0, 0, 8, 8});
    CHECK_FALSE(m.is_game_over());
    CHECK(m.turn() == Player::dark);
    CHECK(m.winner() == Player::neither);
    CHECK(m[{0, 0}] == Player::dark);
    CHECK_FALSE(m.is_game_over());
    CHECK(m.turn() == Player::dark);
    m.play_move({0, 2}, {1,3});
    m.play_move({1, 5}, {2,4});
    CHECK(m.turn() == Player::dark);
    m.play_move({2, 2}, {3,3});
    m.play_move({2, 4}, {0,2});
    m.play_move({3, 3}, {4,4});
    CHECK(m.turn() == Player::light);
    m.play_move({5, 5}, {3,3});
    m.play_move({4,2}, {2,4});
    m.play_move({3, 5}, {1,3});
    m.play_move({3, 1}, {2,2}); //dark
    m.play_move({1,3}, {3,1});
    m.play_move({4, 0}, {2,2});
    m.play_move({0, 6}, {1,5}); //light
    m.play_move({2,2}, {3,3});
    m.play_move({1, 5}, {2,4});
    m.play_move({3, 3}, {1,5}); //dark
    m.play_move({2,6}, {0,4});
    m.play_move({6, 2}, {5,3});
    CHECK_FALSE(m.is_game_over());
    m.play_move({7, 5}, {6,4}); //light
    m.play_move({5, 3}, {7,5}); //dark
    m.play_move({6,6}, {5,5});
    CHECK(m.winner() == Player::neither);
    m.play_move({7, 5}, {6,6});
    m.play_move({5, 7}, {7,5}); //light
    m.play_move({5, 1}, {4,2}); //dark
    m.play_move({7,5}, {6,4});
    m.play_move({4, 2}, {5,3});
    m.play_move({6, 4}, {4,2}); //light
    m.play_move({2, 0}, {3,1}); //dark
    m.play_move({4,2}, {2,0}); //light becomes king
    CHECK(m[{2, 0}] == Player::light);
    //CHECK(m2[{2, 0}] == Piece::king);
    m.play_move({1, 1}, {2,2});
    m.play_move({2, 0}, {3,1}); //light
    m.play_move({0, 0}, {1,1}); //dark
    m.play_move({0,2}, {2,0}); //turned light into king
    m.play_move({7, 1}, {6,2});
    m.play_move({3, 1}, {1,3}); //light king eating backwards
    //CHECK(m[(1,3)] == Player::light];
    m.play_move({6, 2}, {5,3}); //dark
    m.play_move({2,0}, {1,1});
    m.play_move({5, 3}, {4,4});
    CHECK_FALSE(m.is_game_over());
    m.play_move({5, 5}, {3,3}); //light
    m.play_move({6, 0}, {5,1}); //dark
    m.play_move({1,1}, {0,0}); //light
    m.play_move({5, 1}, {4,2});
    CHECK(m.winner() == Player::neither);
    m.play_move({3, 3}, {5,1}); //light eliminates last black
    CHECK(m.is_game_over());
    CHECK(m.winner() == Player::light);
}

TEST_CASE("players loses because it cannot move")
{
    Model m(8, 8);
    Test_access access(m);
    CHECK(m.board() == Model::Rectangle {0, 0, 8, 8});
    CHECK_FALSE(m.is_game_over());
    CHECK(m.turn() == Player::dark);
    CHECK(m.winner() == Player::neither);
    CHECK(m[{0, 0}] == Player::dark);
    CHECK_FALSE(m.is_game_over());
    CHECK(m.turn() == Player::dark);
    m.play_move({6,2}, {7,3}); //light
    m.play_move({1, 5}, {0,4}); //dark
    m.play_move({5,1}, {6,2}); //light
    m.play_move({2, 6}, {1,5});

    m.play_move({0,2}, {1,3}); //light
    m.play_move({7,5}, {6,4}); //dark
    m.play_move({1,1}, {0,2}); //light
    m.play_move({6, 6}, {7,5});
    m.play_move({0, 0}, {1,1}); //light
    m.play_move({5, 7}, {6,6}); //dark
    m.play_move({4,0}, {5,1}); //light
    m.play_move({1, 7}, {2,6});


    m.play_move({2,2}, {3,3}); //light
    m.play_move({0,4}, {2,2}); //light
    m.play_move({2,2}, {0,0}); //light dj
    m.play_move({3, 1}, {2,2});
    m.play_move({3,5}, {4,4}); //light
    m.play_move({2, 0}, {3,1}); //dark
    m.play_move({2,6}, {3,5}); //light
    m.play_move({0,2}, {1,3});
    m.play_move({3,7}, {2,6}); //light
    m.play_move({6, 2}, {5,3}); //dark
    m.play_move({4,4}, {6,2}); //light
    m.play_move({6,2}, {4,0}); //light dj
    m.play_move({6,0}, {5,1}); //light
    m.play_move({4,0}, {6,2}); //dark
    m.play_move({7,1}, {5,3}); //light
    m.play_move({1,5}, {0,4});
    m.play_move({3,3}, {2,4}); //light
    m.play_move({2,6}, {1,5}); //dark
    m.play_move({4,2}, {3,3}); //light
    m.play_move({6,4}, {4,2});
    m.play_move({4,2}, {2,0}); //light
    m.play_move({7,3}, {6,4}); //dark
    m.play_move({7,5}, {5,3}); //light
    m.play_move({3,3}, {4,4});
    m.play_move({5, 5}, {3,3}); //light
    m.play_move({3,3}, {1,1});
    CHECK(m.is_game_over());
    CHECK(m.winner() == Player::light);
}