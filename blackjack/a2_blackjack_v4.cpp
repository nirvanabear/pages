#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <map>
#include <emscripten.h>
#include "dealer.h"

using namespace std;
using namespace dealer;

/*
Understand: 	Analyze the problem statement
    Random 1-10 card generator, deals 2, then deals on command until
    user ends, total is 21 or total is over 21.
Match: 	 	Recall problems you have seen, determine similarities
Plan: 			Perform the steps, write them out, describe the process

    display option to input seed, or just press enter to use time random
        console out
        console in, but where to save?
            save to string
            getline?


    create negative seed int
    user option to input their own seed
    if seed non negative:
        random seeding gets that number
    else
        use random seeding with time
    random number generate twice
    display numbers
    track total
    create loop if not 21
        random number generator
        display number
        dislay total
        if 21, display win
            break
        if over 21, display loss
            break
        user option to break loop


Implement: 	Translate your plan to code
Review: 		Make sure your code works, handle corner cases
Evaluate: 		Determine time & space efficiency of solution
*/


    /* TODO //
        functions for hands

        Multiple rounds of hands

        How many decks??
        Burn card
        Place bet
        Deal one up to player
        Deal one down to dealer
        Deal one up to player
        Deal one up to dealer
        If dealer shows ace, 
            insurance bet option
            peek and if blackjack, everybody loses (excpt bjack)
        Players hand
            if blackjack, pay 3/2 bet
            hit 
                (cannot then split or double down)
            stand
            double down
                only one more card then double the bet
            surrender
                forfeit but keep half the money
            split
                doubles bet
                separate cards and deal a hand to first card
                deal hand to second card
                second split is possible
        */


// TODO // 
    // Reimplement arguments into a single array.
        // But maybe best practice is to move bets and totals to class?

    // Deck is empty breaks the program.

    // Create suit choice for the draw function

    /* Dealer should have hit. The total was only 16:
        Place your bets!

        Bet: ￦0

        Dealer shows: A, ?

        Place insurance bet, or press [enter] to decline: 
        10
        First cards: 6, K
        Total: 16
        Do you want another card? (y/n): n

        Dealer's Turn: 


        Dealer's first cards: A, A
        Dealer's total: 12

        Dealer's card: 4
        Dealer's total: 16


        Standoff!
        Current total winnings: ￦-5
        Do you want to play again? (y/n): 
    */

    // Solved: Card total getting skipped because ace token didn't reset

    // Solved: Not all outcomes properly add or subtract winnings


    // emcc a2_blackjack_v3.cpp dealer.cpp -o blackjack.html -sEXPORTED_FUNCTIONS=_main -O3 --shell-file html_template/shell_minimal.html


// Constants
const int MAXVALUE = 13;
const int MINVALUE = 1;

//Function prototype
unsigned long long int inputToInt(int&, string);
string yesOrNo(char*);
string convertYesNo(int);
void playersHand(int &, int &, int &, int &, int &, int, int &, Dealer &, int &);
void dealersHand(int &, int &, int &, int &, int, int, int, Dealer &, int &);
void outcomes(int, int, int, int, int, int, int, int, int &);


