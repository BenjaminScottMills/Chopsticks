// Rules at en.wikipedia.org/wiki/Chopsticks_(hand_game), this here is playing with the additional rules added by the cutoff and suicide variations (that's how it was played when I was a kid).
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

enum result : char {unknown, p1Win, loop, p2Win, invalid};
enum option : char {hit11, hit12, hit21, hit22, combine1, combine2, combine3};

class Gamestate;

bool contains(vector<int> vec, int val)
{
    for (int i = 0; i < vec.size(); i ++)
    {
        if (vec[i] == val) return true;
    }

    return false;
}

class Gamestate
{
    public:
        short p1h1;
        short p1h2;
        short p2h1;
        short p2h2;
        short distanceToOutcome = 0;
        short id = 6;
        int indexInPreviousStates = -1;
        bool isP1Turn;
        result optResult = unknown;
        vector<int> indexesThatRequireThis = {};
        vector<int> indexesThatThisRequires = {};

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

        // Returns the index of this in previousStates.
        int updatePreviousStates(vector<Gamestate> & previousStates, bool setAsUnknown)
        {
            int size = previousStates.size();

            if(indexInPreviousStates != -1)
            {
                if(!setAsUnknown)
                {
                    previousStates[indexInPreviousStates].optResult = optResult;
                }
                return indexInPreviousStates;
            }
            
            for(int i = 0; i < size; i++)
            {
                if(id == previousStates[i].id)
                {
                    if(!setAsUnknown)
                    {
                        previousStates[i].optResult = optResult;
                    }
                    return i;
                }
            }
        
            previousStates.push_back(*this);
            
            if(setAsUnknown)
            {
                previousStates[size].optResult = unknown;
            }

            previousStates[size].indexInPreviousStates = size;

            return size;
        }

        void updateResult(vector<Gamestate> & previousStates, bool checkingForLoops)
        {
            int size = previousStates.size();

            if(optResult != unknown && optResult != loop) return;

            if(indexInPreviousStates != -1)
            {
                if(checkingForLoops)
                {
                    optResult = previousStates[indexInPreviousStates].optResult;
                }
                else
                {
                    if (previousStates[indexInPreviousStates].optResult == unknown)
                    {
                        optResult = loop;
                    }
                    else
                    {
                        optResult = previousStates[indexInPreviousStates].optResult;
                    }
                }

                return;
            }

            for(int i = 0; i < size; i++)
            {
                if(id == previousStates[i].id)
                {
                    indexInPreviousStates = i;
                    
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

                    return;
                }
            }
        }

        void checkBestOptionRecursive(vector<Gamestate> & previousStates, bool checkingForLoops)
        {
            Gamestate currentNext;

            if(checkingForLoops && optResult != loop)
            {
                return;
            }

            if(checkingForLoops) optResult = unknown;

            indexInPreviousStates = updatePreviousStates(previousStates, true);

            for(char i = 0; i < 7; i++)
            {
                currentNext = getResult((option)i);

                if(currentNext.optResult == unknown)
                {
                    currentNext.updateResult(previousStates, checkingForLoops);
                }

                if (currentNext.optResult == unknown)
                {
                    currentNext.checkBestOptionRecursive(previousStates, checkingForLoops);
                }
                switch(currentNext.optResult)
                {
                    case loop:
                        if((isP1Turn && optResult != p1Win) || (!isP1Turn && optResult != p2Win))
                        {
                            if(!checkingForLoops)
                            {
                                if(!contains(previousStates[currentNext.indexInPreviousStates].indexesThatRequireThis, indexInPreviousStates))
                                {
                                    previousStates[currentNext.indexInPreviousStates].indexesThatRequireThis.push_back(indexInPreviousStates);
                                    indexesThatThisRequires.push_back(currentNext.indexInPreviousStates);
                                }
                            }

                            optResult = loop;
                        }
                        break;
                    case p1Win:
                        if(isP1Turn || optResult == invalid || optResult == unknown)
                        {
                            optResult = p1Win;
                        }
                        break;
                    case p2Win:
                        if(!isP1Turn || optResult == invalid || optResult == unknown)
                        {
                            optResult = p2Win;
                        }
                        break;
                }
            }

            updatePreviousStates(previousStates, false);

            if(optResult != loop)
            {
                for (int i = 0; i < indexesThatThisRequires.size(); i++)
                {
                    for(int j = 0; j < previousStates[indexesThatThisRequires[i]].indexesThatRequireThis.size(); j++)
                    {
                        if(previousStates[indexesThatThisRequires[i]].indexesThatRequireThis[j] == indexInPreviousStates)
                        {
                            previousStates[indexesThatThisRequires[i]].indexesThatRequireThis.erase(previousStates[indexesThatThisRequires[i]].indexesThatRequireThis.begin() + j);
                            break;
                        }
                    }
                }

                indexesThatThisRequires = {};
            }

            return;
        }

