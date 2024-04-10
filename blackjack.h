// The rules are from https://youtu.be/eyoh-Ku9TCI

#include <iostream>
#include <algorithm>
using namespace std;

class Deck
{
private:
    int numServed = 0;
    int A = 11;
    int J = 10;
    int Q = 10;
    int K = 10;
    int cards[52] = {A, 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K,
                     A, 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K,
                     A, 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K,
                     A, 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K};

public:
    Deck()
    {
        unsigned seed = 0;
        random_shuffle(cards, cards + 52);
    }
    void PrintDeck()
    {
        for (int i = 0; i < 52; i++)
        {
            cout << cards[i] << " ";
        }
    }
    int serveCard()
    {
        if (numServed < 52)
        {
            numServed++;
            return cards[numServed - 1];
        }
        else
        {
            return -1;
        }
    }
};

class Dealer
{
private:
    int hand[3] = {0, 0, 0};
    int cardsReceived = 0;

public:
    void receiveCard(int card)
    {
        if (cardsReceived < sizeof(hand) / sizeof(hand[0]))
        {
            hand[cardsReceived] = card;
            cardsReceived++;
        }
    }

    int countHand()
    {
        int sum = 0;
        for (int i = 0; i < sizeof(hand) / sizeof(hand[0]) && hand[i] != 0; i++)
        {
            sum += hand[i];
        }
        return sum;
    }
    void printHand()
    {
        for (int i = 0; i < sizeof(hand) / sizeof(hand[0]) && hand[i] != 0; i++)
        {
            cout << hand[i] << " ";
        }
    }
};

class Player
{
    static int counter;

private:
    int ID = ++counter;
    int hand[52] = {0};
    int cardsReceived = 0;
    double bet = 0;
    int nextMove = 1; // 1 = hit, 0 = stand
    bool isActive = true;

public:
    int getID()
    {
        return ID;
    }
    void setBet(double bet)
    {
        this->bet = bet;
    }
    double getBet()
    {
        return bet;
    }
    void setNextMove(int nextMove)
    {
        this->nextMove = nextMove;
    }
    int getNextMove()
    {
        return nextMove;
    }
    void setActivity(bool isActive)
    {
        this->isActive = isActive;
    }
    bool getActivity()
    {
        return isActive;
    }
    void receiveCard(int card)
    {
        if (cardsReceived < sizeof(hand) / sizeof(hand[0]))
        {
            hand[cardsReceived] = card;
            cardsReceived++;
        }
    }

    void printHand()
    {
        for (int i = 0; i < sizeof(hand) / sizeof(hand[0]) && hand[i] != 0; i++)
        {
            cout << hand[i] << " ";
        }
    }
    int countHand()
    {
        int sum = 0;
        for (int i = 0; i < 52; i++)
        {
            if (hand[i] != 0)
            {
                sum += hand[i];
            }
        }
        return sum;
    }
};
int Player::counter = 0;
class Game
{
private:
public:
    Player *players;
    Dealer dealer;
    Deck deck;
    int numPlayers = 0;
    Game(int numPlayers)
    {
        this->numPlayers = numPlayers;
        players = new Player[numPlayers];
    }
    bool AllStand()
    {
        for (int i = 0; i < numPlayers; i++)
        {
            if (players[i].getNextMove() == 1)
            {
                return false;
            }
        }
        return true;
    }
};

