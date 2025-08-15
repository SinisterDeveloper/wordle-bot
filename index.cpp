#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>
#include <cctype>

using namespace std;

bool checkCorrect(const string &source, const string &target, const vector<int> &correct) {
    for (int pos : correct)
        if (pos < 0 || pos >= static_cast<int>(source.size()) || source[pos] != target[pos])
            return true;
    
    return false;
}

bool checkInaccurate(const string &source, const string &target, const vector<int> &inaccurate) {
    for (int pos : inaccurate) {
        if (pos < 0 || pos >= static_cast<int>(source.size()))
            return true;
        char ch = source[pos];
        if (target[pos] == ch)
            return true;
        if (target.find(ch) == string::npos)
            return true;
    }
    return false;
}

bool checkIncorrect(const string &source, const string &target, const vector<int> &incorrect) {
    for (int pos : incorrect) {
        if (pos < 0 || pos >= static_cast<int>(source.size())) continue;
            
        char ch = source[pos];
        if (target.find(ch) != string::npos)
            return true;
    }
    return false;
}

string guessWord(const vector<string> &words) {
    if (words.empty()) return "";

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, static_cast<int>(words.size()) - 1);

    return words[dist(gen)];
}

void eliminateOptions(vector<string> &words, const string &word, const string &correct, const string &inaccurate) {
    vector<int> correctInt, inaccurateInt, incorrectInt;

    for (char c : correct) 
        if (isdigit(static_cast<unsigned char>(c)))
            correctInt.push_back(c - '0');
    

    for (char c : inaccurate) 
        if (isdigit(static_cast<unsigned char>(c)))
            inaccurateInt.push_back(c - '0');
    

    for (int i = 0; i < 5; ++i) {
        char digit = static_cast<char>('0' + i);

        if (correct.find(digit) == string::npos && inaccurate.find(digit) == string::npos)
            incorrectInt.push_back(i); 
    }

    words.erase(
        remove_if(words.begin(), words.end(),
                  [&word, &correctInt, &inaccurateInt, &incorrectInt](const string &s) -> bool
                  {
                      return checkCorrect(word, s, correctInt) || checkInaccurate(word, s, inaccurateInt) || checkIncorrect(word, s, incorrectInt);
                  }),
        words.end());
}

int main() {
    cout << "Hi sucker. Try beating me in Wordle hehe. Lemme finish grab my gear gimme a sec...\n";

    ifstream file("words.txt");
    if (!file){
        cerr << "Could not open words.txt\n";
        return 1;
    }

    string allowed, word;
    vector<string> words;
    string correct, inaccurate;
    int guesses = 0;

    while (file >> allowed)
        words.push_back(allowed);

    while (guesses < 6) {
        word = guessWord(words);
        if (word.empty()) {
            cout << "No candidates left.\n";
            return 0;
        }

        cout << "Guess #" << guesses + 1 << ": " << word << "\n\n";

        cout << "Enter the positions of correct letters: ";
        cin >> correct;
        if (correct == "none")
            correct.clear();

        if (correct.size() == 5) {
            cout << "Bot reports win.\n";
            return 0;
        }

        cout << "Enter the positions of inaccurate letters: ";
        cin >> inaccurate;
        if (inaccurate == "none")
            inaccurate.clear();

        eliminateOptions(words, word, correct, inaccurate);
        ++guesses;
    }

    cout << "ugh FINE i lose :(";
    return 0;
}
