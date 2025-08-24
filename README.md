# CS-300: Data Structures and Algorithms Portfolio

This repository contains my portfolio work for CS-300, showcasing my ability to analyze, implement, and utilize fundamental data structures and algorithms in C++.

## Project Components

* **Project One: Data Structure Analysis**: An analysis of the run-time and memory usage characteristics of Vector, Linked List, and Hash Table data structures.
    * `CS 300 Pseudocode + Runtime Analysis Document`
* **Project Two: Course Planning Application**: The complete C++ source code for an application that loads university course data into a hash table and allows for searching and sorted printing.
    * `ProjectTwo.cpp`

---

## Reflection

### What was the problem you were solving in the projects for this course?

The core problem was to design a system for a university to efficiently manage and access its course catalog data. This involved reading data from a CSV file and storing it in a way that allowed for two primary use cases:
1.  **Fast Lookup:** Instantly finding a specific course and its information, including any prerequisites.
2.  **Ordered Reporting:** Printing a complete list of all courses, sorted alphanumerically by course number.

This required selecting and implementing data structures that were optimized for these distinct tasks.

### How did you approach the problem? Consider why data structures are important to understand.

Understanding data structures was critical to solving this problem efficiently. A simple approach, like storing everything in a single vector, would have made searching for a course very slow, requiring an O(N) linear scan of every item.

My approach was to use the right tool for the job:

1.  **Hash Table for Fast Lookup:** I implemented a chaining hash table from scratch to store the course data. This provided an average search complexity of O(1), making course lookups nearly instantaneous, which is essential for an interactive application.

2.  **Vector for Sorting:** Since a hash table does not maintain an ordered sequence, my solution for printing the course list involved first extracting all course objects from the hash table into a temporary vector. I then used C++'s built-in sorting algorithms on the vector to produce the alphanumerically sorted list before printing it.

This project was a practical demonstration that the choice of data structure has a profound impact on an application's performance and capabilities.

### How did you overcome any roadblocks you encountered while going through the activities or project?

I encountered several significant roadblocks, both technical and environmental, which became valuable learning experiences.

* **Technical Roadblocks:** I worked through numerous classic C++ challenges, including robustly handling user input (mixing `cin` and `getline`), debugging complex parsing logic for the CSV file (correctly handling empty prerequisites), and implementing a case-insensitive search feature without modifying the original data.

* **Environmental Roadblocks:** The most significant challenge was a critical build failure that occurred when the project was graded. After a thorough investigation, I determined this was caused by an incompatibility between the C++20 features (like `std::ranges`) used during development and the older C++17 compiler environment used for grading. This was a powerful, real-world lesson in the importance of **environmental parity**. It taught me that writing logically correct code is only half the battle; ensuring it is portable and compiles correctly on the target system is just as crucial. Overcoming this required professional communication and a deep dive into compiler settings and standards to diagnose the issue.

### How has your work on this project expanded your approach to designing software and developing programs?

This project fundamentally expanded my approach to software design by moving me beyond just writing code that works to architecting systems that are clean and efficient. The key lesson was the principle of **Composition over Inheritance**.

Initially, my `Menu` class for the user interface inherited from the `HashTable` class. I refactored this so the `Menu` class *contains* a `HashTable` object. This "has-a" relationship is a much better design, as it clearly separates the user interface logic from the data structure logic, making the program easier to understand, maintain, and test.

### How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?

* **Maintainable:** I learned the critical importance of proper memory management. My `HashTable` class allocates memory dynamically for nodes, so I implemented a destructor (`~HashTable`) to meticulously free all allocated memory, preventing memory leaks and ensuring the program is stable and maintainable.

* **Readable:** The refactoring process of separating the `Menu` and `HashTable` classes, and cleaning up the data loading functions to be direct and self-contained, significantly improved the code's readability. Future developers (including my future self) can now easily understand the distinct responsibilities of each part of the application.

* **Adaptable:** The compiler version issue was a direct lesson in writing adaptable code. By replacing a C++20-specific feature (`ranges::sort`) with a more universally supported C++17 equivalent (`std::sort` with iterators), I made the codebase more portable and adaptable to different build environments, which is a critical consideration for any professional software project.
