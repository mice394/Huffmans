#include <vector>    
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

struct Node;
typedef Node* node_pointer;
struct Node{
    node_pointer left;
    node_pointer right;

    int depth = 0;
    int pos = 0;
    unsigned char c;
    bool isInternal;
    string encode = "";
    
    Node() {}
    Node(unsigned char c, int p, int d, bool isInt = false, node_pointer l = nullptr, node_pointer r = nullptr) : 
    c(c), pos(p), depth(d), isInternal(isInt), left(l), right(r) {}
};

void swap(vector<node_pointer>& maxHeap, int idx1, int idx2) {
    auto temp = maxHeap.at(idx1);
    maxHeap.at(idx1) = maxHeap.at(idx2);
    maxHeap.at(idx2) = temp;
}

node_pointer deleteMax(vector<node_pointer>& maxHeap) {
    auto temp = maxHeap.at(0);
    maxHeap.erase(maxHeap.begin()); //remove first element in vector
    return temp;
}

bool shouldSwap(node_pointer& v1, node_pointer& v2) {
    if (v1->depth > v2->depth) {
        return false;
    } else if (v1->depth < v2->depth) {
        return true;
    } else { //depth are equal
        if (v1->pos < v2->pos) {
            return false;
        } else {
            return true;
        }
    }
}

void sort(vector<node_pointer>& maxHeap, int node) {
    int n = maxHeap.size();
    for (int i = 1; i < n; ++i) {
        if (shouldSwap(maxHeap.at(node), maxHeap.at(i))) {
            swap(maxHeap, node, i);
            node = i;
        }
    }
}

void createTrie(vector<node_pointer>& maxHeap) {
    while (maxHeap.empty() == false) {
        node_pointer v1 = deleteMax(maxHeap);
        if (maxHeap.empty()) { 
            maxHeap.push_back(v1);
            break;
        }
        node_pointer v2 = deleteMax(maxHeap);
        node_pointer combinedV = new Node(-1, v1->pos, v1->depth-1, true, v1, v2);
        maxHeap.insert(maxHeap.begin(), combinedV);
        sort(maxHeap, 0);
    }
}

void heapify(vector<node_pointer>& maxHeap) {
    int n = maxHeap.size();
    for (int i = 0; i < n-1; ++i) { 
        for (int j = 0; j < n-i-1; ++j) {
            if (shouldSwap(maxHeap.at(j), maxHeap.at(j+1))) swap(maxHeap, j, j+1);
        }
    }
}

void createMaxHeap(vector<node_pointer>& maxHeap, string text) {
    int i = 0;
    int pos = 0;
    int n = text.size();

    for(int i = 0; i < n; ++i) { //this ++i will deal with the space
        unsigned char c = text[i];
        ++i;
        string depthStr = "";
        while (text[i] != ' ') {
            depthStr += text[i];
            ++i;
        }
        int depth = stoi(depthStr);
        node_pointer node = new Node(c, pos, depth);
        maxHeap.push_back(node);
        ++pos;
    }
}

void decode(node_pointer& trie, string text) {
    int n = text.size();
    node_pointer root = trie;
    node_pointer currNode = trie;
    for (int i = 0; i < n; ++i){
        if (text[i] == '0') {
            currNode = currNode->left;
        } else { //text[i] == '1'
            currNode = currNode->right;
        }
        if (currNode->isInternal == false) {
            cout << currNode->c;
            currNode = root;
        }
    }
}

int main(){ 
    string tree, code;
    getline(cin, tree);
    getline(cin, code);

    vector<node_pointer> maxHeap;

    //create heap
    createMaxHeap(maxHeap, tree);
    heapify(maxHeap);

    //create trie
    createTrie(maxHeap);

    //print everything
    decode(maxHeap.at(0), code);
    cout << endl;
}

