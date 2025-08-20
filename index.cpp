#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>
#include <cctype>
#include<unordered_map>
#include <sstream>
#include <curl/curl.h>

using namespace std;

random_device rd;
mt19937 gen(rd());

string url = "https://gist.githubusercontent.com/cfreshman/a03ef2cba789d8cf00c08f767e0fad7b/raw/c46f451920d5cf6326d550fb2d6abb1642717852/wordle-answers-alphabetical.txt";

size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    string* buffer = static_cast<string*>(userp);
    buffer -> append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

void fetchWords(string &responseStr) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        // Follow redirects if any
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) 
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

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

    cout << "Hi sucker. Try defeating me in Wordle X)\nEnter result of guess\n\n";

    string allowed, guess, fetchResponse, line;
    vector<string> words;
    string result;
    int guesses = 0;

    fetchWords(fetchResponse);

    stringstream ss(fetchResponse);

    while (getline(ss, line)) 
        if (!line.empty())
            words.push_back(line);

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
