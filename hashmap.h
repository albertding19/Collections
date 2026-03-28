
#pragma once
#include "collections.h"
#include "arrayList.h"
#include <algorithm>

static constexpr double LOAD_FACTOR{0.7};
static constexpr std::size_t GROWTH_FACTOR{2};
static constexpr std::size_t DEFAULT_SIZE{32};
static constexpr uint64_t FNV_OFFSET{14695981039346656037ULL};
static constexpr uint64_t FNV_PRIME{1099511628211ULL};

template <typename K>
concept Hashable = requires(const K &k) {
    { std::hash<K>{}(k) } -> std::convertible_to<std::size_t>;
};

template <typename K>
concept MapKey = Comparable<K> && Hashable<K>;

template <typename K, typename V>
struct Entry {
    K key;
    V value;
    bool operator==(const Entry &other) const { // a == operator is required since ResizingArrayList<T> requires T to be comparable
        return key == other.key;
    }
};

template <MapKey K, Comparable V>
class HashMap : public Map<K, V> {
public:
    HashMap() : map(std::make_unique<ResizingArrayList<Entry<K, V>>[]>(DEFAULT_SIZE)), count(0), capacity(DEFAULT_SIZE), growthFactor(GROWTH_FACTOR), maxLoadFactor(LOAD_FACTOR) {}

    ~HashMap() = default;

    void put(const K &key, const V &value) override;

    std::optional<V> get(const K &key) const override;

    V &operator[](const K &key) override;

    bool containsKey(const K &key) const override;

    std::optional<V> remove(const K &key);

    std::size_t size() const override;

    friend std::ostream &operator<<(std::ostream &os, const HashMap &hm) requires Printable<K> && Printable<V> {
        os << "{";
        bool first{true};

        for (std::size_t i = 0; i < hm.capacity; i++) {
            const ResizingArrayList<Entry<K, V>> &bucket = hm.map[i];
            for (std::size_t j = 0; j < bucket.size(); j++) {
                Entry<K, V> entry{bucket.get(j)};
                if (first) {
                    os << entry.key << " : " << entry.value;
                    first = false;
                } else {
                    os << ", " << entry.key << " : " << entry.value;
                }
            }
        }

        os << "}";
        return os;
    }

private:
    std::unique_ptr<ResizingArrayList<Entry<K, V>>[]> map;

    std::size_t count;

    std::size_t capacity;

    std::size_t growthFactor;

    double maxLoadFactor;

    V defaultValue;

    std::size_t hash(const K &key) const;

    double getCurrentLoad() const;

    void resize();
};

template <MapKey K, Comparable V>
void HashMap<K, V>::put(const K &key, const V &val) {
    ResizingArrayList<Entry<K, V>> &bucket = map[hash(key)];

    for (std::size_t i = 0; i < bucket.size(); i++) {
        if (bucket[i].key == key) {
            bucket[i].value = val;
            return;
        }
    }

    bucket.add(Entry{key, val});
    count++;
    if (getCurrentLoad() > maxLoadFactor) {
        resize();
    }
}

template <MapKey K, Comparable V>
V &HashMap<K, V>::operator[](const K &key) {
    ResizingArrayList<Entry<K, V>> &bucket = map[hash(key)];

    for (std::size_t i = 0; i < bucket.size(); i++) {
        if (bucket[i].key == key) {
            return bucket[i].value;
        }
    }

    // auto insert a default value if key not found, like Python defaultDict
    count++;
    if (getCurrentLoad() > maxLoadFactor) {
        resize();
    }
    ResizingArrayList<Entry<K, V>> &newBucket = map[hash(key)]; // recompute hash after resizing
    newBucket.add(Entry<K, V>{key, V{}});
    return newBucket[newBucket.size() - 1].value;
}

template <MapKey K, Comparable V>
std::optional<V> HashMap<K, V>::get(const K &key) const {
    ResizingArrayList<Entry<K, V>> bucket = map[hash(key)];

    for (std::size_t i = 0; i < bucket.size(); i++) {
        if (bucket[i].key == key) {
            return bucket[i].value;
        }
    }

    return std::nullopt;
}

template <MapKey K, Comparable V>
bool HashMap<K, V>::containsKey(const K &key) const {
    ResizingArrayList<Entry<K, V>> bucket = map[hash(key)];

    for (std::size_t i = 0; i < bucket.size(); i++) {
        if (bucket[i].key == key) {
            return true;
        }
    }

    return false;
}

template <MapKey K, Comparable V>
std::size_t HashMap<K, V>::hash(const K &key) const {
    // fnv1a hashing protocol. Start with a fixed offset basis, then:
    // for each byte, XOR with hash, then multiple by a prime
    // size_t is guaranteed to be large enough to represent the size of any object in memory

    // above protocol unused, instead we use C++ built it hashing function
    // the reason is that types containing pointer types are difficult to hash since the memory address (value) of the pointer will differ on each execution
    // resulting in the same key generating difference hashes.

    return std::hash<K>{}(key) % capacity;
}

template <MapKey K, Comparable V>
double HashMap<K, V>::getCurrentLoad() const {
    return static_cast<double>(count) / static_cast<double>(capacity);
}

template <MapKey K, Comparable V>
std::size_t HashMap<K, V>::size() const {
    return count;
}

template <MapKey K, Comparable V>
std::optional<V> HashMap<K, V>::remove(const K &key) {
    ResizingArrayList<Entry<K, V>> &bucket{map[hash(key)]};

    for (std::size_t i = 0; i < bucket.size(); i++) {
        if (bucket[i].key == key) {
            count--;
            std::optional<Entry<K, V>> result = bucket.remove(i);
            return result.value().value;
        }
    }
    return std::nullopt;
}

template <MapKey K, Comparable V>
void HashMap<K, V>::resize() {
    std::size_t oldCapacity{capacity};
    capacity *= growthFactor;
    std::unique_ptr<ResizingArrayList<Entry<K, V>>[]> newMap{std::make_unique<ResizingArrayList<Entry<K, V>>[]>(capacity)};

    for (std::size_t i = 0; i < oldCapacity; i++) {
        const ResizingArrayList<Entry<K, V>> &bucket{map[i]};
        for (std::size_t j = 0; j < bucket.size(); j++) {
            Entry<K, V> entry{bucket.get(j)};
            std::size_t newHash{hash(entry.key)};
            newMap[newHash].add(entry);
        }
    }
    map = std::move(newMap);
}