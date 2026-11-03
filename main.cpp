
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

struct Student {
    int rollNo;
    string name;
    int age;
    string course;
    double marks;
};

const string DATA_FILE = "students.txt";

// Replace delimiter characters in input
string cleanField(string value) {
    for (char &ch : value) {
        if (ch == '|')
            ch = ' ';
    }
    return value;
}

// Load student records from file
vector<Student> loadStudents() {
    vector<Student> students;
    ifstream file(DATA_FILE);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string roll, name, age, course, marks;

        if (getline(ss, roll, '|') &&
            getline(ss, name, '|') &&
            getline(ss, age, '|') &&
            getline(ss, course, '|') &&
            getline(ss, marks)) {

            try {
                Student s;
                s.rollNo = stoi(roll);
                s.name = name;
                s.age = stoi(age);
                s.course = course;
                s.marks = stod(marks);

                students.push_back(s);
            }
            catch (...) {
                // Skip invalid records
            }
        }
    }

    return students;
}

// Save student records to file
bool saveStudents(const vector<Student>& students) {
    ofstream file(DATA_FILE);

    if (!file) {
        cout << "Error opening data file!\n";
        return false;
    }

    for (const Student& s : students) {
        file << s.rollNo << '|'
             << cleanField(s.name) << '|'
             << s.age << '|'
             << cleanField(s.course) << '|'
             << fixed << setprecision(2)
             << s.marks << '\n';
    }

    return true;
}

// Find student using roll number
int findStudentIndex(const vector<Student>& students,
                     int rollNo) {
    for (int i = 0; i < students.size(); i++) {
        if (students[i].rollNo == rollNo)
            return i;
    }

    return -1;
}

// Read integer input safely
int readInt(string prompt) {
    int value;

    while (true) {
        cout << prompt;

        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Please enter a valid number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Read marks between 0 and 100
double readMarks(string prompt) {
    double value;

    while (true) {
        cout << prompt;

        if (cin >> value && value >= 0 && value <= 100) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Enter marks between 0 and 100.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Read text input
string readText(string prompt) {
    string value;

    do {
        cout << prompt;
        getline(cin, value);

        if (value.empty())
            cout << "Field cannot be empty.\n";

    } while (value.empty());

    return cleanField(value);
}

// Display one student record
void displayStudent(const Student& s) {
    cout << left
         << setw(12) << s.rollNo
         << setw(24) << s.name
         << setw(8) << s.age
         << setw(20) << s.course
         << fixed << setprecision(2)
         << s.marks << '\n';
}

// Add a new student
void addStudent() {
    vector<Student> students = loadStudents();
    Student s;

    cout << "\n--- ADD STUDENT ---\n";

    s.rollNo = readInt("Enter Roll Number: ");

    if (findStudentIndex(students, s.rollNo) != -1) {
        cout << "Roll number already exists!\n";
        return;
    }

    s.name = readText("Enter Name: ");

    s.age = readInt("Enter Age: ");

    while (s.age <= 0) {
        cout << "Age must be greater than zero.\n";
        s.age = readInt("Enter Age: ");
    }

    s.course = readText("Enter Course/Class: ");
    s.marks = readMarks("Enter Marks (0-100): ");

    students.push_back(s);

    if (saveStudents(students))
        cout << "\nStudent added successfully!\n";
}

// Display all students
void displayStudents() {
    vector<Student> students = loadStudents();

    cout << "\n--- STUDENT RECORDS ---\n";

    if (students.empty()) {
        cout << "No student records found.\n";
        return;
    }

    cout << left
         << setw(12) << "Roll No."
         << setw(24) << "Name"
         << setw(8) << "Age"
         << setw(20) << "Course"
         << "Marks\n";

    cout << string(76, '-') << '\n';

    for (const Student& s : students) {
        displayStudent(s);
    }
}

// Search student
void searchStudent() {
    vector<Student> students = loadStudents();

    cout << "\n--- SEARCH STUDENT ---\n";

    int rollNo = readInt("Enter Roll Number: ");

    int index = findStudentIndex(students, rollNo);

    if (index == -1) {
        cout << "Student not found!\n";
        return;
    }

    cout << "\nStudent Found:\n";

    cout << left
         << setw(12) << "Roll No."
         << setw(24) << "Name"
         << setw(8) << "Age"
         << setw(20) << "Course"
         << "Marks\n";

    displayStudent(students[index]);
}

// Update student details
void updateStudent() {
    vector<Student> students = loadStudents();

    cout << "\n--- UPDATE STUDENT ---\n";

    int rollNo = readInt("Enter Roll Number: ");

    int index = findStudentIndex(students, rollNo);

    if (index == -1) {
        cout << "Student not found!\n";
        return;
    }

    cout << "\nEnter Updated Details:\n";

    students[index].name = readText("Enter Name: ");

    students[index].age = readInt("Enter Age: ");

    while (students[index].age <= 0) {
        cout << "Age must be greater than zero.\n";
        students[index].age = readInt("Enter Age: ");
    }

    students[index].course = readText("Enter Course/Class: ");

    students[index].marks = readMarks("Enter Marks (0-100): ");

    if (saveStudents(students))
        cout << "\nStudent updated successfully!\n";
}

// Delete student record
void deleteStudent() {
    vector<Student> students = loadStudents();

    cout << "\n--- DELETE STUDENT ---\n";

    int rollNo = readInt("Enter Roll Number: ");

    int index = findStudentIndex(students, rollNo);

    if (index == -1) {
        cout << "Student not found!\n";
        return;
    }

    students.erase(students.begin() + index);

    if (saveStudents(students))
        cout << "\nStudent deleted successfully!\n";
}

// Main menu
int main() {
    int choice;

    do {
        cout << "\n====================================\n";
        cout << "       STUDENT MANAGEMENT SYSTEM\n";
        cout << "====================================\n";

        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "0. Exit\n";

        cout << "------------------------------------\n";

        choice = readInt("Enter Your Choice: ");

        switch (choice) {
            case 1:
                addStudent();
                break;

            case 2:
                displayStudents();
                break;

            case 3:
                searchStudent();
                break;

            case 4:
                updateStudent();
                break;

            case 5:
                deleteStudent();
                break;

            case 0:
                cout << "\nThank you for using "
                     << "Student Management System!\n";
                break;

            default:
                cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
