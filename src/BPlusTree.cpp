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
//insert
void BPlusTree::insert(StockRecord record) {
    const string& key = record.key;

    // traverse down to the leaf we want and keep track of the path 
    vector<Node*> path;
    vector<int> pathIndex;
    Node* current_rt = root;

    while (!current_rt->leaf_node_check) {
        int i = child_idx_find(current_rt, key);
        path.push_back(current_rt);
        pathIndex.push_back(i);
        current_rt = current_rt->children[i];
    }



    // insert into the leaf in sorted order
    int temp_pos = 0;
    while (temp_pos < (int)current_rt->records.size() && current_rt->records[temp_pos].key < key) {
        temp_pos++;
    }


    current_rt->records.insert(current_rt->records.begin() + temp_pos, record);
    current_rt->keys.insert(current_rt->keys.begin() + temp_pos, key);

    // check if it overflows
    if ((int)current_rt->keys.size() < ORDER) {
        return;
    }

    // split the leaf if overflow
    int mid = ORDER / 2;
    Node* right_leaf = new Node(true);

    right_leaf->records.assign(current_rt->records.begin() + mid, current_rt->records.end());
    right_leaf->keys.assign(current_rt->keys.begin() + mid, current_rt->keys.end());
    current_rt->records.resize(mid);
    current_rt->keys.resize(mid);


    // keep the linked list at the bottom
    right_leaf->next = current_rt->next;
    current_rt->next = right_leaf;

    // the key we push up to the parent is the first key of the right leaf
    string node_mv_up = right_leaf->keys[0];
    Node* left_node = current_rt;
    Node* right_node = right_leaf;

    // split up
    while (!path.empty()) {
        Node* parent = path.back();
        path.pop_back();
        int idx = pathIndex.back();
        pathIndex.pop_back();

        // insert the pushed key
        parent->keys.insert(parent->keys.begin() + idx, node_mv_up);
        parent->children.insert(parent->children.begin() + idx + 1, right_node);

        // if no overflow then just return
        if ((int)parent->keys.size() < ORDER) {
            return;
        }

        // split the internal node
        int split_temp = ORDER / 2;
        node_mv_up = parent->keys[split_temp];
        Node* right_internal = new Node(false);

        right_internal->keys.assign(parent->keys.begin() + split_temp + 1, parent->keys.end());
        right_internal->children.assign(parent->children.begin() + split_temp + 1, parent->children.end());
        parent->keys.resize(split_temp);
        parent->children.resize(split_temp + 1);

        left_node = parent;
        right_node = right_internal;
    }

    // make a new root if it keeps splitting this far up 
    Node* root_new = new Node(false);
    root_new->keys.push_back(node_mv_up);
    root_new->children.push_back(left_node);
    root_new->children.push_back(right_node);
    root = root_new;
}

// getting the range
vector<StockRecord> BPlusTree::range_get_keys(const string& starting_key, const string& ending_key) {
    vector<StockRecord> results;

    // get the starting key leaf
    Node* leaf = leaf_find(starting_key);

    while (leaf) {
        for (int i = 0; i < (int)leaf->records.size(); i++) {
            const string& k = leaf->records[i].key;

            // stop wehn at end key
            if (k > ending_key) {
                return results;
            }

            // collect if inside range
            if (k >= starting_key) {
                results.push_back(leaf->records[i]);
            }
        }
        leaf = leaf->next;
    }

    return results;
}

