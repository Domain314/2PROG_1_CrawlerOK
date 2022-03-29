#include <iostream>
#include <conio.h> //this header is needed to use _getch()

using namespace std;

int main () {
	cout << "Do you want to see the secret word? (y/n): ";
	char move = _getch(); //_getch loads move with one character that the user enters
			      //the code continues after one key is pressed
		if(move == 'y') {
			cout << "\nThe secret word is chicken burger!" << endl;
		}
		else if(move == 'n') {
			cout << "\nWhy didn't you want to see the secret word?" << endl;
		}
		else {
			cout << "\nInvalid input." << endl;
		}
		return 0;
}
