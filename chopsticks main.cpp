// Rules at en.wikipedia.org/wiki/Chopsticks_(hand_game), this here is playing with the additional rules added by the cutoff and suicide variations (that's how it was played when I was a kid).
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

enum result : char {unknown, p1Win, loop, p2Win, invalid};
enum option : char {hit11, hit12, hit21, hit22, combine1, combine2, combine3};

// Returns true if val is a member of vec, returns false otherwise.
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
        // Counts for fingers on each hand
        short p1h1;
        short p1h2;
        short p2h1;
        short p2h2;

        bool isP1Turn;

        // The way the game will end from this game state if both players play optimally.
        result optResult = unknown;

        Gamestate(short inP1h1, short inP1h2, short inP2h1, short inP2h2, bool inIsP1Turn, result inResult = unknown)
        {
            p1h1 = inP1h1;
            p1h2 = inP1h2;
            p2h1 = inP2h1;
            p2h2 = inP2h2;
            isP1Turn = inIsP1Turn;
            optResult = inResult;
            
            updateTriviallyIncorrectValues();
        }
        
        Gamestate() {updateTriviallyIncorrectValues();}

        // Prints finger count and turn info to console, with player 1 being described as the computer and player 2 being described as the player.
        void output()
        {
            cout << "Computer hands:    " << p1h1 << "  " << p1h2 << "\n\n";
            cout << "Player hands:      " << p2h1 << "  " << p2h2 << "\n\n";
            
            if((p1h1 == 0 && p1h2 == 0) || (p2h1 == 0 && p2h2 == 0)) return;
            
            if(isP1Turn) cout << "It is the computer's turn.\n";
            else cout << "It is your turn.\n";
        }

        // Returns the Gamestate that follows this if the player whose turn it is takes the option optToCheck this turn. If optToCheck is not a valid option to choose,
        // then returns a gamestate with invalid as its optResult. 
        Gamestate getNext(option optToCheck)
        {
            Gamestate returnState;
            short sumOfHands;

            if(isP1Turn)
            {
                switch (optToCheck)
                {
                    case hit11:
                        if(p1h1 != 0 && p2h1 != 0)
                        {
                            returnState = Gamestate(p1h1, p1h2, p2h1 + p1h1, p2h2, false);
                            return returnState;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                    case hit12:
                        if(p1h1 != 0 && p2h2 != 0)
                        {
                            returnState = Gamestate(p1h1, p1h2, p2h1, p2h2 + p1h1, false);
                            return returnState;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                    case hit21:
                        if(p1h2 != 0 && p2h1 != 0)
                        {
                            returnState = Gamestate(p1h1, p1h2, p2h1 + p1h2, p2h2, false);
                            return returnState;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                    case hit22:
                        if(p1h2 != 0 && p2h2 != 0)
                        {
                            returnState = Gamestate(p1h1, p1h2, p2h1, p2h2 + p1h2, false);
                            return returnState;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                    case combine1:
                        returnState = Gamestate(p1h1, p1h2, p2h1, p2h2, false);
                        sumOfHands = p1h1+p1h2;
                        if(sumOfHands > 4)
                        {
                            returnState.p1h1 = 4;
                            returnState.p1h2 = sumOfHands - 4;
                        }
                        else
                        {
                            returnState.p1h1 = sumOfHands;
                            returnState.p1h2 = 0;
                        }

                        if((returnState.p1h1 == p1h1 && returnState.p1h2 == p1h2) || (returnState.p1h1 == p1h2 && returnState.p1h2 == p1h1)) return Gamestate(0, 0, 0, 0, true, invalid);

                        returnState.updateTriviallyIncorrectValues();
                        return returnState;
                    case combine2:
                        returnState = Gamestate(p1h1, p1h2, p2h1, p2h2, false);
                        sumOfHands = p1h1+p1h2;
                        if(sumOfHands > 6)
                        {
                            return Gamestate(0, 0, 0, 0, true, invalid);
                        }
                        else if(sumOfHands > 4)
                        {
                            returnState.p1h1 = 3;
                            returnState.p1h2 = sumOfHands - 3;
                        }
                        else if(sumOfHands > 1)
                        {
                            returnState.p1h1 = sumOfHands-1;
                            returnState.p1h2 = 1;
                        }
                        else
                        {
                            return Gamestate(0, 0, 0, 0, true, invalid);
                        }

                        if((returnState.p1h1 == p1h1 && returnState.p1h2 == p1h2) || (returnState.p1h1 == p1h2 && returnState.p1h2 == p1h1)) return Gamestate(0, 0, 0, 0, true, invalid);

                        returnState.updateTriviallyIncorrectValues();
                        return returnState;
                    case combine3:
                        if(p1h1+p1h2 == 4 && p1h1 != 2)
                        {
                            returnState = Gamestate(2, 2, p2h1, p2h2, false);
                            returnState.updateTriviallyIncorrectValues();
                            return returnState;
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
                            returnState = Gamestate(p2h1 + p1h1, p1h2, p2h1, p2h2, true);
                            return returnState;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                    case hit12:
                        if(p1h2 != 0 && p2h1 != 0)
                        {
                            returnState = Gamestate(p1h1, p1h2 + p2h1, p2h1, p2h2, true);
                            return returnState;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                    case hit21:
                        if(p1h1 != 0 && p2h2 != 0)
                        {
                            returnState = Gamestate(p1h1 + p2h2, p1h2, p2h1, p2h2, true);
                            return returnState;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                    case hit22:
                        if(p1h2 != 0 && p2h2 != 0)
                        {
                            returnState = Gamestate(p1h1, p1h2 + p2h2, p2h1, p2h2, true);
                            return returnState;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                    case combine1:
                        returnState = Gamestate(p1h1, p1h2, p2h1, p2h2, true);
                        sumOfHands = p2h1+p2h2;
                        if(sumOfHands > 4)
                        {
                            returnState.p2h1 = 4;
                            returnState.p2h2 = sumOfHands - 4;
                        }
                        else
                        {
                            returnState.p2h1 = sumOfHands;
                            returnState.p2h2 = 0;
                        }

                        if((returnState.p2h1 == p2h1 && returnState.p2h2 == p2h2) || (returnState.p2h1 == p2h2 && returnState.p2h2 == p2h1)) return Gamestate(0, 0, 0, 0, true, invalid);

                        returnState.updateTriviallyIncorrectValues();
                        return returnState;
                    case combine2:
                        returnState = Gamestate(p1h1, p1h2, p2h1, p2h2, true);
                        sumOfHands = p2h1+p2h2;
                        if(sumOfHands > 6)
                        {
                            return Gamestate(0, 0, 0, 0, true, invalid);
                        }
                        else if(sumOfHands > 4)
                        {
                            returnState.p2h1 = 3;
                            returnState.p2h2 = sumOfHands - 3;
                        }
                        else if(sumOfHands > 1)
                        {
                            returnState.p2h1 = sumOfHands-1;
                            returnState.p2h2 = 1;
                        }
                        else
                        {
                            return Gamestate(0, 0, 0, 0, true, invalid);
                        }

                        if((returnState.p2h1 == p2h1 && returnState.p2h2 == p2h2) || (returnState.p2h1 == p2h2 && returnState.p2h2 == p2h1)) return Gamestate(0, 0, 0, 0, true, invalid);

                        returnState.updateTriviallyIncorrectValues();
                        return returnState;
                    case combine3:
                        if(p2h1+p2h2 == 4 && p2h1 != 2)
                        {
                            returnState = Gamestate(p1h1, p1h2, 2, 2, true);
                            returnState.updateTriviallyIncorrectValues();
                            return returnState;
                        }
                        return Gamestate(0, 0, 0, 0, true, invalid);
                }
            }
        }

        // Determines the optimal option the player whose turn it is should take from this Gamestate, meaning the option that will (assuming optimal play from both players) lead
        // to the best conclusion to the game and taking either the greatest or least ammount of turns to get there if the best conclusion is a loss or a win respectively.
        option checkBestOption()
        {
            bool alteredThisLoop;
            int size;
            vector<Gamestate> statesVec = {};

            calculateOptResults(statesVec, false);

            size = statesVec.size();

            do// Here, run through and recheck everything. Before this, stuff labeled loop could actually lead to something else. After this, anything labled loop will actually be a loop.
            {
                alteredThisLoop = false;
                for(int i = 0; i < size; i++)
                {
                    if(statesVec[i].optResult != loop && statesVec[i].indexesOfPreviousLoops.size() != 0)
                    {
                        for(int j = 0; j < statesVec[i].indexesOfPreviousLoops.size(); j++)
                        {
                            statesVec[statesVec[i].indexesOfPreviousLoops[j]].calculateOptResults(statesVec, true);
                        }

                        statesVec[i].indexesOfPreviousLoops = {};
                        alteredThisLoop = true;
                    }
                }

            } while (alteredThisLoop);

            pullOptResultFromStatesVec(statesVec, true);

            return checkBestOptionWithDistance(statesVec);
        }
    
    private:
        // The number of turns it will take for the game to end after this if both players play optimally
        short distanceToOutcome = 0;

        // Unique id based on the fingers on each hand and which player's turn it is. 2 Gamestates with the same id (called id-matches) represent the same turn.
        short id = 6;

        // The index of the id-match of this in the statesVec vector used throughout the process of calculating an optimal move
        int indexInStatesVec = -1;

        // Contains the indexes in statesVec of Gamestates which can occur on the turn before this Gamestate and have loop as their optResult. Only used if optResult is identified
        // as loop in phase 1. For the explicit graph that is the focus of phase 2 of the algorithm, indexesOfPreviousLoops is the list of direct successors of this Gamestate.
        vector<int> indexesOfPreviousLoops = {};
        
        // Contains the indexes in statesVec of the Gamestates that contain the index of an id-match of this in their indexesOfPreviousLoops.
        vector<int> indexesOfNextLoops = {};

        // Sets hands with 5 or more fingers to 0, sets id, and sets optResult to p1Win or p2Win if the corresponding player wins this turn.
        void updateTriviallyIncorrectValues()
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

        // If a id-match of this is present in statesVec, updates the optResult of the Gamestate in statesVec if optResultUnfinalised is false.
        // Otherwise, adds a copy of this to the end of statesVec (and sets the optResult to unknown if optResultUnfinalised is true).
        // In either case, returns the index of the id-match of this statesVec.
        int updateStatesVec(vector<Gamestate> & statesVec, bool optResultUnfinalised)
        {
            int size = statesVec.size();

            if(indexInStatesVec != -1)
            {
                if(!optResultUnfinalised)
                {
                    statesVec[indexInStatesVec].optResult = optResult;
                }
                return indexInStatesVec;
            }
            
            for(int i = 0; i < size; i++)
            {
                if(id == statesVec[i].id)
                {
                    if(!optResultUnfinalised)
                    {
                        statesVec[i].optResult = optResult;
                    }
                    return i;
                }
            }
        
            statesVec.push_back(*this);
            
            if(optResultUnfinalised)
            {
                statesVec[size].optResult = unknown;
            }

            statesVec[size].indexInStatesVec = size;

            return size;
        }

        
        // Updates optResult to match the optResult of the id-match of this in statesVec, if there is one. The exception is when checkingForLoops
        // is false and the optResult of the Gamestate in statesVec is unknown: in this case, optResult is set to loop. checkingForLoops should be set to false
        // when calling this in phase 1 of the algorithm, and set to true when calling this in phase 2.
        void pullOptResultFromStatesVec(vector<Gamestate> & statesVec, bool checkingForLoops)
        {
            int size = statesVec.size();

            if(optResult != unknown && optResult != loop) return;

            if(indexInStatesVec != -1)
            {
                if(checkingForLoops)
                {
                    optResult = statesVec[indexInStatesVec].optResult;
                }
                else
                {
                    if (statesVec[indexInStatesVec].optResult == unknown)
                    {
                        optResult = loop;
                    }
                    else
                    {
                        optResult = statesVec[indexInStatesVec].optResult;
                    }
                }

                return;
            }

            for(int i = 0; i < size; i++)
            {
                if(id == statesVec[i].id)
                {
                    indexInStatesVec = i;
                    
                    if(checkingForLoops)
                    {
                        optResult = statesVec[i].optResult;
                    }
                    else
                    {
                        if (statesVec[i].optResult == unknown)
                        {
                            optResult = loop;
                        }
                        else
                        {
                            optResult = statesVec[i].optResult;
                        }
                    }

                    return;
                }
            }
        }

        // statesVec should contain id-matches for any Gamestates that have already had their optResults calculations started. If checkingForLoops is false, this will update the
        // optResult of this and store it in statesVec as well as the optResults of all Gamestates that can come after this at any point later on in the game. Any Gamestates for
        // which it's a possibility that the optResult is loop will have their optResult marked as loop. the indexesOfPreviousLoops and indexesOfNextLoops attributes of the
        // Gamestates in statesVec will also be updated accordingly. If checkingForLoops is true, this will only update the optResult of this and store it in statesVec based on
        // the id-matches in statesVec for the Gamestates that can occur on the next turn. checkingForLoops should be set to false when calling this method in phase 1 of the
        // algorithm, and set to true when calling this in phase 2.
        void calculateOptResults(vector<Gamestate> & statesVec, bool checkingForLoops)
        {
            Gamestate currentNext;

            if(checkingForLoops) optResult = unknown;

            indexInStatesVec = updateStatesVec(statesVec, true);

            for(char i = 0; i < 7; i++)
            {
                currentNext = getNext((option)i);

                if(currentNext.optResult == unknown)
                {
                    currentNext.pullOptResultFromStatesVec(statesVec, checkingForLoops);
                }

                if (currentNext.optResult == unknown && !checkingForLoops)
                {
                    currentNext.calculateOptResults(statesVec, checkingForLoops);
                }
                switch(currentNext.optResult)
                {
                    case loop:
                        if((isP1Turn && optResult != p1Win) || (!isP1Turn && optResult != p2Win))
                        {
                            if(!checkingForLoops)
                            {
                                if(!contains(statesVec[currentNext.indexInStatesVec].indexesOfPreviousLoops, indexInStatesVec))
                                {
                                    statesVec[currentNext.indexInStatesVec].indexesOfPreviousLoops.push_back(indexInStatesVec);
                                    indexesOfNextLoops.push_back(currentNext.indexInStatesVec);
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

            updateStatesVec(statesVec, false);

            if(optResult != loop && !checkingForLoops)
            {
                for (int i = 0; i < indexesOfNextLoops.size(); i++)
                {
                    for(int j = 0; j < statesVec[indexesOfNextLoops[i]].indexesOfPreviousLoops.size(); j++)
                    {
                        if(statesVec[indexesOfNextLoops[i]].indexesOfPreviousLoops[j] == indexInStatesVec)
                        {
                            statesVec[indexesOfNextLoops[i]].indexesOfPreviousLoops.erase(statesVec[indexesOfNextLoops[i]].indexesOfPreviousLoops.begin() + j);
                            break;
                        }
                    }
                }

                indexesOfNextLoops = {};
            }

            return;
        }

        // Determines the optimal option the player whose turn it is should take from this Gamestate, meaning the option that will (assuming optimal play from both players) lead
        // to the best conclusion to the game and taking either the greatest or least ammount of turns to get there if the best conclusion is a loss or a win respectively. Also
        // updates the distanceToOutcome for the id-match of this in statesVec. statesVec must contain id-matches for this and every gamestate that can come after this at any
        // point later on in the game, and those id-matches must all have the correct optResult. Furthermore, when this function is first called, the distanceToOutcome for all
        // Gamestates in statesVec must be 0. Since the returned value isn't used when this function recursively calls itself, arbitrary values may be returned if it has been
        // determined that this is a recursive call.
        option checkBestOptionWithDistance(vector<Gamestate> & statesVec)
        {
            Gamestate currentNext;
            option bestOption;

            distanceToOutcome = -1;// this means the distanceToOutcome is in the process of being determined.

            for(int i = 0; i < statesVec.size(); i++)
            {
                if(id == statesVec[i].id)
                {
                    indexInStatesVec = i;
                }
            }

            if(statesVec[indexInStatesVec].distanceToOutcome == -1)
            {
                distanceToOutcome = -2;// signal value meaning looping.
                return hit11;// this is a recursive call since distanceToOutcome for the Gamestates in statesVec will be 0 otherwise. Thus, what we return doesn't matter here.
            }
            else if (statesVec[indexInStatesVec].optResult != loop)
            {
                if(statesVec[indexInStatesVec].distanceToOutcome == 0)
                {
                    statesVec[indexInStatesVec].distanceToOutcome = -1;
                }
                else
                {
                    distanceToOutcome = statesVec[indexInStatesVec].distanceToOutcome;
                    return hit11;// this is a recursive call since distanceToOutcome for the Gamestates in statesVec will be 0 otherwise. Thus, what we return doesn't matter here.
                }
            }

            for(char i = 0; i < 7; i++)
            {
                currentNext = getNext((option)i);

                if(currentNext.optResult == unknown)
                {
                    currentNext.pullOptResultFromStatesVec(statesVec, true);
                    if(currentNext.optResult != optResult) continue;
                    if(optResult != loop) currentNext.checkBestOptionWithDistance(statesVec);
                }
                else
                {
                    if(currentNext.optResult != optResult) continue;
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

            if(distanceToOutcome == -2) statesVec[indexInStatesVec].distanceToOutcome = 0;
            else statesVec[indexInStatesVec].distanceToOutcome = distanceToOutcome;

            return bestOption;
        }
};

inline void clearScreen()
{
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}

// The player plays a full game of chopsticks against the computer. The player takes the first turn if true is passed in for isPlayerTurn, otherwise the computer goes first. 
void play(bool isPlayerTurn)
{
    Gamestate tempGameState;// For checking if a move the player makes is legal.
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
                if(input == "hll") tempGameState = mainGameState.getNext(hit11);
                else if(input == "hlr") tempGameState = mainGameState.getNext(hit12);
                else if(input == "hrr") tempGameState = mainGameState.getNext(hit22);
                else if(input == "hrl") tempGameState = mainGameState.getNext(hit21);
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
            mainGameState = mainGameState.getNext(mainGameState.checkBestOption());

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
