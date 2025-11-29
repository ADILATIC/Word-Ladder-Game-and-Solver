// Project 4: Open-Ended Word Ladder Builder+Solver
// Description: A word ladder game where the user can choose two words or
// have it be randomly generated and the user has to connect the two words and
// it will create a word ladder
// Course: CS 211, Fall 2025, UIC
// Author: Adi Latic
// ------------------------------------------- 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct WordNode_struct {
    char* myWord;
    struct WordNode_struct* next; 
} WordNode;

typedef struct LadderNode_struct {
    WordNode* topWord;
    struct LadderNode_struct* next; 
} LadderNode;

int countWordsOfLength(char* filename, int wordSize) { 
    FILE* aFile = fopen(filename, "r");             //Open the file
    char words [256];                               //Initalize variables
    int count = 0;

    if(aFile == NULL){                          //if the file is empty return -1
        return -1;
    }

    while(fscanf(aFile, "%s", words) == 1){             //Scan the file and put into words variable
        if(strlen(words) == wordSize){              //Checks if the words is the same size as wordSize and adds to count
            count++;
        }
    }

    fclose(aFile);          //Close file
    return count; //return count
}

bool buildWordArray(char* filename, char** words, int numWords, int wordSize) { 
    FILE* aFile = fopen(filename, "r");             //Open the file
    char line [256];
    int index = 0;

    if(aFile == NULL){                          //if the file is empty return -1
        return false;
    }

    while(fscanf(aFile, "%s", line) == 1){             //Scan the file and put into line variable
        if(strlen(line) == wordSize){              //Checks if the line is the same size as wordSize
            strcpy(words[index], line);             //Copy whats in line to words and starts at index 0 and iterates up
            index++;
        }
    }
    if(index == numWords){                  //If numWords is equal to index
        fclose(aFile);
        return true;
    }
    fclose(aFile);
    return false; //modify this line
}

int findWord(char** words, char* aWord, int loInd, int hiInd) {

    while(loInd <= hiInd){              //While low Index is less then the higher to go through the array
        int mid = (loInd + hiInd)/2;        //Declare mid
        if(strcmp(aWord, words[mid]) == 0){         //If strcmp returns a 0 it means we found the word and we return the index
            return mid;
        } else if (strcmp(aWord, words[mid]) < 0){      //If it returns a - number we search the left side
            hiInd = mid -1;
        } else {                // Else search the right side
            loInd = mid +1;
        }
    }
    return -1;// if it isnt found
}

void freeWords(char** words, int numWords) {

    for(int i = 0; i < numWords; i++){      //Go through the numWords
        free(words[i]);                     //Free the individual index
    }
    free(words);                //Free the array
}

int strCmpCnt(char* str1, char* str2) {
    int biggerStr = 0;      //Declare variables
    int count = 0;

    if (strlen(str1) > strlen(str2)){       //Determines the longer string
        biggerStr = strlen(str1);
    } else {
        biggerStr = strlen(str2);
    }

    for(int i = 0; i < biggerStr; i++){     //Goes the length of the longer str
        if(str1[i] != '\0' && str2[i] != '\0'){     //If both str are not null then check if the chars are different and add to count
            if(str1[i] != str2[i]){
                count++;
            }
        } else {            //Otherwise one of the str hit null so add count for the left over chars
            count++;
        }
    }
    return count; //return count
}

int strCmpInd(char* str1, char* str2) {
    int biggerStr = 0;      //Declare variables

    if (strlen(str1) > strlen(str2)){       //Determines the longer string
        biggerStr = strlen(str1);
    } else {
        biggerStr = strlen(str2);
    }

    for(int i = 0; i < biggerStr; i++){     //Goes the length of the bigger str
        if(str1[i] != str2[i]){             //Once the diff in chars return the index it was found at
            return i;
        }
    }
    return -1; //If there is no different return -1
}

void insertWordAtFront(WordNode** ladder, char* newWord) {
    WordNode* temp = (WordNode*)malloc(sizeof(WordNode));       //Allocate space
    temp->myWord = newWord;                         //Store the data into the new node
    temp->next = (*ladder);             //New Nodes next is pointed to the front
    (*ladder) = temp;                   //The new front is the new node
}

