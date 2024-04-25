// Lab_9 Game
// Wyatt Collier
// 4/5/2024

#include <iostream>
#include "maze.hpp"

using std::cout; using std::cin; using std::endl;

int main() {
	Maze myMaze;
	Room cheeseRoom;
	Room exitRoom;

	cheeseRoom.makeCheeseRoom();
	myMaze.start();
	myMaze.build();
	

	while (true) {
		char selection;
		cout << "Show walls? [y/n]: ";
		cin >> selection;

		if (selection == 'y') {
			myMaze.print();
			cout << endl;
			break;
		}
		else if (selection == 'n') {
			cout << endl;
			break;
		}

		cout << "Please enter either \"y\" or \"n\"." << endl;
	}

	while (!matchRoom(myMaze.getCurrentRoom(), cheeseRoom)) {
		if (matchRoom(myMaze.getCurrentRoom(), exitRoom)) {
			cout << "Exiting program.";
			return 0;
		}

		Room currentRoom = myMaze.getCurrentRoom();
		cout << "Current mouse location: ";
		currentRoom.print();
		cout << endl;

		Room nextRoom = currentRoom.nextMove();  // Get the next room based on user input

		if (!myMaze.move(nextRoom)) {
			cout << "wall\n" << endl;
		}
		else {
			cout << endl;
		}
	}
	
	cout << "you have found the cheese!";
	cout << endl;
	cout << R"(
       (`-()_.-=-.
       /66  ,  ,  \
     =(o_/=//_(   /======`
jgs      ~"` ~"~~`
	  )" << endl;
}