int main() {

    int nullResponse = 0;
    // string introPrompt ="Input one of the following options:\n\t• Input integer for seeded randomizer (19-digit max)\n\t• Press [enter] for non-deterministic randomizer\nInput: ";
    // unsigned long long int seed = inputToInt(nullResponse, introPrompt);  

    // // srand is seeded globally
    // if (nullResponse)
    srand(time(0)); 
    // else
    //     srand(seed);

    Dealer pokerAlice;
    string keepPlaying;
    int playerWinnings = 0;

    do {
        int dealerTotal = 0;
        int dealerBJack = 0;
        pokerAlice.setDealerCards(dealerTotal);

        int total = 0;
        int blackjack = 0;
        int bust = 0;
        int bet = 0;
        int insuranceBet = 0;

        playersHand(total, blackjack, bust, bet, insuranceBet, dealerTotal, dealerBJack, pokerAlice, playerWinnings);

        int dealerBust = 0;

        dealersHand(dealerTotal, dealerBJack, dealerBust, bet, bust, blackjack, total, pokerAlice, playerWinnings);

        outcomes(total, dealerTotal, bust, dealerBust, blackjack, dealerBJack, bet, insuranceBet, playerWinnings);

        pokerAlice.resetDealerCards();

        char anotherHand[] = "Do you want to play again?";
        cout << anotherHand << endl;
        // keepPlaying = yesOrNo(anotherHand);
         
        bool playBool = EM_ASM_INT({
            let message = "Do you want to play again? \nCurrent Winnings: " + $0;
            let play = confirm(message);
            return play;}, playerWinnings
            );
        keepPlaying = convertYesNo(playBool);         

    } while(pokerAlice.deckCount() > 0 && (keepPlaying == "y" || keepPlaying == "Y"));
    // TODO //  Messaging and game flow when deck is empty.

    if (pokerAlice.deckCount() <= 0) {
        printf("The deck is empty! Go home!\n");
        int alert = EM_ASM_INT(
            let alert1 = alert("The deck is empty! Go home!\n");
            return 0;
        );
    }

    if (playerWinnings > 0) {
        printf("\nYou've won ￦%d.\nCongratulations!\n", playerWinnings);
        int alert = EM_ASM_INT({
            let message = "\nYou've won ￦" + $0 + "\nCongratulations!\n";
            let alert1 = alert(message);
            return 0;}, playerWinnings
        );
    }
    else if (playerWinnings < 0) {
        printf("You've lost ￦%d.\nSorry, sucker.\n", playerWinnings);
        int alert = EM_ASM_INT({
            let message = "You've lost ￦"  + $0 + "\nSorry, sucker.\n";
            let alert1 = alert(message);
            return 0;}, playerWinnings
        );
    }
    else
        printf("Hope you had fun!\n");
        int alert = EM_ASM_INT(
            let alert1 = alert("Hope you had fun!\n");
            return 0;
        );

    return 0;
}



unsigned long long int inputToInt(int &nullResponse, string introPrompt = "Please input a valid positive integer or press [enter]: ") {

    unsigned long long int output = 0;
    string userStr = ""; // Adapts to any size.
    nullResponse = 0;

    printf("%s\n", introPrompt.c_str());  

    for (;;){

        userStr = "";
        getline(cin, userStr); 
        cin.clear();



        if (userStr.compare("") == 0) {
            userStr = "0";
            nullResponse = 1;
            break; 
        }      
        else {
            int checkInt = 1;
            for (int i = 0; i < userStr.length(); i++) {
                if (!(isdigit(userStr[i]) || userStr[i] == '\0')) {
                    printf("%c is not an integer. ", userStr[i]);
                    checkInt = 0;
                }
            }
            if (checkInt){
                break;
            }
            else {
                cout << "Please input a valid positive integer or press [enter]: ";
            }  
        } 
    }

    char* userCstr = nullptr;
    userCstr = new char[userStr.size() + 1];
    strcpy(userCstr, userStr.c_str());
    output = strtoull(userCstr, nullptr, 10); //Never throws exceptions.
    delete [] userCstr;

    return output;
}


string yesOrNo(char* query) {
    string response;
    int valid = 0;
    while (valid == 0) {
        printf("%s (y/n): ", query);
        cin >> response;
        cin.ignore();
        if (response[0] == 'y' || response[0] == 'Y' || response[0] =='N' || response[0] == 'n') {
            if (response.size() == 1)
                valid = 1;
            }
        if (response == "n")
            break;
    }
    return response;
}

// TODO // not sure if this should be str or char
string convertYesNo(int jsOut) {
    if (jsOut == 0) {
        return "n";
    }
    else if (jsOut == 1) {
        return "y";
    }
    else
        return "0";
}


