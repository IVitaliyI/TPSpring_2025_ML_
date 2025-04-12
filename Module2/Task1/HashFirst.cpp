#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define MAX_LOAD_FACTOR 0.75
#define PRIME 31

enum class State { NIL, DELETED, OBJECT };

struct hashfunction {
    unsigned operator()(const std::string &str) const {
        unsigned hash = 0;
        for (char ch : str) {
            hash = (hash * PRIME + ch);
        }
        return hash;
    }
};

template <typename T, typename HashFunction = std::hash<T>>
class HashTable {
   public:
    HashTable() : size{0}, capacity{8}, data_object(capacity, {T(), State::NIL}) {}
    ~HashTable(){};
    bool Has(const T &object) const;
    bool Add(const T &object);
    bool Delete(const T &object);

    unsigned GetSize() { return size; }
    unsigned GetCapacity() { return capacity; }

   private:
    HashFunction hashFunc = HashFunction();
    unsigned size;
    unsigned capacity;
    std::vector<std::pair<T, State>> data_object;
    unsigned probe(const T &object, const unsigned i, const unsigned M) const;
    bool isNil(const unsigned pos) const;
    bool isDeleted(const unsigned pos) const;
    bool isObject(const unsigned pos) const;

    float loadFactor() const { return static_cast<float>(size) / capacity; }

    unsigned hashInternal(const unsigned number, const unsigned M) const {
        unsigned hash_calc = std::hash<unsigned>()(number) % M;
        return (hash_calc % 2) ? hash_calc : (hash_calc + 1) % M;
    }

    void reHash();
};

template <typename T, typename HashFunction>
bool HashTable<T, HashFunction>::Has(const T &object) const {
    unsigned i;
    unsigned hash_probe;
    for (i = 0; i < capacity; i++) {
        hash_probe = probe(object, i, capacity);
        if (isNil(hash_probe)) {
            return false;
        }
        if (isDeleted(hash_probe)) {
            continue;
        }
        if (isObject(hash_probe) && data_object[hash_probe].first == object) return true;
    }
    return false;
}

template <typename T, typename HashFunction>
bool HashTable<T, HashFunction>::Add(const T &object) {
    if (Has(object)) return false;

    if (loadFactor() > MAX_LOAD_FACTOR) reHash();

    unsigned i, hash_probe;
    for (i = 0; i < capacity; i++) {
        hash_probe = probe(object, i, capacity);
        if (isNil(hash_probe) || isDeleted(hash_probe)) {
            data_object[hash_probe] = {object, State::OBJECT};
            size++;
            return true;
        } else {
            continue;
        }
    }
    return false;
}

template <typename T, typename HashFunction>
bool HashTable<T, HashFunction>::isNil(const unsigned pos) const {
    return data_object[pos].second == State::NIL;
}

template <typename T, typename HashFunction>
bool HashTable<T, HashFunction>::isDeleted(const unsigned pos) const {
    return data_object[pos].second == State::DELETED;
}

template <typename T, typename HashFunction>
bool HashTable<T, HashFunction>::isObject(const unsigned pos) const {
    return data_object[pos].second == State::OBJECT;
}

template <typename T, typename HashFunction>
bool HashTable<T, HashFunction>::Delete(const T &object) {
    unsigned i;
    unsigned hash_probe;
    for (i = 0; i < capacity; i++) {
        hash_probe = probe(object, i, capacity);
        if (isNil(hash_probe)) {
            return false;
        }

        if (isDeleted(hash_probe)) {
            continue;
        }

        if (isObject(hash_probe)) {
            if (data_object[hash_probe].first == object) {
                data_object[hash_probe].second = State::DELETED;
                size--;
                return true;
            }
        }
    }
    return false;
}

template <typename T, typename HashFunction>
unsigned HashTable<T, HashFunction>::probe(const T &object, const unsigned i,
                                           const unsigned M) const {
    unsigned hashFirst = hashFunc(object) % M;
    return (hashFirst + i * hashInternal(hashFirst, M)) % M;
}

