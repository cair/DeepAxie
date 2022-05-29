//
// Created by 47472 on 13.04.2022.
//
#include "BattleClass.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <map>

using namespace std::chrono;

// void BattleClass::setStrength(Main::axie &a){
//     // print out the a.type
//     if(a.type == "plant"){
//         a.strength = "aqua";
//     }else if(a.type == "aqua"){
//         a.strength ="beast";
//     }else if(a.type == "beast"){
//         a.strength = "plant";
//     }
// }
// create a fucntion that sorts the axies by speed when the axie's health is not 0
std::vector<Main::axie> BattleClass::sort_axies_by_speed(player &playa){
    std::vector<Main::axie> axies_to_sort;
    for (auto &axie : playa.axies)
        if(axie.health > 0)
            axies_to_sort.push_back(axie);

    std::sort(axies_to_sort.begin(), axies_to_sort.end(), [](const Main::axie &a, const Main::axie &b) {
        return a.speed > b.speed;
    });
    return axies_to_sort;
}

void BattleClass::restock_cards(player &p){
    int num_used_cards = 0;
    for (auto & axie : p.axies) {
        for (auto & k : axie.cards) {
            if(k.card_status == Main::card::wait_for_restock){
                num_used_cards +=1;
            }
        }
    }
    // if all cards are used, then change the status to can_be_chosen.
    if (num_used_cards == 8){
        for (auto & a : p.axies){
            if(a.alive){
                for (auto & k : a.cards) {
                    k.card_status = Main::card::cannot_be_chosen;
                }
            }
        }
    }
}


void BattleClass::damageCalculator(Main::axie &attacker_axie, Main::axie &defender_axie, Main::player &p1, Main::player &p2,int attackNum, vector<axie> &sorted_axies) {
    // the damage is the attacker card base damage
    // how to know which card is attacking?
    // the attacker card is the card that is on the top of the stack?
    // could damage be the sum of both cards base damage?
    int total_damage = 0;

    for (int i = 0; i < 4; ++i) {
        //// base damage needs to reset for each card
        int base_damage = 0;
        if (attacker_axie.cards[i].card_status == Main::card::chosen_for_attack) {
            //// base damage is the card damage
            base_damage = attacker_axie.cards[i].damage;
            if (debug) std::cout << "base_damage: " << base_damage << std::endl;
            total_damage += base_damage;
            ////  If the Axie type is the same as the axie card type, the card will deal an extra 10% damage
            if (attacker_axie.cards[i].type == attacker_axie.type) {
                total_damage += base_damage * 0.1;
                if (debug) cout << "added damage! total now: " << total_damage << endl;
            }
            ////If the card class is strong against the Axie class, it will deal +15% bonus damage. Or -15% if it’s the other way.
            if (attacker_axie.strength == defender_axie.type) {
                total_damage += base_damage * 0.15;
                if (debug) cout << "added damage! total now: " << total_damage << endl;
            } else if (attacker_axie.type == defender_axie.strength) {
                total_damage -= base_damage * 0.15;
                if (debug) cout << "decreased damage! total now: " << total_damage << endl;
            }
        }

        //// reduce the health of the defender_axie by total_damage
        if (defender_axie.cards[i].card_status == Main::card::chosen_for_attack) {
            total_damage -= defender_axie.cards[i].defence;
            if (debug) cout << "defending with " << defender_axie.cards[i].defence << ", new total: " << total_damage << endl;
        }
    }

    // axie should not gain health if it defence is higher than the total_damage
    if (total_damage < 0){
        if (debug) cout << "total damage lower than 0 ! " << total_damage <<"... setting damage to zero" << endl;
        total_damage = 0;
    }

    // check if health is less than 0, then set axie.alive to false
    if (debug) cout << "attacker: axie id " << attacker_axie.id << " with " << total_damage << " damage!"<< endl;
    if (debug) cout << "defender: axie id " << defender_axie.id << " entered battle with " << defender_axie.health << " health!" << endl;
    defender_axie.health -= total_damage;
    if (defender_axie.health <= 0) {
        defender_axie.alive = false;
        // when axie is defeated, set all its cards to get restocked
        for (int i = 0; i < 4; i++){
            defender_axie.cards[i].card_status = Main::card::wait_for_restock;
        }
    }
    if (debug) cout << "defender axie updated health: " << defender_axie.health << endl;
    if (debug) cout << "-------------------------------------------------------" << endl;
    // store the damage in the playe vectors
    for (int i = 0; i < 2; ++i) {
        if (attackNum == 1) {
            if (attacker_axie.id == p1.axies[i].id) {
                p1.axies[i] = attacker_axie;
            }
            if (defender_axie.id == p2.axies[i].id) {
                p2.axies[i] = defender_axie;
            }
        }
        if (attackNum == 2) {
            if (attacker_axie.id == p2.axies[i].id) {
                p2.axies[i] = attacker_axie;
            }
            if (defender_axie.id == p1.axies[i].id) {
                p1.axies[i] = defender_axie;
            }
        }

        // loop through sorted_axie and find matching axie in p1 and p2 to update the sorted_axies array with the new health and card states
        for (unsigned int j = 0; j < sorted_axies.size(); ++j) {
            if (sorted_axies[j].id == p1.axies[i].id) {
                sorted_axies[j] = p1.axies[i];
            }
            if (sorted_axies[j].id == p2.axies[i].id) {
                sorted_axies[j] = p2.axies[i];
            }
        }
    }
}


