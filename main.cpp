#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

struct TrieNode {
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;
    TrieNode() : isEndOfWord(false) {}
};

class Trie {
private:
    TrieNode* root;
public:
    Trie() { root = new TrieNode(); }
    void insert(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->isEndOfWord = true;
    }
    bool search(const string& word) const {
        TrieNode* node = root;
        for (char c : word) {
            auto it = node->children.find(c);
            if (it == node->children.end()) return false;
            node = it->second;
        }
        return node->isEndOfWord;
    }
    void remove(const string& word) {
        deleteHelper(root, word, 0);
    }
private:
    bool deleteHelper(TrieNode* node, const string& word, int index) {
        if (!node) return false;
        if (index == (int)word.size()) {
            if (!node->isEndOfWord) return false;
            node->isEndOfWord = false;
            return node->children.empty();
        }
        char c = word[index];
        auto it = node->children.find(c);
        if (it == node->children.end()) return false;
        TrieNode* child = it->second;
        bool shouldDelete = deleteHelper(child, word, index + 1);
        if (shouldDelete) {
            delete child;
            node->children.erase(c);
        }
        return !node->isEndOfWord && node->children.empty();
    }
};

int main() {
    Trie trie;
    trie.insert("apple");
    trie.insert("app");
    trie.insert("banana");
    cout << boolalpha;
    cout << "apple: " << trie.search("apple") << endl;
    cout << "app: " << trie.search("app") << endl;
    trie.remove("app");
    cout << "app after delete: " << trie.search("app") << endl;
    cout << "apple: " << trie.search("apple") << endl;
    return 0;
}