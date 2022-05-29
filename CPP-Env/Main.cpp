#include <iostream>
#include <vector>
#include <fstream>
#include "include/jsoncpp.cpp"
#include "BattleClass.h"
#include <cstdlib>   // rand and srand
#include <ctime>     // For the time function

void setStrength(Main::axie &a){
    // print out the a.type
    if(a.type == "plant"){
        a.strength = "aqua";
    }else if(a.type == "aqua"){
        a.strength ="beast";
    }else if(a.type == "beast"){
        a.strength = "plant";
    }
}

// a dynamic function that select maximum four numbers randomly from between 1 and 8.
vector<int> Main::selectFourNumbers(player &p){
    BattleClass battleState;
    battleState.restock_cards(p);
    for (auto & axie : p.axies) {
        for (auto & j : axie.cards) {
            if (j.card_status == Main::card::can_be_chosen) {
                j.card_status = Main::card::cannot_be_chosen;
            }
        }
    }
    // create a int array to store maximum 8 numbers.
    vector<int> numbers;
    // loop through p1 and p2's axies and add the card id of cards with status of can_be_chosen to the array.
    for (auto & axie : p.axies) {
        for (auto & j : axie.cards) {
            if (j.card_status == Main::card::cannot_be_chosen) {
                // if the j.id is not already in the array, then push.
                if (find(numbers.begin(), numbers.end(), j.id) == numbers.end() && axie.alive) {
                    numbers.push_back(j.id);
                }
            }
        }
    }
    // get four random numbers from numbers. if the size of numbers is less than 4, then get all numbers.
    if (numbers.size() > 4) {
        // create a vector to store four random numbers.
        vector<int> four_numbers;
        // loop through four times.
        for (int i = 0; i < 4; i++) {
            // Get the system time.
            unsigned seed = time(0);

            // Seed the random number generator.
            srand(seed);

            // get a random number from numbers.
            int random_number = numbers[rand() % numbers.size()];
            if (debug) cout << "random number: " << random_number << endl;
                // check if the random_number is already in four_numbers.
                if (find(four_numbers.begin(), four_numbers.end(), random_number) != four_numbers.end()) {
                    // if the random_number is already in four_numbers, then get another random number.
                    i--;
                }
                else {
                    // if the random_number is not in four_numbers, then push the random number to four_numbers.
                    four_numbers.push_back(random_number);
                }
            // remove the random number from numbers.
            numbers.erase(remove(numbers.begin(), numbers.end(), random_number), numbers.end());
        }
        // return four_numbers.
        numbers = four_numbers;
    }

    for_each(numbers.begin(), numbers.end(), [&](int &i) {
        for (auto &axie : p.axies) {
            for (auto &card : axie.cards) {
                if (card.id == i) {
                    card.card_status = Main::card::can_be_chosen;
                }
            }
        }
    });
    return numbers;
}

int Main::returnOrderNum(Main::axie &a, vector<axie> axies) {
    int num = 0;
    for (int j = 0; j < 4; ++j) {
        if (a.id == axies[0].id) {
            num = 1;
            return num;
        } else if (a.id == axies[1].id) {
            num = 2;
            return num;
        } else if (a.id == axies[2].id) {
            num = 3;
            return num;
        } else if (a.id == axies[3].id) {
            num = 4;
            return num;
        }
    }
    cout << "There is an ERROR with your numer in --> Main.cpp:104 <--" << endl;
    return -1;
}


