#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define BOLD    "\033[1m"
class Item;
class Helper;
class Hurdle;
class Player;
class Board;
class AdventureQuest;
class Item
{
protected:
    string name;
    int X, Y;

public:
    Item(int x, int y) : X(x), Y(y) {}
    virtual ~Item() {}
    virtual void interact(Player& player) = 0;
    virtual Item* clone() const = 0;
    string getName() const { return name; }
    virtual string getShortName() const { return name.substr(0, 3); }
    int getX() const { return X; }
    int getY() const { return Y; }
};
class Board
{
private:
    int size;
    int level;
    Item*** cells;
    bool** player1Path;
    bool** player2Path;
    int** cellNumbers;
    int* numberToPosX;
    int* numberToPosY;

public:
    Board(int level);
    Board(const Board& other);
    Board& operator=(const Board& other);
    ~Board();
    void initializeItems();
    void print(const Player* player1, const Player* player2) const;
    void markPlayerPath(int x, int y, bool isPlayer1);
    int getSize() const { return size; }
    Item* getItem(int x, int y) const { return cells[x][y]; }
    void setItem(int x, int y, Item* item) { cells[x][y] = item; }
    pair<int, int> numberToPosition(int num) const;
    int positionToNumber(int x, int y) const;
    int getLevel() const { return level; }

    friend class Player;
    friend class AdventureQuest;
};
class Player
{
private:
    int X, Y;
    int currentNumber;
    int gold, silver;
    int points;
    Item** inventory;
    Board* board;
    int invSize;
    int invCapacity;
    int blockedTurns;
    static const int MAX_HISTORY = 100;
    int moveHistory[MAX_HISTORY];
    int historyCount;
    int historyStart;
    int currentLevel;
    bool hasReachedGoal;

    bool checkInventory(const string& itemName) const;
    void removeFromInventory(int index);

public:
    Player(int startNum, Board* b, int level);
    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player();
    void addGold(int g);
    void addSilver(int s);
    void addItem(Item* item);
    void block(int turns);
    bool isBlocked() const { return blockedTurns > 0; }
    void unblock();
    void clearBlock();
    void printStatus() const;
    void deductPoints(int amount);
    int getPoints() const;
    int getGold() const;
    int getSilver() const;
    void deductGold(int amount);
    void deductSilver(int amount);
    bool hasWater() const;
    bool hasSword() const;
    bool hasShield() const;
    bool hasKey() const;
    void useWater();
    void useSword();
    void useShield();
    void useKey();
    void recordMove(int num);
    void moveBack(int cells);
    void levelUp(int startNum);
    void placeHurdle(Item* hurdle);
    string getCurrentHurdle() const;
    void clearHurdle();
    void displayInventory() const;
    bool useItem(int index);
    int getX() const { return X; }
    int getY() const { return Y; }
    int getCurrentNumber() const { return currentNumber; }
    void setPosition(int num);
    bool hasReachedGoalState() const { return hasReachedGoal; }
    void setReachedGoal(bool reached) { hasReachedGoal = reached; }

