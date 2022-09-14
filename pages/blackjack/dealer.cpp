#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include <emscripten.h>
#include "dealer.h"

using namespace std;



namespace dealer {

string Dealer::draw(int &total) {
    if (countCards <= 0) {
        return "Deck is empty.";
    }
    else {
        int maxCard = 0;
        string show;
        do {
            maxCard = 0;
            newCard = (rand() % (MAXVALUE - MINVALUE + 1)) + MINVALUE;
            deck[newCard] += 1;
            if (deck[newCard] > 4) {
                maxCard = 1;
                // printf("No more %ds!\n", newCard);
            }
        } while(maxCard == 1);

        if (newCard == 1) {
            ace = 1;
            // cout << "Ace" << endl;
        }
        if (newCard > 10) {
            // cout << "Card: " << newCard << endl;
            total += 10;
            // cout << "New total: " << total << endl;
        }
        else if (newCard == 1) {
            total += 11;
        }
        else {
            // cout << "1" << endl;
            total += newCard;
        }
        if (ace == 1 && total > 21) {
            total -= 10;
            ace = 0; 
        }
    
    if (newCard < 2 || newCard > 10) {
        show = display[newCard];
    }
    else
        show = to_string(newCard);
    countCards--;
    return show;
    }
}


int Dealer::deckCount() {
    return countCards;
}

void Dealer::resetAce() {
    ace = 0;
}

void Dealer::setDealerCards(int &total) {
    dealerCard1 = draw(total);
    dealerCard2 = draw(total);
}

string Dealer::getDealerCard1() {
    return dealerCard1;
}

string Dealer::getDealerCard2() {
    return dealerCard2;
}

void Dealer::resetDealerCards() {
    dealerCard1 = "";
    dealerCard2 = "";
}

} // End of namespace Dealer.