//// returns cards that CAN be chosen by player
vector<int> Main::showCardsDrawn(Main::player &p, vector<axie> &all_axies_sorted) {

    vector<int> cards_drawn = selectFourNumbers(p);
    // check if the number is between 0 and 3, if so find the card that have the same card id
    Main::axie &_axie1 = p.axies[0];
    Main::axie &_axie2 = p.axies[1];
    cout << "player-id " << p.id << " is choosing cards:" << endl;
    for (unsigned int i = 0; i < cards_drawn.size(); ++i) {
        int uc_num = cards_drawn[i];
        if(uc_num >= 1 && uc_num <= 4){
//            _axie1.cards[uc_num-1].card_status = Main::card::can_be_chosen;
            cout << _axie1.cards[uc_num-1].id << "- Axie "<< Main::returnOrderNum(_axie1,all_axies_sorted)  <<":"<< _axie1.cards[uc_num-1].type <<" - dmg:" << _axie1.cards[uc_num-1].damage <<" - def:"  << _axie1.cards[uc_num-1].defence << endl;

        } else if(uc_num >= 5 && uc_num <= 8){
//            _axie2.cards[uc_num-5].card_status = Main::card::can_be_chosen;
            cout << _axie2.cards[uc_num-5].id  << "- Axie "<<Main::returnOrderNum(_axie2,all_axies_sorted)  <<":"<< _axie2.cards[uc_num-5].type <<" - dmg:"  << _axie2.cards[uc_num-5].damage <<" - def:"  << _axie2.cards[uc_num-5].defence << endl;
        }
    }
    return cards_drawn;
}



//// prints the cards that ARE chosen by player
void Main::PrintChosenCards(Main::player &p) {
    //find cards that are chosen_for_attack and show them for echa axie in the player
    for (int i = 0; i < 4; ++i) {
        if(p.axies[0].cards[i].card_status == Main::card::chosen_for_attack){
            cout << "Player " << p.id << ", card from Axie 1 with id: " << p.axies[0].cards[i].id <<" is Chosen" << endl;
        }
        if(p.axies[1].cards[i].card_status == Main::card::chosen_for_attack){
            cout << "Player " << p.id << ", card from Axie 2 with id: " << p.axies[1].cards[i].id <<" is Chosen" << endl;
        }
    }
}

//// returns Axies sorted by speed, fastest to slowest
std::vector<Main::axie> Main::sort_axies(player &playa1, player &playa2){
    std::vector<Main::axie> axies_to_sort;
    for (auto &axie : playa1.axies)
        axies_to_sort.push_back(axie);

    for (auto &axie : playa2.axies)
        axies_to_sort.push_back(axie);

    std::sort(axies_to_sort.begin(), axies_to_sort.end(), [](const Main::axie &a, const Main::axie &b) {
        return a.speed > b.speed;
    });
    return axies_to_sort;
}


//// returns the position for each axie as a string
string Main::axiePosition(axie &axie, bool is_first, string stats){
    string pos;
    if (!axie.alive){
        stats = "defeated";
    }
    if (is_first){
        if (axie.position == Main::axie::front){
            pos = "          |"+ stats +"";
        } else {
            pos = ""+ stats +"|          ";
        }
    } else {
        if (axie.position == Main::axie::front){
            pos = ""+stats+"|          ";
        } else {
            pos = "          |"+ stats+ "";
        }
    }
    return pos;
}

//// returns all axies on the board as a string
string Main::printAxies(player &playa1, player &playa2, vector<axie> axies){
    string first_axie, second_axie, third_axie, fourth_axie;
    for (int i = 0; i < 4; i++){
        stringstream s_position, s_health;
        s_position << i + 1;
        s_health << axies[i].health;

        if(playa1.axies[0].id == axies[i].id)
            first_axie = axiePosition(axies[i], true, (s_position.str() + ":" + axies[i].type + ":" + s_health.str()));
        else if(playa2.axies[0].id == axies[i].id)
            second_axie = axiePosition(axies[i], false, (s_position.str() + ":" + axies[i].type + ":" + s_health.str()));
        else if(playa1.axies[1].id == axies[i].id)
            third_axie = axiePosition(axies[i], true, (s_position.str() + ":" + axies[i].type + ":" + s_health.str()));
        else if(playa2.axies[1].id == axies[i].id)
            fourth_axie = axiePosition(axies[i], false, (s_position.str() + ":" + axies[i].type + ":" + s_health.str()));
    }
    return first_axie + "     " + second_axie + "\n" + third_axie + "     " + fourth_axie;
}

//// prints the game board with Round, Energy and all axies
string Main::PrintGameBoard(Main::player &playa1, Main::player &playa2, int round) {
    Main main;
    stringstream ss;

    // sort axies by speed.
    vector<Main::axie> axies = main.sort_axies(playa1, playa2);
    ss  << "----------------------------------------------------\n"
        << "Round: " << round << " - (attack order : type : health)\n"
        << "Energy: " << playa1.energy 
        << "\n   back   |           front          |   back\n"
        << main.printAxies(playa1, playa2, axies) << endl;
    
    return ss.str();
}

