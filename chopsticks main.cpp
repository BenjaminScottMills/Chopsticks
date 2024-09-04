// Rules at en.wikipedia.org/wiki/Chopsticks_(hand_game), this here is playing with the additional rules added by the cutoff and suicide variations (that's how it was played when I was a kid).
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

enum result : char {unknown, p1Win, loop, p2Win, invalid};
enum option : char {hit11, hit12, hit21, hit22, combine1, combine2, combine3};

class Gamestate;

class Gamestate
{
    public:
        short p1h1;
        short p1h2;
        short p2h1;
        short p2h2;
        unsigned short distanceToOutcome = 0;
        short id = 6;
        bool isP1Turn;
        result optResult = unknown;
        vector<unsigned int> indexesThatRequireThis = {};

        void process()
        {
            if (p1h1 > 4)
            {
                p1h1 = 0;
            }
            if (p1h2 > 4)
            {
                p1h2 = 0;
            }
            if (p2h1 > 4)
            {
                p2h1 = 0;
            }
            if (p2h2 > 4)
            {
                p2h2 = 0;
            }

            id = p1h1+(10*p1h2)+(100*p2h1)+(1000*p2h2);
            if(isP1Turn) id *= -1;
            
            if(optResult == invalid) return;

            if(p1h1 == 0 && p1h2 == 0)
            {
                optResult = p2Win;
            }
            else if(p2h1 == 0 && p2h2 == 0)
            {
                optResult = p1Win;
            }
        }

        Gamestate(short inP1h1, short inP1h2, short inP2h1, short inP2h2, bool inIsP1Turn, result inResult = unknown)
        {
            p1h1 = inP1h1;
            p1h2 = inP1h2;
            p2h1 = inP2h1;
            p2h2 = inP2h2;
            isP1Turn = inIsP1Turn;
            optResult = inResult;
            
            process();
        }
        
        Gamestate() {process();}

        void output()
        {
            cout << "Computer hands:    " << p1h1 << "  " << p1h2 << "\n\n";
            cout << "Player hands:      " << p2h1 << "  " << p2h2 << "\n\n";
            
            if((p1h1 == 0 && p1h2 == 0) || (p2h1 == 0 && p2h2 == 0)) return;
            
            if(isP1Turn) cout << "It is the computer's turn.\n";
            else cout << "It is your turn.\n";
        }

