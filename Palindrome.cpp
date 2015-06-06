

#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "simpio.h"
using namespace std;

Lexicon englishWords = Lexicon("EnglishWords.dat");

bool isPalindrome(string word)
{
    string palindrome = "";
    for(int i = 0; i < word.size(); i++)
    {
        if(word[i] != word[(word.size() - 1) - i])
        {
            //cout << "is not a palindrome" << endl;
            return false;
        }
        else
        {
            palindrome.push_back(word[(word.size() - 1) - i]);

        }
    }
    if(englishWords.contains(palindrome))
    {
        return true;
    }

    return false;
}

Vector<string> findAllPalindromes()
{
    Vector<string> listOfWords = Vector<string>();

    for(string word : englishWords)
    {
        if(isPalindrome(word))
        {
            listOfWords.add(word);
        }
    }
    return listOfWords;
}

int main()
{

    while(true)
    {
        cout << "Find all palindromes?(Y/N)";
        string decision = getLine();
        if(decision == "Y")
        {
            Vector<string> words = findAllPalindromes();
            for(string word : words)
            {
                cout << word << endl;
            }
        }
        else
        {
            cout << "Enter word for palindrome: ";
            string wordToCheck = getLine();
            cout << endl;
            if(wordToCheck == " " || wordToCheck == "\n") break;

            if(isPalindrome(wordToCheck))
            {
                cout << wordToCheck << " is a palindrome!" << endl;
            }
        }
    }

    return 0;
}