void Main::SelectCards(Main::player &player, vector<Main::axie> all_axies_sorted) {
    Main main;
    // print the cards that are available to chose for attack
    vector<int> cards_drawn = main.showCardsDrawn(player, all_axies_sorted);
    int choose_card_input;
    cout << "Choose cards, enter 0 to skip " << endl;

    for (int i = 0; i < 2; ++i) {
        if (player.energy == 0)
            break;

        cin >> choose_card_input;
        if (choose_card_input == 0)
            break;
        if (choose_card_input <= 4 && player.axies[0].cards[choose_card_input-1].card_status == Main::card::can_be_chosen){
            player.axies[0].cards[choose_card_input-1].card_status = Main::card::chosen_for_attack;
        }
        else if (choose_card_input <= 8 && player.axies[1].cards[choose_card_input-5].card_status == Main::card::can_be_chosen){
            player.axies[1].cards[choose_card_input-5].card_status = Main::card::chosen_for_attack;
        }

        player.energy -= 1;
    }

    main.PrintChosenCards(player);
};

void Main::SelectCards(Main::player &player, vector<int> input){
    // print the cards that are available to chose for attack

    for (int i = 0; i < 2; ++i) {
        if (player.energy == 0)
            break;

        if (input[i] == 0)
            break;
        if (input[i] <= 4 && player.axies[0].cards[input[i]-1].card_status == Main::card::can_be_chosen){
            player.axies[0].cards[input[i]-1].card_status = Main::card::chosen_for_attack;
            player.energy -= 1;
        }
        else if (input[i] <= 8 && player.axies[1].cards[input[i]-5].card_status == Main::card::can_be_chosen){
            player.axies[1].cards[input[i]- 5].card_status = Main::card::chosen_for_attack;
            player.energy -= 1;
        }
        
    }
};

//// Card selection state
class Card_Selection_State : public State {
    Main main;
public:
    void UpdateState()  override{
//        cout << "Cards are selected!"<< endl;
    };


    void SelectCards(Main::player &player, const vector<Main::axie> &all_axies_sorted){
        // print the cards that are available to chose for attack
        main.SelectCards(player, all_axies_sorted);
    };

    void SelectCards(Main::player &player, const vector<int> &input){
        // print the cards that are available to chose for attack
        main.SelectCards(player, input);
    };

    // Constructor
    Card_Selection_State(Main::player &p1, Main::player &p2){
//        main.PrintChosenCards(p1);
//        cout << "Card_Selection_State is created!" << endl;

        vector<Main::axie> sorted_a = main.sort_axies(p1, p2);
        cout << "----------------------------------------------------" << endl;
        SelectCards(p1,sorted_a);
        cout << "----------------------------------------------------" << endl;
        SelectCards(p2,sorted_a);

    }
    ~Card_Selection_State(){
//        cout << "Card_Selection_State is discarded" << endl;
    };
};

//// Attack state
class Attack_State: public State{
    Main main;
    BattleClass battleclass;
public:
    void UpdateState() override{
        cout << "attack done" << endl;
    }
//    void PrintPlayer(Main::player &p1, Main::player &p2){
//        main.PrintGameBoard(p1, p2, 1);
//        cout << "Player1: " << p1.axies->cards[0].card_status << endl;
//        cout << "Player2: "<< p2.axies->type << endl;
//    }

    // Constructor
    Attack_State(Main::player &p1, Main::player &p2){
//        cout << "Attack_State is created" << endl;
        battleclass.battle(p1, p2);
    }

    ~Attack_State(){
//        cout << "Attack_State is discarded!" << endl;
    };
};

//// State Controller
class StateController{
private:
    State* currentState = nullptr;

public:
    void Init(Main::player &p1, Main::player &p2){
        currentState = new Card_Selection_State(p1,p2);
    }

    void Update(){
        currentState->UpdateState();
    }