    friend class Board;
    friend class AdventureQuest;
};
class GoldCoin : public Item
{
public:
    GoldCoin(int x, int y);
    void interact(Player& player) override;
    Item* clone() const override;
    string getShortName() const override { return "GLD"; }
};
class SilverCoin : public Item
{
public:
    SilverCoin(int x, int y);
    void interact(Player& player) override;
    Item* clone() const override;
    string getShortName() const override { return "SIL"; }
};
class Helper : public Item
{
protected:
    int cost;
    int uses;

public:
    Helper(int x, int y, int c, int u);
    virtual void use(Player& player) = 0;
    virtual bool canUseOn(const string& hurdleType) const = 0;
    int getCost() const;
    int getUses() const;
    virtual ~Helper();
    void decrementUse();
};
class Sword : public Helper
{
public:
    Sword(int x, int y);
    void interact(Player& player) override;
    void use(Player& player) override;
    bool canUseOn(const string& hurdleType) const override;
    Item* clone() const override;
    string getShortName() const override { return "SWD"; }
};
class Shield : public Helper
{
public:
    Shield(int x, int y);
    void interact(Player& player) override;
    void use(Player& player) override;
    bool canUseOn(const string& hurdleType) const override;
    Item* clone() const override;
    string getShortName() const override { return "SHD"; }
};
class Water : public Helper
{
public:
    Water(int x, int y);
    void interact(Player& player) override;
    void use(Player& player) override;
    bool canUseOn(const string& hurdleType) const override;
    Item* clone() const override;
    string getShortName() const override { return "WTR"; }
};
class Key : public Helper
{
public:
    Key(int x, int y);
    void interact(Player& player) override;
    void use(Player& player) override;
    bool canUseOn(const string& hurdleType) const override;
    Item* clone() const override;
    string getShortName() const override { return "KEY"; }
};
class Hurdle : public Item
{
protected:
    int cost;
    int penalty;
    int blockTurns;

public:
    Hurdle(int x, int y, int c, int p, int turns);
    virtual void applyEffect(Player& player) = 0;
    virtual void interact(Player& player) override;
    int getCost() const;
    int getPenalty() const;
    int getBlockTurns() const { return blockTurns; }
    virtual ~Hurdle();
};
class Fire : public Hurdle
{
public:
    Fire(int x, int y);
    void interact(Player& player) override;
    void applyEffect(Player& player) override;
    Item* clone() const override;
    string getShortName() const override { return "FIR"; }
};
class Snake : public Hurdle
{
public:
    Snake(int x, int y);
    void interact(Player& player) override;
    void applyEffect(Player& player) override;
    Item* clone() const override;
    string getShortName() const override { return "SNK"; }
};
class Ghost : public Hurdle
{
public:
    Ghost(int x, int y);
    void interact(Player& player) override;
    void applyEffect(Player& player) override;
    Item* clone() const override;
    string getShortName() const override { return "GST"; }
};
class Lion : public Hurdle
{
public:
    Lion(int x, int y);
    void interact(Player& player) override;
    void applyEffect(Player& player) override;
    Item* clone() const override;
    string getShortName() const override { return "LIO"; }
};
class Lock : public Hurdle
{
public:
    Lock(int x, int y);
    void interact(Player& player) override;
    void applyEffect(Player& player) override;
    Item* clone() const override;
    string getShortName() const override { return "LCK"; }
};
class AdventureQuest
{
private:
    Board* board;
    Player* player1;
    Player* player2;
    int currentLevel;
    AdventureQuest* savedState;
    int goalNumber;
    bool player1ReachedFirst;
    bool player2ReachedFirst;

public:
    AdventureQuest();
    AdventureQuest(const AdventureQuest& other);
    AdventureQuest& operator=(const AdventureQuest& other);
    ~AdventureQuest();
    void saveGame();
    void loadGame();
    void playTurn(Player* player, Player* opponent, bool isPlayer1);
    void startGame();
    void determineWinner();
    void levelUp();
    int getBoardLevel() const { return board->getLevel(); }
};
Board::Board(int level) : level(level)
{
    if (level == 1)
    {
        size = 5;
    }
    else if (level == 2)
    {
        size = 7;
    }
    else if (level == 3)
    {
        size = 9;
    }
    else
    {
        size = 11;
    }

    cells = new Item * *[size];
    player1Path = new bool* [size];
    player2Path = new bool* [size];
    cellNumbers = new int* [size];
    for (int i = 0; i < size; ++i)
    {
        cells[i] = new Item * [size];
        player1Path[i] = new bool[size];
        player2Path[i] = new bool[size];
        cellNumbers[i] = new int[size];
        for (int j = 0; j < size; ++j)
        {
            cells[i][j] = nullptr;
            player1Path[i][j] = false;
            player2Path[i][j] = false;
        }
    }

    int totalCells = size * size;
    numberToPosX = new int[totalCells + 1];
    numberToPosY = new int[totalCells + 1];
    int num = 1;
    for (int i = 0; i < size; ++i)
    {
        if (i % 2 == 0)
        {
            for (int j = 0; j < size; ++j)
            {
                cellNumbers[i][j] = num;
                numberToPosX[num] = i;
                numberToPosY[num] = j;
                num++;
            }
        }
        else
        {
            for (int j = size - 1; j >= 0; --j)
            {
                cellNumbers[i][j] = num;
                numberToPosX[num] = i;
                numberToPosY[num] = j;
                num++;
            }
        }
    }
    initializeItems();
}