int getLadderHeight(WordNode* ladder) {
    int words = 0;              //Declare variables
    WordNode* current = ladder;     //Create a new Node and set it to the front/head

    if(current == NULL){
        return words;
    }

    while(current != NULL){     //Traverse the linked list and count every node
        current = current->next;
        words++;
    }
    return words; // Return the words
}

bool checkForValidWord(char** words, int numWords, int wordSize, WordNode* ladder, char* aWord) {

    // sample print statement for various scenarios:
    if(strcmp(aWord, "DONE") == 0){
        // user entered "DONE" - valid, top (#1) priority
        printf("Ending game with an incomplete word ladder...\n");
        return true;
    }
    if(strlen(aWord) != wordSize){
        // user entered a word that is too long/short - invalid, priority #2
        printf("Invalid word - wrong length. Try again...\n");
        return false;
    }
    if(findWord(words, aWord,0, numWords-1) == -1){
        // user entered a word that is not found in the dictionary - invalid, priority #3
        printf("Invalid word - not a word. Try again...\n");
        return false;
    }       
    if(strCmpCnt(aWord, ladder->myWord) > 1){
        // user entered a word that requires changing more than one-character - invalid, priority #4a
        printf("Invalid word - too many differences. Try again...\n");
        return false;
    } else if (strCmpCnt(aWord, ladder->myWord) == 0){
        // user entered a word that requires no character difference (the same word) - invalid, priority #4b
        printf("Invalid word - identical word. Try again...\n");
        return false;
    } else {
        // user entered a word a valid word - valid, priority #5 (default)
        printf("Valid word - adding it to the word ladder.\n");
        return true;
    }
}

bool isLadderComplete(WordNode* ladder, char* finalWord) {

    if(strcmp(ladder->myWord, finalWord) == 0){
        return true;
    }
    return false; //modify this line
}

WordNode* copyLadder(WordNode* ladder) {
    if(ladder == NULL){         //Checks if the linkedlist is empty
        return NULL;
    }

    WordNode* newHead = (WordNode*)malloc(sizeof(WordNode));       //Allocate space for the copy
    newHead->next = NULL;                   //Set newHead next to NULL
    newHead->myWord = ladder->myWord;   //Store the data from the current ladder into the copy

    WordNode* temp = newHead;
    ladder = ladder->next;

    while(ladder!=NULL){
        WordNode* copy = (WordNode*)malloc(sizeof(WordNode));           //Allocate space
        copy->next = NULL;
        copy->myWord = ladder->myWord;      //Store the data from the current ladder into the copy
        temp->next = copy;                  
        temp = copy;
        ladder = ladder->next;       //Iterate through the linkedlist
    }
    return newHead;         //Return the newHead
}

void freeLadder(WordNode* ladder) {
    WordNode* current;
    //WordNode* temp = ladder;

    while(ladder != NULL){          //Iterate to the end of the linked list
        current = ladder;              //Store the next now into ladder
        ladder = ladder->next;      //Store the next into a temp
        free(current);               //Free the current node
        
    }
}

void insertLadderAtBack(LadderNode** list, WordNode* newLadder) {
    LadderNode* temp = (LadderNode*)malloc(sizeof(LadderNode));     //Allocate new space
    LadderNode* current = *list;            //Set current  to the front
    temp->next = NULL;              //Set temp next to be NULL
    temp->topWord = newLadder;          //Get the data from the new ladder

    if(*list == NULL){      //If list is null set the front to be temp and exit the func
        *list = temp;
        return;
    }

    while(current->next != NULL){       //Traverse through the list
        current = current->next;
    }
    current->next = temp;       //Store the new node at the last spot
}

WordNode* popLadderFromFront(LadderNode** list) {
    if(*list == NULL){              //Checks if the list is empty
        return NULL;
    }
    LadderNode* temp = (*list);           //Store list into temp
    WordNode* saved = temp->topWord;        //Save the temp topword/data
    *list = temp->next;      //Set the front to be the next
    free(temp);                 //Free the temp
    
    return saved; //return the data that is saved
}

