#pragma comment(lib, "winmm.lib")
#include <iostream>
#include <conio.h>
#include <fstream>
#include <windows.h>
#include "BST.h"
class Wordle {
private:
	string toUse;										//Input word from file
	BST toSearch;										//To be stored in here for searching. Max amount of searches will be n = 30
	short CONTROL_VARIABLE;
public:
	Wordle()
	{
		srand(time(0));									//Constructor randomizes seed based on current time
		toUse = "";										//Constructor initializes the string with NULL. BST already has a constructor so need to
		CONTROL_VARIABLE = 6;							//initialize BST
	}
	void takingInput()
	{
		ifstream fin("Dictionary.txt");						//My dictionary of capital 5 letter words
		fin.seekg(0, ios::end);								//Jump to the last byte which is the last character in the file
		unsigned short wordNum = (fin.tellg() / 7) + 1;		//Divide the total number of bytes by 5(Number of letters in each word) + 2(2 bytes taken
		fin.seekg((rand() % wordNum) * 7, ios::beg);		//by \n). Then we choose a random word by taking mod of random number by total number of
		fin >> toUse;										//words and multiply by number of bytes for jumping to the starting byte of the chosen
		fin.close();										//word. We input the word and close the file.
	}
	void storeBST()
	{
		toSearch.cleanBST(toSearch.getRoot());				//If a previous word was stored in the tree we need to clean it.
		if (toUse.size())									//If the size of the word is greater than 0 then we store in BST. Just a small error handle
		{
			for (unsigned short loop = 0; loop < toUse.size(); loop++)
			{
				toSearch.insert(toUse[loop]);				//Insert every character in the BST
			}
		}
	}
	void printTitle()						//Prints the title of the game
	{
		cout << "                                                                   WORDLE\n\n\n\n";
	}
	void printCurrentWord(HANDLE& h, string& temp, unsigned short colorArray[])	//Prints the word supplied in temp with correct color codes
	{
		cout << "WORD:                                ";
		for (unsigned short loop = 0; loop < 5; loop++)
		{
			cout << "|";									//Prints start of column in normal terminal color
			SetConsoleTextAttribute(h, colorArray[loop]);   //Change the color of the next character to be printed to correct code provided in array
			cout << temp[loop];								//Print the character
			SetConsoleTextAttribute(h, 7);					//Reset the color to default
			cout << "|";									//Print the end of current column
		}
		cout << "\n";										//Move to next line
	}
	void askInput(string& temp)
	{
		cout << "\n\nPlease enter an Uppercase 5 letter word:\t     ";		//Ask for input to be checked
		cin >> temp;
	}
	void firstErrorHandle(HANDLE& h, string temp[], unsigned short colorArray[][5], unsigned short counter)
	{
		bool errorer = false;					//Checks for error and then handles them
		for (unsigned long long loop = 0; loop < temp[counter + 1].size(); loop++)	//Checking if the entered word is uppercase or not
		{
			if (temp[counter + 1][loop] < 'A' || temp[counter + 1][loop] > 'Z')
			{
				errorer = true;					//If a non uppercase letter is found then error handle
				break;
			}
		}
		while (temp[counter + 1].size() != 5 || errorer)			//If the size is not 5 or another error condition was encountered
		{
			system("CLS");
			for (unsigned short loop = 5; loop > 0; loop--)			//Ask the player to enter again
			{
				cout << "Please enter correctly again (" << loop << ")";
				Sleep(800);
				system("CLS");
			}
			printTitle();											//Bring the old screen back by reprinting the title and everything
			for (unsigned short loop = 0; loop <= counter; loop++)  //that followed
			{
				printCurrentWord(h, temp[loop], colorArray[loop]);
			}
			askInput(temp[counter + 1]);							//Prompt for input
			errorer = false;
			for (unsigned long long loop = 0; loop < temp[counter + 1].size(); loop++) //Checking if the new entered word is uppercase or not
			{
				if (temp[counter + 1][loop] < 'A' || temp[counter + 1][loop] > 'Z')
				{
					errorer = true;				//If a non uppercase letter is found then error handle
					break;
				}
			}
		}
	}
	void checkPresence(string& temp, unsigned short colorArray[])  //Checks the characters if they are in the actual word or not
	{
		for (unsigned short loop = 0; loop < 5; loop++)
		{
			if (toSearch.search(temp[loop]))		//If the letter is present in the word then
			{
				if (toUse[loop] == temp[loop])		//If the letter is present at the same location as is encountered in the user string
				{
					colorArray[loop] = 47;			//The word is at the correct location so update the color which is green
					continue;						//check next
				}
				colorArray[loop] = 111;				//If the letter is present in the word but not at the same location it is encountered then
				continue;							//update the corresponding color which is gold
			}
			colorArray[loop] = 64;					//If the letter was not found then update the color which is red
		}
	}
	bool winCheck(unsigned short colorArray[])		//Check if the user won in this try
	{
		for (unsigned short loop = 0; loop < 5; loop++)	//Check every letter's status in the current input
		{
			if (colorArray[loop] != 47)					//If all characters are green then the user won else we return false
			{
				return false;
			}
		}
		return true;
	}
	void run()
	{
		unsigned short count = 0;			//Count for how many tries the user took to guess the letter
		takingInput();						//Getting a word
		storeBST();							//Storing the word
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);			//Getting the terminal access for color changing
		string temp[7] = { "     " ,"     ","     ","     ","     ","     ", "     " };   //Save every input made by user for interfacing
		unsigned short colorArray[7][5] = { { 64, 64, 64, 64, 64 },				//Corresponding color codes for each letter in each word
											{ 64, 64, 64, 64, 64 },
											{ 64, 64, 64, 64, 64 },
											{ 64, 64, 64, 64, 64 },
											{ 64, 64, 64, 64, 64 },
											{ 64, 64, 64, 64, 64 },
											{ 64, 64, 64, 64, 64 } };
		bool win = false;												//Win status
		for (unsigned short counter = 0; counter < 6; counter++)		//Give the user 6 tries
		{
			if (win = winCheck(colorArray[counter]))					//exit condition for winning
			{
				break;
			}
			printTitle();								//Start interfacing
			for (unsigned short counter1 = 0; counter1 <= counter; counter1++)		//Print all the tries that have been made till now
			{
				printCurrentWord(hConsole, temp[counter1], colorArray[counter1]);   //Pass each try to be printed
			}
			askInput(temp[counter + 1]);								//Ask for input
			firstErrorHandle(hConsole, temp, colorArray, counter);		//Error handle the input
			checkPresence(temp[counter + 1], colorArray[counter + 1]);	//Check if the current input is correct or not
			count++;
			system("CLS");												//Clean for next input
		}
		printTitle();								//Printing all the tries the user made in the whole game
		for (unsigned short counter1 = 0; counter1 <= count; counter1++)
		{
			printCurrentWord(hConsole, temp[counter1], colorArray[counter1]);
		}
		if (win)						//Self explanatory
		{
			cout << "\n\n\n\t\t\tYou win!";
		}
		else
		{
			cout << "\n\n\n\t\t\tYou lose";
		}
		Sleep(3000);					//Start new game
		system("CLS");
		cout << "Returning to Main menu.";
		Sleep(3000);
		system("CLS");
		playInterface();
	}
	void playInterface()
	{

		if (CONTROL_VARIABLE == 0) //Music 1
		{
			PlaySound(TEXT("8 bit oop.wav"), NULL, SND_LOOP | SND_ASYNC);
		}
		else if (CONTROL_VARIABLE == 1) //Music 2
		{
			PlaySound(TEXT("Arabic oop.wav"), NULL, SND_LOOP | SND_ASYNC);
		}
		else if (CONTROL_VARIABLE == 2) //Music 3
		{
			PlaySound(TEXT("Blade runner resonance oop.wav"), NULL, SND_LOOP | SND_ASYNC);
		}
		else if (CONTROL_VARIABLE == 3) //Music 4
		{
			PlaySound(TEXT("L Equipe Du Son oop.wav"), NULL, SND_LOOP | SND_ASYNC);
		}
		else if (CONTROL_VARIABLE == 4) //Music 5
		{
			PlaySound(TEXT("Whistle oop.wav"), NULL, SND_LOOP | SND_ASYNC);
		}
		else if (CONTROL_VARIABLE == 5) //Music 6
		{
			PlaySound(TEXT("Ching chang hanji oop.wav"), NULL, SND_LOOP | SND_ASYNC);
		}

		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		short enter = '1';
		short arrowKey = '1';
		short enter1 = 192;
		short enter2 = 7;
		short enter3 = 7;
		short enter4 = 7;
		string coutArray[5];
		coutArray[0].erase();
		coutArray[0] += "\n\n\t\t\tStart Game";
		coutArray[1].erase();
		coutArray[1] += "\n\n\t\t\tDefault Music";
		coutArray[2].erase();
		coutArray[2] += "\n\n\t\t\tHelp";
		coutArray[3].erase();
		coutArray[3] += "\n\n\t\t\tExit";
		short coutArrayIndex = 0;
		system("CLS");

		cout << "\n\n\n\t\t\t";
		SetConsoleTextAttribute(h, enter1);
		cout << coutArray[0];
		SetConsoleTextAttribute(h, enter2);
		cout << coutArray[1];
		SetConsoleTextAttribute(h, enter3);
		cout << coutArray[2];
		SetConsoleTextAttribute(h, enter4);
		cout << coutArray[3];
		SetConsoleTextAttribute(h, 7);
		cout << endl;

		while (enter != 13)
		{
			enter = _getch();
			if (enter == 13)
			{
				break;
			}
			//Enter = 13 and \n = 10
			if (enter == 224)
			{
				arrowKey = _getch();
				if (arrowKey == 72) //up
				{
					if (coutArrayIndex == 0)
					{
						coutArrayIndex = 3;
					}
					else
					{
						coutArrayIndex--;
					}
				}
				else if (arrowKey == 80) //down
				{
					if (coutArrayIndex == 3)
					{
						coutArrayIndex = 0;
					}
					else
					{
						coutArrayIndex++;
					}
				}
				if (coutArrayIndex == 0)
				{
					enter1 = 192;
					enter2 = 7;
					enter3 = 7;
					enter4 = 7;
				}
				else if (coutArrayIndex == 1)
				{
					enter1 = 7;
					enter2 = 192;
					enter3 = 7;
					enter4 = 7;
				}
				else if (coutArrayIndex == 2)
				{
					enter1 = 7;
					enter2 = 7;
					enter3 = 192;
					enter4 = 7;
				}
				else if (coutArrayIndex == 3)
				{
					enter1 = 7;
					enter2 = 7;
					enter3 = 7;
					enter4 = 192;
				}
				system("CLS");
				cout << "\n\n\n\t\t\t";
				SetConsoleTextAttribute(h, enter1);
				cout << coutArray[0];
				SetConsoleTextAttribute(h, enter2);
				cout << coutArray[1];
				SetConsoleTextAttribute(h, enter3);
				cout << coutArray[2];
				SetConsoleTextAttribute(h, enter4);
				cout << coutArray[3];
				SetConsoleTextAttribute(h, 7);
				cout << endl;
			}
			else
			{
				continue;
			}
		}
		if (coutArrayIndex == 0) //Start Game
		{
			system("CLS");
			run();
			return;
		}
		else if (coutArrayIndex == 1) //Default Music
		{
			system("CLS");
			HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
			short enter = '1';
			short arrowKey = '1';
			short enter1 = 192;
			short enter2 = 7;
			short enter3 = 7;
			short enter4 = 7;
			short enter5 = 7;
			short enter6 = 7;
			string coutArray[6];
			coutArray[0].erase();
			coutArray[0] += "\n\n\t\t\tMusic 1";
			coutArray[1].erase();
			coutArray[1] += "\n\n\t\t\tMusic 2";
			coutArray[2].erase();
			coutArray[2] += "\n\n\t\t\tMusic 3";
			coutArray[3].erase();
			coutArray[3] += "\n\n\t\t\tMusic 4";
			coutArray[4].erase();
			coutArray[4] += "\n\n\t\t\tMusic 5";
			coutArray[5].erase();
			coutArray[5] += "\n\n\t\t\tMusic 6";
			short coutArrayIndex = 0;
			system("CLS");
			cout << "\n\n\n\t\t\t";
			SetConsoleTextAttribute(h, enter1);
			cout << coutArray[0];
			SetConsoleTextAttribute(h, enter2);
			cout << coutArray[1];
			SetConsoleTextAttribute(h, enter3);
			cout << coutArray[2];
			SetConsoleTextAttribute(h, enter4);
			cout << coutArray[3];
			SetConsoleTextAttribute(h, enter5);
			cout << coutArray[4];
			SetConsoleTextAttribute(h, enter6);
			cout << coutArray[5];
			SetConsoleTextAttribute(h, 7);
			cout << endl;
			while (enter != 13)
			{
				enter = _getch();
				if (enter == 13)
				{
					break;
				}
				else if (enter == 27)
				{
					playInterface();
					return;
				}

				//Enter = 13 and \n = 10
				if (enter == 224)
				{
					arrowKey = _getch();
					if (arrowKey == 72) //up
					{
						if (coutArrayIndex == 0)
						{
							coutArrayIndex = 5;
						}
						else
						{
							coutArrayIndex--;
						}
					}
					else if (arrowKey == 80) //down
					{
						if (coutArrayIndex == 5)
						{
							coutArrayIndex = 0;
						}
						else
						{
							coutArrayIndex++;
						}
					}
					if (coutArrayIndex == 0)
					{
						enter1 = 192;
						enter2 = 7;
						enter3 = 7;
						enter4 = 7;
						enter5 = 7;
						enter6 = 7;
					}
					else if (coutArrayIndex == 1)
					{
						enter1 = 7;
						enter2 = 192;
						enter3 = 7;
						enter4 = 7;
						enter5 = 7;
						enter6 = 7;
					}
					else if (coutArrayIndex == 2)
					{
						enter1 = 7;
						enter2 = 7;
						enter3 = 192;
						enter4 = 7;
						enter5 = 7;
						enter6 = 7;
					}
					else if (coutArrayIndex == 3)
					{
						enter1 = 7;
						enter2 = 7;
						enter3 = 7;
						enter4 = 192;
						enter5 = 7;
						enter6 = 7;
					}
					else if (coutArrayIndex == 4)
					{
						enter1 = 7;
						enter2 = 7;
						enter3 = 7;
						enter4 = 7;
						enter5 = 192;
						enter6 = 7;
					}
					else if (coutArrayIndex == 5)
					{
						enter1 = 7;
						enter2 = 7;
						enter3 = 7;
						enter4 = 7;
						enter5 = 7;
						enter6 = 192;
					}
					system("CLS");
					cout << "\n\n\n\t\t\t";
					SetConsoleTextAttribute(h, enter1);
					cout << coutArray[0];
					SetConsoleTextAttribute(h, enter2);
					cout << coutArray[1];
					SetConsoleTextAttribute(h, enter3);
					cout << coutArray[2];
					SetConsoleTextAttribute(h, enter4);
					cout << coutArray[3];
					SetConsoleTextAttribute(h, enter5);
					cout << coutArray[4];
					SetConsoleTextAttribute(h, enter6);
					cout << coutArray[5];
					SetConsoleTextAttribute(h, 7);
					cout << endl;

				}
				else
				{
					continue;
				}
			}
			if (coutArrayIndex == 0) //Music 1
			{
				PlaySound(TEXT("8 bit oop.wav"), NULL, SND_LOOP | SND_ASYNC);
				CONTROL_VARIABLE = 0;
				playInterface();
				return;
			}
			else if (coutArrayIndex == 1) //Music 2
			{
				PlaySound(TEXT("Arabic oop.wav"), NULL, SND_LOOP | SND_ASYNC);
				CONTROL_VARIABLE = 1;
				playInterface();
				return;
			}
			else if (coutArrayIndex == 2) //Music 3
			{
				PlaySound(TEXT("Blade runner resonance oop.wav"), NULL, SND_LOOP | SND_ASYNC);
				CONTROL_VARIABLE = 2;
				playInterface();
				return;
			}
			else if (coutArrayIndex == 3) //Music 4
			{
				PlaySound(TEXT("L Equipe Du Son oop.wav"), NULL, SND_LOOP | SND_ASYNC);
				CONTROL_VARIABLE = 3;
				playInterface();
				return;
			}
			else if (coutArrayIndex == 4) //Music 5
			{
				PlaySound(TEXT("Whistle oop.wav"), NULL, SND_LOOP | SND_ASYNC);
				CONTROL_VARIABLE = 4;
				playInterface();
				return;
			}
			else if (coutArrayIndex == 5) //Music 6
			{
				PlaySound(TEXT("Ching chang hanji oop.wav"), NULL, SND_LOOP | SND_ASYNC);
				CONTROL_VARIABLE = 5;
				playInterface();
				return;
			}
			//play();
			return;
		}
		else if (coutArrayIndex == 2) //Help menu
		{
		system("CLS");
		SetConsoleTextAttribute(h, 96);
		cout << "\n\n\n\t\t\tHow To Play" << endl << endl << endl;
		SetConsoleTextAttribute(h, 7);
		cout << "\n\n\n\tGuess the Word in 6 tries\n"
			<< "\tEach guess must be a 5-letter word. The color of the tiles will change to show how close your guess was to the word\n"
			<< "\tWhen you Enter a word,If the respected Tile turns Green, It means the respected Tile Letter is in the Word and in the correct Tile\n"
			<< "\t.IF the respected Tile turns Yellow, It means the respected Tile Letter is in the Wordand in the wrong spot/Tile\n";
		_getch();
		playInterface();
		return;
		}
		else if (coutArrayIndex == 3) //Exit
		{
		cout << "\n\n\n\n\n\n\n";
		exit(0);
		}
		playInterface();
		return;
	}
};
int main()
{
	Wordle a;
	a.playInterface();
	_getch();
	return 0;
}