template <typename T, typename HashFunction>
void HashTable<T, HashFunction>::reHash() {
    unsigned new_capacity = 2 * capacity;
    std::vector<std::pair<T, State>> new_data(new_capacity, {T(), State::NIL});
    unsigned i, j;
    unsigned new_hash;
    for (i = 0; i < capacity; i++) {
        if (isObject(i)) {
            new_hash = probe(data_object[i].first, 0, new_capacity);
            j = 1;
            while (new_data[new_hash].second == State::OBJECT) {
                new_hash = probe(data_object[i].first, j++, new_capacity);
            }
            new_data[new_hash] = {data_object[i].first, State::OBJECT};
        }
    }
    data_object = std::move(new_data);
    capacity = new_capacity;
}

void run(std::istream &input = std::cin, std::ostream &output = std::cout) {
    std::string task;
    std::string str;
    HashTable<std::string, hashfunction> hashtb;
    while (input >> task >> str) {
        if (input.fail()) {
            return;
        }
        if (task.empty() || str.empty()) break;
        if (task == "+") {
            if (hashtb.Add(str)) {
                output << "OK" << std::endl;
            } else {
                output << "FAIL" << std::endl;
            }
        } else if (task == "-") {
            if (hashtb.Delete(str)) {
                output << "OK" << std::endl;
            } else {
                output << "FAIL" << std::endl;
            }
        } else if (task == "?") {
            if (hashtb.Has(str)) {
                output << "OK" << std::endl;
            } else {
                output << "FAIL" << std::endl;
            }
        }
    }
    return;
}

void test_case() {
    {
        std::cout << "Test 1" << std::endl;
        std::stringstream input, output;
        input << "+ hello" << std::endl;
        input << "+ bye" << std::endl;
        input << "? bye" << std::endl;
        input << "+ bye" << std::endl;
        input << "- bye" << std::endl;
        input << "? bye" << std::endl;
        input << "? hello" << std::endl;
        run(input, output);
        assert(output.str() == "OK\nOK\nOK\nFAIL\nOK\nFAIL\nOK\n");
        std::cout << "Test 1 -> OK!" << std::endl;
    }
    {
        std::cout << "Test 2 — удаление несуществующего элемента" << std::endl;
        std::stringstream input, output;
        input << "- world" << std::endl;
        input << "? world" << std::endl;
        run(input, output);
        assert(output.str() == "FAIL\nFAIL\n");
        std::cout << "Test 2 -> OK!" << std::endl;
    }
    {
        std::cout << "Test 3 — множественные вставки и удаления" << std::endl;
        std::stringstream input, output;
        input << "+ one\n+ two\n+ three\n+ four\n+ five\n";
        input << "? one\n? five\n- two\n- two\n+ two\n? two\n";
        run(input, output);
        assert(output.str() == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nFAIL\nOK\nOK\n");
        std::cout << "Test 3 -> OK!" << std::endl;
    }
    {
        std::cout << "Test 4 — проверка reHash (массовые вставки)" << std::endl;
        std::stringstream input, output;
        for (int i = 0; i < 20; ++i) {
            input << "+ str" << i << "\n";
        }
        for (int i = 0; i < 20; ++i) {
            input << "? str" << i << "\n";
        }
        run(input, output);
        std::stringstream expected;
        for (int i = 0; i < 20; ++i) expected << "OK\n";
        for (int i = 0; i < 20; ++i) expected << "OK\n";
        assert(output.str() == expected.str());
        std::cout << "Test 4 -> OK!" << std::endl;
    }
    {
        std::cout << "Test 5 — добавление после удаления" << std::endl;
        std::stringstream input, output;
        input << "+ alpha\n";
        input << "- alpha\n";
        input << "+ alpha\n";
        input << "? alpha\n";
        run(input, output);
        assert(output.str() == "OK\nOK\nOK\nOK\n");
        std::cout << "Test 5 -> OK!" << std::endl;
    }
}

int main() {
    // test_case();
    run();
    return 0;
}