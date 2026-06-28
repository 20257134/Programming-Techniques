/* Student Results Management System
This program lets the user add students, view results,
load from a file, save a report, and see class statistics. */
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <array>
#include <fstream>
#include <sstream>
#include <cctype>
#include <limits>
using namespace std;
// These are the grade categories, they stay the same, so I used an array.
const array<string, 4> GRADES = {"Fail", "Pass", "Merit", "Distinction"};
string trim(string text);
string titleCase(string text);
bool validScore(double score);
double getValidScore();
void addStudent(vector<string>& names, map<string, double>& scores, list<string>& log, string name, double score);
string calculateGrade(double score);
bool passStatus(double score);
void displayResults(const vector<string>& names, const map<string, double>& scores);
double recursiveTotal(const vector<string>& names, const map<string, double>& scores, int index);
void loadStudents(vector<string>& names, map<string, double>& scores, list<string>& log);
void writeReport(const vector<string>& names, const map<string, double>& scores);
void displayStatistics(const vector<string>& names, const map<string, double>& scores);
void searchStudent(const map<string, double>& scores);
void showMenu();
// This removes extra spaces before and after the text.
string trim(string text) {
int start = 0, end = static_cast<int>(text.length()) - 1;
while (start <= end && isspace(static_cast<unsigned char>(text[start]))) {
    start++;
}

while (end >= start && isspace(static_cast<unsigned char>(text[end]))) {
    end--;
}

if (start > end) {
    return "";
}

return text.substr(start, end - start + 1);
}
// This makes the student's name look neater, for example "danielle rowe" becomes "Danielle Rowe".
string titleCase(string text) {
text = trim(text);
bool newWord = true;
for (int i = 0; i < static_cast<int>(text.length()); i++) {
    if (isspace(static_cast<unsigned char>(text[i]))) {
        newWord = true;
    } else if (newWord) {
        text[i] = static_cast<char>(toupper(static_cast<unsigned char>(text[i])));
        newWord = false;
    } else {
        text[i] = static_cast<char>(tolower(static_cast<unsigned char>(text[i])));
    }
}

return text;
}
// This checks if the score is in the correct range.
bool validScore(double score) {
return score >= 0 && score <= 100;
}
// This keeps asking until the user enters a proper score.
double getValidScore() {
double score;
while (true) {
    cout << "Enter score from 0 to 100: ";
    cin >> score;

    if (cin.fail()) {
        cout << "Please enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } else if (!validScore(score)) {
        cout << "Score must be between 0 and 100.\n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } else {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return score;
    }
}
}
// This adds a student to the vector and map.
void addStudent(vector<string>& names, map<string, double>& scores, list<string>& log, string name, double score) {
name = titleCase(name);
if (name == "") {
    cout << "Name cannot be blank.\n";
    return;
}

if (!validScore(score)) {
    cout << "Invalid score. Student not added.\n";
    return;
}

if (scores.find(name) == scores.end()) {
    names.push_back(name);
}

scores[name] = score;
log.push_back("Saved " + name);

cout << "Student saved.\n";
}
// This works out the grade using if, else if, and else.
string calculateGrade(double score) {
if (score < 50) {
return GRADES[0];
} else if (score >= 50 && score < 65) {
return GRADES[1];
} else if (score >= 65 && score < 80) {
return GRADES[2];
} else {
return GRADES[3];
}
}
// This stores the pass or fail result as a bool.
bool passStatus(double score) {
bool passed = score >= 50 && score <= 100;
return passed;
}
// This displays all students and also calls calculateGrade().
void displayResults(const vector<string>& names, const map<string, double>& scores) {
if (names.empty()) {
cout << "No student records available.\n";
return;
}
cout << "\n----------- Student Results -----------\n";
cout << left << setw(20) << "Name"
     << right << setw(8) << "Score"
     << "   " << left << setw(14) << "Grade"
     << "Status\n";

cout << string(55, '-') << "\n";

for (int i = 0; i < static_cast<int>(names.size()); i++) {
    string name = names[i];
    double score = scores.at(name);

    cout << left << setw(20) << name
         << right << setw(8) << fixed << setprecision(1) << score
         << "   " << left << setw(14) << calculateGrade(score)
         << (passStatus(score) ? "Pass" : "Fail") << "\n";
}
}
// This uses recursion to add up all the scores.
double recursiveTotal(const vector<string>& names, const map<string, double>& scores, int index) {
if (index >= static_cast<int>(names.size())) {
return 0;
}
double total = scores.at(names[index]);
total += recursiveTotal(names, scores, index + 1);
return total;
}
// This loads students from students.txt.
void loadStudents(vector<string>& names, map<string, double>& scores, list<string>& log) {
ifstream file("students.txt");
if (!file) {
    cout << "students.txt was not found.\n";
    return;
}

string line, namePart, scorePart;
int loaded = 0, skipped = 0;

while (getline(file, line)) {
    stringstream ss(line);

    if (getline(ss, namePart, ',') && getline(ss, scorePart)) {
        try {
            double score = stod(trim(scorePart));

            if (validScore(score)) {
                addStudent(names, scores, log, namePart, score);
                loaded += 1;
            } else {
                skipped += 1;
            }
        } catch (...) {
            skipped += 1;
        }
    } else {
        skipped += 1;
    }
}

cout << loaded << " record(s) loaded. " << skipped << " skipped.\n";
}
// This saves the results into results_report.txt.
void writeReport(const vector<string>& names, const map<string, double>& scores) {
ofstream file("results_report.txt");
if (!file) {
    cout << "Could not create results_report.txt.\n";
    return;
}

int count = static_cast<int>(names.size());
double total = recursiveTotal(names, scores, 0);
double average = 0;

// This prevents division by zero because the second part only runs if count is not 0.
if (count != 0 && total / count >= 0) {
    average = total / count;
}

file << "STUDENT RESULTS REPORT\n";
file << string(55, '=') << "\n";

file << left << setw(20) << "Name"
     << right << setw(8) << "Score"
     << "   " << left << setw(14) << "Grade"
     << "Status\n";

file << string(55, '-') << "\n";

for (int i = 0; i < static_cast<int>(names.size()); i++) {
    string name = names[i];
    double score = scores.at(name);

    file << left << setw(20) << name
         << right << setw(8) << fixed << setprecision(1) << score
         << "   " << left << setw(14) << calculateGrade(score)
         << (passStatus(score) ? "Pass" : "Fail") << "\n";
}

file << string(55, '-') << "\n";
file << "Students: " << count << "\n";
file << "Total: " << fixed << setprecision(1) << total << "\n";
file << "Average: " << fixed << setprecision(1) << average << "\n";

cout << "Report saved to results_report.txt\n";
}
// This shows extra class information such as total, average, highest, and lowest.
void displayStatistics(const vector<string>& names, const map<string, double>& scores) {
if (names.empty()) {
cout << "No data for statistics.\n";
return;
}
int count = static_cast<int>(names.size());
int passCount = 0;

double total = recursiveTotal(names, scores, 0);
double average = 0;

string highName = names[0];
string lowName = names[0];

double highScore = scores.at(highName);
double lowScore = scores.at(lowName);

if (count != 0 && total / count >= 0) {
    average = total / count;
}

for (int i = 0; i < count; i++) {
    string name = names[i];
    double score = scores.at(name);

    if (score > highScore) {
        highScore = score;
        highName = name;
    }

    if (score < lowScore) {
        lowScore = score;
        lowName = name;
    }

    if (passStatus(score)) {
        passCount += 1;
    }
}

double passRate = (static_cast<double>(passCount) / count) * 100;

cout << "\n----------- Class Statistics -----------\n";
cout << "Students: " << count << "\n";
cout << "Total: " << fixed << setprecision(1) << total << "\n";
cout << "Average: " << average << "\n";
cout << "Highest: " << highName << " - " << highScore << "\n";
cout << "Lowest: " << lowName << " - " << lowScore << "\n";
cout << "Pass Rate: " << passRate << "%\n";
}
// This lets the user search for one student.
void searchStudent(const map<string, double>& scores) {
string name;
cout << "Enter student name: ";
getline(cin, name);

name = titleCase(name);

if (scores.find(name) != scores.end()) {
    double score = scores.at(name);

    cout << "Name: " << name << "\n";
    cout << "Score: " << fixed << setprecision(1) << score << "\n";
    cout << "Grade: " << calculateGrade(score) << "\n";
    cout << "Status: " << (passStatus(score) ? "Pass" : "Fail") << "\n";
} else {
    cout << "Student not found.\n";
}
}
// This shows the options for the user.
void showMenu() {
cout << "\n===== Student Results Management System =====";
cout << "\n1. Add Student";
cout << "\n2. Display All Results";
cout << "\n3. Search Student";
cout << "\n4. Load Students From students.txt";
cout << "\n5. Save Report";
cout << "\n6. Display Class Statistics";
cout << "\n7. Exit";
cout << "\nEnter your choice: ";
}
int main() {
// Main variables used in the program.
int choice;
double score;
char confirm;
string name;
bool running = true;
// Containers used to store the student data.
vector<string> studentNames;
map<string, double> studentScores;
list<string> activityLog;

cout << "Welcome to the Student Results Management System. Please select an option from the menu below.\n";

// This loop keeps the program running until the user exits.
while (running) {
    showMenu();
    cin >> choice;

    if (cin.fail()) {
        cout << "Invalid menu input.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        continue;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 1) {
        cout << "Enter student name: ";
        getline(cin, name);

        score = getValidScore();

        addStudent(studentNames, studentScores, activityLog, name, score);

    } else if (choice == 2) {
        displayResults(studentNames, studentScores);

    } else if (choice == 3) {
        searchStudent(studentScores);

    } else if (choice == 4) {
        loadStudents(studentNames, studentScores, activityLog);

    } else if (choice == 5) {
        writeReport(studentNames, studentScores);

    } else if (choice == 6) {
        displayStatistics(studentNames, studentScores);

    } else if (choice == 7) {
        cout << "Exit program? (Y/N): ";
        cin >> confirm;

        if (confirm == 'Y' || confirm == 'y') {
            running = false;
            cout << "Goodbye!\n";
        }

    } else {
        cout << "Invalid choice. Please choose 1 to 7.\n";
    }
}

return 0;
}

