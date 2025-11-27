#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
using namespace std;
#include <sstream>

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
    vector<string> autocomplete(const string& prefix);
    pair<bool, vector<string>> spellCheck(const string& word);
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
    void collectWords(TrieNode* node, const string& current, vector<string>& words);
    int countWordsHelper(TrieNode* node);
};

void Trie::collectWords(TrieNode* node, const string& current, vector<string>& words) {
    if (node->isEndOfWord) words.push_back(current);
    for (auto& p : node->children) {
        collectWords(p.second, current + p.first, words);
    }
}

int Trie::countWordsHelper(TrieNode* node) {
    int count = node->isEndOfWord ? 1 : 0;
    for (auto& p : node->children) {
        count += countWordsHelper(p.second);
    }
    return count;
}

vector<string> Trie::autocomplete(const string& prefix) {
    TrieNode* node = root;
    for (char c : prefix) {
        auto it = node->children.find(c);
        if (it == node->children.end()) return {};
        node = it->second;
    }
    vector<string> words;
    collectWords(node, prefix, words);
    return words;
}

int countOccurrences(const string& text, const string& pattern) {
    int count = 0;
    size_t pos = 0;
    while ((pos = text.find(pattern, pos)) != string::npos) {
        ++count;
        pos += 1;
    }
    return count;
}

pair<bool, vector<string>> Trie::spellCheck(const string& word) {
    bool found = search(word);
    vector<string> suggestions;
    if (!found) {
        string prefix = "";
        TrieNode* node = root;
        for (char c : word) {
            auto it = node->children.find(c);
            if (it == node->children.end()) break;
            prefix += c;
            node = it->second;
        }
        if (!prefix.empty()) {
            suggestions = autocomplete(prefix);
        }
    }
    return {found, suggestions};
}

int main() {
    Trie trie;
    // Pre-insert some words
    trie.insert("apple");
    trie.insert("app");
    trie.insert("banana");
    trie.insert("application");
    trie.insert("apply");

    cout << boolalpha;
    while (true) {
        cout << "\nTrie Operations Menu:" << endl;
        cout << "1. Insert a word" << endl;
        cout << "2. Search for a word" << endl;
        cout << "3. Delete a word" << endl;
        cout << "4. Autocomplete" << endl;
        cout << "5. Count words with prefix" << endl;
        cout << "6. Spell check" << endl;
        cout << "7. Quit" << endl;
        cout << "Enter your choice (1-7): ";
        int choice;
        cin >> choice;
        cin.ignore(); // Ignore newline

        if (choice == 1) {
            cout << "Enter word to insert: ";
            string word;
            getline(cin, word);
            trie.insert(word);
            cout << "Inserted: " << word << endl;
        } else if (choice == 2) {
            cout << "Enter word to search: ";
            string word;
            getline(cin, word);
            cout << "Found: " << trie.search(word) << endl;
        } else if (choice == 3) {
            cout << "Enter word to delete: ";
            string word;
            getline(cin, word);
            trie.remove(word);
            cout << "Deleted: " << word << endl;
        } else if (choice == 4) {
            cout << "Enter prefix for autocomplete: ";
            string prefix;
            getline(cin, prefix);
            auto words = trie.autocomplete(prefix);
            cout << "Suggestions: ";
            for (auto& w : words) cout << w << " ";
            cout << endl;
        } else if (choice == 5) {
            cout << "Enter word: ";
            string word;
            getline(cin, word);
            cout << "Enter prefix to count: ";
            string prefix;
            getline(cin, prefix);
            cout << "Count: " << countOccurrences(word, prefix) << endl;
        } else if (choice == 6) {
            cout << "Enter word to spell check: ";
            string word;
            getline(cin, word);
            auto result = trie.spellCheck(word);
            if (result.first) {
                cout << "'" << word << "' is spelled correctly." << endl;
            } else {
                cout << "'" << word << "' is misspelled. Suggestions: ";
                for (auto& s : result.second) cout << s << " ";
                cout << endl;
                // Optionally, offer to insert a suggestion
                if (!result.second.empty()) {
                    cout << "Would you like to insert '" << result.second[0] << "'? (y/n): ";
                    char yn;
                    cin >> yn;
                    cin.ignore();
                    if (yn == 'y' || yn == 'Y') {
                        trie.insert(result.second[0]);
                        cout << "Inserted: " << result.second[0] << endl;
                    }
                }
            }
        } else if (choice == 7) {
            break;
        } else {
            cout << "Invalid choice. Try again." << endl;
        }
    }
    return 0;
}