/* This is the console executable, that makes use of the BullCow Class
This acts as the view in a MVC pattern and is responsible for all user interaction. 
For game logic see the FBullCowGame class.
*/

#pragma once
#include <iostream>
#include <string>
#include "FBullCowGame.h"
#include <windows.h>

// to make syntax Unreal friendly
using FText = std::string;
using int32 = int;

// nice colours
constexpr auto GRAY = 7;
constexpr auto DARK = 8;
constexpr auto BLUE = 9;
constexpr auto GREEN = 10;
constexpr auto AQUA = 11;
constexpr auto RED = 12;
constexpr auto PINK = 13;
constexpr auto YELLOW = 14;
constexpr auto WHITE = 15;

//function prototypes as outside a class
void PrintIntro();
void PlayGame();
FText GetValidGuess();
bool AskToPlayAgain();
void PrintGameSummary();
void PrintHint();

FBullCowGame BCGame; // instantiate a new game, which we re-use across plays

// the entry point for our application
int main()
{
	bool bPlayAgain = false;
	do
	{
		PrintIntro();
		PlayGame();
		bPlayAgain = AskToPlayAgain();
	} 
	while (bPlayAgain);

	return 0; // exit the application
}

void PrintIntro()
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, GREEN);

	std::cout << "\n\nWelcome to ";
	SetConsoleTextAttribute(hConsole, WHITE);
	std::cout << "Bulls & Cows";
	SetConsoleTextAttribute(hConsole, GREEN);
	std::cout << ", a fun word game!" << std::endl;
	SetConsoleTextAttribute(hConsole, WHITE);
	std::cout << "\n\\|/          (__)                           " << std::endl;
	std::cout << "     `\\------(oo)                             " << std::endl;
	std::cout << "       ||     (..)                             " << std::endl;
	std::cout << "    &  ||,---|| O    \\|/                      " << std::endl;
	std::cout << "   \\|/                            __          " << std::endl;
	std::cout << "                                 (oo)       ,  " << std::endl;
	std::cout << "       \\|/                      (__)-------/  " << std::endl;
	std::cout << "                                   \\\\     || " << std::endl;
	std::cout << "                                    ||---W|| & " << std::endl;
	std::cout << "                 \\|/                          " << std::endl;
	std::cout << "                                  \\|/         " << std::endl;
	std::cout << "                                               " << std::endl;
	SetConsoleTextAttribute(hConsole, GREEN);
	std::cout << "Can you guess the ";
	SetConsoleTextAttribute(hConsole, WHITE);
	std::cout << BCGame.GetHiddenWordLength() << " letter isogram";
	SetConsoleTextAttribute(hConsole, GREEN);
	std::cout << " I'm thinking of?\n";
	std::cout << std::endl;
	return;
}
//plays a single game to completion
void PlayGame()
{

	BCGame.Reset();
	int32 MaxTries = BCGame.GetMaxTries();

	// loop asking for guesses while the game 
	// is NOT won and there are still tries remaining
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries) { 
		FText Guess = GetValidGuess();

		// submit valid guess to the game, and recieve counts
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		std::cout << "Bulls = " << BullCowCount.Bulls << "\n";
		std::cout << "Cows = " << BullCowCount.Cows << "\n\n";

		
	}
	PrintHint();

	PrintGameSummary();
}

// loop continually until the user gives a valid guess
FText GetValidGuess() 
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	SetConsoleTextAttribute(hConsole, WHITE);
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do {
		// get a guess from the player and check validity
		int32 CurrentTry = BCGame.GetCurrentTry();
		SetConsoleTextAttribute(hConsole, DARK);
		std::cout << "Try " << CurrentTry << " of " << BCGame.GetMaxTries() << ". ";
		SetConsoleTextAttribute(hConsole, GREEN);
		std::cout << "Enter your guess: ";
		SetConsoleTextAttribute(hConsole, WHITE);
		std::getline(std::cin, Guess);

		//check status and give feedback
		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Wrong_Lenght:
			SetConsoleTextAttribute(hConsole, RED);
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::Not_Isogram:
			SetConsoleTextAttribute(hConsole, RED);
			std::cout << "Please enter a word without repeating letters.\n\n";
			break;
		case EGuessStatus::Not_Lowercase:
			SetConsoleTextAttribute(hConsole, RED);
			std::cout << "Please enter all lowercase letters.\n\n";
			break;
		default:
			// assume the guess is valid
			break;
		}
	} while (Status != EGuessStatus::OK); // keep looping until we get no errors
	return Guess;
}

bool AskToPlayAgain()
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, GREEN);
	std::cout << "\nDo you want to ";
	SetConsoleTextAttribute(hConsole, WHITE);
	std::cout << "play again";
	SetConsoleTextAttribute(hConsole, GREEN);
	std::cout << " with the same hidden word?";
	SetConsoleTextAttribute(hConsole, DARK);
	std::cout << "(y / n)";
	FText Response = "";
	std::getline(std::cin, Response);
	return (Response[0] == 'y') || (Response[0] == 'Y');
}

void PrintHint()
{
	
	{
		std::cout << "Bulls represent letters that are in the right place. In other words - Bullseye!\nCows represent letters that belong to the word, but are in the wrong place.\n\n";
	}
}

void PrintGameSummary()
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (BCGame.IsGameWon())
	{
		SetConsoleTextAttribute(hConsole, GREEN);
		std::cout << "WELL DONE - ";
		SetConsoleTextAttribute(hConsole, WHITE);
		std::cout << "YOU WIN!\n";
	}
	else
	{
		SetConsoleTextAttribute(hConsole, RED);
		std::cout << "Better luck next time!\n";
	}
}
