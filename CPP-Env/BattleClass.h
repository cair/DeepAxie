//
// Created by 47472 on 13.04.2022.
//

#ifndef RL_PROJECT_BATTLECLASS_H
#define RL_PROJECT_BATTLECLASS_H
#include "Main.h"


class BattleClass : public Main {
public:
    bool debug = Main::debug;

    void damageCalculator(Main::axie &attacker, Main::axie &defender_axie, Main::player &player1, Main::player &player2, int num, vector<axie> &sorted_axie);
    std::vector<Main::axie> sort_axies_by_speed(player &player);
    void axieattack();
    int battle(player &player1, player &player2);
    int reward();

    void setStrength(Main::axie &a);

    void restock_cards(player &p);
};


#endif //RL_PROJECT_BATTLECLASS_H
