#include "MinHeap.h"
#include <stdexcept>
#include <vector>
#include <string>

using namespace std;

MinHeap::MinHeap() {}

// heapify_up w/ swap
void MinHeap::heapify_up(int i) {
    while (i > 0 && heap[i] < heap[parent(i)]) {
        swap(heap[i], heap[parent(i)]);
        i = parent(i);
    }
}


// heapify_down 
void MinHeap::heapify_down(int i) {
    int smallest = i;
    int left = left_child(i);
    int right = right_child(i);
    int n = heap.size();

    if (left < n && heap[left] < heap[smallest]) {
        smallest = left;
    }

    if (right < n && heap[right] < heap[smallest]) {
        smallest = right;
    }

    if (smallest != i) {
        swap(heap[i], heap[smallest]);
        heapify_down(smallest);
    }
}

// insert logic

void MinHeap::insert(StockRecord record) {
    // just push it to the end and then we just keep swapping up
    heap.push_back(record);
    heapify_up(heap.size() - 1);
}

// getting the min value

StockRecord MinHeap::get_min_val() {
    if (empty_check()) {
        // throw an error if its empty. just a catch but it shouldn't really occur
        throw runtime_error("Heap is empty.");
    }

    return heap[0];
}

//extracting the min value

StockRecord MinHeap::extract_min_val() {
    if (empty_check()) {
        // same error if the heap is emtpty
        throw runtime_error("Heap is empty.");
    }

    StockRecord min_val_store = heap[0];

    heap[0] = heap.back();
    heap.pop_back();

    if (!empty_check()) {
        heapify_down(0);
    }

    return min_val_store;
}

// search the heap

StockRecord* MinHeap::search(const string& key) {
    for (int i = 0; i < (int)heap.size(); i++) {
        if (heap[i].key == key) {

            return &heap[i];
        }
    }

    return nullptr;
}

// del node

bool MinHeap::delete_node(const string& key) {
    // make a default value and then we can start deleting
    int index = -1;
    
    for (int i = 0; i < (int)heap.size(); i++) {
        if (heap[i].key == key) {
            // set the index to the found index, but if it's not it will got to the default -1
            index = i;
            break;
        }
    }

    //handle the -1 default value if its not found
    if (index == -1) {
        return false;
    }

    // replace the index with last element and we can delete it
    heap[index] = heap.back();
    heap.pop_back();


    // jjust reuse the heapify ups and downs to fix it back to normal
    if (index < (int)heap.size()) {
        heapify_up(index);
        heapify_down(index);
    }

    return true;
}

//getting a range of values

vector<StockRecord> MinHeap::range_get_keys(const string& starting_key, const string& ending_key) {
    vector<StockRecord> results;

    for (int i = 0; i < (int)heap.size(); i++) {
        if (heap[i].key >= starting_key && heap[i].key <= ending_key) {
            results.push_back(heap[i]);
        }
    }

    return results;
}

//call function to get size
int MinHeap::size() {
    return heap.size();
}

// check if it's empty
bool MinHeap::empty_check() {
    return heap.empty();
}