        Gamestate getResult(option optToCheck)
        {
            Gamestate temp;
            short sumOfHands;

            if(isP1Turn)
            {
                switch (optToCheck)
                {
                    case hit11:
                        if(p1h1 != 0 && p2h1 != 0)
                        {
                            temp = Gamestate(p1h1, p1h2, p2h1 + p1h1, p2h2, false);
                            return temp;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                    case hit12:
                        if(p1h1 != 0 && p2h2 != 0)
                        {
                            temp = Gamestate(p1h1, p1h2, p2h1, p2h2 + p1h1, false);
                            return temp;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                    case hit21:
                        if(p1h2 != 0 && p2h1 != 0)
                        {
                            temp = Gamestate(p1h1, p1h2, p2h1 + p1h2, p2h2, false);
                            return temp;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                    case hit22:
                        if(p1h2 != 0 && p2h2 != 0)
                        {
                            temp = Gamestate(p1h1, p1h2, p2h1, p2h2 + p1h2, false);
                            return temp;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                    case combine1:
                        temp = Gamestate(p1h1, p1h2, p2h1, p2h2, false);
                        sumOfHands = p1h1+p1h2;
                        if(sumOfHands > 4)
                        {
                            temp.p1h1 = 4;
                            temp.p1h2 = sumOfHands - 4;
                        }
                        else
                        {
                            temp.p1h1 = sumOfHands;
                            temp.p1h2 = 0;
                        }

                        if((temp.p1h1 == p1h1 && temp.p1h2 == p1h2) || (temp.p1h1 == p1h2 && temp.p1h2 == p1h1)) return Gamestate(0, 0, 0, 0, true, invalid);

                        temp.process();
                        return temp;
                    case combine2:
                        temp = Gamestate(p1h1, p1h2, p2h1, p2h2, false);
                        sumOfHands = p1h1+p1h2;
                        if(sumOfHands > 6)
                        {
                            return Gamestate(0, 0, 0, 0, true, invalid);
                        }
                        else if(sumOfHands > 4)
                        {
                            temp.p1h1 = 3;
                            temp.p1h2 = sumOfHands - 3;
                        }
                        else if(sumOfHands > 1)
                        {
                            temp.p1h1 = sumOfHands-1;
                            temp.p1h2 = 1;
                        }
                        else
                        {
                            return Gamestate(0, 0, 0, 0, true, invalid);
                        }

                        if((temp.p1h1 == p1h1 && temp.p1h2 == p1h2) || (temp.p1h1 == p1h2 && temp.p1h2 == p1h1)) return Gamestate(0, 0, 0, 0, true, invalid);

                        temp.process();
                        return temp;
                    case combine3:
                        if(p1h1+p1h2 == 4 && p1h1 != 2)
                        {
                            temp = Gamestate(2, 2, p2h1, p2h2, false);
                            temp.process();
                            return temp;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                }
            }
            else
            {
                switch (optToCheck)
                {
                    case hit11:
                        if(p1h1 != 0 && p2h1 != 0)
                        {
                            temp = Gamestate(p2h1 + p1h1, p1h2, p2h1, p2h2, true);
                            return temp;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                    case hit12:
                        if(p1h2 != 0 && p2h1 != 0)
                        {
                            temp = Gamestate(p1h1, p1h2 + p2h1, p2h1, p2h2, true);
                            return temp;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                    case hit21:
                        if(p1h1 != 0 && p2h2 != 0)
                        {
                            temp = Gamestate(p1h1 + p2h2, p1h2, p2h1, p2h2, true);
                            return temp;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                    case hit22:
                        if(p1h2 != 0 && p2h2 != 0)
                        {
                            temp = Gamestate(p1h1, p1h2 + p2h2, p2h1, p2h2, true);
                            return temp;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                    case combine1:
                        temp = Gamestate(p1h1, p1h2, p2h1, p2h2, true);
                        sumOfHands = p2h1+p2h2;
                        if(sumOfHands > 4)
                        {
                            temp.p2h1 = 4;
                            temp.p2h2 = sumOfHands - 4;
                        }
                        else
                        {
                            temp.p2h1 = sumOfHands;
                            temp.p2h2 = 0;
                        }

                        if((temp.p2h1 == p2h1 && temp.p2h2 == p2h2) || (temp.p2h1 == p2h2 && temp.p2h2 == p2h1)) return Gamestate(0, 0, 0, 0, true, invalid);

                        temp.process();
                        return temp;
                    case combine2:
                        temp = Gamestate(p1h1, p1h2, p2h1, p2h2, true);
                        sumOfHands = p2h1+p2h2;
                        if(sumOfHands > 6)
                        {
                            return Gamestate(0, 0, 0, 0, true, invalid);
                        }
                        else if(sumOfHands > 4)
                        {
                            temp.p2h1 = 3;
                            temp.p2h2 = sumOfHands - 3;
                        }
                        else if(sumOfHands > 1)
                        {
                            temp.p2h1 = sumOfHands-1;
                            temp.p2h2 = 1;
                        }
                        else
                        {
                            return Gamestate(0, 0, 0, 0, true, invalid);
                        }

                        if((temp.p2h1 == p2h1 && temp.p2h2 == p2h2) || (temp.p2h1 == p2h2 && temp.p2h2 == p2h1)) return Gamestate(0, 0, 0, 0, true, invalid);

                        temp.process();
                        return temp;
                    case combine3:
                        if(p2h1+p2h2 == 4 && p2h1 != 2)
                        {
                            temp = Gamestate(p1h1, p1h2, 2, 2, true);
                            temp.process();
                            return temp;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                }
            }
        }
        
        unsigned int findIndex(const vector<Gamestate> & previousStates)
        {
            unsigned int size = previousStates.size();

            for(unsigned int i = 0; i < size; i++)
            {
                if(id == previousStates[i].id)
                {
                    return i;
                }
            }
            return -1;
        }

        void updatePreviousStates(vector<Gamestate> & previousStates, bool setAsUnknown)
        {
            int size = previousStates.size();

            for(int i = 0; i < size; i++)
            {
                if(id == previousStates[i].id)
                {
                    if(!setAsUnknown)
                    {
                        previousStates[i].optResult = optResult;
                        previousStates[i].distanceToOutcome = distanceToOutcome;
                    }
                    return;
                }
            }
        
            previousStates.push_back(*this);
            
            if(setAsUnknown)
            {
                previousStates[size].optResult = unknown;
            }
        }

        void updateResult(vector<Gamestate> & previousStates, bool checkingForLoops)
        {
            int size = previousStates.size();

            if(optResult != unknown && optResult != loop) return;

            for(int i = 0; i < size; i++)
            {
                if(id == previousStates[i].id)
                {
                    if(checkingForLoops)
                    {
                        optResult = previousStates[i].optResult;
                    }
                    else
                    {
                        if (previousStates[i].optResult == unknown)
                        {
                            optResult = loop;
                        }
                        else
                        {
                            optResult = previousStates[i].optResult;
                        }
                    }

                    distanceToOutcome = previousStates[i].distanceToOutcome;

                    return;
                }
            }
        }

        option checkBestOption(vector<Gamestate> & previousStates, bool firstCall = true, bool checkingForLoops = false)
        {
            Gamestate temp;
            option bestOption;
            bool alteredThisLoop;
            unsigned int size;

            if(checkingForLoops && optResult != loop)
            {
                return combine3;// this value will never be used.
            }

            if(checkingForLoops) optResult = unknown;

            distanceToOutcome = 0;

            updatePreviousStates(previousStates, true);

            for(char i = 0; i < 7; i++)
            {
                temp = getResult((option)i);

                if(temp.optResult == unknown)
                {
                    temp.updateResult(previousStates, checkingForLoops);
                }

                if (temp.optResult == unknown)
                {
                    temp.checkBestOption(previousStates, false, checkingForLoops);
                }
                switch(temp.optResult)
                {
                    case loop:
                        if((isP1Turn && optResult != p1Win) || (!isP1Turn && optResult != p2Win))
                        {
                            if(!checkingForLoops)
                            {
                                previousStates[temp.findIndex(previousStates)].indexesThatRequireThis.push_back(findIndex(previousStates));// we will get replicas, but its worth.
                            }

                            optResult = loop;
                            bestOption = (option)i;
                            distanceToOutcome = 0;
                        }
                        break;
                    case p1Win:
                        if(isP1Turn && (optResult != p1Win || distanceToOutcome > temp.distanceToOutcome))
                        {
                            optResult = p1Win;
                            bestOption = (option)i;
                            distanceToOutcome = temp.distanceToOutcome + 1;
                        }
                        if(!isP1Turn && (optResult == invalid || optResult == unknown || (optResult == p1Win && distanceToOutcome < temp.distanceToOutcome)))
                        {
                            optResult = p1Win;
                            bestOption = (option)i;
                            distanceToOutcome = temp.distanceToOutcome + 1;
                        }
                        break;
                    case p2Win:
                        if(!isP1Turn && (optResult != p2Win || distanceToOutcome > temp.distanceToOutcome))
                        {
                            optResult = p2Win;
                            bestOption = (option)i;
                            distanceToOutcome = temp.distanceToOutcome + 1;
                        }
                        if(isP1Turn && (optResult == invalid || optResult == unknown || (optResult == p2Win && distanceToOutcome < temp.distanceToOutcome)))
                        {
                            optResult = p2Win;
                            bestOption = (option)i;
                            distanceToOutcome = temp.distanceToOutcome + 1;
                        }
                        break;
                }
            }

            updatePreviousStates(previousStates, false);

            if(firstCall)
            {
                size = previousStates.size();

                do// Here, run through and recheck everything. Before this, stuff labeled loop could actually lead to something else. After this, anything labled loop will actually be a loop.
                {
                    alteredThisLoop = false;
                    for(unsigned int i = 0; i < size; i++)
                    {
                        if(previousStates[i].optResult != loop && previousStates[i].indexesThatRequireThis.size() != 0)
                        {
                            for(unsigned int j = 0; j < previousStates[i].indexesThatRequireThis.size(); j++)
                            {
                                previousStates[previousStates[i].indexesThatRequireThis[j]].checkBestOption(previousStates, false, true);
                                if(previousStates[previousStates[i].indexesThatRequireThis[j]].optResult != loop)
                                {
                                    alteredThisLoop = true;
                                }
                            }
                            previousStates[i].indexesThatRequireThis = {};
                        }
                    }

                } while (alteredThisLoop);

                optResult = unknown;

                return checkBestOption(previousStates, false, false);
            }

            return bestOption;
        }
};

inline void clearScreen()
{
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}

void play(bool isPlayerTurn)
{
    Gamestate tempGameState;
    Gamestate mainGameState = Gamestate(1, 1, 1, 1, !isPlayerTurn);
    string input;
    vector<Gamestate> listOfStates;

    if(!isPlayerTurn)
    {
        clearScreen();
        mainGameState.output();
        getline(cin, input);
    }

    while(true)
    {
        if(isPlayerTurn)
        {
            clearScreen();

            mainGameState.output();

            if(mainGameState.p2h1 == 0 && mainGameState.p2h2 == 0)
            {
                cout << "\nYou lose... Press enter to return to the main menu.\n";
                getline(cin, input);
                return;
            }

            getline(cin, input);

            if(input[0] == 'r' && input.length() == 3 && input[1] < '5' && input[2] < '5' && input[1] >= '0' && input[2] >= '0')
            {
                tempGameState = mainGameState;

                tempGameState.p2h1 = input[1] - 48;
                tempGameState.p2h2 = input[2] - 48;
                tempGameState.isP1Turn = true;

                if(tempGameState.p2h1 + tempGameState.p2h2 != mainGameState.p2h1 + mainGameState.p2h2)
                {
                    cout << "Not a legal move. You can't change your total number of fingers when rearranging.\n";
                    getline(cin, input);
                }
                else if(tempGameState.p2h1 == mainGameState.p2h1 || tempGameState.p2h1 == mainGameState.p2h2)
                {
                    cout << "Not a legal move. When rearranging, you need to meaningfully change what your fingers are (swapping your left and right hands doesn't count).\n";
                    getline(cin, input);
                }
                else
                {
                    mainGameState = tempGameState;
                    isPlayerTurn = false;

                    clearScreen();

                    mainGameState.output();
                    getline(cin, input);
                }
            }
            else if(input[0] == 'h')
            {
                if(input == "hll") tempGameState = mainGameState.getResult(hit11);
                else if(input == "hlr") tempGameState = mainGameState.getResult(hit12);
                else if(input == "hrr") tempGameState = mainGameState.getResult(hit22);
                else if(input == "hrl") tempGameState = mainGameState.getResult(hit21);
                else
                {
                    cout << "Not a valid command.\n\n\nControls:\n\nHitting the opponent's fingers is done by typing 'h', then 'l' or 'r' for which of your hands you want to hit with, then 'l' or 'r' for which of your opponent's hands you want to hit.\nFor example, to hit the opponent's left hand with your right you would type 'hrl'.\n\nRedistributing your own fingers is done by typing 'r', then the number of fingers you want on your left hand, then the number of fingers you want on your right hand.\nFor example, if you wanted to go from having one finger on each hand to 2 fingers on your left and none on your right you would type 'r20'.\n\n";
                    getline(cin, input);

                    tempGameState = mainGameState;
                }

                if(tempGameState.optResult == invalid)
                {
                    cout << "Not a legal move. You can only hit with a hand that has more than 0 fingers, and you can't hit a hand that has 0 fingers.\n";
                    getline(cin, input);
                }
                else if(tempGameState.isP1Turn != mainGameState.isP1Turn)
                {
                    mainGameState = tempGameState;

                    isPlayerTurn = !mainGameState.isP1Turn;

                    clearScreen();

                    mainGameState.output();

                    if(mainGameState.p1h1 == 0 && mainGameState.p1h2 == 0)
                    {
                        cout << "Congratulations! You won! Press enter to return to the main menu.\n";
                        getline(cin, input);
                        return;
                    }
                    
                    getline(cin, input);
                }
            }
            else
            {
                cout << "Not a valid command.\n\n\nControls:\n\nHitting the opponent's fingers is done by typing 'h', then 'l' or 'r' for which of your hands you want to hit with, then 'l' or 'r' for which of your opponent's hands you want to hit.\nFor example, to hit the opponent's left hand with your right you would type 'hrl'.\n\nRedistributing your own fingers is done by typing 'r', then the number of fingers you want on your left hand, then the number of fingers you want on your right hand.\nFor example, if you wanted to go from having one finger on each hand to 2 fingers on your left and none on your right you would type 'r20'.\n\n";
                getline(cin, input);
            }
        }
        else
        {
            mainGameState = mainGameState.getResult(mainGameState.checkBestOption(listOfStates));

            isPlayerTurn = true;
        }
    }
}

int main()
{
    string input;
    while(true)
    {
        cout << "Play chopsticks against the computer, cutoff and suicide variation.\nSee en.wikipedia.org/wiki/Chopsticks_(hand_game) for more info.\n\n\nControls:\n\nHitting the opponent's fingers is done by typing 'h', then 'l' or 'r' for which of your hands you want to hit with, then 'l' or 'r' for which of your opponent's hands you want to hit.\nFor example, to hit the opponent's left hand with your right you would type 'hrl'.\n\nRedistributing your own fingers is done by typing 'r', then the number of fingers you want on your left hand, then the number of fingers you want on your right hand.\nFor example, if you wanted to go from having one finger on each hand to 2 fingers on your left and none on your right you would type 'r20'.\n\n\nEnter 'p' for the player to take the first move or 'c' for the computer to take the first move.\n";
        getline(cin, input);

        if(input == "p")
        {
            play(true);
            clearScreen();
        }
        else if(input == "c")
        {
            play(false);
            clearScreen();
        }
        else
        {
            clearScreen();
            cout << "\nThat is not one of the options.\n";
        }
    }
}
