#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>
#include <cctype>
#include <unordered_map>
#include <sstream>
#include <curl/curl.h>
#undef min

using namespace std;

random_device rd;
mt19937 gen(rd());
string url = "https://gist.githubusercontent.com/cfreshman/a03ef2cba789d8cf00c08f767e0fad7b/raw/c46f451920d5cf6326d550fb2d6abb1642717852/wordle-answers-alphabetical.txt";


size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    string* buffer = static_cast<string*>(userp);
    buffer->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

void fetchWords(string &responseStr) {
    CURL *curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
        }
        curl_easy_cleanup(curl);
    }
}

bool validateResult(const string &r) {
    if (r.size() != 5) return false;
    for (char c : r) 
        if (c != 'G' && c != 'Y' && c != 'B') return false;
    
    return true;
}

string getFeedback(const string& word, const string& guess) {
    string feedback(5, 'B');
    unordered_map<char, int> freq;
    for (char c : word) freq[c]++;

    for (int i = 0; i < 5; i++) 
        if (word[i] == guess[i]) {
            feedback[i] = 'G';
            freq[guess[i]]--;
        }
    
    for (int i = 0; i < 5; i++) 
        if (feedback[i] == 'B' && freq.count(guess[i]) && freq[guess[i]] > 0) {
            feedback[i] = 'Y';
            freq[guess[i]]--;
        }
    
    return feedback;
}

void eliminateOptions(vector<string> &words, const string &guess, const string& result) {
    words.erase(
        remove_if(words.begin(), words.end(),
            [&](const string &word) {
                return getFeedback(word, guess) != result;
            }),
        words.end());
}

string guessWord(const vector<string> &words) {
    if (words.empty()) return "";
    if (words.size() <= 2) return words[0]; 

    string bestWord;
    int bestScore = INT_MAX;
    const vector<string>& candidates = words; 

    for (const string &candidate : candidates) {
        unordered_map<string, int> partitions;
        for (const string &target : words) 
            partitions[getFeedback(target, candidate)]++;
        

        int worstCase = 0;
        for (const auto &p : partitions) 
            worstCase = max(worstCase, p.second);
        

        if (worstCase < bestScore) {
            bestScore = worstCase;
            bestWord = candidate;
        }
    }
    return bestWord;
}

int solveForWord(const string &hiddenWord, const vector<string> &allWords) {
    vector<string> possibleWords = allWords;
    for (int guesses = 0; guesses < 6; ++guesses) {
        string guess;
        if (guesses == 0) guess = "crane";
        else guess = guessWord(possibleWords);
        
        if (guess.empty()) return -1;
        if (guess == hiddenWord) return guesses + 1;
        string feedback = getFeedback(hiddenWord, guess);
        eliminateOptions(possibleWords, guess, feedback);
    }
    return -1;
}

void playInteractive() {
    cout << "Fetching word list, please wait...\n";
    string fetchResponse;
    fetchWords(fetchResponse);

    vector<string> words;
    stringstream ss(fetchResponse);
    string line;
    while (getline(ss, line)) 
        if (!line.empty()) words.push_back(line);
    
    
    cout << "\nHi sucker. Try defeating me in Wordle X)\n";

    for (int guesses = 0; guesses < 6; ++guesses) {
        string guess;
        if (guesses == 0) {
            uniform_int_distribution<int> dist(0, static_cast<int>(words.size()) - 1);
            guess = words[dist(gen)];
        } else guess = guessWord(words);
    

        if (guess.empty()) {
            cout << "I'm stumped! You must have made a mistake with the feedback.\n";
            return;
        }

        cout << "\nGuess #" << guesses + 1 << ": " << guess << "\n";
        
        string result;
        cout << "Enter the result of the guess (e.g., BGYBB): ";
        cin >> result;
        transform(result.begin(), result.end(), result.begin(), ::toupper);

        if (!validateResult(result)) {
            cout << "Invalid Result! Please use 5 characters: G, Y, or B.\n";
            return;
        }
        if (result == "GGGGG") {
            cout << "\nI won in " << guesses + 1 << " guesses! HAHA.\n";
            return;
        }
        eliminateOptions(words, guess, result);
    }
    cout << "\nUgh, FINE, I lose. I couldn't guess it in 6 tries :(\n";
}


int main() {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    int option;
    cout << "Hi! I'm Wordle-Bot developed by SinisterDeveloper\n\n[1] - New Game\n[2] - Testing\n\n[3] - View Code Repository\n\nPick an option(number): ";
    cin >> option;

    if (option == 1) 
        playInteractive();
     
    else if (option == 2) {
        cout << "Fetching word list for testing...\n";
        string fetchResponse;
        fetchWords(fetchResponse);

        vector<string> allWords;
        stringstream ss(fetchResponse);
        string line;
        while (getline(ss, line)) {
            if (!line.empty()) allWords.push_back(line);
        }

        if (allWords.empty()) {
            cerr << "Failed to fetch or parse the word list. Cannot run tests.\n";
            return 1;
        }
        
        cout << "Starting tests against the fetched word list (" << allWords.size() << " words)...\n";
        cout << "This will take some time.\n";
        int totalGuesses = 0;
        int failures = 0;
        int maxGuesses = 0;
        string worstWord;
        int wordsTested = 0;

        for (const auto &hidden : allWords) {
            wordsTested++;

            cout << "\rTesting [" << wordsTested << "/" << allWords.size() << "]: " << hidden << flush;

            int result = solveForWord(hidden, allWords);
            if (result == -1) {
                ++failures;
                cout << "\n >> Failed on: " << hidden << '\n';
            } else {
                totalGuesses += result;
                if (result > maxGuesses) {
                    maxGuesses = result;
                    worstWord = hidden;
                }
            }
        }

        cout << "\n";

        int succeeded = static_cast<int>(allWords.size()) - failures;
        cout << "\n--- Testing Complete ---\n";
        cout << "Total Words Tested: " << allWords.size() << '\n';
        cout << "Successes: " << succeeded << " (" << (double)succeeded / allWords.size() * 100.0 << "%)\n";
        cout << "Failures: " << failures << '\n';

        if (succeeded > 0) 
            cout << "Average Guesses (for successes): " << static_cast<double>(totalGuesses) / succeeded << '\n';
        
        if (!worstWord.empty()) 
            cout << "Hardest Word: " << worstWord << " (" << maxGuesses << " guesses)\n";
    
    }

    curl_global_cleanup();
    return 0;
}