Board::Board(const Board& other) : size(other.size), level(other.level)
{
    cells = new Item * *[size];
    player1Path = new bool* [size];
    player2Path = new bool* [size];
    cellNumbers = new int* [size];
    for (int i = 0; i < size; ++i)
    {
        cells[i] = new Item * [size];
        player1Path[i] = new bool[size];
        player2Path[i] = new bool[size];
        cellNumbers[i] = new int[size];
        for (int j = 0; j < size; ++j)
        {
            cells[i][j] = other.cells[i][j] ? other.cells[i][j]->clone() : nullptr;
            player1Path[i][j] = other.player1Path[i][j];
            player2Path[i][j] = other.player2Path[i][j];
            cellNumbers[i][j] = other.cellNumbers[i][j];
        }
    }
    int totalCells = size * size;
    numberToPosX = new int[totalCells + 1];
    numberToPosY = new int[totalCells + 1];
    for (int i = 1; i <= totalCells; ++i)
    {
        numberToPosX[i] = other.numberToPosX[i];
        numberToPosY[i] = other.numberToPosY[i];
    }
}
Board& Board::operator=(const Board& other)
{
    if (this != &other)
    {
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                delete cells[i][j];
            }
            delete[] cells[i];
            delete[] player1Path[i];
            delete[] player2Path[i];
            delete[] cellNumbers[i];
        }
        delete[] cells;
        delete[] player1Path;
        delete[] player2Path;
        delete[] cellNumbers;
        delete[] numberToPosX;
        delete[] numberToPosY;

        size = other.size;
        level = other.level;
        cells = new Item * *[size];
        player1Path = new bool* [size];
        player2Path = new bool* [size];
        cellNumbers = new int* [size];
        for (int i = 0; i < size; ++i)
        {
            cells[i] = new Item * [size];
            player1Path[i] = new bool[size];
            player2Path[i] = new bool[size];
            cellNumbers[i] = new int[size];
            for (int j = 0; j < size; ++j)
            {
                cells[i][j] = other.cells[i][j] ? other.cells[i][j]->clone() : nullptr;
                player1Path[i][j] = other.player1Path[i][j];
                player2Path[i][j] = other.player2Path[i][j];
                cellNumbers[i][j] = other.cellNumbers[i][j];
            }
        }
        int totalCells = size * size;
        numberToPosX = new int[totalCells + 1];
        numberToPosY = new int[totalCells + 1];
        for (int i = 1; i <= totalCells; ++i)
        {
            numberToPosX[i] = other.numberToPosX[i];
            numberToPosY[i] = other.numberToPosY[i];
        }
    }
    return *this;
}
Board::~Board()
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            delete cells[i][j];
        }
        delete[] cells[i];
        delete[] player1Path[i];
        delete[] player2Path[i];
        delete[] cellNumbers[i];
    }
    delete[] cells;
    delete[] player1Path;
    delete[] player2Path;
    delete[] cellNumbers;
    delete[] numberToPosX;
    delete[] numberToPosY;
}
void Board::initializeItems()
{
    srand(static_cast<unsigned int>(time(0)));
    int minItems = size == 5 ? 4 : size * 2 - 2;
    int maxItems = size == 5 ? 10 : size * 4;
    int numItems = minItems + (rand() % (maxItems - minItems + 1));

    int goalX = size / 2;
    int goalY = size / 2;
    int player1StartX = size - 1;
    int player1StartY = size - 1;
    int player2StartX = 0;
    int player2StartY = 0;

    for (int i = 0; i < numItems; ++i)
    {
        int x, y;
        do
        {
            x = rand() % size;
            y = rand() % size;
        } while (cells[x][y] != nullptr || (x == goalX && y == goalY) || (x == player2StartX && y == player2StartY) || (x == player1StartX && y == player1StartY));
        if (rand() % 2)
        {
            cells[x][y] = new GoldCoin(x, y);
        }
        else
        {
            cells[x][y] = new SilverCoin(x, y);
        }
    }
}
void Board::markPlayerPath(int x, int y, bool isPlayer1)
{
    if (x >= 0 && x < size && y >= 0 && y < size)
    {
        if (isPlayer1)
        {
            player1Path[x][y] = true;
        }
        else
        {
            player2Path[x][y] = true;
        }
    }
}
void Board::print(const Player* player1, const Player* player2) const
{
    int goalX = size / 2;
    int goalY = size / 2;

    for (int j = 0; j < size; ++j)
    {
        cout << "+-----";
    }
    cout << "+" << endl;

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            cout << "|";
            string color = RESET;
            if (i == goalX && j == goalY)
            {
                color = GREEN;
            }
            else if (player1Path[i][j])
            {
                color = RED;
            }
            else if (player2Path[i][j])
            {
                color = BLUE;
            }
            cout << color << " ";

            if (player1->getX() == i && player1->getY() == j)
            {
                cout << BOLD << "P1" << RESET;
            }
            else if (player2->getX() == i && player2->getY() == j)
            {
                cout << BOLD << "P2" << RESET;
            }
            else if (i == goalX && j == goalY)
            {
                cout << "GOAL";
            }
            else if (cells[i][j])
            {
                cout << cells[i][j]->getShortName();
            }
            else
            {
                cout << "   ";
            }
            cout << " " << RESET;
        }
        cout << "|" << endl;

        for (int j = 0; j < size; ++j)
        {
            cout << "|" << setw(3) << cellNumbers[i][j] << "  ";
        }
        cout << "|" << endl;

        for (int j = 0; j < size; ++j)
        {
            cout << "+-----";
        }
        cout << "+" << endl;
    }
}
pair<int, int> Board::numberToPosition(int num) const
{
    if (num < 1 || num > size * size)
    {
        return { -1, -1 };
    }
    return { numberToPosX[num], numberToPosY[num] };
}

int Board::positionToNumber(int x, int y) const
{
    if (x < 0 || x >= size || y < 0 || y >= size)
    {
        return -1;
    }
    return cellNumbers[x][y];
}

