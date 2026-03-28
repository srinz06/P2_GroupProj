# Stock Exchange Explorer

**Team:** Wisp (Steven Rinzler and Mason Montgomery)

**GitHub:** https://github.com/srinz06/P2_GroupProj.git

**Video:** https://youtu.be/mS5-JDC8u2A

---

## About

This program loads trading data from 14 global stock exchanges and compares the performance of a **B+ Tree** and a **Min Heap** using different functions such as insert, search, delete, and getting a range of value operations. All data structures were implemented from scratch in C++.

---

## Requirements

- g++ (C++17)
- data.csv must be in the same folder as the source files when you run the program

---

## How to Compile and Run

**Step 1 - Compile:**
g++ -o stock main.cpp BPlusTree.cpp MinHeap.cpp -std=c++17


**Step 2 — Run:**
./stock

The program will automatically load **data.csv** and build both data structures and print the times it took to build each.

---

## Features

1. Search by Index + Date: find a specific trading day by exchange and date

2. Range Query: find all records for an exchange within a date range

3. Get Minimum Key: find the record with the smallest composite key

4. Insert a new record: add a new entry to both structures

5. Delete a record: remove an entry from both structures

6. List all stock exchanges: view all 14 exchanges with region, currency, and date range

7. Exit

Every operation runs on both structures simultaneously and then the times it took for each is printed so we can compare. Quick note is that the timing is kept in ms, but sometimes the operations would run too fast so the timing would get rounded down to 0ms.

---

## Notes

- Dates must be entered in **YYYY-MM-DD** format
- Index names are case sensitive (e.g. `NYA`, `N225`, `KS11`). Option 6 prints all the valid indexes
- Records with null values in the dataset are automatically skipped during loading