void playersHand(int &total, int &blackjack, int &bust, int &bet, int &insuranceBet, int dealerTotal, int &dealerBJack, Dealer &pokerAlice, int &playerWinnings) {

    int nullResponse = 0;
    total = 0;
    blackjack = 0;
    bust = 0;
    bet = 0;

    pokerAlice.resetAce();

    // bet = inputToInt(nullResponse, "Place your bets!");
    bet = EM_ASM_INT({   
        let total = $0;
        let message = "Current winnings: " + total.toString() + "\nPlace your bets!";
        let number = prompt(message);
        return number; 
        }, playerWinnings
    );


    cout << "Bet: ￦" << bet << endl;

    cout << endl << "Dealer shows: " << pokerAlice.getDealerCard1() << ", ?" << endl << endl;

    // if (pokerAlice.getDealerCard1() == "A") {
    //     // int noInsurance = 0;
    //     // insuranceBet = inputToInt(noInsurance, "Place insurance bet, or press [enter] to decline: ");
    //     insuranceBet = EM_ASM_INT({
    //         let dealerCard = UTF8ToString($0);
    //         let message = "Dealer shows: " + dealerCard + "\nPlace insurance bet, or press [enter] to decline: ";
    //         let number = prompt(message);
    //         return number;
    //         }, pokerAlice.getDealerCard1().c_str()
    //     );
        

    }
    // TODO // Change in playerWinnings if insurance bet loses.
    
    if (dealerTotal == 21) {
        dealerBJack = 1;
    }
    else {

        string firstCard;
        string currentCard;

        firstCard = pokerAlice.draw(total);
        currentCard = pokerAlice.draw(total);
        
        string playersCards = "Player's cards: " + firstCard + ", " + currentCard;
        printf("Player's cards: %s, %s\n", firstCard.c_str(), currentCard.c_str());
        string cardTotal = "Total: " + to_string(total);
        printf("Total: %d\n", total);

        if (total == 21) {
            blackjack = 1;
            cout << "Blackjack!" << endl;
            cardTotal += "\nBlackjack!";
            int alert = EM_ASM_INT({
                let message = UTF8ToString($0);
                let alert1 = alert(message);
                return 0;}, cardTotal.c_str()
            );
        }

        string deal;
        int counter = 0;

        do {
            if (counter) {
                currentCard = pokerAlice.draw(total);

                playersCards += ", " + currentCard;
                printf("Card: %s\n", currentCard.c_str());
                cardTotal = "Total:" + to_string(total);
                printf("Total: %d\n", total);
            }

            if (total == 21) {
                cout << "Vingt-et-Un!" << endl;
                cardTotal += "\nVingt-et-Un!";
                int alert = EM_ASM_INT({
                    let message = UTF8ToString($0) + "\nPlayer's total: " + $1 + "\nVingt-et-Un!";
                    let alert1 = alert(message);
                    return 0;}, playersCards.c_str(), total
                );
                break;
            }
            else if (total > 21) {
                cout << "Bust!" << endl;
                cardTotal += "\nBust!";
                bust = 1;
                break;
            }

            char anotherCard[] = "Do you want another card?";
            cout << anotherCard << endl;
            // deal = yesOrNo(anotherCard);
            bool dealBool = EM_ASM_INT({
                let message = "Dealer shows: " + UTF8ToString($0) + ", ?\n" + UTF8ToString($1) + "\nPlayer's total: " + $2 + "\nDo you want another card?";
                let hitStay = confirm(message);
                return hitStay;}, pokerAlice.getDealerCard1().c_str(), playersCards.c_str(), total
            );
            deal = convertYesNo(dealBool); 
            counter++;
        }
        while(deal == "y" || deal == "Y");
    }
}


