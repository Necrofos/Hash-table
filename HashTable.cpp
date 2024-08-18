#include "HashTable.h"

HashTable::HashTable(size_t size):table(size) {
        _capacity = size;
        _filled = 0;
    }

void HashTable::insert(const KeyType &key, const ValueType &value) {
    if (getLoadFactor() > 0.75) {
        _capacity *= 2;
        std::vector<std::list<std::pair<KeyType, ValueType>>> newTable(_capacity);
        for (auto &bucket : table) {
            for (auto &pair : bucket) {
                size_t newHash = hash_function(pair.first) % _capacity;
                newTable[newHash].emplace_back(pair);
            }
        }
        table = std::move(newTable);
    }
    size_t hash = hash_function(key) % _capacity;
    for (auto &pair : table[hash]) {
        if (pair.first == key) {
            pair.second = value;
            return;
        }
    }
    table[hash].emplace_back(key, value);
    ++_filled;
}

bool HashTable::find(const KeyType &key, ValueType &value) const {
    size_t hash = hash_function(key) % _capacity;
    for (const auto &pair : table[hash]) {
        if (pair.first == key) {
            value = pair.second;
            return true;
        }
    }
    return false;
}

void HashTable::remove(const KeyType &key) {
    size_t hash = hash_function(key) % _capacity;
    auto &bucket = table[hash];
    for (auto it = bucket.begin(); it != bucket.end(); ++it) {
        if (it->first == key) {
            bucket.erase(it);
            --_filled;
            return;
        }
    }
}

ValueType& HashTable::operator[](const KeyType &key) {
    size_t hash = hash_function(key) % _capacity;
    for (auto &pair : table[hash]) {
        if (pair.first == key) {
            return pair.second;
        }
    }
    table[hash].emplace_back(key, ValueType{});
    ++_filled;
    return table[hash].back().second;
}

double HashTable::getLoadFactor() {
    return static_cast<double>(_filled) / _capacity;
}

size_t HashTable::hash_function(const KeyType &key)const{
    std::hash<KeyType> hashFn;
    return hashFn(key);
}
