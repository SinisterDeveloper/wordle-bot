#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>
#include <cctype>
#include<unordered_map>

using namespace std;

random_device rd;
mt19937 gen(rd());

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

    uniform_int_distribution<int> dist(0, static_cast<int>(words.size()) - 1);

    return words[dist(gen)];
}

/**
 * 
 * Correct - G (green)
 * Inaccurate - Y (yellow)
 * Incorrect - B (black/grey)
 * 
 * 
 */
string getFeedback(string word, string guess) {
    string feedback(5, 'B');
    unordered_map<char, int> freq;

    for (char c : word) freq[c]++;

    for (int i = 0; i < 5; i++) 
        if (word[i] == guess[i]) {
            feedback[i] = 'G';
            freq[guess[i]]--;
        }
    

    for (int i = 0; i < 5; i++) 
        if (feedback[i] == 'B' && freq[guess[i]] > 0) {
            feedback[i] = 'Y';
            freq[guess[i]]--;
        }
    

    return feedback;
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
                      return checkCorrect(word, s, correctInt)
                          || checkInaccurate(word, s, inaccurateInt)
                          || checkIncorrect(word, s, incorrectInt);
                  }),
        words.end());
}

int play(const string &hiddenWord = "") {

    // Internal Testing Purposes
    if (!hiddenWord.empty()) {
        ifstream file("words.txt");

        vector<string> words;
        string allowed;
        while (file >> allowed)
            words.push_back(allowed);

        int guesses = 0;
        while (guesses < 6) {
            string word = guessWord(words);
            if (word.empty())
                return -1;
            

            if (word == hiddenWord)
                return guesses + 1;

            string correct, inaccurate;
            for (int i = 0; i < 5; ++i) {

                if (word[i] == hiddenWord[i])
                    correct.push_back('0' + i);

                else if (hiddenWord.find(word[i]) != string::npos)
                    inaccurate.push_back('0' + i);

            }

            eliminateOptions(words, word, correct, inaccurate);
            ++guesses;
        }

        return -1;
    }

    cout << "Hi sucker. Try defeating me in Wordle X)\nEnter positions as contiguous digits (e.g. 013). Enter 'none' for no positions.\n\n";

    ifstream file("words.txt");
    if (!file) {
        cerr << "Could not open words.txt\n";
        return 0;
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
            cout << "No words left.\n";
            return 0;
        }

        cout << "\nGuess #" << guesses + 1 << ": " << word << "\n\n";

        cout << "Enter the positions of correct letters: ";
        cin >> correct;
        if (correct == "none")
            correct.clear();

        if (correct.size() == 5)
        {
            cout << "I won! haha! you suck lol\n";
            return 0;
        }

        cout << "Enter the positions of inaccurate letters: ";
        cin >> inaccurate;
        if (inaccurate == "none")
            inaccurate.clear();

        eliminateOptions(words, word, correct, inaccurate);
        ++guesses;
    }

    cout << "ugh FINE i lose :(\n";
    return 0;
}

int main() {
    int option;
    cout << "Welcome to Wordle-Bot\n\n[1] - New Game\n[2] - Testing\n\nPick an option: ";
    cin >> option;

    if (option == 1) 
        play();
    
    else if (option == 2) {
        ifstream answersFile("words.txt");

        vector<string> answers;
        string w;
        while (answersFile >> w)
            answers.push_back(w);


        int totalGuesses = 0;
        int failures = 0;
        int maxGuesses = 0;
        string worstWord;

        for (const auto &hidden : answers) {
            int result = play(hidden);
            if (result == -1) {
                ++failures;
                cout << "Failed on: " << hidden << '\n';
            }
            else {
                totalGuesses += result;
                if (result > maxGuesses) {
                    maxGuesses = result;
                    worstWord = hidden;
                }
            }
        }

        int succeeded = static_cast<int>(answers.size()) - failures;
        cout << "\nTesting complete\n";
        cout << "Total tested: " << answers.size() << '\n';
        cout << "Failures: " << failures << '\n';

        if (succeeded > 0) 
            cout << "Average guesses (succeeded only): " << (double)totalGuesses / succeeded << '\n';
        if (!worstWord.empty())
            cout << "Worst-case word: " << worstWord << " (" << maxGuesses << " guesses)\n";
    }

    return 0;
}