Player::Player(int startNum, Board* b, int level) : board(b), currentLevel(level), gold(10), silver(20), points(0), invSize(0), invCapacity(10), blockedTurns(0), historyCount(0), historyStart(0), hasReachedGoal(false)
{
    inventory = new Item * [invCapacity]();
    for (int i = 0; i < invCapacity; ++i)
    {
        inventory[i] = nullptr;
    }
    setPosition(startNum);
}
Player::Player(const Player& other) : X(other.X), Y(other.Y), currentNumber(other.currentNumber), board(other.board), gold(other.gold), silver(other.silver), points(other.points), invSize(other.invSize), invCapacity(other.invCapacity), blockedTurns(other.blockedTurns), currentLevel(other.currentLevel), historyCount(other.historyCount), historyStart(other.historyStart), hasReachedGoal(other.hasReachedGoal)
{
    inventory = new Item * [invCapacity]();
    for (int i = 0; i < invCapacity; ++i)
    {
        inventory[i] = nullptr;
    }
    for (int i = 0; i < invSize && i < invCapacity; ++i)
    {
        if (other.inventory[i])
        {
            inventory[i] = other.inventory[i]->clone();
        }
    }
    for (int i = 0; i < MAX_HISTORY; ++i)
    {
        moveHistory[i] = other.moveHistory[i];
    }
}
Player& Player::operator=(const Player& other)
{
    if (this != &other)
    {
        for (int i = 0; i < invSize; ++i)
        {
            delete inventory[i];
        }
        delete[] inventory;

        X = other.X;
        Y = other.Y;
        currentNumber = other.currentNumber;
        board = other.board;
        gold = other.gold;
        silver = other.silver;
        points = other.points;
        invSize = other.invSize;
        invCapacity = other.invCapacity;
        blockedTurns = other.blockedTurns;
        currentLevel = other.currentLevel;
        historyCount = other.historyCount;
        historyStart = other.historyStart;
        hasReachedGoal = other.hasReachedGoal;

        inventory = new Item * [invCapacity]();
        for (int i = 0; i < invCapacity; ++i)
        {
            inventory[i] = nullptr;
        }
        for (int i = 0; i < invSize && i < invCapacity; ++i)
        {
            if (other.inventory[i])
            {
                inventory[i] = other.inventory[i]->clone();
            }
        }
        for (int i = 0; i < MAX_HISTORY; ++i)
        {
            moveHistory[i] = other.moveHistory[i];
        }
    }
    return *this;
}
Player::~Player()
{
    for (int i = 0; i < invSize; ++i)
    {
        delete inventory[i];
    }
    delete[] inventory;
}
void Player::addGold(int g)
{
    gold += g;
    points += g * 10;
}
void Player::addSilver(int s)
{
    silver += s;
    points += s * 5;
}
void Player::addItem(Item* item)
{
    if (invSize < invCapacity)
    {
        inventory[invSize++] = item->clone();
    }
}
void Player::block(int turns)
{
    blockedTurns = max(blockedTurns, turns);
}
void Player::unblock()
{
    if (blockedTurns > 0)
    {
        blockedTurns--;
    }
}
void Player::clearBlock()
{
    blockedTurns = 0;
}
void Player::printStatus() const
{
    cout << "Cell: " << currentNumber << " (Position: (" << X << "," << Y << ")), Points=" << points << ", Gold=" << gold << ", Silver=" << silver << endl;
    cout << "Inventory: ";
    if (invSize == 0)
    {
        cout << "Empty";
    }
    else
    {
        for (int i = 0; i < invSize; ++i)
        {
            if (inventory[i])
            {
                cout << inventory[i]->getName();
                if (Helper* helper = dynamic_cast<Helper*>(inventory[i]))
                {
                    cout << " (" << helper->getUses() << ")";
                }
                if (i < invSize - 1)
                {
                    cout << ", ";
                }
            }
        }
    }
    cout << endl;
}
void Player::deductPoints(int amount)
{
    points -= amount;
    if (points < 0)
    {
        points = 0;
    }
}
int Player::getPoints() const
{
    return points;
}
int Player::getGold() const
{
    return gold;
}
int Player::getSilver() const
{
    return silver;
}
void Player::deductGold(int amount)
{
    gold -= amount;
    if (gold < 0) {
        gold = 0;
    }
}
void Player::deductSilver(int amount)
{
    silver -= amount;
    if (silver < 0)
    {
        silver = 0;
    }
}
bool Player::checkInventory(const string& itemName) const
{
    for (int i = 0; i < invSize; ++i)
    {
        if (inventory[i] && inventory[i]->getName() == itemName)
        {
            return true;
        }
    }
    return false;
}
void Player::removeFromInventory(int index)
{
    if (index < 0 || index >= invSize)
    {
        return;
    }
    delete inventory[index];
    for (int i = index; i < invSize - 1; ++i)
    {
        inventory[i] = inventory[i + 1];
    }
    inventory[invSize - 1] = nullptr;
    invSize--;
}
bool Player::hasWater() const
{
    return checkInventory("Water");
}
bool Player::hasSword() const
{
    return checkInventory("Sword");
}
bool Player::hasShield() const
{
    return checkInventory("Shield");
}
bool Player::hasKey() const
{
    return checkInventory("Key");
}
void Player::useWater()
{
    for (int i = 0; i < invSize; ++i)
    {
        if (inventory[i] && inventory[i]->getName() == "Water")
        {
            removeFromInventory(i);
            break;
        }
    }
}
void Player::useSword()
{
    for (int i = 0; i < invSize; ++i)
    {
        if (inventory[i] && inventory[i]->getName() == "Sword")
        {
            Helper* helper = dynamic_cast<Helper*>(inventory[i]);
            helper->decrementUse();
            if (helper->getUses() <= 0)
            {
                removeFromInventory(i);
            }
            break;
        }
    }
}
void Player::useShield()
{
    for (int i = 0; i < invSize; ++i)
    {
        if (inventory[i] && inventory[i]->getName() == "Shield")
        {
            removeFromInventory(i);
            break;
        }
    }
}
void Player::useKey()
{
    for (int i = 0; i < invSize; ++i)
    {
        if (inventory[i] && inventory[i]->getName() == "Key")
        {
            removeFromInventory(i);
            break;
        }
    }
}
void Player::recordMove(int num)
{
    int index = (historyStart + historyCount) % MAX_HISTORY;
    moveHistory[index] = num;

    if (historyCount < MAX_HISTORY)
    {
        historyCount++;
    }
    else
    {
        historyStart = (historyStart + 1) % MAX_HISTORY;
    }
    auto pos = board->numberToPosition(num);
    board->markPlayerPath(pos.first, pos.second, this->X == 0 && this->Y == 0);
}
void Player::moveBack(int cells)
{
    if (historyCount <= 1)
    {
        return;
    }

    int movesToGoBack = min(cells, historyCount - 1);
    int newIndex = (historyStart + historyCount - 1 - movesToGoBack) % MAX_HISTORY;
    int newNum = moveHistory[newIndex];

    setPosition(newNum);
    historyCount -= movesToGoBack;

    cout << "Moved back " << movesToGoBack << " cells to cell " << currentNumber << " (" << X << "," << Y << ")" << endl;
}
void Player::levelUp(int startNum)
{
    currentLevel++;
    gold += 5;
    silver += 10;
    historyCount = 0;
    historyStart = 0;
    hasReachedGoal = false;
    blockedTurns = 0;
    setPosition(startNum);
}
void Player::placeHurdle(Item* hurdle)
{
    if (!board || !hurdle)
    {
        delete hurdle;
        return;
    }

    int placeNum;
    cout << "Enter cell number to place hurdle (1-" << board->getSize() * board->getSize() << "): ";
    cin >> placeNum;

    auto pos = board->numberToPosition(placeNum);
    if (pos.first == -1)
    {
        cout << "Invalid cell number!" << endl;
        delete hurdle;
        return;
    }

    if (board->cells[pos.first][pos.second])
    {
        cout << "Cell already occupied!" << endl;
        delete hurdle;
        return;
    }

    board->cells[pos.first][pos.second] = hurdle;
    cout << "Placed " << hurdle->getName() << " at cell " << placeNum << " (" << pos.first << "," << pos.second << ")" << endl;
}
string Player::getCurrentHurdle() const
{
    if (board && X >= 0 && X < board->getSize() && Y >= 0 && Y < board->getSize())
    {
        Item* item = board->cells[X][Y];
        if (Hurdle* hurdle = dynamic_cast<Hurdle*>(item))
        {
            return hurdle->getName();
        }
    }
    return "None";
}
void Player::clearHurdle()
{
    if (board && X >= 0 && X < board->getSize() && Y >= 0 && Y < board->getSize())
    {
        Item* cell = board->cells[X][Y];
        if (dynamic_cast<Hurdle*>(cell))
        {
            delete cell;
            board->cells[X][Y] = nullptr;
        }
    }
}
void Player::displayInventory() const
{
    if (invSize == 0)
    {
        cout << "Inventory is empty!" << endl;
        return;
    }
    cout << "Inventory:" << endl;
    for (int i = 0; i < invSize; i++)
    {
        if (inventory[i])
        {
            cout << i + 1 << ". " << inventory[i]->getName();
            if (Helper* helper = dynamic_cast<Helper*>(inventory[i]))
            {
                cout << " (Uses: " << helper->getUses() << ")";
            }
            cout << endl;
        }
    }
}
bool Player::useItem(int index)
{
    if (index < 0 || index >= invSize || !inventory[index])
    {
        return false;
    }

    Helper* helper = dynamic_cast<Helper*>(inventory[index]);
    if (!helper)
    {
        return false;
    }

    string currentHurdle = getCurrentHurdle();
    if (!helper->canUseOn(currentHurdle))
    {
        cout << helper->getName() << " cannot be used on " << currentHurdle << "!" << endl;
        return false;
    }

    helper->use(*this);
    if (helper->getUses() <= 0)
    {
        removeFromInventory(index);
    }
    return true;
}

