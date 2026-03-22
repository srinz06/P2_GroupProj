#pragma once

#include <vector>
#include <string>
#include "StockRecord.h"

using namespace std;

class BPlusTree 
{
public:

    // max number of children for each node
    static const int ORDER = 64;

    struct Node {
        bool leaf_node_check;
        //vector for keys
        vector<string> keys;

        vector<StockRecord> records;  

        // only used in internal nodes since those juts point to the next node down
        vector<Node*> children;

        // linked list at the bottom     
        Node* next;                   

        // constructor sets leaf type and null next pointer
        Node(bool leaf) 
        {
            leaf_node_check = leaf;
            next = nullptr;
        }
    };

    BPlusTree();
    ~BPlusTree();

    
    void insert(StockRecord record);

   
    bool delete_node(const string& key);

    vector<StockRecord> range_get_keys(const string& starting_key, const string& ending_key);

    StockRecord* get_min_val();
    StockRecord* search(const string& key);

private:
    Node* root;
    // helper functions


    void destroy_tree(Node* node);

    
    int child_idx_find(Node* node, const string& key);
    Node* leaf_find(const string& key);
    Node* parent_find(Node* cursor_tmp, Node* child);
    
};