// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"
#include "CoreMinimal.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    Isograms = GetValidWords(Words);
    SetupGame();
    

    
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    ProcessGuess(PlayerInput);
}

void UBullCowCartridge::SetupGame() 
{
    
    //Welcoming the Player
    PrintLine(TEXT("Welcome to Bull Cows!"));
    HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num()-1)];
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives"), Lives);
    PrintLine(TEXT("Type in your guess. \nPress Enter to continue..."));//Prompt PLayer for Guess
    PrintLine(TEXT("The Hidden Word is : %s."), *HiddenWord);//DebugLine
    
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again..."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{  
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        if (Guess == HiddenWord)
        {
            PrintLine(TEXT("You have won !"));
            EndGame();
            return;
        }
       
        else
        {

            PrintLine(TEXT("Lost a life, bitch!"));
             --Lives;
            if (Lives > 0)
            {
                if (Guess.Len() != HiddenWord.Len())
                {
                    PrintLine(TEXT("You have %i lives remaining:"), Lives);
                    return;
                }
                //Check IsIsogram
                if (!IsIsogram(Guess))

                {
                    PrintLine(TEXT("No repeating letters, guess again"));
                }
            }
            else
            {
                PrintLine(TEXT("You have no lives left !"));
                PrintLine(TEXT("The Hidden Word was : %s."), *HiddenWord);
                EndGame();
            }

        }
    }


    //Show the player Bulls and Cows

    FBullCowCount Score= GetBullCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    
}




bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index=0 ;Index<Word.Len(); Index++)
    {
        for (int32 Comparison=Index+1 ;Comparison<Word.Len();Comparison++)
        {
            if (Word[Index]==Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;   
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>&WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word:WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {   
            ValidWords.Emplace(Word);    
        } 
    }
    return ValidWords;
}

TArray<FString> UBullCowCartridge::GetSuggestedWords(const FString& PlayerInput)
{
    TArray<FString>SuggestedWords;
    for (auto& Word:HiddenWord)
    {
        if (Word.StartsWith(PlayerInput))
        {
            SuggestedWords.Add(Word);
        }
    }
    return SuggestedWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}
   

