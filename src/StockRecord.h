#pragma once

#include <string>

using namespace std;

// make a struct for StockRecord since we want it public by default
struct StockRecord {
    string key;
    string index;
    string date;
    double open;
    double high;
    double low;
    double close;
    double adjClose;
    long long volume;

    // take each of the columns in the csv file and make them a variable
    StockRecord() {
        open = 0;
        high = 0;
        low = 0;
        close = 0;
        adjClose = 0;
        volume = 0;
    }

    // add in compare ops for <, >, =, and whatever else we need and have them return bool
    bool operator<(const StockRecord& other) const { 
        return (key < other.key); 
    }
    
    bool operator>(const StockRecord& other) const { 
        return (key > other.key); 
    }

    bool operator==(const StockRecord& other) const { 
        return (key == other.key); 
    }

    bool operator<=(const StockRecord& other) const { 
        return (key <= other.key); 
    }

    bool operator>=(const StockRecord& other) const { 
        return (key >= other.key);
    }

};