//del node
bool BPlusTree::delete_node(const string& key) {
    // go to leaf tracking path
    vector<Node*> path;
    vector<int> pathIndex;
    Node* current_rt = root;

    while (!current_rt->leaf_node_check) {
        int i = child_idx_find(current_rt, key);
        path.push_back(current_rt);
        pathIndex.push_back(i);
        current_rt = current_rt->children[i];
    }

    // find/remove the data leaf
    // same -1 handling from minheap
    int temp_pos = -1;
    for (int i = 0; i < (int)current_rt->records.size(); i++) {
        if (current_rt->records[i].key == key) {
            temp_pos = i;
            break;
        }
    }

    //if -1 then key isnt found
    if (temp_pos == -1) {
        return false;
    }



    current_rt->records.erase(current_rt->records.begin() + temp_pos);
    current_rt->keys.erase(current_rt->keys.begin() + temp_pos);

    // finish if theres no root or no overflow
    if (current_rt == root || (int)current_rt->keys.size() >= (ORDER - 1) / 2) {
        return true;
    }

    int min_keys = (ORDER - 1) / 2;

    // fix underflow w/ traversing back up
    while (!path.empty()) {
        Node* parent = path.back();
        path.pop_back();
        int idx = pathIndex.back();
        pathIndex.pop_back();

        // cehck underflow again
        if ((int)current_rt->keys.size() >= min_keys) {
            return true;
        }

        //take a value from the left side sibling node
        if (idx > 0) {
            Node* left_siblng = parent->children[idx - 1];

            if ((int)left_siblng->keys.size() > min_keys) {
                if (current_rt->leaf_node_check) {
                    current_rt->records.insert(current_rt->records.begin(), left_siblng->records.back());
                    current_rt->keys.insert(current_rt->keys.begin(), left_siblng->keys.back());
                    left_siblng->records.pop_back();
                    left_siblng->keys.pop_back();
                    parent->keys[idx - 1] = current_rt->keys[0];
                } 
                else {
                    current_rt->keys.insert(current_rt->keys.begin(), parent->keys[idx - 1]);
                    current_rt->children.insert(current_rt->children.begin(), left_siblng->children.back());
                    parent->keys[idx - 1] = left_siblng->keys.back();
                    left_siblng->keys.pop_back();
                    left_siblng->children.pop_back();
                }


                return true;
            }
        }

        //now try from right sib
        if (idx < (int)parent->children.size() - 1) 
        {
            Node* right_siblng = parent->children[idx + 1];

            if ((int)right_siblng->keys.size() > min_keys) {
                if (current_rt->leaf_node_check) {
                    current_rt->records.push_back(right_siblng->records.front());
                    current_rt->keys.push_back(right_siblng->keys.front());
                    right_siblng->records.erase(right_siblng->records.begin());
                    right_siblng->keys.erase(right_siblng->keys.begin());
                    parent->keys[idx] = right_siblng->keys[0];
                } 
                else {
                    current_rt->keys.push_back(parent->keys[idx]);
                    current_rt->children.push_back(right_siblng->children.front());
                    parent->keys[idx] = right_siblng->keys.front();
                    right_siblng->keys.erase(right_siblng->keys.begin());
                    right_siblng->children.erase(right_siblng->children.begin());
                }

                return true;
            }
        }

        //merge w/ left sibling
        if (idx > 0) {
            Node* left_siblng = parent->children[idx - 1];

            if (current_rt->leaf_node_check) {
                for (int i = 0; i < (int)current_rt->records.size(); i++) {
                    left_siblng->records.push_back(current_rt->records[i]);
                }


                for (int i = 0; i < (int)current_rt->keys.size(); i++) {
                    left_siblng->keys.push_back(current_rt->keys[i]);
                }

                left_siblng->next = current_rt->next;
            } 

            else {
                left_siblng->keys.push_back(parent->keys[idx - 1]);
                for (int i = 0; i < (int)current_rt->keys.size(); i++) {
                    left_siblng->keys.push_back(current_rt->keys[i]);
                }


                for (int i = 0; i < (int)current_rt->children.size(); i++) {
                    left_siblng->children.push_back(current_rt->children[i]);
                }
            }


            parent->keys.erase(parent->keys.begin() + idx - 1);
            parent->children.erase(parent->children.begin() + idx);
            delete current_rt;
            current_rt = parent;

            if (current_rt == root && current_rt->keys.empty() && !current_rt->children.empty()) {
                root = current_rt->children[0];
                delete current_rt;
                return true;
            }

            // use continue to keep the loop going
            continue;
        }

        //merging with right sib
        if (idx < (int)parent->children.size() - 1) {
            Node* right_siblng = parent->children[idx + 1];

            if (current_rt->leaf_node_check) {
                for (int i = 0; i < (int)right_siblng->records.size(); i++) {
                    current_rt->records.push_back(right_siblng->records[i]);
                }

                for (int i = 0; i < (int)right_siblng->keys.size(); i++) {
                    current_rt->keys.push_back(right_siblng->keys[i]);
                }


                current_rt->next = right_siblng->next;
            } 


            else {
                current_rt->keys.push_back(parent->keys[idx]);

                for (int i = 0; i < (int)right_siblng->keys.size(); i++) {
                    current_rt->keys.push_back(right_siblng->keys[i]);
                }

                
                for (int i = 0; i < (int)right_siblng->children.size(); i++) {
                    current_rt->children.push_back(right_siblng->children[i]);
                }
            }

            parent->keys.erase(parent->keys.begin() + idx);
            parent->children.erase(parent->children.begin() + idx + 1);
            delete right_siblng;
            current_rt = parent;



            if (current_rt == root && current_rt->keys.empty() && !current_rt->children.empty()) {
                root = current_rt->children[0];
                delete current_rt;
                return true;
            }


            continue;
        }
        break;
    }

    return true;
}

// fidning a parent node

BPlusTree::Node* BPlusTree::parent_find(Node* cursor_tmp, Node* child) {
    if (cursor_tmp->leaf_node_check) {
        return nullptr;
    }


    
    for (int i = 0; i < (int)cursor_tmp->children.size(); i++) {
        if (cursor_tmp->children[i] == child) {
            return cursor_tmp;
        }

        Node* res = parent_find(cursor_tmp->children[i], child);

        if (res) {
            return res;
        }
    }

    return nullptr;
}