void Play()
{
    int numPlayers = 0;

    cout << "Welcome to Blackjack!" << endl;
    cout << endl;

    // Initialize the players
    while (numPlayers < 1 || numPlayers > 7)
    {
        cout << "Enter number of players: ";
        cin >> numPlayers;
        if (numPlayers < 1 || numPlayers > 7)
        {
            cout << "Invalid input" << endl;
        }
    }
    cout << endl;

    // Initialize the game
    Game game(numPlayers);

    // Set the bets
    for (int i = 0; i < numPlayers; i++)
    {
        double bet = 0;
        while (bet <= 0)
        {
            cout << "Enter the bet of Player #" << game.players[i].getID() << ": ";
            cin >> bet;
            if (bet <= 0)
            {
                cout << "Invalid input" << endl;
            }
        }
        game.players[i].setBet(bet);
    }
    cout << endl;

    // Deal the first 2 cards
    for (int i = 0; i < numPlayers; i++)
    {
        game.players[i].receiveCard(game.deck.serveCard());
    }
    game.dealer.receiveCard(game.deck.serveCard());
    for (int i = 0; i < numPlayers; i++)
    {
        game.players[i].receiveCard(game.deck.serveCard());
    }
    game.dealer.receiveCard(game.deck.serveCard());

    // Check the first 2 cards
    for (int i = 0; i < numPlayers; i++)
    {
        if (game.players[i].countHand() == 21)
        {
            cout << "Player #" << game.players[i].getID() << " has " << game.players[i].countHand() << " points" << endl;
            cout << "Player #" << game.players[i].getID() << " has won!" << endl;
            game.players[i].setBet(game.players[i].getBet() * 1.5);
            game.players[i].setNextMove(0);     // stand till the rest of the game
            game.players[i].setActivity(false); // stop playing
        }
        else
        {
            cout << "Player #" << game.players[i].getID() << " has " << game.players[i].countHand() << " points" << endl;
        }
    }
    cout << endl;
    // Play until all players stand or bust
    while (game.AllStand() == false)
    {
        // Set the next moves
        for (int i = 0; i < numPlayers; i++)
        {
            if (game.players[i].getNextMove() == 1)
            {
                int move = -1;
                while (move != 0 && move != 1)
                {
                    cout << "Player #" << game.players[i].getID() << " move (1 = hit, 0 = stand): ";
                    cin >> move;
                    game.players[i].setNextMove(move);
                }
            }
        }
        cout << endl;

        // Deal the next cards for players who chose to hit
        for (int i = 0; i < numPlayers; i++)
        {
            if (game.players[i].getNextMove() == 1)
            {
                game.players[i].receiveCard(game.deck.serveCard());
            }
        }

        // Check the status of players' hands
        for (int i = 0; i < numPlayers; i++)
        {
            if (game.players[i].getNextMove() == 1)
            {
                int handSum = game.players[i].countHand();
                if (handSum == 21)
                {
                    cout << "Player #" << game.players[i].getID() << " has 21 points - WINNER" << endl;
                    game.players[i].setBet(game.players[i].getBet() * 1.5);
                    game.players[i].setNextMove(0);     // Stand for the rest of the game
                    game.players[i].setActivity(false); // stop playing
                }
                else if (handSum > 21)
                {
                    cout << "Player #" << game.players[i].getID() << " has busted with " << handSum << " points" << endl;
                    game.players[i].setBet(0);
                    game.players[i].setNextMove(0); // Stand for the rest of the game
                }
                else
                {
                    cout << "Player #" << game.players[i].getID() << " has " << handSum << " points" << endl;
                }
            }
        }
        cout << endl;
    }

    // Dealer's turn
    cout << "Dealer has " << game.dealer.countHand() << " points" << endl;
    if (game.dealer.countHand() <= 16)
    {
        cout << "Dealer receives another card" << endl;
        game.dealer.receiveCard(game.deck.serveCard());
        cout << "Dealer has " << game.dealer.countHand() << " points" << endl;
    }
    else if (game.dealer.countHand() > 16 && game.dealer.countHand() <= 21)
    {
        cout << "Dealer stands" << endl;
    }

    if (game.dealer.countHand() > 21)
    {
        cout << "Dealer busts" << endl;
        for (int i = 0; i < numPlayers; i++)
        {
            if (game.players[i].getActivity() == true)
            {
                cout << "Player #" << game.players[i].getID() << " has won!" << endl;
                game.players[i].setBet(game.players[i].getBet() * 2);
            }
        }
        cout << endl;
    }
    else
    {
        // Check the status of players' hands
        for (int i = 0; i < numPlayers; i++)
        {
            if (game.players[i].getActivity() == true && game.dealer.countHand() < game.players[i].countHand())
            {
                cout << "Player #" << game.players[i].getID() << " has won!" << endl;
                game.players[i].setBet(game.players[i].getBet() * 2);
            }
            else if (game.players[i].getActivity() == true && game.dealer.countHand() >= game.players[i].countHand())
            {
                cout << "Player #" << game.players[i].getID() << " has lost!" << endl;
                game.players[i].setBet(0);
            }
        }
        cout << endl;
    }

    cout << "GAME OVER" << endl;
    cout << endl;

    cout << "STATS:" << endl;
    for (int i = 0; i < numPlayers; i++)
    {
        cout << "Player #" << game.players[i].getID() << " has $" << game.players[i].getBet() << endl;
    }

    // for debug purposes
    // game.deck.PrintDeck();
    // cout << endl;
    // for (int i = 0; i < numPlayers; i++)
    // {
    //     cout << "Player #" << game.players[i].getID() << ": ";
    //     game.players[i].printHand();
    //     cout << endl;
    // }
    // cout << endl;
    // game.dealer.printHand();
    // cout << endl;
    //
}