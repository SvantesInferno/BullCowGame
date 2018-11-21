#pragma once
#include "FBullCowGame.h"
#include <map>
// to make syntax Unreal friendly
#define TMap std::map
using int32 = int;

FBullCowGame::FBullCowGame() { Reset(); } // default constructor

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }

int32 FBullCowGame::GetMaxTries() const 
{ 
	TMap<int32, int32> WordLengthToMaxTries{ {3, 4}, {4, 5}, {5, 10}, {6, 16}, {7, 21} };
	return WordLengthToMaxTries[MyHiddenWord.length()];
}

void FBullCowGame::Reset()
{
	const FString HIDDEN_WORD = "planet"; // this MUST be an isogram
	MyHiddenWord = HIDDEN_WORD;
	MyCurrentTry = 1;
	bGameIsWon = false;
	
	return;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (!IsIsogram(Guess))
	{
		return EGuessStatus::Not_Isogram;
	}
	else if (!IsLowercase(Guess))
	{
		return EGuessStatus::Not_Lowercase;
	}
	else if (Guess.length() != GetHiddenWordLength())
	{

		return EGuessStatus::Wrong_Lenght;
	}
	else
	{
		return EGuessStatus::OK;
	}
	return EGuessStatus::OK;
}

// recieves a VALID guess, increments turn, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount; // setup a return value
	int32 WordLength = MyHiddenWord.length(); // assuming same lenght as guess

	// loop through all letters in the hidden word
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++)
	{
		// compare letters against the guess
		for (int32 GChar = 0; GChar < WordLength; GChar++)
		{
			// if they match then
			if (Guess[GChar] == MyHiddenWord[MHWChar])
			{
				//if they're in the same place
				if (MHWChar == GChar)
				{
					BullCowCount.Bulls++; //increment bulls 
				}
				else
				{
					BullCowCount.Cows++; // increment cows if not
				}
			}
		}
	}
	if (BullCowCount.Bulls == WordLength) { 
		bGameIsWon = true; 
	}
	else 
	{ 
		bGameIsWon = false; 
	}
	return BullCowCount;
}

// sets up a map and checks letters for duplicates
bool FBullCowGame::IsIsogram(FString Word) const
{
	if (Word.length() <= 1) { return true; }	// treat 0 and 1 letter words as isograms
	
	TMap<char, bool> LetterSeen;	//setup our map
	for (auto Letter : Word)		// for all letters of the word
	{
		Letter = tolower(Letter);	// handle mixed case
		if (LetterSeen[Letter])
		{
			return false;			// we do NOT have an isogram 
		}
		else
		{
			LetterSeen[Letter] = true; // add the letter to the map as seen
		}
	}
	return true; // for example in cases where /0 is entered
}

// checks letters for lowercase
bool FBullCowGame::IsLowercase(FString Word) const
{
	for (auto Letter : Word)		// for all letters of the word
	{
		if (!islower(Letter))	// if not a lowercase letter
		{
			return false;			// it is not a valid guess
		}
	}
	return true;
}
