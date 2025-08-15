#include<iostream>
#include<string>
#include<vector>
#include<fstream>

using namespace std;

bool isCorrect() {

}

bool isInaccurate() {

}

bool isIncorrect() {
    
}

string guessWord(vector<string>& words) {

}

void eliminateValids(vector<string>& words, string word, vector<int> correct, vector<int> inaccurate) {
    
}

int main() {
    cout << "Hi sucker. Try beating me in Wordle XD. Lemme finish grab my gear gimme a sec...";

    ifstream file("words.txt");
    string allowed, word;
    vector<string> words;
    int guesses = 0;

    while (file >> allowed)
        words.push_back(allowed);


    while (guesses < 5) {
        cout << "Guess #" << guesses + 1 << ": " << guessWord(words);
        guesses++;
    }

}