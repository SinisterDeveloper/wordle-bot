#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<random>

using namespace std;

// bool checkCorrect() {

// }

// bool checkInaccurate() {

// }

// bool checkIncorrect() {

// }

string guessWord(vector<string>& words) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, words.size() - 1);

    return words[dist(gen)];
}

// void eliminateValids(vector<string>& words, string word, vector<int> correct, vector<int> inaccurate) {
    
// }

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

        guesses++;
    }

    return 0;

}