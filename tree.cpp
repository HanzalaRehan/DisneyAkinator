/**
 * Author(s): 1. Hanzala B. Rehan
 * Description: Function to read questions from a CSV file and parse them into Question objects with properly populated sets for positive and negative IDs.
 * Date created: November 27th, 2024
 * Date last modified: December 4th, 2024
*/
/**
 * Changes Made:
 * Date         Author      Edit
 * 2024-11-27   1           Added structs for character, question. Utility Functions: parseSet, readQuestions, readCharacters. buildTree function.
 * 2024-12-04   1           Fixed csv and utility fuctions. Added game logic functions: getQuestion, getCharacter, setAnswer.
*/

// All necessary imports.
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

struct Character
{
public:
    int char_id;        // Unique character ID
    string name;        // Character Name.
    string image_path;  // Image path for this character.

    Character(int id, const string &n, const string &img)
        : char_id(id), name(n), image_path(img) {}
};

Character readCharacterByID(const string &filename, int search_id)
{
    /*
    Desc: Opens a file and reads its contents line by line to find a character with the specified ID.
    returns:
    (Character): A Character object containing the ID, name, and image path.
    Parameters:
        filename (const string &): The path to the file to be read.
        search_id (int): The ID of the character to search for in the file.
    */
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Error: Unable to open file " << filename << endl;
        throw runtime_error("File not found");
    }

    string line;
    bool firstLine = true;

    // Read file line by line
    while (getline(file, line))
    {
        if (firstLine)
        {
            firstLine = false; // Skip the header
            continue;
        }

        istringstream stream(line);
        string id_str, name, image_path;

        // Read fields separated by commas
        getline(stream, id_str, ',');
        getline(stream, name, ',');
        getline(stream, image_path, ',');

        // Convert ID to integer
        int id = stoi(id_str);

        // Check if this is the desired ID
        if (id == search_id)
        {
            file.close();
            return Character(id, name, image_path);
        }
    }

    file.close();
    throw runtime_error("Character with the given ID not found");
}

class Question
{
public:
    int q_id;                  // Unique question ID
    string text;               // Question text
    set<int> positive_ids;     // Characters for "yes" answers
    set<int> negative_ids;     // Characters for "no" answers
    Question *left = nullptr;  // Pointer to the "yes" subtree
    Question *right = nullptr; // Pointer to the "no" subtree

    // Constructor
    Question(int id, string txt, set<int> pos_ids, set<int> neg_ids)
        : q_id(id), text(txt), positive_ids(pos_ids), negative_ids(neg_ids) {}
};

set<int> parseSet(const string &setStr)
{
    /*
    Desc: Parses a set of integers from a string representation.
    returns:
    (set<int>): A set of integers parsed from the string.
    Parameters:
        setStr (const string &): A string representing a set of integers enclosed in curly braces (e.g., "{1.2.3}").
    */
    set<int> result;
    string numbers = setStr.substr(1, setStr.size() - 2); // Remove { and }
    stringstream ss(numbers);
    string item;
    while (getline(ss, item, '.'))
    {
        result.insert(stoi(item));
    }
    return result;
}

vector<Question *> readQuestionsFromCSV(const string &filename)
{
    /*
    Desc: Reads questions from a CSV file and stores them as Question objects.
    returns:
    (vector<Question *>): A vector of pointers to Question objects.
    Parameters:
        filename (const string &): The path to the CSV file containing questions data.
    */
    vector<Question *> questions;
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return questions;
    }

    string line;
    getline(file, line); // Skip header line

    while (getline(file, line))
    {
        stringstream ss(line);
        string idStr, text, trueSetStr, falseSetStr;

        // Read fields separated by commas
        getline(ss, idStr, ',');
        getline(ss, text, ',');
        getline(ss, trueSetStr, ',');
        getline(ss, falseSetStr, ',');

        // Remove extra quotes from the question text
        if (text.front() == '"' && text.back() == '"')
        {
            text = text.substr(1, text.size() - 2);
        }

        // Parse fields
        int id = stoi(idStr);
        set<int> trueSet = parseSet(trueSetStr);
        set<int> falseSet = parseSet(falseSetStr);

        // Create and store Question object
        questions.push_back(new Question(id, text, trueSet, falseSet));
    }

    file.close();
    return questions;
}

