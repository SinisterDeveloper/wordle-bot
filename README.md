# WordleMax

WordleMax is a Wordle solver/bot programmed by [SinisterDeveloper](https://github.com/SinisterDeveloper) with an accuracy of ~99.5% and an average guess count of 3.78 (tested against all possible solutions in NYT Wordle)

### Comparison against known Top Tier bots

| Bot/Strategy | Accuracy (Hard Mode) | Average Guesses (Hard Mode) | Algorithm
| :--- | :--- | :--- | :--- |
| **WordleMax (this)** | 99.5% | 3.78 | MiniMax |
| **3Blue1Brown** (varies by implementation and dictionary) | ~99.2% - 99.9% | ~3.5 - 3.7 | Entropy |
| **Bot Plays Wordle** | ~99.6% | 3.95 | Entropy |


## Installation

### Windows (One of the below): 

* `Release` - Find the latest release in the `Releases` section, unzip the content and run `wordle.exe`
* `Compile` - if you want to compile the code yourself, [read here](https://github.com/SinisterDeveloper/wordle-bot/blob/stable/BUILD.md)

### Linux

[Read here](https://github.com/SinisterDeveloper/wordle-bot/blob/stable/BUILD.md) to easily compile the code in couple of commands and run on your preferred Linux distro

## Algorithm

### MiniMax Search

- Wordlemax uses an algorithm `MiniMax` to aggressively minimising the _Worst-case Depth_ resulting in stronger "average guess" count compared to elite solvers which use an `Entropy` algorithm
- When using MiniMax: Hard Mode is naturally enforced as all the previous guess' results are used for the subsequent guesses
- This algorithm is meant to minimise the largest possible set of remaining solutions by going one move ahead to compute the future results, hence optimising the worst case scenario
- The stronger **average-guess** count is the result of Minimax ensuring that even the hardest partitions are resolved effeciently
- `Entropy` bots on the other hand, optimise for _expected_ information gain, and perform _slightly worse_ in Hard Mode

## Benchmarking

To facilitate transparent benchmarking, `test` mode within the bot solves against all possible solutions, displaying the results for failed attempts, accuracy and average guess count

## Support

If you find issues or need help running it, you may open an issue in the `Issues` section and I will help you out!


if you liked the bot, please star and/or share the repository with your friends or people who may be interested!

Thanks and have a great day!