void Player::setPosition(int num)
{
    auto pos = board->numberToPosition(num);
    if (pos.first == -1) return;
    X = pos.first;
    Y = pos.second;
    currentNumber = num;
    recordMove(num);
}
GoldCoin::GoldCoin(int x, int y) : Item(x, y)
{
    name = "Gold";
}
void GoldCoin::interact(Player& player)
{
    player.addGold(1);
    cout << "Picked up Gold Coin! (+10 points)" << endl;
}
Item* GoldCoin::clone() const
{
    return new GoldCoin(*this);
}
SilverCoin::SilverCoin(int x, int y) : Item(x, y)
{
    name = "Silver";
}

void SilverCoin::interact(Player& player)
{
    player.addSilver(1);
    cout << "Picked up Silver Coin! (+5 points)" << endl;
}
Item* SilverCoin::clone() const
{
    return new SilverCoin(*this);
}
Helper::Helper(int x, int y, int c, int u) : Item(x, y), cost(c), uses(u) {}
int Helper::getCost() const
{
    return cost;
}
int Helper::getUses() const
{
    return uses;
}
Helper::~Helper() {}
void Helper::decrementUse()
{
    if (uses > 0)
    {
        uses--;
    }
}
Sword::Sword(int x, int y) : Helper(x, y, 40, 2)
{
    name = "Sword";
}

