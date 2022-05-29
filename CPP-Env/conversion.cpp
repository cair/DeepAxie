#include <iostream>
#include "BattleClass.cpp"

using namespace std;


class Conv {
public:
//    string convertCard(Main::card &card){
//        std::ostringstream oss;
//        oss << R"("card":{ "id": )" << card.id
//            << R"(, "name": ")" << card.name
//            << R"(", "type": ")" << card.type
//            << R"(", "damage": )" << card.damage
//            << R"(, "defence": )" << card.defence
//            << R"(, "status": )" << card.card_status
//            << " }";
//
//        return oss.str();
//    }
//
//
//    string convertAxie(Main::axie &axie){
//        std::ostringstream oss;
//        oss << R"("axie":{ "id": )" << axie.id
//            << R"(, "type": )" << axie.type
//            << R"(, "health": )" << axie.health
//            << R"(, "alive": )" << axie.alive
//            << R"(, "speed": )" << axie.speed
//            << R"(, "position": )" << axie.position
//            << R"(, "cards": [{)" << convertCard(axie.cards[0])
//            << R"(}, {)" << convertCard(axie.cards[1])
//            << "}] }";
//
//        return oss.str();
//    }
//
//    string convert_test(Main::player &playa){
//        std::ostringstream oss;
//        oss << R"({"player":{ "id": )" << playa.id
//            << R"(, "energy": )" << playa.energy
//            << R"(, "rank": )" << playa.rank
//            << R"(, "axies": [{)" << convertAxie(playa.axies[0])
//            << R"(}, {)" << convertAxie(playa.axies[1])
//            << "}] }}";
//
//        return oss.str();
//    }

    int convert_str_to_int(string type){
        //create cases for the types plant, aqua and beast and return int 0, 1 or 2
        if(type == "plant"){
            return 0;
        }
        else if(type == "aqua"){
            return 1;
        }
        else if(type == "beast"){
            return 2;
        }
        cout << "There is an ERROR with your Axie in --> conversion.cpp:61 <--" << endl;
        return -1;
    }

    // merge to vectors
    vector<int> merge_vectors(vector<int> &vector1, vector<int> vector2){
        vector<int> returnVector;
        for(int i : vector1)
            returnVector.push_back(i);

        for(int i : vector2)
            returnVector.push_back(i);

        return returnVector;
    }

    vector<int> convert_to_bits(int number, unsigned int size){
        int a[size], i;
        vector <int> returnVector1, returnVector2;

        for(i = 0; number > 0; i++){
            a[i] = number % 2;
            number = number / 2;
        }

        for(i=i-1 ;i>=0 ;i--)
            returnVector1.push_back(a[i]);

        while (returnVector1.size()+returnVector2.size() < size)
            returnVector2.push_back(0);

        return merge_vectors(returnVector2,returnVector1);
    }

    // create a function that converts a card into a vector of ints
    vector<int> card_conv(Main::card &card){
        vector<int> returnVector1, returnVector2;

        returnVector1 = convert_to_bits(card.card_status, 2);
        returnVector2 = merge_vectors(returnVector1, convert_to_bits(convert_str_to_int(card.type),2));
        returnVector1 = merge_vectors(returnVector2, convert_to_bits(card.damage,8));
        returnVector2 = merge_vectors(returnVector1, convert_to_bits(card.defence,7));

        return returnVector2;
    }

    vector<int> axie_conv(Main::axie &axie){
        vector<int> returnVector1, returnVector2;

        returnVector1 = card_conv(axie.cards[3]);
        returnVector2 = merge_vectors(returnVector1, card_conv(axie.cards[2]));
        returnVector1 = merge_vectors(returnVector2, card_conv(axie.cards[1]));
        returnVector2 = merge_vectors(returnVector1, card_conv(axie.cards[0]));
        returnVector1 = merge_vectors(returnVector2, convert_to_bits(convert_str_to_int(axie.type),2));
        returnVector1.push_back(axie.position);
        returnVector1.push_back(axie.alive);
        returnVector2 = merge_vectors(returnVector1, convert_to_bits(axie.speed,6));
        returnVector1 = merge_vectors(returnVector2, convert_to_bits(axie.health,10));

        return returnVector1;
    }

    vector<int> convert_to_vec_bit(Main::player &playa){
        vector<int> returnVector1, returnVector2;
        Main::axie axie;
        returnVector1 = axie_conv(playa.axies[1]);
        returnVector2 = merge_vectors(returnVector1, axie_conv(playa.axies[0]));
        returnVector1 = merge_vectors(returnVector2, convert_to_bits(playa.energy,4));
        returnVector2 = merge_vectors(returnVector1, convert_to_bits(playa.rank,12)); /// can achieve rank 4096

        return returnVector2;
    }

    vector<int> card_conv_TEST(Main::card &card){
        vector<int> returnVector;

        returnVector.push_back(card.card_status);
        returnVector.push_back(convert_str_to_int(card.type));
        returnVector.push_back(card.damage);
        returnVector.push_back(card.defence);

        return returnVector;
    }

    vector<int> axie_conv_TEST(Main::axie &axie){
        vector<int> returnVector1, returnVector2;

        returnVector1 = card_conv_TEST(axie.cards[3]);
        returnVector2 = merge_vectors(returnVector1, card_conv_TEST(axie.cards[2]));
        returnVector1 = merge_vectors(returnVector2, card_conv_TEST(axie.cards[1]));
        returnVector2 = merge_vectors(returnVector1, card_conv_TEST(axie.cards[0]));
        returnVector2.push_back(convert_str_to_int(axie.type));
        returnVector2.push_back(axie.position);
        returnVector2.push_back(axie.alive);
        returnVector2.push_back(axie.speed);
        returnVector2.push_back(axie.health);

        return returnVector2;
    }

    vector<int> convert_to_vec_decimal(Main::player &playa){
        vector<int> returnVector1, returnVector2;

        returnVector1 = axie_conv_TEST(playa.axies[1]);
        returnVector2 = merge_vectors(returnVector1, axie_conv_TEST(playa.axies[0]));
        returnVector2.push_back(playa.energy);
        returnVector2.push_back(playa.rank);

        return returnVector2;
    }

};