    void TransitionTo (string c, Main::player &p1, Main::player &p2){
        delete currentState;

        if(c == "attack"){
            currentState = new Attack_State(p1, p2);
        } else if(c == "choose cards"){
            currentState = new Card_Selection_State(p1, p2);
        }
    }

    ~StateController(){
        delete currentState;
    }
};

//// Create player with axies and cards, reads from JSON file.
Main::player Main::createPlayer(int team_id){
     ifstream file("axie_teams.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(file, obj);
    stringstream ss;
    string extra = "";
    ss << team_id;
    if (team_id <= 9)
        extra ="0";

    Json::Value team = obj["Team-" + extra + ss.str()];
    Main::player player;

    // json_counter is 1 because we start axie_num with 0, and Axie in the Json start with 1
    for (int axie_num = 0, json_counter = 1; axie_num < 2 ; ++axie_num, json_counter++) {
        player.energy = 2;
        player.id = team["Team-id"].asInt();
        string json_team = "Axie-0" + to_string(json_counter);

        // set axie id, type, health (calcultated), speed, skille, morale
        player.axies[axie_num].id = team[json_team]["Axie-id"].asInt();
        player.axies[axie_num].type = team[json_team]["Type"].asString();
        setStrength(player.axies[axie_num]);
        player.axies[axie_num].health = team[json_team]["Health"].asInt() * 6 + 150;
        player.axies[axie_num].speed = team[json_team]["Speed"].asInt();
        player.axies[axie_num].skill = team[json_team]["Skill"].asInt();
        player.axies[axie_num].morale = team[json_team]["Morale"].asInt();
        if (team[json_team]["Placement"].asString() == "back"){
            player.axies[axie_num].position = Main::axie::position::back;
        }

        // loads two cards for each axie
        for (int card_num = 0; card_num < 2; ++card_num) {
            player.axies[axie_num].cards[card_num].type = team[json_team]["Cards"][card_num]["type"].asString();
            player.axies[axie_num].cards[card_num].damage = team[json_team]["Cards"][card_num]["attack"].asInt();
            player.axies[axie_num].cards[card_num].defence = team[json_team]["Cards"][card_num]["defence"].asInt();

            // player.axies[axie_num].cards[card_num].card_status = Main::card::wait_for_restock;
        }

        // Copies the two loaded cards to give each axie 4 cards
        player.axies[axie_num].cards[2] = player.axies[axie_num].cards[0];
        player.axies[axie_num].cards[3] = player.axies[axie_num].cards[1];

        int num = 1;
        if (axie_num == 1) {
            num = 5;
        }

        for (int k = 0; k < 4; ++k) {
            player.axies[axie_num].cards[k].id = k + num;
        }
    }
    return player;
}


// create a function that checks if all cards card_status for each axie is chosen_for_attack if true,
// then change the card_status to wait_for_restock

int main() {
    vector<Main::player> players;
    Main main;
    StateController stateController;


    // ask user to choose Axie teams
    for (int i = 0; i < 2; i++) {
        int axie_team;
        cout << "Choose your axie team: (1-20)" << endl;
        cin >> axie_team;
        players.push_back(main.createPlayer(axie_team));
    }

    int game_round = 1;
    cout << main.PrintGameBoard(players[0],players[1],game_round) << endl;

    stateController.Init(players[0], players[1]);

    string str = "attack";
    while (game_round != 0){
        stateController.Update();


        // stateConroller.TransitionTo returns value 0 if game is over
        stateController.TransitionTo(str, players[0], players[1]);
        if ((!players[0].axies[0].alive && !players[0].axies[1].alive) || (!players[1].axies[0].alive && !players[1].axies[1].alive))
            break;


        if (str == "attack"){
            str = "choose cards";
            cout << main.PrintGameBoard(players[0],players[1],game_round) << endl;
        } else if (str == "choose cards") {
            str = "attack";
            game_round++;
        }
    }

    if (!players[0].axies[0].alive && !players[0].axies[1].alive) {
        cout << "Player 2 has won!" << endl;
    }
    else if (!players[1].axies[0].alive && !players[1].axies[1].alive) {
        cout << "Player 1 has won!" << endl;
    }
    return 0;
}