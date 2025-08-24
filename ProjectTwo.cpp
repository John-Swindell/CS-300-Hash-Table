#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;

constexpr unsigned int DEFAULT_SIZE = 100;
string DEFAULT_FILE = "CS 300 ABCU_Advising_Program_Input.csv";

// Course object to hold all course information
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
    Course() {
        // Initializes the member variables to be empty
        courseNumber = "";
        courseName = "";
    }
};

// Define a class containing data members and methods to implement a hash table with chaining
class HashTable {
private:
    // Internal storage within HashTable for Course objects
    struct Node {
        Course course;
        unsigned int key{};
        Node* next{};

        Node(const Course& newCourse, unsigned int newKey) {
            course = newCourse;
            key = newKey;
        }
    };

    // Vector of linked lists - HashTable storage implementation
    vector<Node*> buckets;

    // Used for default size of buckets vector if no argument is passed
    unsigned int bucketsSize = DEFAULT_SIZE;

    unsigned int currentSize = 0;

public:
    HashTable();
    explicit HashTable(unsigned int size);
    unsigned int GetSize() const;
    void Insert(const Course& newCourse);
    ~HashTable();
    void printCourseCatalog();
    void loadCourses(const string& file);
    unsigned int hash(const string& key) const;
    Course searchForCourse(const string& courseNumber) const;

};

HashTable::HashTable() {
    buckets.resize(bucketsSize);
    // Initialize each bucket to nullptr using default_size
    for (int i = 0; i < bucketsSize; ++i) {
        buckets.at(i) = nullptr;
    }
}

HashTable::HashTable(unsigned int size) {
    // Resize the vector to specified number of buckets instead of default (100)
    this->bucketsSize = size;
    buckets.resize(size);

    // Initialize each bucket to nullptr
    for (int i = 0; i < bucketsSize; ++i) {
        buckets.at(i) = nullptr;
    }
}

unsigned int HashTable::GetSize() const {
    return this->currentSize;
}

unsigned int HashTable::hash(const string& key) const {
    unsigned int total{};
    for (char c : key) {
        total += total + c;
    }
    return total % bucketsSize;
}

void HashTable::Insert(const Course& newCourse) {
    cout << "Course Added: ";
    cout << "" << newCourse.courseNumber << "" << endl;
    unsigned int key = hash(newCourse.courseNumber);
    currentSize += 1;

    Node* node = buckets.at(key);

    if (node == nullptr) {
        auto newCourseNode = new Node(newCourse, key);
        buckets.at(key) = newCourseNode;
    } else {
        while (node->next != nullptr) {
            node = node->next;
        }
        node->next = new Node(newCourse, key);
    }
}

