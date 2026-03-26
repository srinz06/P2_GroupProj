#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include <vector>
#include <chrono>

#include <limits>
#include <map>
#include <tuple>

#include "StockRecord.h"
#include "BPlusTree.h"
#include "MinHeap.h"

using namespace std;
using namespace chrono;

// loading the CSV data
// added a thing to skip null lines

vector<StockRecord> CSV_load(const string& filename) {
    vector<StockRecord> records;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open " << filename << endl;
        return records;
    }

    string line;

    getline(file, line);

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        string token;
        StockRecord r;


        // found an example online for how to do this type of parsing
        try {
            getline(ss, r.index, ',');
            getline(ss, r.date, ',');
            getline(ss, token, ','); r.open = stod(token);
            getline(ss, token, ','); r.high = stod(token);
            getline(ss, token, ','); r.low = stod(token);
            getline(ss, token, ','); r.close = stod(token);
            getline(ss, token, ','); r.adjClose = stod(token);
            getline(ss, token, ','); r.volume = stoll(token);

            // full key  is = index + "_" + date because entries are "NYA_2020-01-01"
            r.key = r.index + "_" + r.date;
            records.push_back(r);
        } 

        //also found online how this can catch null entries
        catch (...) {}
    }

    file.close();
    return records;
}

// printing the record
void print_record(const StockRecord& r) {
    cout << fixed << setprecision(2);
    cout << "  Index    : " << r.index << endl;
    cout << "  Date     : " << r.date << endl;
    cout << "  Open     : " << r.open << endl;
    cout << "  High     : " << r.high << endl;
    cout << "  Low      : " << r.low << endl;
    cout << "  Close    : " << r.close << endl;
    cout << "  Adj Close: " << r.adjClose << endl;
    cout << "  Volume   : " << r.volume << endl;
}

// getting the timing for the operations

void print_times(const string& operation, double bplus_time, const string& bplus_result, double heap_time, const string& heap_result) 
{
    cout << "\n--- " << operation << " ---" << endl;
    cout << left << setw(20) << "Structure" << setw(20) << "Time (ms)" << "Result" << endl;
    cout << string(60, '-') << endl;
    cout << left << setw(20) << "B+ Tree" << setw(20) << fixed << setprecision(4) << bplus_time << bplus_result << endl;
    cout << left << setw(20) << "Min Heap" << setw(20) << fixed << setprecision(4) << heap_time << heap_result << endl;
}


int main() {
    cout << "========================================" << endl;
    cout << "         Stock Exchange Explorer        " << endl;
    cout << "========================================" << endl;

    // added a loading msg just to make it look cool
    cout << "Loading data.csv..." << endl;

    vector<StockRecord> records = CSV_load("data.csv");

    // give a message if we didn't load records
    if (records.empty()) {
        cerr << "No records loaded. Exiting." << endl;
        return 1;
    }

    cout << records.size() << " records loaded." << endl;

    // Making B+ tree
    cout << "\nBuilding B+ Tree..." << endl;
    BPlusTree bpt;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < (int)records.size(); i++) {
        bpt.insert(records[i]);
    }

    auto end = high_resolution_clock::now();

    // msg for time for building in ms
    cout << "  B+ Tree built in " << fixed << setprecision(2) << duration<double, milli>(end - start).count() << " ms" << endl;




    // making min heap 
    cout << "Building Min Heap..." << endl;
    MinHeap heap;

    start = high_resolution_clock::now();

    for (int i = 0; i < (int)records.size(); i++) {
        heap.insert(records[i]);
    }
    end = high_resolution_clock::now();

    cout << "  Min Heap built in " << fixed << setprecision(2) << duration<double, milli>(end - start).count() << " ms" << endl;




    // earliest and latest date for each index
    map<string, pair<string,string>> indexInfo;

    for (int i = 0; i < (int)records.size(); i++) {
        string idx = records[i].index;
        string date = records[i].date;

        if (indexInfo.find(idx) == indexInfo.end()) {
            indexInfo[idx] = {date, date};
        } 

        else {
            if (date < indexInfo[idx].first) {
                indexInfo[idx].first = date;
            }

            if (date > indexInfo[idx].second) {
                indexInfo[idx].second = date;
            }
        }
    }

    // Print a list of all the possible stock exchange that are in the CRV
    map<string, tuple<string,string,string>> ex_list;
    ex_list["NYA"] = make_tuple("United States", "New York Stock Exchange", "USD");
    ex_list["IXIC"] = make_tuple("United States", "NASDAQ", "USD");
    ex_list["HSI"] = make_tuple("Hong Kong", "Hong Kong Stock Exchange", "HKD");
    ex_list["000001.SS"] = make_tuple("China", "Shanghai Stock Exchange", "CNY");
    ex_list["399001.SZ"] = make_tuple("China", "Shenzhen Stock Exchange", "CNY");
    ex_list["N225"] = make_tuple("Japan", "Tokyo Stock Exchange", "JPY");
    ex_list["N100"] = make_tuple("Europe", "Euronext", "EUR");
    ex_list["GSPTSE"] = make_tuple("Canada", "Toronto Stock Exchange", "CAD");
    ex_list["NSEI"] = make_tuple("India", "National Stock Exchange", "INR");
    ex_list["GDAXI"] = make_tuple("Germany", "Frankfurt Stock Exchange", "EUR");
    ex_list["KS11"] = make_tuple("Korea", "Korea Exchange", "KRW");
    ex_list["SSMI"] = make_tuple("Switzerland", "SIX Swiss Exchange", "CHF");
    ex_list["TWII"] = make_tuple("Taiwan", "Taiwan Stock Exchange", "TWD");
    ex_list["J203.JO"] = make_tuple("South Africa", "Johannesburg Stock Exch.", "ZAR");

    
    // menu options
    int choice = 0;

    while (choice != 7) 
    {
        cout << "\n========================================" << endl;
        cout << "  1. Search by Index & Date" << endl;
        cout << "  2. Range of Entries" << endl;
        cout << "  3. Get Minimum (earliest key, not date)" << endl;
        cout << "  4. Insert a new record" << endl;
        cout << "  5. Delete a record" << endl;
        cout << "  6. List all stock exchanges" << endl;
        cout << "  7. Exit" << endl;
        cout << "========================================" << endl;
        cout << "Choose an option: ";
        cin >> choice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // option 1 behavior
        if (choice == 1) {
            string index, date, key;
            cout << "Enter index (i.e NYA): ";
            getline(cin, index);
            cout << "Enter date (YYYY-MM-DD): ";
            getline(cin, date);
            key = index + "_" + date;

            start = high_resolution_clock::now();
            StockRecord* bpt_result = bpt.search(key);
            end = high_resolution_clock::now();
            double b_plus_time = duration<double, milli>(end - start).count();

            start = high_resolution_clock::now();
            StockRecord* heapRes = heap.search(key);
            end = high_resolution_clock::now();
            double heap_time = duration<double, milli>(end - start).count();

            string bpt_status;
            string heap_status;

            if (bpt_result) {
                bpt_status = "Found";
            } 
            
            else {
                bpt_status = "Not found";
            }

            if (heapRes) {
                heap_status = "Found";
            } 
            
            else {
                heap_status = "Not found";
            }

            print_times("Search: " + key, b_plus_time, bpt_status, heap_time, heap_status);

            if (bpt_result) {
                cout << "\nRecord details:" << endl;
                print_record(*bpt_result);
            }
        }

        // continue with reset of choices




    }


    return 0;
}