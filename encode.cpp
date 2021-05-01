#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

struct Node;
typedef Node* node_pointer;
struct Node{
    node_pointer left;
    node_pointer right;

    int freq = 0;
    int depth = 0;
    unsigned char c;
    bool isInternal;
    string encode = "";
    
    Node() {}
    Node(unsigned char c, int f, int d = 0, bool isInt = false, node_pointer l = nullptr, node_pointer r = nullptr) : 
    c(c), freq(f), depth(d), isInternal(isInt), left(l), right(r) {}
};

int find(unsigned char c, vector<node_pointer>& minHeap) {
    int idx = 0;
    for (auto &it: minHeap) {
        if (it->c == c) return idx;
        ++idx;
    }
    return -1; // can't find it
}

int getLeftChildIdx(int idx) {
    return 2*idx + 1;
}

int getRightChildIdx(int idx) {
    return 2*idx + 2;
}

// return -1 if no parent (aka. it's a root)
int getParentIdx(int idx) { //c++ division always round down
    return ((idx - 1)/2 >= 0) ? (idx - 1)/2 : -1; 
}

void swap(vector<node_pointer>& minHeap, int idx1, int idx2) {
    auto temp = minHeap.at(idx1);
    minHeap.at(idx1) = minHeap.at(idx2);
    minHeap.at(idx2) = temp;
}

void fixUp(vector<node_pointer>& minHeap, int idx) {
    if (idx != 0) {
        int parentIdx = getParentIdx(idx);
        int parentFreq = minHeap.at(parentIdx)->freq;
        int currFreq = minHeap.at(idx)->freq;
        if (parentFreq > currFreq) {
            swap(minHeap, parentIdx, idx);
            fixUp(minHeap, parentIdx);
        }
    }
}

void fixDown(vector<node_pointer>& minHeap, int idx) {
    int leftChildIdx = getLeftChildIdx(idx);
    int rightChildIdx = getRightChildIdx(idx);
    int n = minHeap.size() - 1;
    int minIdx;
    if (rightChildIdx >= n) {
        if (leftChildIdx >= n) return;
        minIdx = leftChildIdx;
    } else {
        minIdx = (minHeap.at(leftChildIdx)->freq <= minHeap.at(rightChildIdx)->freq) ? leftChildIdx : rightChildIdx;
    }
    if (minHeap.at(idx)->freq > minHeap.at(minIdx)->freq) {
        swap(minHeap, minIdx, idx);
        fixDown(minHeap, minIdx);
    }
}

// insert each char into heap once; no duplicates
void createHeap(string text, vector<node_pointer>& minHeap) {
    for (auto &it: text) {
        int isExist = find(it, minHeap);
        unsigned char c = it;
        //if node with this c does not exist, make new node
        if (isExist < 0) {
            node_pointer node = new Node(c, 1);
            minHeap.push_back(node);
        } else { //freq += 1 for that node
            ++minHeap.at(isExist)->freq;
        }
    }
}

// make the minHeap an actual min heap (aka. reorder it)
void heapify(vector<node_pointer>& minHeap) {
    vector<node_pointer> newMinHeap;
    while (minHeap.empty() == false) {
        newMinHeap.push_back(minHeap.back());
        minHeap.pop_back();
        fixUp(newMinHeap, newMinHeap.size() - 1);
    }
    minHeap = newMinHeap;
}

node_pointer deleteMin(vector<node_pointer>& minHeap) {
    node_pointer root = minHeap.at(0);
    swap(minHeap, 0, minHeap.size()-1); //swap minimum with last element in array
    minHeap.pop_back(); //pop the minimum
    fixDown(minHeap, 0);
    return root;
}

void createTrie(vector<node_pointer>& minHeap) {
    while (minHeap.empty() == false) {
        node_pointer T1 = deleteMin(minHeap);
        if (minHeap.empty()) { 
            minHeap.push_back(T1);
            break;
        }
        fixUp(minHeap, minHeap.size() -1);
        node_pointer T2 = deleteMin(minHeap);
        int combinedFreq = T1->freq + T2->freq;

        node_pointer combinedT = new Node(-1, combinedFreq, true); //c = -1 because it holds nothing
        combinedT->left = T1;
        combinedT->right = T2;
        minHeap.push_back(combinedT); //insert new node
        fixUp(minHeap, minHeap.size() - 1);
    }
}

// make it reverse-alphabetical order so that we can pop_back() when we insert in alphabetical order
void reverseAlpha(vector<node_pointer>& minHeap) {
    int n = minHeap.size() - 1;
    for (int i = n; i >= 0; --i) {
        for (int j = n; j > n - i; --j) {
            if (static_cast<int>(minHeap.at(j)->c) > static_cast<int>(minHeap.at(j-1)->c)) {
                swap(minHeap, j, j-1);
            }
        }
    }
}

void fixDepthAndEncode(node_pointer& trie, int level, string encodedText) {
    if (trie == nullptr) return;

    trie->depth = level;
    if (trie->isInternal == false) {
        trie->encode = encodedText;
    }
    fixDepthAndEncode(trie->left, level + 1, encodedText + "0");
    fixDepthAndEncode(trie->right, level + 1, encodedText + "1");
}

void printCharAndDepth(node_pointer& trie) {
    if (trie == nullptr) return;
    auto leftT = trie->left;
    auto rightT = trie->right;
    if (leftT == nullptr && rightT == nullptr) {
        cout << trie->c << trie->depth << " ";
        return;
    }
    if (leftT != nullptr) printCharAndDepth(leftT);
    if (rightT != nullptr) printCharAndDepth(rightT);
}

void printEncodedText(vector<node_pointer>& map, string text) {
    for (auto &it : text) {
        int charIdx = find(it, map);
        cout << map.at(charIdx)->encode;
    }
}

int main(){ 
    string text;
    getline(cin, text);

    vector<node_pointer> minHeap;
    vector<node_pointer> map;

    //initialize heap
    createHeap(text, minHeap);
    map = minHeap;
    reverseAlpha(minHeap);
    heapify(minHeap);

    //make the trie
    createTrie(minHeap);
    fixDepthAndEncode(minHeap.at(0), 0, "");

    //print everything
    printCharAndDepth(minHeap.at(0));
    cout << endl;
    printEncodedText(map, text);
}