        // Does 2 things: updates distanceToOutcome (important for the recursive case) and determines the best option (important for the base case).
        option checkBestOptionWithDistance(vector<Gamestate> & previousStates)
        {
            Gamestate currentNext;
            option bestOption;

            distanceToOutcome = -1;

            for(int i = 0; i < previousStates.size(); i++)
            {
                if(id == previousStates[i].id)
                {
                    indexInPreviousStates = i;
                }
            }

            if(previousStates[indexInPreviousStates].distanceToOutcome == -1)
            {
                distanceToOutcome = -2;// signal value meaning looping.
                return hit11;// what we return doesn't matter here
            }
            else if (previousStates[indexInPreviousStates].optResult != loop && previousStates[indexInPreviousStates].distanceToOutcome == 0)
            {
                previousStates[indexInPreviousStates].distanceToOutcome = -1;
            }
            else if (previousStates[indexInPreviousStates].optResult != loop)
            {
                distanceToOutcome = previousStates[indexInPreviousStates].distanceToOutcome;
                return hit11;// what we return doesn't matter here
            }

            for(char i = 0; i < 7; i++)
            {
                currentNext = getResult((option)i);

                if(currentNext.optResult == unknown)
                {
                    currentNext.updateResult(previousStates, true);
                    if(currentNext.optResult != optResult) continue;
                    if(optResult != loop) currentNext.checkBestOptionWithDistance(previousStates);
                }
                else
                {
                    if(currentNext.optResult != optResult) continue;// potentially this needs to be changed.
                }

                switch(optResult)
                {
                    case loop:
                        distanceToOutcome = 0;
                        return (option)i;
                    case p1Win:
                        if(currentNext.distanceToOutcome == -2)
                        {
                            if(!isP1Turn)
                            {
                                distanceToOutcome = -2;
                                bestOption = (option)i;
                                i = 7;
                            }
                            else if (distanceToOutcome == -1)
                            {
                                distanceToOutcome = -2;
                                bestOption = (option)i;
                            }
                        }
                        else if((isP1Turn && (distanceToOutcome > currentNext.distanceToOutcome || distanceToOutcome <= -1)) || (!isP1Turn && distanceToOutcome < currentNext.distanceToOutcome))
                        {
                            bestOption = (option)i;
                            distanceToOutcome = currentNext.distanceToOutcome + 1;
                        }
                        break;
                    case p2Win:
                        if(currentNext.distanceToOutcome == -2)
                        {
                            if(isP1Turn)
                            {
                                distanceToOutcome = -2;
                                bestOption = (option)i;
                                i = 7;
                            }
                            else if (distanceToOutcome == -1)
                            {
                                distanceToOutcome = -2;
                                bestOption = (option)i;
                            }
                        }
                        else if((!isP1Turn && (distanceToOutcome > currentNext.distanceToOutcome || distanceToOutcome <= -1)) || (isP1Turn && distanceToOutcome < currentNext.distanceToOutcome))
                        {
                            bestOption = (option)i;
                            distanceToOutcome = currentNext.distanceToOutcome + 1;
                        }
                        break;
                }
            }

            if(distanceToOutcome == -2) previousStates[indexInPreviousStates].distanceToOutcome = 0;
            else previousStates[indexInPreviousStates].distanceToOutcome = distanceToOutcome;

            return bestOption;
        }

        option checkBestOption()
        {
            bool alteredThisLoop;
            int size;
            vector<Gamestate> previousStates = {};

            checkBestOptionRecursive(previousStates, false);

            size = previousStates.size();

            do// Here, run through and recheck everything. Before this, stuff labeled loop could actually lead to something else. After this, anything labled loop will actually be a loop.
            {
                alteredThisLoop = false;
                for(int i = 0; i < size; i++)
                {
                    if(previousStates[i].optResult != loop && previousStates[i].indexesThatRequireThis.size() != 0)
                    {
                        for(int j = 0; j < previousStates[i].indexesThatRequireThis.size(); j++)
                        {
                            previousStates[previousStates[i].indexesThatRequireThis[j]].checkBestOptionRecursive(previousStates, true);
                        }

                        previousStates[i].indexesThatRequireThis = {};
                        alteredThisLoop = true;
                    }
                }

            } while (alteredThisLoop);

            updateResult(previousStates, true);

            return checkBestOptionWithDistance(previousStates);
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
            mainGameState = mainGameState.getResult(mainGameState.checkBestOption());

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