class QuestionTree
{
private:
    Question *root;                                     // Root of the question tree
    const string charactersFilename = "characters.csv";  // Filename for the characters csv.
    set<int> characters = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                            30, 31, 32};                // Set for IDs of all characters.


    // Constructor
    QuestionTree(string filename){
        vector<Question *> questions = readQuestionsFromCSV(filename);
        root = buildTree(characters, questions);
    }
    
    Question* buildTree(set<int> remaining_ids, vector<Question *> &questions)
    {
        /*
        Desc: Recursively builds a decision tree by selecting questions that best split the set of remaining character IDs.
        returns:
        (Question*): A pointer to the root of the decision tree.
        Parameters:
            remaining_ids (set<int>): The set of character IDs that need to be distinguished.
            questions (vector<Question *> &): A vector of available questions to use for splitting.
        */
        if (remaining_ids.size() == 1)
        {
            // Terminal condition: only one character left
            return new Question(-1, "Character identified: " + to_string(*remaining_ids.begin()), {}, {});
        }

        if (questions.empty())
        {
            // Terminal condition: no more questions
            return new Question(-1, "No more questions. Unable to identify.", {}, {});
        }

        // Select the best question (most balanced split)
        Question *best_question = nullptr;
        int min_difference = INT_MAX;

        for (auto *q : questions)
        {
            set<int> pos_ids;
            set<int> neg_ids;

            // Intersection of the question's IDs with remaining IDs
            set_intersection(remaining_ids.begin(), remaining_ids.end(),
                                q->positive_ids.begin(), q->positive_ids.end(),
                                inserter(pos_ids, pos_ids.begin()));
            set_intersection(remaining_ids.begin(), remaining_ids.end(),
                                q->negative_ids.begin(), q->negative_ids.end(),
                                inserter(neg_ids, neg_ids.begin()));

            int difference = abs((int)pos_ids.size() - (int)neg_ids.size());
            if (difference < min_difference)
            {
                min_difference = difference;
                best_question = q;
            }
        }

        if (!best_question)
        {
            // No suitable question found
            return new Question(-1, "Unable to further differentiate.", {}, {});
        }

        // Partition remaining IDs
        set<int> pos_ids;
        set<int> neg_ids;

        set_intersection(remaining_ids.begin(), remaining_ids.end(),
                            best_question->positive_ids.begin(), best_question->positive_ids.end(),
                            inserter(pos_ids, pos_ids.begin()));
        set_intersection(remaining_ids.begin(), remaining_ids.end(),
                            best_question->negative_ids.begin(), best_question->negative_ids.end(),
                            inserter(neg_ids, neg_ids.begin()));

        // Remove the chosen question from the list
        vector<Question *> remaining_questions;
        for (auto *q : questions)
        {
            if (q != best_question)
            {
                remaining_questions.push_back(q);
            }
        }

        // Recursively build subtrees
        Question *node = new Question(best_question->q_id, best_question->text,
                                        best_question->positive_ids, best_question->negative_ids);
        node->left = buildTree(pos_ids, remaining_questions);  // "yes" branch
        node->right = buildTree(neg_ids, remaining_questions); // "no" branch

        return node;
    }

public:
    string getQuestionText() {
        /*
        Desc: Retrieves the text of the current question.
        returns:
        (string): The text content of the current question.
        */
        return root->text;
    }

    Character getCharacter() {
        /*
        Desc: Retrieves a character based on the current state of the characters set.
        returns:
        (Character): A Character if game is over, null otherwise.
        */
        if (characters.size() > 1) {
            return;
        }
        else if (characters.size() < 1) {
            return readCharacterByID(charactersFilename, 0);
        }
        else {
            int characterID = *characters.begin();
            return readCharacterByID(charactersFilename, characterID);
        }
    }

    void setAnswer(bool Answer){
        /*
        Desc: Updates the set of characters based on the answer provided and navigates the decision tree accordingly.
        Parameters:
            Answer (bool): The answer to the current question, where 'true' or 'false' affects character selection.
        */
        if (Answer) {
            // Creating a temporary set for the difference of characters set and negative ids.
            set<int> diff;
            set_difference(characters.begin(), characters.end(),
                        root->negative_ids.begin(), root->negative_ids.end(),
                        inserter(diff, diff.begin()));
            
            characters = diff;
            root = root->left;
        }
        else {
            // Creating a temporary set for the difference of characters set and positive ids.
            set<int> diff;
            set_difference(characters.begin(), characters.end(),
                        root->positive_ids.begin(), root->positive_ids.end(),
                        inserter(diff, diff.begin()));
            
            characters = diff;
            root = root->right;
        }
    }
};