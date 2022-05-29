#include <vector>
#include <pybind11/pybind11.h>  /// NEEDS TO BE INCLUDED BEFORE THE REST
#include <pybind11/stl.h> /// THIS IS NEEDED FOR CONVERSION BETWEEN CPP-VECTOR AND PYTHON-ARRAY
#include "conversion.cpp"
#include "Main.cpp"
#include <filesystem>

namespace py = pybind11;

vector<int> ChooseTeam(int input){
    Conv conv;
    Main main;
    Main::player playerToSend = main.createPlayer(input);
    return conv.convert_to_vec_decimal(playerToSend);
}

vector<int> lilbits(int input, int size){
    Conv conv;
    return conv.convert_to_bits(input, size);
}

struct GameState {
    Main main;
    GameState(const int &playerChoice_1, const int &playerChoice_2) : player1(main.createPlayer(playerChoice_1)), player2(main.createPlayer(playerChoice_2)) { }
    Conv conv;
    BattleClass battleClass;

    Main::player player1;
    Main::player player2;


    void chooseCards(int player, vector<int> input) {
        if (player == 1) 
            main.SelectCards(player1, input);
        else if (player == 2) 
            main.SelectCards(player2, input);
    }

    vector<int> drawCardsFromDeck(int player) {
        if (player == 1) 
            return main.selectFourNumbers(player1);
        else if (player == 2) 
            return main.selectFourNumbers(player2);
        
        cout << "There is an ERROR with your player in --> DeepAxie.cpp:45 <--" << endl;
        return {};
    }
    
    vector<int> playersMatrixBits() {
        vector<int> p1 = conv.convert_to_vec_bit(player1);
        vector<int> p2 = conv.convert_to_vec_bit(player2);
        return conv.merge_vectors(p1, p2);
    }

    vector<int> playersMatrixDecimal() {
        vector<int> p1 = conv.convert_to_vec_decimal(player1);
        vector<int> p2 = conv.convert_to_vec_decimal(player2);
        return conv.merge_vectors(p1, p2);
    }

    int attack() {
        return battleClass.battle(player1, player2);
    }

    string printGameBoard(int gameRound) {
        return main.PrintGameBoard(player1, player2, gameRound);
    }

    void setName(const std::string &name_) { player1.axies[0].type = name_; }
    const std::string &getName() const { return player1.axies[0].type; }
};

string PrintWorkingDir() {
    return std::filesystem::current_path().string();
}

PYBIND11_MODULE(DeepAxie, m){
    m.def("ChooseTeam", &ChooseTeam);

    m.def("lilbits", &lilbits);
    m.def("PrintWorkingDir", &PrintWorkingDir);

    py::class_<GameState>(m, "GameState")
        .def(py::init<const int &, const int &>())
        .def("drawCardsFromDeck", &GameState::drawCardsFromDeck)
        .def("chooseCards", &GameState::chooseCards)
        .def("attack", &GameState::attack)
        .def("playersMatrixBits", &GameState::playersMatrixBits)
        .def("playersMatrixDecimal", &GameState::playersMatrixDecimal)
        .def("printGameBoard", &GameState::printGameBoard)
        .def("getName", &GameState::getName)
        ;

}

PYBIND11_MODULE(DeepAxieLinux, m){
    m.def("ChooseTeam", &ChooseTeam);

    m.def("lilbits", &lilbits);
    m.def("PrintWorkingDir", &PrintWorkingDir);

    py::class_<GameState>(m, "GameState")
        .def(py::init<const int &, const int &>())
        .def("drawCardsFromDeck", &GameState::drawCardsFromDeck)
        .def("chooseCards", &GameState::chooseCards)
        .def("attack", &GameState::attack)
        .def("playersMatrixBits", &GameState::playersMatrixBits)
        .def("playersMatrixDecimal", &GameState::playersMatrixDecimal)
        .def("printGameBoard", &GameState::printGameBoard)
        .def("getName", &GameState::getName)
        ;

}