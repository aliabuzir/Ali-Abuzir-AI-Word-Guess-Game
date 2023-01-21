//  main.c
//  Have the program do the guessing to discover the secret wordle word.
//
//  Written by: Ali Abuzir, 11/06/2022
//  System: macOs using Replit
//  Course: CS 211 Fall 2022
//
//  Links to wordle dictionary words at:
//    https://www.reddit.com/r/wordle/comments/s4tcw8/a_note_on_wordles_word_list/
/*  Running the program looks like:
        Using file wordsLarge.txt with 12947 words. 
        -----------------------------------------------------------
        
        enter a secret word or just r to choose one at random: dream
        Trying to find secret word: 
               d r e a m 
        
            1. s o r e s 
                   * *   
            2. s e r e r 
                 * *     
            3. r a r e r 
               * *   *   
            4. r a r e e 
               * *   *   
            5. s a r e e 
                 * * *   
            6. p a r e r 
                 * * *   
            7. c a r e r 
                 * * *   
            8. b a r e r 
                 * * *   
            9. f a r e r 
                 * * *   
           10. D a r e r 
                 * * *   
           11. D e a r e 
                 * * *   
           12. D e b A r 
                 *     * 
           13. D R E A r 
                       * 
           14. D R E A d 
                         
           15. D R E A M 
                         
        Got it!

     ... and then it runs two more times ...
 */
#include <stdio.h>    // for printf(), scanf()
#include <stdlib.h>   // for exit( -1)
#include <string.h>   // for strcpy
#include <assert.h>   // for assert() sanity checks
#include <ctype.h>    // for toupper()
#include <time.h>     // for time()

// Declare globals
#define WORD_LENGTH 5     // All words have 5 letters, + 1 NULL at the end when stored
#define WORDS_FILE_NAME "wordsLarge.txt"
//#define WORDS_FILE_NAME  "wordsTiny.txt"
#define MAX_NUMBER_OF_WORDS 12947   // Number of words in the full set of words file
#define true 1   // Make boolean logic easier to understand
#define false 0  // Make boolean logic easier to understand

typedef struct wordCount wordCountStruct;
struct wordCount{
    char word[ WORD_LENGTH + 1];   // The word length plus NULL
    int score;                     // Score for the word
    _Bool eliminated;
};


//-----------------------------------------------------------------------------------------
// Read in words from file into array.  We've previously read the data file once to
// find out how many words are in the file.
void readWordsFromFile(
            char fileName[],        // Filename we'll read from
            wordCountStruct *words, // Array of words where we'll store words we read from file
            int *wordCount)          // How many words.  Gets updated here and returned
{
    FILE *inFilePtr  = fopen(fileName, "r");  // Connect logical name to filename
    assert(inFilePtr != NULL);               // Ensure file open worked correctly

    // Read each word from file and store into array, initializing the score for that word to 0.
    char inputString[6];
    *wordCount = 0;
    while( fscanf( inFilePtr, "%s", inputString) != EOF) {
        strcpy( words[ *wordCount].word, inputString);
        words[ *wordCount].score = 0;
        words[*wordCount].eliminated = false;
        (*wordCount)++;
    }

    // Close the file
    fclose( inFilePtr);
} // end readWordsFromFile(..)


void checkWordFound(char matchedIndex[], _Bool *wordFound) {
  *wordFound = true;
  for (int checkFound = 0; checkFound < WORD_LENGTH; ++checkFound) {
    if (!isalpha(matchedIndex[checkFound])) {
      *wordFound = false;
    }
  }
}


void mismatchedIndex(char matchedIndex[], wordCountStruct allWords[], int allIndex) {
  for (int indexMatch = 0; indexMatch < WORD_LENGTH; ++indexMatch) {
    if (allWords[allIndex].word[indexMatch] != matchedIndex[indexMatch] && isalpha(matchedIndex[indexMatch])) {
      allWords[allIndex].eliminated = true;
      break;
    }
  }
}


void incrementAllIndex(int *allIndex, int wordCount, _Bool *wordExists, char secretWord[]) {
  ++(*allIndex);
  if (*allIndex == wordCount / 2) {
    *wordExists = false;
    printf("%s does not exist.\n", secretWord);
  }
  if (*allIndex >= wordCount) {
    *allIndex = 0;
  }
}


void letterNotContained(char letterNotIn[], wordCountStruct allWords[], int allIndex) {
  for (int checkEachLetter = 0; checkEachLetter < WORD_LENGTH; ++checkEachLetter) {
    int checkNotLetters = 0;
    while (letterNotIn[checkNotLetters] != '\0') {
      if (allWords[allIndex].word[checkEachLetter] == letterNotIn[checkNotLetters]) {
        allWords[allIndex].eliminated = true;
        break;
      }
      ++checkNotLetters;
    }
    if (allWords[allIndex].eliminated) {
      break;
    }
  }
}


