/*
 * File: Boggle.cpp
 * ----------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the main starter file for Assignment #4, Boggle.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include <time.h>
#include "gboggle.h"
#include "grid.h"
#include "gwindow.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
#include "vector.h"
using namespace std;

/* Constants */

const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;

const string STANDARD_CUBES[16]  = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
 
const string BIG_BOGGLE_CUBES[25]  = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

/* Function prototypes */

void welcome();
void giveInstructions();
Grid<char> randomizeCubes(bool isCustom);
void printVector(Vector<string> &toPrint);
void makeGrid(Grid<char> &grid,string s,bool isCustom);
void drawCubes(Grid<char> &grid);
void playerTurn(Grid<char> &grid);
void computerTurn(Grid<char> &grid);
bool containsWord(Vector<string> &v,string word);
bool hasLetter(int row, int col, Grid<char> &grid, char letter);
bool findWord2(int row, int col, string soFar, string remaining, Grid<char> &grid);
bool lookForWord(string word, Grid<char> &grid);
void searchGrid(Grid<char> &grid, Lexicon words, Vector<string> &foundWords);
void recursiveFindWord(int row, int col, std::string soFar, Grid<char> &grid, Lexicon words, Vector<string> &foundWords);

/* Main program */

int main() {
    GWindow gw(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
    initGBoggle(gw);
	while(true){
		welcome();
		giveInstructions();

		Grid<char> boardGrid(4,4);

		string customBoardPrompt = getLine("Custom board?(Y/N)");
		if(customBoardPrompt == "y" || customBoardPrompt == "Y"){
			boardGrid = randomizeCubes(true);
		}else{
			boardGrid = randomizeCubes(false);
		}

        //set up the board
		drawBoard(boardGrid.numRows(),boardGrid.numCols());
		drawCubes(boardGrid);

        playerTurn(boardGrid);
        computerTurn(boardGrid);

		cout << "game over" << endl;
		string playAgain = getLine("Want to play again?(Y/N)");
		if(playAgain == "n" || playAgain == "N")
			break;
	}
	gw.close();

    return 0;
}

/*
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */

void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/*
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */

void giveInstructions() {
    cout << endl;
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl << endl;
    cout << "Hit return when you're ready...";
    getLine();
}

// [TODO: Fill in the rest of the code]

Grid<char> randomizeCubes(bool isCustom)
{
	Vector<string> randomCubes;
	//fill vector with the array of letter cubes
	for(int i = 0; i < 16; i++){
		randomCubes.add(STANDARD_CUBES[i]);
	}

    srand(time(0));
	//shuffle the vector
	for(int i = 0; i < randomCubes.size(); i++){
		int r = rand() % (randomCubes.size()-i)+i;
		string temp = randomCubes[r];
		randomCubes[r] = randomCubes[i];
		randomCubes[i] = temp;
	}
	
	//add a single random letter from each string for the boggle board
	string letters = "";
	for(int i = 0; i < randomCubes.size(); i++){
		string currString = randomCubes[i];
		int r = rand() % currString.size();
		letters += currString[r];
	}
	
	Grid<char> randomGrid(4,4);
	makeGrid(randomGrid,letters,isCustom);

	return randomGrid;
}

void makeGrid(Grid<char> &grid,string s,bool isCustom)
{
	int row = 0;
	int col = 0;
	for(int i = 0; i < s.size(); i++){
		if(col == 4 || col == 8 || col == 12){
			row++;
			col = 0;
		}
		
		if(isCustom){
			string letter = getLine("Enter a letter: ");
			grid[row][col] = letter[0];
		}else{
			grid[row][col] = s[i];
		}
		col++;
	}
	if(isCustom){
		cout << "Grid is finished" << endl;
	}
	return;
}

void drawCubes(Grid<char> &grid)
{
	for(int i = 0; i < grid.numRows(); i++){
		for(int j = 0; j < grid.numCols(); j++){
			labelCube(i,j,grid.get(i,j));
		}
	}
}

void playerTurn(Grid<char> &grid)
{
	Vector<string> playerWords;
    Lexicon englishWords("dictionary.txt");
	while(true)
	{
		string guessWord = getLine("Guess a word: ");
		if(guessWord == " " || guessWord == "")
			break;
		
		if(lookForWord(guessWord,grid)){
            //if the word is 4 or more letters and is a valid word, update player points
			if(guessWord.length() >= 4 && englishWords.contains(guessWord) && !containsWord(playerWords,guessWord)){
				recordWordForPlayer(guessWord,HUMAN);
				playerWords.add(guessWord);	
			}
		}
	}
	return;
}

bool containsWord(Vector<string> &v,string word)
{
	for(int i = 0; i < v.size(); i++){
		if(v.get(i) == word){
			return true;
		}
	}
	return false;
}

//helper to start recursive function
bool lookForWord(string word, Grid<char> &grid)
{
	for(int i = 0; i < grid.numRows(); i++){
		for(int j = 0; j < grid.numCols(); j++){
			if(findWord2(i,j,"",word,grid)){
				cout << "found word" << endl;
				return true;
			}
		}
	}
	return false;
}

bool findWord2(int row, int col, string soFar, string remaining, Grid<char> &grid)
{
	if(remaining == " " || remaining == "")
		return true;
	if(row == grid.numRows() || col == grid.numCols() || (row == grid.numRows() && col == grid.numCols())){
		return false;
	}
    //highlightCube(row,col,true);
	if(!hasLetter(row,col,grid,remaining[0])){
        //highlightCube(row,col,false);
		return false;
	}
    //check the squares around the current square
	if(hasLetter(row,col,grid,remaining[0])){
		soFar = soFar + remaining[0];
		if(findWord2(row+1,col,soFar,remaining.substr(1),grid))
			return true;
		else if(findWord2(row,col+1,soFar,remaining.substr(1),grid))
			return true;
		else if(findWord2(row+1,col+1,soFar,remaining.substr(1),grid))
			return true;
		else if(findWord2(row+1,col-1,soFar,remaining.substr(1),grid))
			return true;
		else if(findWord2(row,col-1,soFar,remaining.substr(1),grid))
			return true;
		else if(findWord2(row-1,col-1,soFar,remaining.substr(1),grid))
			return true;
		else if(findWord2(row-1,col,soFar,remaining.substr(1),grid))
			return true;
	}else{
		if(findWord2(row,col+1,soFar,remaining,grid))
			return true;
		else if(findWord2(row+1,col,soFar,remaining,grid))
			return true;
		else if(findWord2(row+1,col+1,soFar,remaining,grid))
			return true;
	}
    //highlightCube(row,col,false);
	return false;
}

void computerTurn(Grid<char> &grid)
{
    Lexicon words("dictionary.txt");
    Vector<std::string> foundWords;
    searchGrid(grid,words,foundWords);
}

void searchGrid(Grid<char> &grid, Lexicon words, Vector<std::string> &foundWords)
{
    for(int i = 0; i < grid.numRows()-1; i++){
        for(int j = 0; j < grid.numCols()-1; j++){
            recursiveFindWord(i,j,"",grid,words,foundWords);
            if(foundWords.isEmpty() != true && foundWords.get(foundWords.size()-1) != "" && foundWords.get(foundWords.size()-1).size() >= 4){
                recordWordForPlayer(foundWords.get(foundWords.size()-1),COMPUTER);
            }
        }
    }
}

//credit to erickwill, his code guided me to the answer
void recursiveFindWord(int row, int col,std::string soFar, Grid<char> &grid, Lexicon words, Vector<std::string> &foundWords)
{
    char orig = grid.get(row,col);
    soFar = soFar + grid.get(row,col);
    drawCubes(grid);
    cout << "row = " << row << " col = " << col << endl;
    cout << "sofar = " << soFar << endl;

    if(words.contains(soFar) && !containsWord(foundWords,soFar)){
        cout << "contains word: " << soFar << endl;
        foundWords.push_back(soFar);
        return;
    }
    if(!words.containsPrefix(soFar)){
        cout << "does not contain prefix: " << soFar << endl;
        return;
    }

    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            if(row+i>=0 && col+j >=0 && row+i < grid.numRows() && col+j < grid.numCols() && (row+i < grid.numRows() && col+j < grid.numCols())
               && !(i==0 && j==0) && grid.get(row+i,col+j) != '~')
               {
                grid.set(row,col,'~');
                recursiveFindWord(row+i,col+j,soFar,grid,words,foundWords);
                grid.set(row,col,orig);
               }
            }

    }
   return;
}

bool hasLetter(int row, int col, Grid<char> &grid, char letter)
{
	if(grid[row][col] == letter){
		return true;
	}
	return false;
}

void printVector(Vector<string> &toPrint)
{
	for(int i = 0; i < toPrint.size(); i++){
		cout << toPrint[i] << endl;
	}
}
