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

string guessWord(const vector<string> &words) {
    if (words.empty()) return "";

    uniform_int_distribution<int> dist(0, static_cast<int>(words.size()) - 1);

    return words[dist(gen)];
}

auto validateResult = [](const string &r) {
    if (r.size() != 5) return false;

    for (char c : r) if (c != 'G' && c != 'Y' && c != 'B') return false;

    return true;
};

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

void eliminateOptions(vector<string> &words, const string &guess, const string& result) {
    words.erase(
        remove_if(words.begin(), words.end(),
                  [&guess, &result](const string &word) -> bool
                  {
                      return getFeedback(word, guess) != result;
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
            string guess = guessWord(words);
            if (guess.empty())
                return -1;

            if (guess == hiddenWord)
                return guesses + 1;

            string correct, inaccurate;

            eliminateOptions(words, guess, getFeedback(hiddenWord, guess));
            ++guesses;
        }

        return -1;
    }

    cout << "Hi sucker. Try defeating me in Wordle X)\nEnter result of guess\n\n";

    ifstream file("words.txt");
    if (!file) {
        cerr << "Could not open words.txt\n";
        return 0;
    }

    string allowed, guess;
    vector<string> words;
    string result;
    int guesses = 0;

    while (file >> allowed)
        words.push_back(allowed);

    while (guesses < 6) {
        guess = guessWord(words);

        if (guess.empty()) {
            cout << "No words left.\n";
            return 1;
        }

        cout << "\nGuess #" << guesses + 1 << ": " << guess << "\n\n";

        cout << "Enter the result of guess: ";
        cin >> result;

        transform(result.begin(), result.end(), result.begin(), ::toupper);

        if (!validateResult(result)) {
            cout << "Invalid Result!";
            return 1;
        }

        if (result == "GGGGG") {
            cout << "I won HAHA";
            return 0;
        }

        eliminateOptions(words, guess, result);
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