Main::axie Find_defender_axie(Main::player p){
    
    //  if only one axie is alive:
    if (p.axies[0].alive && !p.axies[1].alive){
        return p.axies[0];
    } else if (!p.axies[0].alive && p.axies[1].alive){
        return p.axies[1];
    }
    // if both axies are alive and one is front, then choose the front axie:
    if (p.axies[0].position == Main::axie::position::front && p.axies[1].position == Main::axie::position::back){
        return p.axies[0];
    } else if (p.axies[0].position == Main::axie::position::back && p.axies[1].position == Main::axie::position::front){
        return p.axies[1];
    }
    
    //  if both are alive, pick a random axie
    if ((p.axies[0].position == Main::axie::position::back && p.axies[1].position == Main::axie::position::back) ||
            (p.axies[0].position == Main::axie::position::front && p.axies[1].position == Main::axie::position::front)){
        srand(time(0));
        int random_num = rand() % 2;
        if (random_num == 0) {
            return p.axies[0];
        } else {
            return p.axies[1];
        }
    }
    cout << "There is an ERROR with your Axie in --> BattleClass.cpp:172 <--" << endl;
    return Main::axie();
}

int BattleClass::battle(Main::player &p1, Main::player &p2){
    BattleClass battleclass;
    Main main;
    //Establish the players that are battling -- done
    //determine the order of the axies -- done

    //input the attacking axie and the front defending axie into damage calculator


    /// sort axies in the game based on speed, first axie will be the first attacker, form player1
    vector<axie> player1_axies_sorted = battleclass.sort_axies_by_speed(p1);
    vector<axie> player2_axies_sorted = battleclass.sort_axies_by_speed(p2);

    vector<axie> sorted_axies = main.sort_axies(p1, p2);

    // for each axie in sorted_axies, check if any axie has a card that is chosen_for_attack, if not pop from the vector
    for (unsigned int i = 0, non_attacking_cards = 0; i < sorted_axies.size(); i++) {
        for (auto & card : sorted_axies[i].cards) {
            if(card.card_status != Main::card::chosen_for_attack){
                non_attacking_cards += 1;
                if (non_attacking_cards == 4) {
                    sorted_axies.erase(sorted_axies.begin() + i);
                }
            }
        }
    }

    // create a mapping with axie as key and player as value
    std::map<int, Main::player> axie_player_map;

    for (unsigned int i = 0; i < sorted_axies.size(); i++) {
        for (int j = 0; j < 2; j++){
            if (sorted_axies[i].id == p1.axies[j].id) {
                axie_player_map.insert(pair<int, Main::player>(sorted_axies[i].id, p1));
            }
            if (sorted_axies[i].id == p2.axies[j].id) {
                axie_player_map.insert(pair<int, Main::player>(sorted_axies[i].id, p2));
            }
        }
    }


    //// while at least one axie on each player is alive
    for (unsigned int i = 0; i < sorted_axies.size() ; i++){
        if ((p1.axies[0].alive || p1.axies[1].alive)  && (p2.axies[0].alive || p2.axies[1].alive)) {
            // setStrength(sorted_axies[i]);

            Main::axie defender_axie;
            int attackNum = 0;
            // cout << "player-id" << axie_player_map[sorted_axies[i].id].id << endl;
            if (axie_player_map[sorted_axies[i].id].id == p1.id){
                defender_axie = Find_defender_axie(p2);
                attackNum = 1;
            } else if (axie_player_map[sorted_axies[i].id].id == p2.id){
                defender_axie = Find_defender_axie(p1);
                attackNum = 2;
            }

            battleclass.damageCalculator(sorted_axies[i], defender_axie, p1, p2, attackNum, sorted_axies);
        }
    }

    //// set the used card to wait_for_restock
    for (int i = 0; i < 2; i++){
        for (int j = 0; j <4; j++){
            if (p1.axies[i].cards[j].card_status == Main::card::chosen_for_attack){
                p1.axies[i].cards[j].card_status = Main::card::wait_for_restock;
            }
            if (p2.axies[i].cards[j].card_status == Main::card::chosen_for_attack){
                p2.axies[i].cards[j].card_status = Main::card::wait_for_restock;
            }
        }
    }

    // check for winner
    if (!p1.axies[0].alive && !p1.axies[1].alive) {
        p1.losses += 1;
        p2.wins += 1;
        return 2;
    } else if (!p2.axies[0].alive && !p2.axies[1].alive) {
        p2.losses += 1;
        p1.wins += 1;
        return 1;
    }

    /// replenish players energy after attack round
    int energy_replenishment = 2;
    p1.energy += energy_replenishment;
    p2.energy += energy_replenishment;

    battleclass.restock_cards(p1);
    battleclass.restock_cards(p2);

    return 0;
}