void Sword::interact(Player& player)
{
    if (player.getPoints() >= getCost())
    {
        player.deductPoints(getCost());
        player.addItem(this);
        cout << "Bought Sword! (-40 points)" << endl;
    }
    else
    {
        cout << "Not enough points to buy Sword!" << endl;
    }
}
void Sword::use(Player& player)
{
    player.clearHurdle();
    cout << "Used Sword to clear hurdle!" << endl;
}
bool Sword::canUseOn(const string& hurdleType) const
{
    return hurdleType == "Snake" || hurdleType == "Lion";
}
Item* Sword::clone() const
{
    Sword* copy = new Sword(*this);
    copy->uses = this->uses;
    return copy;
}
Shield::Shield(int x, int y) : Helper(x, y, 30, 1)
{
    name = "Shield";
}
void Shield::interact(Player& player)
{
    if (player.getPoints() >= getCost())
    {
        player.deductPoints(getCost());
        player.addItem(this);
        cout << "Bought Shield! (-30 points)" << endl;
    }
    else
    {
        cout << "Not enough points to buy Shield!" << endl;
    }
}
void Shield::use(Player& player)
{
    player.clearHurdle();
    cout << "Used Shield to clear hurdle!" << endl;
}
bool Shield::canUseOn(const string& hurdleType) const
{
    return hurdleType == "Ghost";
}
Item* Shield::clone() const
{
    Shield* copy = new Shield(*this);
    copy->uses = this->uses;
    return copy;
}
Water::Water(int x, int y) : Helper(x, y, 50, 1)
{
    name = "Water";
}
void Water::interact(Player& player)
{
    if (player.getPoints() >= getCost())
    {
        player.deductPoints(getCost());
        player.addItem(this);
        cout << "Bought Water! (-50 points)" << endl;
    }
    else {
        cout << "Not enough points to buy Water!" << endl;
    }
}
void Water::use(Player& player)
{
    player.clearHurdle();
    cout << "Used Water to clear hurdle!" << endl;
}
bool Water::canUseOn(const string& hurdleType) const
{
    return hurdleType == "Fire";
}
Item* Water::clone() const
{
    Water* copy = new Water(*this);
    copy->uses = this->uses;
    return copy;
}
Key::Key(int x, int y) : Helper(x, y, 60, 1)
{
    name = "Key";
}
void Key::interact(Player& player)
{
    if (player.getSilver() >= 12)
    {
        player.deductSilver(12);
        player.addItem(this);
        cout << "Bought Key! (-12 silver)" << endl;
    }
    else
    {
        cout << "Not enough silver to buy Key!" << endl;
    }
}
void Key::use(Player& player)
{
    player.clearHurdle();
    player.clearBlock();
    cout << "Used Key to unlock!" << endl;
}
bool Key::canUseOn(const string& hurdleType) const
{
    return hurdleType == "Lock";
}
Item* Key::clone() const
{
    Key* copy = new Key(*this);
    copy->uses = this->uses;
    return copy;
}
Hurdle::Hurdle(int x, int y, int c, int p, int turns) : Item(x, y), cost(c), penalty(p), blockTurns(turns) {}
void Hurdle::interact(Player& player)
{
    if (player.getPoints() >= getCost())
    {
        player.deductPoints(getCost());
        player.placeHurdle(this->clone());
    }
    else
    {
        cout << "Not enough points to place " << name << "!" << endl;
    }
}
int Hurdle::getCost() const
{
    return cost;
}
int Hurdle::getPenalty() const
{
    return penalty;
}
Hurdle::~Hurdle() {}
Fire::Fire(int x, int y) : Hurdle(x, y, 50, 50, 2)
{
    name = "Fire";
}
void Fire::interact(Player& player)
{
    Hurdle::interact(player);
}
void Fire::applyEffect(Player& player)
{
    if (!player.hasWater())
    {
        player.block(getBlockTurns());
        player.deductPoints(getPenalty());
        cout << "Blocked by Fire! -50 points, wait 2 turns." << endl;
    }
    else
    {
        player.useWater();
    }
}
Item* Fire::clone() const
{
    return new Fire(*this);
}
Snake::Snake(int x, int y) : Hurdle(x, y, 30, 30, 3)
{
    name = "Snake";
}

void Snake::interact(Player& player)
{
    Hurdle::interact(player);
}

