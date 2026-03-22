#include "BPlusTree.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

BPlusTree::BPlusTree() {
    root = new Node(true);
}



BPlusTree::~BPlusTree() {
    destroy_tree(root);
}

void BPlusTree::destroy_tree(Node* node) {
    if (!node) {
        return;
    }

    // if internal node, recurrent_rtsively destroy all children first
    if (!node->leaf_node_check) {
        for (int i = 0; i < (int)node->children.size(); i++) {
            destroy_tree(node->children[i]);
        }
    }

    delete node;
}

// getting index of child

int BPlusTree::child_idx_find(Node* node, const string& key) {
    int i = 0;
    while (i < (int)node->keys.size() && key >= node->keys[i]) {
        i++;
    }
    return i;
}

// finding a leaf

BPlusTree::Node* BPlusTree::leaf_find(const string& key) {
    Node* current_rt = root;

    while (!current_rt->leaf_node_check) {
        current_rt = current_rt->children[child_idx_find(current_rt, key)];
    }
    return current_rt;
}

//search the tree 
StockRecord* BPlusTree::search(const string& key) {
    Node* leaf = leaf_find(key);

    for (int i = 0; i < (int)leaf->records.size(); i++) {
        if (leaf->records[i].key == key) {
            return &leaf->records[i];
        }
    }

    return nullptr;
}

//getting min val
StockRecord* BPlusTree::get_min_val() {
    Node* current_rt = root;

    while (!current_rt->leaf_node_check) {
        current_rt = current_rt->children[0];
    }

    if (current_rt->records.empty()) {
        return nullptr;
    }

    return &current_rt->records[0];
}