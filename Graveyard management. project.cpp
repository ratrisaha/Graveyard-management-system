
#include <iostream>
#include <fstream>
#include <string>
#include <cctype> // Include for isalpha function

using namespace std;

struct Person {
    string name;
    string birthdate;
};

// Function to check if a string contains only alphabets and spaces
bool isAlphabetAndSpace(const string& str) {
    for (char c : str) {
        if (!isalpha(c) && c != ' ') {
            return false;
        }
    }
    return true;
}

void displayGraveyardFromFile(int rows, int cols) {
    ifstream file("graveyard.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open the file.\n";
        return;
    }

    char** graveyardStatus = new char*[rows];
    for (int i = 0; i < rows; ++i) {
        graveyardStatus[i] = new char[cols];
        for (int j = 0; j < cols; ++j) {
            graveyardStatus[i][j] = ' '; // Initialize all slots as empty
        }
    }

    string name, birthdate;
    int row, col;
    while (file >> name >> birthdate >> row >> col) {
        graveyardStatus[row][col] = 'X'; // Mark the slot as filled
    }

    cout << "Graveyard Status:\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i == 0 && j == 0) {
                cout << "[^]";
            } else {
                cout << "[" << graveyardStatus[i][j] << "]";
            }
        }
        cout << endl;
    }

    // Cleanup
    for (int i = 0; i < rows; ++i) {
        delete[] graveyardStatus[i];
    }
    delete[] graveyardStatus;

    file.close();
}

void displayNumberOfSlots(int rows, int cols) {
    int totalSlots = rows * cols;
    int filledSlots = 0;
    int emptySlots = 0;

    ifstream file("graveyard.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open the file.\n";
        return;
    }

    char** graveyardStatus = new char*[rows];
    for (int i = 0; i < rows; ++i) {
        graveyardStatus[i] = new char[cols];
        for (int j = 0; j < cols; ++j) {
            graveyardStatus[i][j] = ' '; // Initialize all slots as empty
        }
    }

    string name, birthdate;
    int row, col;
    while (file >> name >> birthdate >> row >> col) {
        graveyardStatus[row][col] = 'X'; // Mark the slot as filled
        filledSlots++;
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (graveyardStatus[i][j] == ' ') {
                emptySlots++;
            }
        }
    }

    cout << "Total Slots: " << totalSlots << endl;
    cout << "Filled Slots: " << filledSlots << endl;
    cout << "Empty Slots: " << emptySlots << endl;

    // Cleanup
    for (int i = 0; i < rows; ++i) {
        delete[] graveyardStatus[i];
    }
    delete[] graveyardStatus;

    file.close();
}

void addPerson(Person** graveyard, int rows, int cols) {
    Person person;

    cout << "Enter the name: ";
    cin.ignore(); // Ignore any previous newline character
    getline(cin, person.name);

    // Validate the name (allow only alphabets and spaces)
    while (!isAlphabetAndSpace(person.name)) {
        cout << "Invalid character in the name. Please enter only alphabets and spaces.\n";
        cout << "Enter the name: ";
        getline(cin, person.name);
    }

    cout << "Enter the birthdate 'dd/mm/yyyy': ";
    cin >> person.birthdate;

    int row, col;
    do {
        cout << "Enter the row and column for the person (0 to " << rows - 1 << "): ";
        cin >> row >> col;
    } while (row < 0 || row >= rows || col < 0 || col >= cols);

    // Display entry gate symbol '^' instead of '[]'
    if (row == 0 && col == 0) {
        cout << "Cannot add a person to the entry gate slot. Please choose another slot.\n";
        return;
    }

    graveyard[row][col] = person;

    // Save data to file
    ofstream file("graveyard.txt", ios::app);
    file << person.name << " " << person.birthdate << " " << row << " " << col << endl;
    file.close();

    // Remove the first slot if it was the entry gate
    if (row == 0 && col == 0) {
        ifstream infile("graveyard.txt");
        ofstream outfile("temp.txt");

        string line;
        getline(infile, line); // Read and discard the first line (entry gate)

        while (getline(infile, line)) {
            outfile << line << endl;
        }

        infile.close();
        outfile.close();

        remove("graveyard.txt");
        rename("temp.txt", "graveyard.txt");
    }

    cout << "Person added to the graveyard.\n";
}

void findPerson(Person** graveyard, int rows, int cols) {
    string searchName, searchBirthdate;
    cout << "Enter the name to search: ";
    cin >> searchName;
    cout << "Enter the birthdate to search 'dd/mm/yyyy': ";
    cin >> searchBirthdate;

    // Load data from file
    ifstream file("graveyard.txt");
    string name, birthdate;
    int row, col;
    while (file >> name >> birthdate >> row >> col) {
        if (name == searchName && birthdate == searchBirthdate) {
            cout << "Person found at position (" << row << ", " << col << ").\n";
            file.close();
            return;
        }
    }
    file.close();

    cout << "Person not found in the graveyard.\n";
}

int main() {
    int rows, cols;

    cout << "*******************************\n";
    cout << "   GRAVEYARD MANAGEMENT\n";
    cout << "*******************************\n";

    cout << "Enter the number of rows in the graveyard: ";
    cin >> rows;
    cout << "Enter the number of columns in the graveyard: ";
    cin >> cols;

    Person** graveyard = new Person*[rows];
    for (int i = 0; i < rows; ++i) {
        graveyard[i] = new Person[cols];
    }

    char choice;
    do {
        cout << "\n1. Add a person\n";
        cout << "2. Find a person\n";
        cout << "3. Display graveyard\n";
        cout << "4. Show the number of slots\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case '1':
                addPerson(graveyard, rows, cols);
                break;
            case '2':
                findPerson(graveyard, rows, cols);
                break;
            case '3':
                displayGraveyardFromFile(rows, cols);
                break;
            case '4':
                displayNumberOfSlots(rows, cols);
                break;
            case '5':
                cout << "Exiting the program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != '5');

    // Cleanup
    for (int i = 0; i < rows; ++i) {
        delete[] graveyard[i];
    }
    delete[] graveyard;

    return 0;
}
