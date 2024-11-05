#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct YearlyNames {
    int year;
    string femaleName;
    string maleName;
};

enum Gender {
    MALE,
    FEMALE
};

void printSeparator() {
    cout << "****************************************************************************************************" << endl;
}

int main() {
    // Variable to hold the name of the input file
    string filename;

    // Welcome user to the program
    cout << "Welcome to CPHW2." << endl;
    printSeparator();

    // Asks user for input filename
    cout << "Please enter the file name containing the baby names per year: ";
    cin >> filename;
    cout << endl << endl;

    // Open the file name according to input, and test whether it opens correctly
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cout << "Could not open file '" << filename << "'. Please re-run the program and enter a valid file name." << endl;
        return 1;
    }
    cout << "File '" << filename << "' opened successfully." << endl;

    // list of yearly male and female names (YearlyNames structs)
    vector<YearlyNames> yearlyNamesList;

    // smallest year in input file
    int minYear = 100000;
    // latest year in input file
    int maxYear = 0;
    // current line number for user feedback
    int lineNumber = 0;
    // holds current line
    string line;
    // iterate through each line and load into the list
    while (getline(inputFile, line, '\r')) {
        // increment current line number
        lineNumber++;

        // split the current line into the 3 year/female name/male name tokens
        istringstream stream(line);
        string token;
        vector<string> tokens;
        while (stream >> token) {
            tokens.push_back(token);
        }

        // check that the 3 tokens are available
        if (tokens.size() != 3) {
            cout << tokens.size() << endl;
            cout << "Line '" << line << "' at line " << lineNumber << " is incorrect: it should contain '[Year] [FemaleName] [MaleName]'." << endl;
            return 1;
        }

        // skip header
        if (tokens[0] == "Year") {
            continue;
        }

        // parse year to an int
        int year;
        if(sscanf(tokens[0].c_str(), "%d", &year) != 1) {
            cout << "Line '" << line << "' at line " << lineNumber << " has an incorrect date." << endl;
            return 1;
        }

        // create an instance of YearlyNames and add it to the list
        YearlyNames yn;
        yn.year = year;
        yn.femaleName = tokens[1];
        yn.maleName = tokens[2];
        yearlyNamesList.push_back(yn);

        // update minYear and maxYear if necessary
        if (year < minYear) {
            minYear = year;
        }
        if (year > maxYear) {
            maxYear = year;
        }
    }
    // close input file and notify user
    inputFile.close();
    cout << "Finished loading file '" << filename << "': " << yearlyNamesList.size() << " years loaded." << endl;

    // enter the processing loop
    while (true) {
        cout << endl << endl;
        printSeparator();

        // ask the user for the gender
        Gender gender = Gender::FEMALE;
        string genderStr;
        cout << "What gender are you interested in?" << endl;
        cout << "Please choose between male or female: ";
        cin >> genderStr;
        if (genderStr == "male") {
            gender = Gender::MALE;
        } else if (genderStr != "female") {
            cout << "Wrong gender '" << genderStr << "': should be either 'male' or 'female'" << endl;
            continue;
        }

        // ask the user for the year
        string yearStr;
        cout << "What year do you want the top name for?" << endl;
        cout << "Please choose between " << minYear << " and " << maxYear << ": ";
        cin >> yearStr;
        int year;
        if(sscanf(yearStr.c_str(), "%d", &year) != 1) {
            cout << "Unable to parse year '" << yearStr << "'" << endl;
            continue;
        }
        if (year < minYear || year > maxYear) {
            cout << "Year should be between " << minYear << " and " << maxYear << ": ";
            continue;
        }

        // scan the list to find the result name
        string result;
        for (YearlyNames yn : yearlyNamesList) {
            if (year == yn.year) {
                switch(gender) {
                    case Gender::FEMALE: result = yn.femaleName; break;
                    case Gender::MALE: result = yn.maleName; break;
                }
                // loop can be exited
                break;
            }
        }

        // scan a 2nd time to count the number of times it appears in the data
        int nbTimes = 0;
        switch(gender) {
            case Gender::FEMALE:
                for (YearlyNames yn : yearlyNamesList) {
                    if (yn.femaleName == result) {
                        nbTimes++;
                    }
                }
                break;
            case Gender::MALE:
                for (YearlyNames yn : yearlyNamesList) {
                    if (yn.maleName == result) {
                        nbTimes++;
                    }
                }
                break;
        }

        // show the answer to the user
        printSeparator();
        cout << "The top " << genderStr << " name in " << year << " was '" << result << "' and appears " << nbTimes << " times" << endl;
        cout << endl << endl;

        // ask the user if he wants to run a new query
        cout << "Would you like to search for another name (Y/N)? ";
        string reply;
        cin >> reply;
        if (reply != "Y") {
            break;
        }
    }

    return 0;
}