void incorrectSpot(wordCountStruct allWords[], char letterNotAt0[], char letterNotAt1[], char letterNotAt2[], char letterNotAt3[], char letterNotAt4[], int allIndex) {
  for (int wrongLocation = 0; wrongLocation < WORD_LENGTH; ++wrongLocation) {
    switch (wrongLocation) {
      case 0: {
        int notIn0 = 0;
        while (letterNotAt0[notIn0] != '\0') {
          if (allWords[allIndex].word[wrongLocation] == letterNotAt0[notIn0]) {
            allWords[allIndex].eliminated = true;
            break;
          }
          ++notIn0;
        }
        break;
      }
      case 1: {
        int notIn1 = 0;
        while (letterNotAt1[notIn1] != '\0') {
          if (allWords[allIndex].word[wrongLocation] == letterNotAt1[notIn1]) {
            allWords[allIndex].eliminated = true;
            break;
          }
          ++notIn1;
        }
        break;
      }
      case 2: {
        int notIn2 = 0;
        while (letterNotAt2[notIn2] != '\0') {
          if (allWords[allIndex].word[wrongLocation] == letterNotAt2[notIn2]) {
            allWords[allIndex].eliminated = true;
            break;
          }
          ++notIn2;
        }
        break;
      }
      case 3: {
        int notIn3 = 0;
        while (letterNotAt3[notIn3] != '\0') {
          if (allWords[allIndex].word[wrongLocation] == letterNotAt3[notIn3]) {
            allWords[allIndex].eliminated = true;
            break;
          }
          ++notIn3;
        }
        break;
      }
      case 4: {
        int notIn4 = 0;
        while (letterNotAt4[notIn4] != '\0') {
          if (allWords[allIndex].word[wrongLocation] == letterNotAt4[notIn4]) {
            allWords[allIndex].eliminated = true;
            break;
          }
          ++notIn4;
        }
        break;
      }
    }
  }
}