void freeLadderList(LadderNode* myList) {
    LadderNode* current = myList;

    while(myList != NULL){          //Iterate to the end of the linked list
        current = myList;              //Store the next now into ladder
        myList = myList->next;      //Store the next into a temp
        freeLadder(current->topWord);
        free(current);               //Free the current node
        
    }
}

WordNode* findShortestWordLadder(char** words, bool* usedWord, int numWords, int wordSize, char* startWord, char* finalWord ) {
    LadderNode* myList = NULL;              //Set both to empty
    WordNode* myLadder = NULL;

    insertWordAtFront(&myLadder, startWord);        //Inserts starter word at front
    insertLadderAtBack(&myList, myLadder);           //Append myLadder to the back of myList 

    int firstIndex = findWord(words, startWord, 0, numWords-1); //Gets the first index

    if(firstIndex == -1){   //If it is empty return null
        return NULL;
    }
    usedWord[firstIndex] = true;            //Set to true since its been used
    
    if(strcmp(startWord, finalWord) == 0){      //If they are the same return the ladder
        return myLadder;
    }

    while(myList != NULL){                          //Go through the list
        WordNode* currLadder = popLadderFromFront(&myList);     //Pop the head LadderNode off the front of myList, call it myLadder
        for(int i = 0; i < wordSize; i++){
            char firstCharacter = currLadder->myWord[i];        //Gets the first character in the current word
            char copyWord[wordSize+1];                          //Copy word is initialized including the null char
            strcpy(copyWord, currLadder->myWord);               //Copy the word to copyword
            for(int j = 0; j < 26; j++){                //Iterates through the alphabet
               char check = (char) ('a' + j);           //Check matches which char that is needed to be found
                if(firstCharacter == check){            //If the first char are the same then continue
                    continue;
                }else {
                    copyWord[i] = check;                    //Stores the check into the copyWord index
                    copyWord[wordSize] = '\0';
                    int wordIndex = findWord(words, copyWord, 0, numWords-1);           //Captures the index
                if(wordIndex != -1 && !usedWord[wordIndex] ){               //If it is found and usedWord hasnt been used
                    usedWord[wordIndex] = true;                     //usedWord is set to true
                    if(strcmp(copyWord, finalWord) == 0){           //Compare the two words
                        insertWordAtFront(&currLadder, finalWord);       //Prepend w2 to the front of myLadder
                        freeLadderList(myList);
                        return currLadder;
                    } else {
                        WordNode* anotherLadder = copyLadder(currLadder);   // Copy myLadder to anotherLadder
                        insertWordAtFront(&anotherLadder, words[wordIndex]);    //Prepend neighbor word to the front of anotherLadder
                        insertLadderAtBack(&myList, anotherLadder);
                    }
                }
            }
        }
    }
    freeLadder(currLadder);         //free the ladder
}
freeLadderList(myList);             //free the list
    return NULL; 
}

//------------------- \/\/\/ TOP OF OTHERS \/\/\/ -------------------
void TEST_countWords(){

    if(countWordsOfLength("simple3.txt", 8) == 0){
        printf("PASS, there is no words that have 8 characters");
    } else {
        printf("FAIL, WORD WAS FOUND");
    }
}

// void TEST_buildWordArray(){

//     if(buildWordArray("simple3.txt", words, numWords, wordSize) == true){
//         printf("PASS, BUILD WAS SUCESSFUL");
//     } else{
//         printf("FAIL, BUILD WAS NOT SUCESSFUL");
//     }
// }

// void TEST_findWord(char** words, char* aWord){
//     if(findWord(words,aWord,0,numWords - 1) > -1){
//         printf("PASS, BUILD WAS SUCESSFUL");
//     } else {
//         printf("FAIL, BUILD WAS NOT SUCESSFUL");
//     }
// }

void TEST_ALL(){
    TEST_countWords();
    //TEST_buildWordArray(filename, words, numWords, wordSize);
    //TEST_findWord(words, aWord, 0, numWords-1);
}


// randomly set a word from the dictionary word array
void setWordRand(char** words, int numWords, int wordSize, char* aWord) {
    printf("  Picking a random word for you...\n");
    strcpy(aWord,words[rand()%numWords]);
    printf("  Your word is: %s\n",aWord);
}

