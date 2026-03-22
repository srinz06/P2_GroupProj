#pragma once

#include <vector>
#include <string>
#include "StockRecord.h"

using namespace std;

class MinHeap {
public:
    MinHeap();

    // do the functions for MinHeap
    // should have insert, getting min value, extracting min, searching, delete, and then a function to get a range
    void insert(StockRecord record);
    bool delete_node(const string& key);
    int size();
    bool empty_check();


    StockRecord get_min_val();
    StockRecord extract_min_val();

    StockRecord* search(const string& key);
    
    vector<StockRecord> range_get_keys(const string& starting_key, const string& ending_key);

    

private:
    vector<StockRecord> heap;
    int parent(int i) { 
        return (i - 1) / 2; 
    }

    int left_child(int i) { 
        return 2 * i + 1; 
    }

    int right_child(int i) { 
        return 2 * i + 2; 
    }

    // heapify up and dwn
    void heapify_up(int i);
    void heapify_down(int i);
};