void examineWords(wordCountStruct allWords[], char letterNotIn[], char letterNotAt0[], char letterNotAt1[], char letterNotAt2[], char letterNotAt3[], char letterNotAt4[], char matchedIndex[], char computerGuess[], char secretWord[], char foundElsewhere[]) {
    for (int guessIterate = 0; guessIterate < WORD_LENGTH; ++guessIterate) {
      _Bool letterInWord = false;
      for (int secretIterate = 0; secretIterate < WORD_LENGTH; ++secretIterate) {
        if (computerGuess[guessIterate] == secretWord[secretIterate]) {
          letterInWord = true;
          if (guessIterate == secretIterate) {
            matchedIndex[secretIterate] = computerGuess[secretIterate];
            computerGuess[secretIterate] = toupper(computerGuess[secretIterate]);
            break;
          }
          else if (computerGuess[guessIterate] == secretWord[guessIterate]) {
            matchedIndex[guessIterate] = secretWord[guessIterate];
            computerGuess[guessIterate] = toupper(computerGuess[guessIterate]);
            break;
          }
          else {
            switch (guessIterate) {
              case 0: 
                strncat(letterNotAt0, &(computerGuess[guessIterate]), 1);
                break;
              case 1:
                strncat(letterNotAt1, &(computerGuess[guessIterate]), 1);
                break;
              case 2:
                strncat(letterNotAt2, &(computerGuess[guessIterate]), 1);
                break;
              case 3: 
                strncat(letterNotAt3, &(computerGuess[guessIterate]), 1);
                break; 
              case 4:
                strncat(letterNotAt4, &(computerGuess[guessIterate]), 1);
                break;
            }
            foundElsewhere[guessIterate] = '*';
            break;
          }
        }
      }
          
      if (!letterInWord) {
        int checkExists = 0;
        _Bool exists = false;
        while (letterNotIn[checkExists] != '\0') {
          if (letterNotIn[checkExists] == computerGuess[guessIterate]) {
            exists = true;
          }
          ++checkExists;
        }
        
        if (!exists) {
          strncat(letterNotIn, &(computerGuess[guessIterate]), 1);
        }
      }
    }
}
// -----------------------------------------------------------------------------------------
// Find a secret word
void findSecretWord(
        wordCountStruct allWords[],    // Array of all the words
        int wordCount,                  // How many words there are in allWords
        char secretWord[])              // The word to be guessed
{
  char computerGuess[ 6];  // Allocate space for the computer guess
    
  printf("Trying to find secret word: \n");
  // Display secret word with a space between letters, to match the guess words below.
  printf("       ");

  for( int i=0; i<WORD_LENGTH; i++) {
    printf("%c ", secretWord[i]);
  }
  printf("\n");
  printf("\n");
    
  // Loop until the word is found
  int guessNumber = 1;
  _Bool wordFound = false;
  _Bool wordExists = true;
  char letterNotIn[27] = "";

  char letterNotAt0[27] = "";
  char letterNotAt1[27] = "";
  char letterNotAt2[27] = "";
  char letterNotAt3[27] = "";
  char letterNotAt4[27] = "";

  char matchedIndex[WORD_LENGTH] = "/////";
    
  int allIndex = wordCount / 2;
  
  while(!wordFound && wordExists) {
    checkWordFound(matchedIndex, &wordFound);
    if (wordFound) {
      continue;
    }
   
    allWords[allIndex].eliminated = false; // restart condition when looking for a new secret word
  
    mismatchedIndex(matchedIndex, allWords, allIndex);
        
    if (allWords[allIndex].eliminated) {
      incrementAllIndex(&allIndex, wordCount, &wordExists, secretWord);
      continue;
    }

    letterNotContained(letterNotIn, allWords, allIndex);
      
    if (allWords[allIndex].eliminated) {
      incrementAllIndex(&allIndex, wordCount, &wordExists, secretWord);
      continue;
    }

    incorrectSpot(allWords, letterNotAt0, letterNotAt1, letterNotAt2, letterNotAt3, letterNotAt4, allIndex);

    if (allWords[allIndex].eliminated) {
      incrementAllIndex(&allIndex, wordCount, &wordExists, secretWord);
      continue;
    }


    char foundElsewhere[WORD_LENGTH + 1] = "     ";
    strcpy(computerGuess, allWords[allIndex].word);

    examineWords(allWords, letterNotIn, letterNotAt0, letterNotAt1, letterNotAt2, letterNotAt3, letterNotAt4, matchedIndex, computerGuess, secretWord, foundElsewhere);
    
    printf("%d. ", guessNumber);
    for (int printGuess = 0; printGuess < WORD_LENGTH; ++printGuess) {
      printf("%c ", computerGuess[printGuess]);
    }
    printf("\n   ");
    for (int printAstriskSpots = 0; printAstriskSpots < WORD_LENGTH; ++printAstriskSpots) {
      printf("%c ", foundElsewhere[printAstriskSpots]);
    }
    printf("\n");
      
    ++guessNumber;
     
    incrementAllIndex(&allIndex, wordCount, &wordExists, secretWord);
  }
} //end findSecretWord


// -----------------------------------------------------------------------------------------
int main() {
    char wordsFileName[81];                   // Stores the answers file name
    strcpy(wordsFileName, WORDS_FILE_NAME);   // Set the filename, defined at top of program.
    srand( (unsigned) time( NULL));           // Seed the random number generator to be current time
    // Declare space for all the words, of a maximum known size.
    wordCountStruct allWords[ MAX_NUMBER_OF_WORDS];
    // Start out the wordCount to be the full number of words.  This will decrease as
    //    play progresses each time through the game.
    int wordCount = 0;
    // The secret word that the computer will try to find, plus the return character from fgets.
    char secretWord[ WORD_LENGTH + 1];
    char userInput[ 81];                // Used for menu input of secret word

    // Read in words from file, update wordCount and display information
    readWordsFromFile( wordsFileName, allWords, &wordCount);
    //computeScore(allWords, wordCount, secretWord);
    printf("Using file %s with %d words. \n", wordsFileName, wordCount);

    // Run the word-guessing game three times
    for( int i=0; i<3; i++) {
        // Reset secret Word
        strcpy( secretWord, "");
        // Display prompt
        printf("-----------------------------------------------------------\n");
        printf("\n");
        printf("Enter a secret word or just r to choose one at random: ");
        scanf(" %s", userInput);
        // Eliminate the return character at end or userInput if it is there
        int length = (int) strlen( userInput);
        if( userInput[ length] == '\n') {
            userInput[ length] = '\0';
        }
        strcpy( secretWord, userInput);   // Store the secret word from user input
        //computeScore(allWords, wordCount, secretWord);
                
        // If input was 'r' then choose a word at random.
        if( strlen( secretWord) <= 1) {
            // Randomly select a secret word to be guessed.
            int randomIndex = rand() % wordCount;
            strcpy( secretWord, allWords[ randomIndex].word);
        }
        // Run the game once with the current secret word
        findSecretWord( allWords, wordCount, secretWord);
    }

  printf("Done\n");
  printf("\n");
    
  return 0;
} // end main()