// interactive user-input to set a word;
//  ensures the word is in the dictionary word array
void setWord(char** words, int numWords, int wordSize, char* aWord) {
    bool valid = false;
    if (strcmp(aWord,"RAND") != 0) printf("  Enter a %d-letter word (enter RAND for a random word): ", wordSize);
    int count = 0;
    while (!valid) {
        if (strcmp(aWord,"RAND") != 0) scanf("%s",aWord);
        count++;
        valid = (strlen(aWord) == wordSize);
        if (valid) {
            int wordInd = findWord(words, aWord, 0, numWords-1);
            if (wordInd < 0) {
                valid = false;
                printf("    Entered word %s is not in the dictionary.\n",aWord);
                printf("  Enter a %d-letter word (enter RAND for a random word): ", wordSize);
            }
        } else if (strcmp(aWord,"RAND") != 0) {
            printf("    Entered word %s is not a valid %d-letter word.\n",aWord,wordSize);
            printf("  Enter a %d-letter word (enter RAND for a random word): ", wordSize);
        }
        if (!valid && (count >= 5 || strcmp(aWord,"RAND") == 0)) { //too many tries, picking random word
            setWordRand(words, numWords, wordSize, aWord);
            valid = true;
        }
    }
}

// helpful debugging function to print a single Ladder
void printLadder(WordNode* ladder) {
    WordNode* currNode = ladder;
    while (currNode != NULL) {
        printf("\t\t\t%s\n",currNode->myWord);
        currNode = currNode->next;
    }
}

// helpful debugging function to print the entire list of Ladders
void printList(LadderNode* list) {
    printf("\n");
    printf("Printing the full list of ladders:\n");
    LadderNode* currList = list;
    while (currList != NULL) {
        printf("  Printing a ladder:\n");
        printLadder(currList->topWord);
        currList = currList->next;
    }
    printf("\n");
}

