//
// Created by alex on 13/04/2022.
//

#ifndef GAME_ENGINE_MAIN_H
#define GAME_ENGINE_MAIN_H
#include <iostream>
#include <vector>
using namespace std;

class State {
public:
    virtual void UpdateState() = 0;
    virtual ~State(){};

};



class Main {
public:
    bool debug = false;

    struct card{
        int id = 999;
        string type = "";
        int damage = 0;
        int defence = 0;

        //// can_be_chosen = can be chosen by player for attack
        //// chosen_for_attack = are chosen for attack
        //// wait_for_restock = are used and must wait to be restocked before being used again
        enum status {cannot_be_chosen, can_be_chosen, chosen_for_attack, wait_for_restock} card_status = cannot_be_chosen;
    };

    struct axie{
        unsigned int id = 0;
        string type = "";
        int health = 0;
        bool alive = true;
        unsigned int morale = 0;
        unsigned int speed = 0;
        unsigned int skill = 0;
        enum position {front,back} position = front;
        string strength = "";
        card cards[4];
    };

    struct player{
        unsigned int id = 0;
        axie axies [2];
        unsigned int energy = 0; // allows player use cards in battle
        unsigned int rank = 1200; // player rank
        unsigned int wins = 0; // player wins
        unsigned int losses = 0; // player losses
    };

    std::vector<Main::axie> sort_axies(player &playa1, player &playa2);
    string axiePosition(axie &axie, bool is_first, string stats);
    string printAxies(player &playa1, player &playa2, vector<axie> axies);
    string PrintGameBoard(player &player1, player &player2, int round);
    std::vector<int> selectFourNumbers(player &pPlayer);
    std::vector<int> showCardsDrawn(Main::player &p, vector<axie> &axies);
    void PrintChosenCards(Main::player &p);
    player createPlayer(int team_id);
    void SelectCards(Main::player &player, vector<Main::axie> axies);
    void SelectCards(Main::player &player, vector<int> input);

    int returnOrderNum(axie &p, vector<axie> axies);
};



#endif //GAME_ENGINE_MAIN_H
