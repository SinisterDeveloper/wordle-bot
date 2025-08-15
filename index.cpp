#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<random>

using namespace std;

bool checkCorrect(string& source, string& target, vector<int>& correct) {
    for (int pos : correct) 
        if (source[pos] != target[pos]) 
            return true;
        
    return false;
}

bool checkInaccurate(string &source, string &target, vector<int> &inaccurate) {
    for (int pos : inaccurate)
        if (target[pos] == source[pos] || target.find(source[pos]) == string::npos)
            return true;

    return false;
}

bool checkIncorrect(string &source, string &target, vector<int> &incorrect) {
    for (int pos : incorrect) 
        if (target.find(source[pos]) != string::npos) 
            return true;
        
    return false;
}

string guessWord(vector<string>& words) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, words.size() - 1);

    return words[dist(gen)];
}

void eliminateOptions(vector<string>& words, string& word, string& correct, string& inaccurate) {
    
}

int main() {
    cout << "Hi sucker. Try beating me in Wordle hehe. Lemme finish grab my gear gimme a sec...\n";

    ifstream file("words.txt");

    string allowed, word;
    vector<string> words;
    string correct, inaccurate;
    int guesses = 0;

    while (file >> allowed)
        words.push_back(allowed);


    while (guesses < 5) {
        word = guessWord(words);
        cout << "Guess #" << guesses + 1 << ": " << word << "\n\n";

        cout << "Enter the positions of correct letters";
        cin >> correct;

        if (correct.size() == 5) {
            cout << "Looks like I won! haha!\n";
            return 1;
        }

        cout << "Enter the positions of inaccurate letters";
        cin >> inaccurate;

        eliminateOptions(words, word, correct, inaccurate);

        guesses++;
    }

    return 0;
}