//-----------------------------------------------------
// The primary application is mostly fully-develop as
//  provided in main(); changes in main() should be
//  limited to updates made for the game play task(s)
//  and testing-related purposes (such as command-line
//  arguments for "TESTING MODE" to call a test case 
//  master function, or something similar)
//-----------------------------------------------------
int main(int argc, char* argv[]) {

    printf("\n");
    printf("--------------------------------------------\n");
    printf("Welcome to the CS 211 Word Ladder Generator!\n");
    printf("--------------------------------------------\n\n");
    

    //-------------- \/\/\/ TOP OF PROGRAM SETTINGS \/\/\/ --------------
    //--- COMMAND-LINE ARGUMENTS AND/OR INTERACTIVE USER-INPUT \/\/\/ ---

    
    // default values for program parameters that may be set with
    //  command-line arguments
    int wordSize = -2114430;
    char dict[100] = "notAfile";
    char startWord[30] = "notAword";
    char finalWord[30] = "notValid";
    bool playMode = false;
    bool testMode = false;
    
    printf("\nProcessing command-line arguments...\n");

    //-------------------------------------------------------------------
    // command-line arguments:
    //  [-n wordLen] = sets word length for word ladder;
    //                 if wordLen is not a valid input
    //                 (cannot be less than 2 or greater than 20),
    //                 or missing from command-line arguments,
    //                 then let user set it using interactive user input
    // [-d dictFile] = sets dictionary file;
    //                 if dictFile is invalid (file not found) or
    //                 missing from command-line arguments, then let
    //                 user set it using interactive user input
    // [-s startWord] = sets the starting word;
    //                  if startWord is invalid
    //                  (not in dictionary or incorrect length) or
    //                  missing from command-line arguments, then let
    //                  user set it using interactive user input
    // [-f finalWord] = sets the final word;
    //                  if finalWord is invalid
    //                  (not in dictionary or incorrect length) or
    //                  missing from command-line arguments, then let
    //                  user set it using interactive user input
    // [-p playModeSwitch] = turns playMode ON if playModeSwitch is "ON"
    //                       or leaves playMode OFF if playModeSwitch is
    //                       anything else, including "OFF"
    //-------------------------------------------------------------------

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i],"-n") == 0 && argc > i+1) {
            wordSize = atoi(argv[i+1]);
            ++i;
        } else if (strcmp(argv[i],"-d") == 0 && argc > i+1) {
            strcpy(dict, argv[i+1]);
            ++i;
        } else if (strcmp(argv[i],"-s") == 0 && argc > i+1) {
            strcpy(startWord, argv[i+1]);
            ++i;
        } else if (strcmp(argv[i],"-f") == 0 && argc > i+1) {
            strcpy(finalWord, argv[i+1]);
            ++i;
        } else if (strcmp(argv[i],"-p") == 0 && argc > i+1) {
            playMode = (strcmp(argv[i+1],"ON") == 0);
            ++i;
        } else if (strcmp(argv[i],"-t") == 0 && argc > i+1) {
            testMode = (strcmp(argv[i+1],"ON") == 0);
            ++i;
        }
    }
    
    srand((int)time(0));
    
    // set word length using interactive user-input
    //  if wordSize == -2114430, it was NOT set with command-line args
    while (wordSize < 2 || wordSize > 20) {
        if (wordSize != -2114430) printf("Invalid word size for the ladder: %d\n", wordSize);
        printf("Enter the word size for the ladder: ");
        scanf("%d",&wordSize);
        printf("\n");
    }

    printf("This program is a word ladder building game and a solver that\n");
    printf("finds the shortest possible ");
    printf("word ladder between two %d-letter words.\n\n",wordSize);
    
    // interactive user-input to set the dictionary file;
    //  check that file exists; if not, user enters another file
    //  if file exists, count #words of desired length [wordSize];
    //  if dict == "notAfile", it was NOT set with command-line args
    int numWords = countWordsOfLength(dict,wordSize);
    while (numWords < 0) {
        if (strcmp(dict, "notAfile") != 0) {
            printf("  Dictionary %s not found...\n",dict);
        }
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        numWords = countWordsOfLength(dict,wordSize);
    }
    
    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("  Dictionary %s contains insufficient %d-letter words...\n",dict,wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    
    // allocate heap memory for the word array; only words with desired length
    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; ++i) {
        words[i] = (char*)malloc((wordSize+1)*sizeof(char));
    }
    
    // [usedWord] bool array has same size as word array [words];
    //  all elements initialized to [false];
    //  later, usedWord[i] will be set to [true] whenever
    //      words[i] is added to ANY partial word ladder;
    //      before adding words[i] to another word ladder,
    //      check for previous usage with usedWord[i]
    bool* usedWord = (bool*)malloc(numWords*sizeof(bool));
    for (int i = 0; i < numWords; ++i) {
        usedWord[i] = false;
    }
    
    // build word array (only words with desired length) from dictionary file
    printf("Building array of %d-letter words... ", wordSize);
    bool status = buildWordArray(dict,words,numWords,wordSize);
    if (!status) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of %d-letter words.\n",wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    printf("Done!\n\n");

    // set the two ends of the word ladder using interactive user-input
    //  make sure start and final words are in the word array,
    //  have the correct length (implicit by checking word array), AND
    //  that the two words are not the same
    // start/final words may have already been set using command-line arguments
    // the start/final word can also be set to "RAND" resulting in a random
    //  assignment from any element of the words array
    if (strcmp(startWord,"RAND")==0) {
        printf("Setting the start word randomly...\n");
        setWordRand(words, numWords, wordSize, startWord);
    } else if (findWord(words, startWord,0, numWords-1) < 0 || strlen(startWord) != wordSize) {
        if (strcmp(startWord,"notAword")==0) {
            printf("Setting the start %d-letter word... \n", wordSize);
        } else {
            printf("Invalid start word %s. Resetting the start %d-letter word... \n", startWord, wordSize);
        }
        setWord(words, numWords, wordSize, startWord);
    }
    printf("\n");
    
    if (strcmp(finalWord,"RAND")==0) {
        printf("Setting the final word randomly...\n");
        setWordRand(words, numWords, wordSize, finalWord);
    } else if (findWord(words, finalWord,0, numWords-1) < 0 || strlen(finalWord) != wordSize) {
        if (strcmp(finalWord,"notValid")==0) {
            printf("Setting the final %d-letter word... \n", wordSize);
        } else {
            printf("Invalid final word %s. Resetting the final %d-letter word... \n", finalWord, wordSize);
        }
        setWord(words, numWords, wordSize, finalWord);
    }
    while (strcmp(finalWord,startWord) == 0) {
        printf("  The final word cannot be the same as the start word (%s).\n",startWord);
        printf("Setting the final %d-letter word... \n", wordSize);
        setWord(words, numWords, wordSize, finalWord);
    }
    printf("\n");
    
    if (!playMode) {
        printf("\n");
        printf("---------------------------------------------\n");
        printf("No Word Ladder Builder Game; Play Mode is OFF\n");
        printf("---------------------------------------------\n");
        printf("\n");
    } else {
        printf("\n");
        printf("-----------------------------------------------\n");
        printf("Welcome to the CS 211 Word Ladder Builder Game!\n");
        printf("-----------------------------------------------\n");
        printf("\n");

        printf("Your goal is to make a word ladder between two ");
        printf("%d-letter words: \n  %s -> %s\n\n",wordSize, startWord,finalWord);
        
        WordNode* userLadder = NULL;
        int ladderHeight = 0; // initially, the ladder is empty
        int startInd = findWord(words, startWord, 0, numWords-1);
        insertWordAtFront(&userLadder, words[startInd]);
        ladderHeight++; // Now, the ladder has a start word
            
        char aWord[30] = "XYZ";
        printf("\n");
        
        // The basic gameplay lets the user build a word ladder interactively & iteratively.
        bool gameOver = false;
        while ( !gameOver ) { 
            bool validCheck = false;
            printLadder(userLadder);
            // pseudocode for one round of the basic gameplay:
            //  - display the incomplete ladder to this point
            printf("Please enter a new Word: ");
            scanf("%s", aWord);
            int indexCheck = findWord(words, aWord, 0, numWords-1);

            if(strcmp(aWord,"RESET") == 0){         //When the user enters RESET
                WordNode* temp = NULL;                  //Set temp to NULL
                freeLadder(userLadder);                 //free the ladder
                insertWordAtFront(&temp,startWord);     //Puts the last word in the front
                userLadder = temp;                       //Store temp into userLadder
                ladderHeight = 1;                       //Set the height to 1
                continue;               
            }  

            //  - prompt and scan a new word from the user
            while(validCheck == false){
                if(checkForValidWord(words,numWords,wordSize, userLadder, aWord) == false){   //check that the word is valid
                    printf("Please enter a new Word: ");                                       //if invalid, repeatedly scan another word until one is valid
                    scanf("%s", aWord);
                    indexCheck = findWord(words, aWord, 0, numWords-1);
                } else {
                    validCheck = true;
                }
            }
            
            insertWordAtFront(&userLadder, words[indexCheck]);       //prepend (insert at front) the new word to the ladder
            //  - repeat this entire process until either...
            //      - (1) the user enter DONE to stop the game, or
            //      - (2) the final word is entered and the ladder is complete
            if(strcmp(aWord, "DONE") == 0 || isLadderComplete(userLadder, finalWord)){
                gameOver = true; //only when user enters DONE or the ladder is complete
            }
        }
        // Once the game is over, print the ladder one last time;
        printLadder(userLadder);
        // then, report the ladder height and whether the ladder is complete or not.

        // Finally, make sure to free any heap-memory allocated for the game play
        freeLadder(userLadder);
    }
    
    
    printf("\n\n");
    printf("-----------------------------------------\n");
    printf("Welcome to the CS 211 Word Ladder Solver!\n");
    printf("-----------------------------------------\n");
    printf("\n");
    
    // run the algorithm to find the shortest word ladder
    WordNode* myLadder = findShortestWordLadder(words, usedWord, numWords, wordSize, startWord, finalWord);

    // display word ladder and its height if one was found
    if (myLadder == NULL) {
        printf("There is no possible word ladder from %s to %s\n",startWord,finalWord);
    } else {
        printf("Shortest Word Ladder found!\n\n");
        printLadder(myLadder);
    }
    printf("\nWord Ladder height = %d\n\n",getLadderHeight(myLadder));
    
    // free the heap-allocated memory for the shortest ladder
    freeLadder(myLadder);
    // free the heap-allocated memory for the words array
    freeWords(words,numWords);
    free(usedWord);

    return 0;
}