// Destructor implementation to free allocated memory
HashTable::~HashTable() {
    for (unsigned int i = 0; i < bucketsSize; ++i) {
        Node* current = buckets.at(i);
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
}

// Takes course catalog and name as input, returns a single Course object where Course.name == courseName.
Course HashTable::searchForCourse(const string& courseNumber) const {
    // Gets the hash for the string passed; finds the bucket the course should be in
    string userCourseNumber;
    for (int i = 0; i < courseNumber.length(); ++i) {
        userCourseNumber += static_cast<char>(toupper(courseNumber.at(i)));
    }

    unsigned int key = hash(userCourseNumber);
    Node* node = buckets.at(key);

    while (node != nullptr) {
        // Convert the stored course number once
        string currCourseNumber;
        for (char c : node->course.courseNumber) {
            currCourseNumber += static_cast<char>(toupper(c));
        }
        // If found, return course immediately
        if (currCourseNumber == userCourseNumber) {
            return node->course;
        }
        node = node->next;
    }

    // Return an empty course if not found within the loop
    return Course{};
}

// Parses hash table and print all courses in alphanumeric order
void HashTable::printCourseCatalog() {
    vector<Course> courseVector{};
    for (Node* bucket : buckets) {
        while (bucket != nullptr) {
            courseVector.push_back(bucket->course);
            bucket = bucket->next;
        }
    }
    std::sort(courseVector.begin(), courseVector.end(), [](const Course& a, const Course& b) {
        return a.courseNumber < b.courseNumber;
    });
    for (const Course& course : courseVector) {
        cout << course.courseNumber << ", " << course.courseName << endl;
    }
}

// Loads all courses from a file specified by the user; returns a vector of Course objects
void HashTable::loadCourses(const string& file) {
    ifstream infile(file);

    // Return empty if the file can't be opened
    if (!infile) {
        cout << "Error opening file." << endl;
        return;
    }

    string line;
    // Uses the sstream library for lines and parsing
    while (getline(infile, line)) {
        Course course;
        stringstream ss(line);
        string prerequisite;

        // Assumes the format of the CSV is courseNumber, courseName, prerequisites...
        getline(ss, course.courseNumber, ',');
        getline(ss, course.courseName, ',');

        while (getline(ss, prerequisite, ',')) {
            if (!prerequisite.empty()) {
                course.prerequisites.push_back(prerequisite);
            }
        }
        // Inserts into the HashTable this function was called on
        Insert(course);
    }

    infile.close();
    cout << "File has been successfully loaded." << endl;
}

class Menu {
private:
    // Reference to the course catalog
    HashTable& courseCatalog;
    // Reference to the filename variable
    string& fileName;

public:
    // Constructor to initialize the menu with references to course catalog and filename
    Menu(HashTable& catalog, string& file)
        : courseCatalog(catalog), fileName(file) {}

    // Display the menu options to the user
    static void displayMenu() {
        cout << "ABCU Course Planner Menu" << endl;
        cout << "----------------------------" << endl;
        cout << "1. Load Course Data" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Find a Course" << endl;
        cout << "9. Exit" << endl;
    }

    // Handle the menu selection and perform actions based on user choice
    void handleMenuSelection() {

        int menuSelection = 0;
        while (menuSelection != 9) {
            displayMenu();
            cout << "What would you like to do?: ";
            cin >> menuSelection;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << "Please enter a valid number:" << endl;
                cin >> menuSelection; // Read the input again here
                if (cin.fail()) { // Optional: handle a second failure
                    menuSelection = 0; // Assign a default invalid option
                }
            }
            cin.ignore();

            switch (menuSelection) {
                case 1: {
                    // Load Course Data
                    cout << "Please enter the name of the file you wish to load, or type 'y' "
                         "to use the default file name - " << endl
                         << "\"CS 300 ABCU_Advising_Program_Input.csv\"" << endl;
                    cin >> fileName;
                    cin.ignore();

                    if (fileName == "y" || fileName == "Y") {
                        // Set the default file name
                        fileName = DEFAULT_FILE;
                    }

                    if (fileName.empty()) {
                        cout << "File name cannot be empty!" << endl;
                    } else {
                        cout << "Attempting to load file..." << endl;
                        // Load courses into the course catalog from the file
                        courseCatalog.loadCourses(fileName);
                    }
                    break;
                }

                case 2:
                    // Print Course List
                    if (courseCatalog.GetSize() == 0) {
                        cout << "No courses loaded yet. Please load course data first." << endl;
                    } else {
                        // Function to print all courses
                        courseCatalog.printCourseCatalog();
                    }
                    break;

                case 3: {
                    // Find a Course
                    cout << "What course do you want to know about?" << endl;
                    string courseSearch;
                    getline(cin, courseSearch);

                    // Function to search for a course
                    Course course = courseCatalog.searchForCourse(courseSearch);
                    cin.clear();

                    if (!course.courseNumber.empty()) {
                        cout << "Course Information:" << endl;
                        cout << course.courseNumber << ", " << course.courseName << endl;
                        cout << "Prerequisites: ";
                        if (course.prerequisites.empty()) {
                            cout << "None.";
                        } else {
                            for (const auto& preReq : course.prerequisites) {
                                cout << preReq << ", ";
                            }
                        }
                        cout << endl;
                    } else {
                        cout << "Course not found." << endl;
                    }
                    break;
                }

                case 9:
                    // Exit the program
                    cout << "Thank you for using the course planner!" << endl;
                    break;

                default:
                    cout << menuSelection << " is not a valid option." << endl;
                    break;
            }
        }
    }
};


int main() {
    // HashTable object to hold courses
    HashTable courseCatalog;
    // File name - set by the user or default
    string fileName;
    // Instantiate the Menu class
    Menu menu(courseCatalog, fileName);
    // Handle menu selection and perform actions
    menu.handleMenuSelection();

    return 0;
}