void dealersHand(int &dealerTotal, int &dealerBJack, int &dealerBust, int &bet, int bust, int blackjack, int total, Dealer &pokerAlice, int &playerWinnings) {

    string firstCard;
    string currentCard;

    pokerAlice.resetAce();

    if (!bust) {

        cout << endl << "Dealer's Turn: " << endl << endl;
        // cin.get();
        int alert = EM_ASM_INT(
            let alert1 = alert("Dealer's Turn");
            return 0;);

        // firstCard = pokerAlice.draw(dealerTotal);
        // currentCard = pokerAlice.draw(dealerTotal);

        // string dealersCards = "Dealer's cards: %s, %s\n", pokerAlice.getDealerCard1().c_str(), pokerAlice.getDealerCard2().c_str();
        printf("Dealer's cards: %s, %s\n", pokerAlice.getDealerCard1().c_str(), pokerAlice.getDealerCard2().c_str());
        // string dealersTotal = "Dealer's total: %d\n", dealerTotal;
        printf("Dealer's total: %d\n", dealerTotal);
        // cin.get(); 

        alert = EM_ASM_INT({
            // TODO // Keep working from here to update.
            let message =  "Dealer's cards: " + UTF8ToString($0) + ", " + UTF8ToString($1) + "\nDealer's total: " + $2;
            let alert1 = alert(message);
            return 0;}, pokerAlice.getDealerCard1().c_str(), pokerAlice.getDealerCard2().c_str(), dealerTotal
        );

        if (dealerTotal == 21) {
            dealerBJack = 1;
        }   

        string allDealerCards = pokerAlice.getDealerCard1() + ", " + pokerAlice.getDealerCard2();

        while(dealerTotal < 17 && dealerTotal < total) {
            currentCard = pokerAlice.draw(dealerTotal);
            allDealerCards += ", " + currentCard;
            printf("Dealer's card: %s\n", currentCard.c_str());
            // dealersTotal = "Dealer's total: %d\n\n", dealerTotal;
            printf("Dealer's total: %d\n\n", dealerTotal);
            // cin.get();
            // dealersCards += ", " + currentCard.c_str();
            alert = EM_ASM_INT({
                let message =  "Dealer's cards: " + UTF8ToString($0) + "\nDealer's total: " + $1;
                let alert1 = alert(message);
                return 0;}, allDealerCards.c_str(), dealerTotal
            );

        }
        if (dealerTotal > 21) {
            dealerBust = 1;
        }
    }
}


void outcomes(int total, int dealerTotal, int bust, int dealerBust, int blackjack, int dealerBJack, int bet, int insuranceBet, int &playerWinnings) {
    if (dealerBust == 1) {
        cout << "Dealer busts!" << endl;
        cout << "You win ￦" << bet << "!" << endl;
        playerWinnings += bet;
        int alert = EM_ASM_INT({
                    let message = "Dealer busts! \nYou win ￦" + $0;
                    let alert1 = alert(message);
                    return 0;}, bet
                );
    }
    else if ((dealerTotal > total && !dealerBust) || bust) {
        cout << "You lose ￦" << bet << "!" << endl;
        playerWinnings -= bet;
        // cout << "Winnings: " << playerWinnings << endl;
        int alert = EM_ASM_INT({
                    let message = "You lose ￦" + $0;
                    let alert1 = alert(message);
                    return 0;}, bet
                );
    }
    else if (dealerTotal == total) {
        if (blackjack && dealerBJack) {
            cout << "Blackjack standoff!" << endl;
            int alert = EM_ASM_INT({
                    let alert1 = alert("Blackjack standoff!");
                    return 0;}
            );
        }
        else if (blackjack) {
            cout << "Blackjack! You win ￦" << bet << "!" << endl;
            playerWinnings += bet;
            int alert = EM_ASM_INT({
                    let message = "Blackjack! You win ￦" + $0 + "!";
                    let alert1 = alert(message);
                    return 0;}, bet
            );
        }
        else if (dealerBJack) {
            cout << "Dealer's blackjack! You lose ￦" << bet << "!" << endl;
            int alert = EM_ASM_INT({
                    let message = "Dealer's blackjack! You lose ￦" + $0;
                    let alert1 = alert(message);
                    return 0;}, bet
            );
            // if (insuranceBet > 0) {
            //     cout << "But you bet insurance of ￦" << insuranceBet << endl;
            //     cout << "You win ￦" << insuranceBet * 2 << endl;
            //     playerWinnings += insuranceBet * 2;
            //     int alert = EM_ASM_INT({
            //         let message = "But you won insurance bet of ￦" + $0;
            //         let alert1 = alert(message);
            //         return 0;}, (insuranceBet * 2)
            //     );
            // }
            playerWinnings -= bet;
        }
        else
            cout << "Standoff!" << endl;
            int alert = EM_ASM_INT({
                    let alert1 = alert("Standoff!");
                    return 0;}
            );
    }
    else {
        cout << "You win ￦" << bet << "!" << endl;
        playerWinnings += bet;
        int alert = EM_ASM_INT({
                    let message = "You win ￦" + $0;
                    let alert1 = alert(message);
                    return 0;}, bet
                );
    }
    
    printf("Current total winnings: ￦%d\n", playerWinnings);
}

