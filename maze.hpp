// Lab_9 Game
// Wyatt Collier
// 4/5/2024

#include <cstdlib>
#include <ctime>


#ifndef MAZE_HPP_
#define MAZE_HPP_

// room coordinates
class Room {
public:

    // void constructor, assigns -1 to X coord, and * to Y coord
    // use initializer list for implementation
    // initializer list is important for efficient code
    // better than placing in the body of function
    Room() : x_(-1), y_('*') {

    }

    // selects a random room within the maze
    void pick() {
        x_ = rand() % mazeSize_ + 1;
        y_ = rand() % mazeSize_ + 'a';
    }

    // selects a random adjacent room
    // uses goodDirection() and createAdjacent()
    const Room pickAdjacent() {
        Room randRoom;
        bool selection = true;
        while (selection) {
            char randDir = rand() % 4 + 1;
            switch (randDir) {
            case 1:
                randDir = 'u';
                break;
            case 2:
                randDir = 'd';
                break;
            case 3:
                randDir = 'l';
                break;
            case 4:
                randDir = 'r';
                break;
            }

            if (goodDirection(randDir)) {
                randRoom = createAdjacent(randDir);
                selection = false;
            }
        }

        return randRoom;
    }

    // asks the user for the room to move adjacent to this room
    // returns {-1,*} if the user wants to quit
    // uses goodDirection() and createAdjacent()
    const Room nextMove() const {
        Room nextRoom;
        bool validSel = false;

        while (!validSel) {
            char dr;
            std::cout << "Next move [u/d/l/r/q]: ";
            std::cin >> dr;
            std::string validChars = "udlrq"; //similar to txt just Lab

            if (validChars.find(dr) == std::string::npos) {  // if direction not found in string evaluates true
                std::cout << std::endl;
                std::cout << "Please enter a valid character.";
                std::cout << std::endl;
                continue;
            }

            if (dr == 'q') {
                return Room();
            }

            if (goodDirection(dr)) {
                nextRoom = createAdjacent(dr); // checks for good dir then assigns adj room
                return nextRoom;
            }

            std::cout << "wall\n" << std::endl; // outputs wall to show edge of maze
  
        }
    }

    // prints room coordinates in format "a1"
    void print() const {
        std::cout << x_;
        std::cout << y_;
    }

    // make this room the start room
    void makeStartRoom() { x_ = 1; y_ = 'a'; }

    // make this room - the cheese room
    void makeCheeseRoom() { x_ = mazeSize_, y_ = 'a' + mazeSize_ - 1; };

    // predicate returns true if the coordinates of the two rooms match
    friend bool matchRoom(const Room& one, const Room& two) {
        return one.x_ == two.x_ && one.y_ == two.y_;
    }

private:
    // checks if this direction keeps the mouse inside the maze
    bool goodDirection(const char dr) const {
        switch (dr) {
        case 'u':
            return y_ + 1 <= 'a' + mazeSize_ - 1;
        case 'd':
            return y_ - 1 >= 'a';
        case 'l':
            return x_ - 1 >= 1;
        case 'r':
            return x_ + 1 <= mazeSize_;
        }
    }

    // returns the room adjacent to this room in the specified direction
    // assumes the direction is inside the maze
    // adjusts either x or y coor by one depending on input
    const Room createAdjacent(const char dr) const {
        int adjX = x_;
        char adjY = y_;
        Room newRoom;

        switch (dr) {
        case 'u':
            adjY += 1;
            break;
        case 'd':
            adjY -= 1;
            break;
        case 'l':
            adjX -= 1;
            break;
        case 'r':
            adjX += 1;
            break;
        default:
            std::cout << "Please enter a valid character.\n";
            break;
        }

        newRoom.x_ = adjX;
        newRoom.y_ = adjY;
        return newRoom;
    }

    static const int mazeSize_ = 4; // maze is mazeSize * mazeSize
    int x_;  // 1 through mazeSize
    char y_; // 'a' through mazeSize
};


// internal wall or next move
class RoomPair {
public:
    RoomPair() {} // void constructor implicitly invokes void constructors on member variables

    // makes a roomPair out of two rooms
    RoomPair(const Room& one, const Room& two) :one_(one), two_(two) {}

    //selects random room
    //selects a random wall, uses Room::pickAdjacent()
    void pick() {
        one_.Room::pick();
        two_ = one_.Room::pickAdjacent();
    }

    // prints the locations of the two rooms in format a1|b1
    // uses Room::print()
    void print() const {
        one_.Room::print();
        std::cout << '|';
        two_.Room::print();
    }


    // returns true if two pairs of adjacent rooms are the same,
    // returns false otherwise, uses matchRoom() note that r1|r2 matches r2|r1
    // checks that RoomPair values can match in both directions
    friend bool matchPair(const RoomPair& pair1, const RoomPair& pair2) {
        return (matchRoom(pair1.one_, pair2.one_) && matchRoom(pair1.two_, pair2.two_)) ||
            (matchRoom(pair1.one_, pair2.two_) && matchRoom(pair1.two_, pair2.one_));
    }

private:
    Room one_;
    Room two_;
};

class Maze {
public:
    // places every wall between two rooms where x-coordinate is -1
    // and y-coordinate is '*' (a star) to signify that
    // the wall is not built yet
    Maze() {
        for (int i = 0; i < numWalls_; i++) {
            walls_[i] = RoomPair(Room(), Room());
        }
    }

    // prints the locations of all the internal walls of the maze
    // and current mouse location
    void print() const {
        for (int i = 0; i < numWalls_; i++) {
            walls_[i].print();
            std::cout << std::endl;
        }


        std::cout << std::endl;
    }

    // places internal walls in random locations of the maze
    // all walls are different
    void build() {
        for (int i = 0; i < numWalls_; i++) {
            RoomPair wall;
            do {
                wall.pick();
            } while (checkMaze(wall)); // loops till non matching wall in array
            walls_[i] = wall;
        }
    }

    // places current mouse location in startRoom_;
    void start() { currentRoom_.makeStartRoom(); }

    // takes the room to move the mouse to
    // moves the mouse there and returns true if no walls (move is possible)
    // does not move the mouse and returns false if wall
    bool move(const Room& newRoom) {
        RoomPair newMove(currentRoom_, newRoom);
        if (checkMaze(newMove)) { // expression is true if move matches wall
            return false;
        }

        currentRoom_ = newRoom; // no wall, updates value
        return true;
    }

    // returns current mouse location
    const Room getCurrentRoom() { return currentRoom_; }

private:
    // returns true if RoomPair matches one of the walls in the maze
    bool checkMaze(const RoomPair& move) const {
        for (int i = 0; i < numWalls_; i++) {
            if (matchPair(walls_[i], move)) {
                return true; // move matches wall in maze
            }
        }
        return false;
    }

    Room currentRoom_; // current mouse Location

    static const int numWalls_ = 2;  // number of internal walls
    RoomPair walls_[numWalls_]; // array of internal maze walls
};

#endif // MAZE_HPP_