void Snake::applyEffect(Player& player)
{
    if (!player.hasSword())
    {
        player.block(getBlockTurns());
        player.moveBack(3);
        player.deductPoints(getPenalty());
        cout << "Blocked by Snake! -30 points, moved back 3 cells, wait 3 turns." << endl;
    }
    else
    {
        player.useSword();
    }
}
Item* Snake::clone() const
{
    return new Snake(*this);
}
Ghost::Ghost(int x, int y) : Hurdle(x, y, 20, 20, 1)
{
    name = "Ghost";
}
void Ghost::interact(Player& player)
{
    Hurdle::interact(player);
}
void Ghost::applyEffect(Player& player)
{
    if (!player.hasShield())
    {
        player.block(getBlockTurns());
        player.deductPoints(getPenalty());
        cout << "Blocked by Ghost! -20 points, wait 1 turn." << endl;
    }
    else
    {
        player.useShield();
    }
}
Item* Ghost::clone() const
{
    return new Ghost(*this);
}
Lion::Lion(int x, int y) : Hurdle(x, y, 50, 50, 4)
{
    name = "Lion";
}
void Lion::interact(Player& player)
{
    if (player.getPoints() >= getCost() && player.getGold() >= 1)
    {
        player.deductPoints(getCost());
        player.deductGold(1);
        player.placeHurdle(this->clone());
    }
    else
    {
        cout << "Not enough points or gold to place Lion!" << endl;
    }
}
void Lion::applyEffect(Player& player)
{
    if (!player.hasSword())
    {
        player.block(getBlockTurns());
        player.deductPoints(getPenalty());
        cout << "Blocked by Lion! -50 points, wait 4 turns." << endl;
    }
    else
    {
        player.useSword();
    }
}
Item* Lion::clone() const
{
    return new Lion(*this);
}
Lock::Lock(int x, int y) : Hurdle(x, y, 60, 0, 99)
{
    name = "Lock";
}
void Lock::interact(Player& player)
{
    if (player.getSilver() >= 12)
    {
        player.deductSilver(12);
        player.placeHurdle(this->clone());
    }
    else
    {
        cout << "Not enough silver to place Lock!" << endl;
    }
}
void Lock::applyEffect(Player& player)
{
    if (!player.hasKey())
    {
        player.block(getBlockTurns());
        cout << "Blocked by Lock! Need a key to proceed." << endl;
    }
    else
    {
        player.useKey();
    }
}
Item* Lock::clone() const
{
    return new Lock(*this);
}
AdventureQuest::AdventureQuest() : currentLevel(1), player1ReachedFirst(false), player2ReachedFirst(false)
{
    board = new Board(currentLevel);
    int totalCells = board->getSize() * board->getSize();
    player1 = new Player(totalCells, board, currentLevel);
    player2 = new Player(1, board, currentLevel);
    goalNumber = (totalCells + 1) / 2;
    savedState = nullptr;
}
AdventureQuest::AdventureQuest(const AdventureQuest& other) : currentLevel(other.currentLevel), goalNumber(other.goalNumber), player1ReachedFirst(other.player1ReachedFirst), player2ReachedFirst(other.player2ReachedFirst)
{
    board = new Board(*other.board);
    player1 = new Player(*other.player1);
    player2 = new Player(*other.player2);
    player1->board = board;
    player2->board = board;
    savedState = nullptr;
}
AdventureQuest& AdventureQuest::operator=(const AdventureQuest& other)
{
    if (this != &other)
    {
        delete board;
        delete player1;
        delete player2;
        delete savedState;

        board = new Board(*other.board);
        player1 = new Player(*other.player1);
        player2 = new Player(*other.player2);
        player1->board = board;
        player2->board = board;
        savedState = nullptr;
        currentLevel = other.currentLevel;
        goalNumber = other.goalNumber;
        player1ReachedFirst = other.player1ReachedFirst;
        player2ReachedFirst = other.player2ReachedFirst;
    }
    return *this;
}
AdventureQuest::~AdventureQuest()
{
    delete board;
    delete player1;
    delete player2;
    delete savedState;
}
void AdventureQuest::saveGame()
{
    delete savedState;
    savedState = new AdventureQuest(*this);
    cout << "Game state saved!" << endl;
}
void AdventureQuest::loadGame()
{
    if (savedState) {
        AdventureQuest* temp = new AdventureQuest(*savedState);
        *this = *temp;
        delete temp;
        cout << "Game state loaded!" << endl;
    }
    else
    {
        cout << "No saved game available!" << endl;
    }
}
void AdventureQuest::playTurn(Player* player, Player* opponent, bool isPlayer1)
{
    cout << endl;
    cout << (isPlayer1 ? "Player 1" : "Player 2") << "'s turn:" << endl;
    player->printStatus();
    board->print(player1, player2);

    if (player->isBlocked())
    {
        cout << (isPlayer1 ? "Player 1" : "Player 2") << " is blocked for " << player->blockedTurns << " more turns!" << endl;
        player->unblock();
        return;
    }

    if (player->hasReachedGoalState())
    {
        cout << "Player has reached the goal and is waiting." << endl;
        return;
    }

    int choice;
    cout << "1. Move" << endl;
    cout << "2. Place Hurdle" << endl;
    cout << "3. Buy Helper" << endl;
    cout << "4. Use Item" << endl;
    cout << "Choice: ";
    cin >> choice;

    if (choice == 1)
    {
        int nextNum;
        if (isPlayer1)
        {
            nextNum = player->getCurrentNumber() - 1;
        }
        else
        {
            nextNum = player->getCurrentNumber() + 1;
        }

        if (nextNum < 1 || nextNum > board->getSize() * board->getSize())
        {
            cout << "Cannot move further!" << endl;
            return;
        }

        auto pos = board->numberToPosition(nextNum);
        if (pos.first == opponent->getX() && pos.second == opponent->getY() && nextNum != goalNumber)
        {
            cout << "Cell occupied by opponent! Wait for your next turn." << endl;
            return;
        }

        player->setPosition(nextNum);

        Item* item = board->getItem(pos.first, pos.second);
        if (item)
        {
            if (Hurdle* hurdle = dynamic_cast<Hurdle*>(item))
            {
                hurdle->applyEffect(*player);
            }
            else
            {
                item->interact(*player);
                board->setItem(pos.first, pos.second, nullptr);
                delete item;
            }
        }

        if (nextNum == goalNumber)
        {
            player->setReachedGoal(true);
            if (isPlayer1) {
                player1ReachedFirst = true;
            }
            else
            {
                player2ReachedFirst = true;
            }
            cout << (isPlayer1 ? "Player 1" : "Player 2") << " has reached the goal (cell " << goalNumber << ")!" << endl;
        }
    }
    else if (choice == 2)
    {
        int hurdleChoice;
        cout << "Select hurdle to place:" << endl;
        cout << "1. Fire (50 pts)" << endl;
        cout << "2. Snake (30 pts)" << endl;
        cout << "3. Ghost (20 pts)" << endl;
        cout << "4. Lion (50 pts + 1 gold)" << endl;
        cout << "5. Lock (12 silver)" << endl;
        cout << "Choice: ";
        cin >> hurdleChoice;

        Hurdle* newHurdle = nullptr;
        switch (hurdleChoice) {
        case 1: newHurdle = new Fire(player->getX(), player->getY()); break;
        case 2: newHurdle = new Snake(player->getX(), player->getY()); break;
        case 3: newHurdle = new Ghost(player->getX(), player->getY()); break;
        case 4: newHurdle = new Lion(player->getX(), player->getY()); break;
        case 5: newHurdle = new Lock(player->getX(), player->getY()); break;
        default: cout << "Invalid choice!" << endl; return;
        }
        newHurdle->interact(*player);
    }
    else if (choice == 3)
    {
        int helperChoice;
        cout << "Select helper to buy:" << endl;
        cout << "1. Sword (40 pts)" << endl;
        cout << "2. Shield (30 pts)" << endl;
        cout << "3. Water (50 pts)" << endl;
        cout << "4. Key (12 silver)" << endl;
        cout << "Choice: ";
        cin >> helperChoice;

        Helper* newHelper = nullptr;
        switch (helperChoice) {
        case 1: newHelper = new Sword(player->getX(), player->getY()); break;
        case 2: newHelper = new Shield(player->getX(), player->getY()); break;
        case 3: newHelper = new Water(player->getX(), player->getY()); break;
        case 4: newHelper = new Key(player->getX(), player->getY()); break;
        default: cout << "Invalid choice!" << endl; return;
        }
        newHelper->interact(*player);
        delete newHelper;
    }
    else if (choice == 4)
    {
        player->displayInventory();
        if (player->invSize == 0)
        {
            return;
        }
        cout << "Select item to use (1-" << player->invSize << "): ";
        int itemChoice;
        cin >> itemChoice;
        player->useItem(itemChoice - 1);
    }
}
void AdventureQuest::startGame()
{
    player1ReachedFirst = false;
    player2ReachedFirst = false;

    while (!(player1->hasReachedGoalState() && player2->hasReachedGoalState()))
    {
        cout << endl;
        cout << "P1: ";
        playTurn(player1, player2, true);

        cout << endl;
        cout << "P2: ";
        playTurn(player2, player1, false);

        if (player1->hasReachedGoalState() && player2->hasReachedGoalState())
        {
            determineWinner();
            break;
        }

        cout << endl;
        cout << "1. Continue" << endl;
        cout << "2. Save" << endl;
        cout << "3. Load" << endl;
        cout << "Choice: ";
        int choice;
        cin >> choice;
        if (choice == 2)
        {
            saveGame();
        }
        else if (choice == 3)
        {
            loadGame();
        }
    }
}
void AdventureQuest::determineWinner()
{
    cout << endl;
    cout << "Game Over!" << endl;
    cout << "Player 1 Points: " << player1->getPoints() << endl;
    cout << "Player 2 Points: " << player2->getPoints() << endl;

    bool p1ReachedFirst = player1ReachedFirst;
    bool p2ReachedFirst = player2ReachedFirst;
    int p1Points = player1->getPoints();
    int p2Points = player2->getPoints();

    if (p1ReachedFirst && p1Points > p2Points)
    {
        cout << "Player 1 wins!" << endl;
    }
    else if (p2ReachedFirst && p2Points > p1Points)
    {
        cout << "Player 2 wins!" << endl;
    }
    else
    {
        cout << "It's a draw!" << endl;
    }
}
void AdventureQuest::levelUp()
{
    if (currentLevel >= 4)
    {
        cout << "Maximum level reached!" << endl;
        return;
    }

    currentLevel++;
    delete board;
    board = new Board(currentLevel);

    int totalCells = board->getSize() * board->getSize();
    goalNumber = (totalCells + 1) / 2;

    player1->levelUp(totalCells);
    player2->levelUp(1);

    cout << "Level Up! Now playing on " << board->getSize() << "x" << board->getSize() << " board!" << endl;
}
int main()
{
    int choice;
    AdventureQuest* game = nullptr;

    while (true)
    {
        cout << "=== Adventure Quest ===" << endl;
        cout << "1. New Game" << endl;
        cout << "2. Load Game" << endl;
        cout << "3. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1)
        {
            delete game;
            game = new AdventureQuest();
            while (game->getBoardLevel() <= 4)
            {
                game->startGame();
                if (game->getBoardLevel() >= 4)
                {
                    cout << "Maximum level reached!" << endl;
                    break;
                }
                cout << "1. Next Level" << endl;
                cout << "2. Exit" << endl;
                cout << "Choice: ";
                cin >> choice;
                if (choice == 1)
                {
                    game->levelUp();
                }
                else
                {
                    delete game;
                    game = nullptr;
                    break;
                }
            }
            if (game)
            {
                delete game;
                game = nullptr;
            }
        }
        else if (choice == 2)
        {
            if (game)
            {
                game->loadGame();
                game->startGame();
            }
            else
            {
                cout << "No saved game found! Starting new game..." << endl;
                game = new AdventureQuest();
                game->startGame();
            }
        }
        else if (choice == 3)
        {
            delete game;
            cout << "Thanks for playing!" << endl;
            break;
        }
        else
        {
            cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}
