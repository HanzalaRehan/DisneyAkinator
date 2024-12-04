
# Decision Tree-Based Character Guessing Game

This program implements a decision tree to identify a character based on a series of questions answered by the user. It utilizes CSV files for loading character and question data.

## Features

- **Interactive Gameplay:** A dynamic series of questions guides the player to identify a specific character.
- **Tree-Based Logic:** Questions are structured in a binary decision tree for efficient character identification.
- **Customizable Data:** The game supports user-defined characters and questions via CSV files.

## Usage

1. **Retrieve the Current Question:**
   - Use the `getQuestion()` method to fetch the current question text.

2. **Set the User's Answer:**
   - Call `setAnswer(bool answer)` to provide the user's response:
     - `true` for "yes"
     - `false` for "no"

3. **Check Game Status:**
   - Call `getCharacter()`:
     - If it returns a `Character`, the game ends, and the identified character is provided.
     - Otherwise, proceed with the next question.

### Example Workflow

```cpp
QuestionTree tree("questions.csv");

// UI Loop
while (true) {
    string question = tree.getQuestionText();
    cout << question << endl;

    bool userAnswer = getUserInput(); // Implement this function to capture input
    tree.setAnswer(userAnswer);

    Character result = tree.getCharacter();
    if (result) {
        cout << "Game Over! Character: " << result.name << endl;
        break;
    }
}
```

## File Formats

### Questions CSV File
Each row should contain:
- `Question ID`
- `Question Text`
- `Positive IDs` (e.g., `{1.2.3}`)
- `Negative IDs` (e.g., `{4.5.6}`)

### Characters CSV File
Each row should contain:
- `Character ID`
- `Name`
- `Image Path`


## Code Structure

- **Classes:**
  - `Character`: Represents a character with a unique ID, name, and image path.
  - `Question`: Represents a question with associated IDs for "yes" and "no" answers.
  - `QuestionTree`: Manages the tree structure and gameplay logic.

- **Key Methods:**
  - `getQuestionText()`: Fetches the text of the current question.
  - `setAnswer(bool answer)`: Updates the tree based on the user's answer.
  - `getCharacter()`: Determines if the game is over and provides the identified character.


## Requirements

- **Compiler:** C++17 or higher
- **Libraries:** Standard C++ libraries (`<iostream>`, `<fstream>`, `<sstream>`, `<string>`, `<set>`, `<vector>`)


## Credits
- `Dataset`: Initial dataset, "data.csv", taken from https://github.com/ZhannDark/AkinatorGame

## Data preparation:
- Raw data from `data.csv` was transformed to useable CSVs, `characters.csv` and `questions.csv`, using `dataHandler.ipynb`

## Author

- **Hanzala B. Rehan**

## Changelog

- **2024-11-27:** Added core tree-building logic and utility functions.
- **2024-12-04:** Fixed CSV handling and implemented game logic (`getQuestion`, `getCharacter`, `setAnswer`).