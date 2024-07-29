#include <bits/stdc++.h>

using namespace std;

const uint32_t P = std::pow(2,31) - 1; // 2^31 - 1

class LGC
{
private:
    uint32_t currentRandom;
    bool firstCall = true;

    uint32_t R = std::pow(2,32) - 1; // 2^32
    uint32_t A = 1664525;
    uint32_t C = 1013904223;
public:

    LGC(uint32_t seed) {
        currentRandom = seed;
    }

    uint32_t next() {
        if (firstCall) {
            firstCall = false;
            return currentRandom;
        }
        currentRandom = (A * currentRandom + C) % R;
        return currentRandom;
    }
};

class OpenTable
{
private:
public:

    LGC& rng;
    uint32_t a, b;
    std::function<uint32_t(uint32_t)> hashFunction;

    std::list<uint32_t>* table;

    uint32_t capacity;
    int Length = 0;

    OpenTable(uint32_t newCapacity, LGC& rngRef) : capacity(newCapacity), rng(rngRef) {
        this->table = new std::list<uint32_t>[this->capacity];


        this->a = 1 + rngRef.next() % (P - 1);
        this->b = rngRef.next() % P;

        this->hashFunction = [&](uint32_t k) {
            return ((this->a * k + this->b) % P) % this->capacity;
        };
    }

    ~OpenTable() {
        delete[] this->table;
    }

    std::tuple<uint32_t,int> get(uint32_t k) {
        uint32_t i = this->hashFunction(k);

        int idx = 0; 
        for (const uint32_t& elem : this->table[i]) {
            if (elem == k) return std::make_tuple(i,idx);
            idx++;
        }
        return std::make_tuple(i,-1);
    }

    std::tuple<uint32_t,int> set(uint32_t k) {

        if ( 2 * this->Length > this->capacity ) {
            uint32_t oldM = this->capacity;
            this->capacity = 2 * oldM + 1;
            
            this->a = 1 + this->rng.next() % (P - 1);
            this->b = this->rng.next() % P;

            std::list<uint32_t>* newTable = new std::list<uint32_t>[this->capacity];

            for (int list = 0; list < oldM; list++) {
                for (const uint32_t& elem : this->table[list]) {

                    uint32_t i = this->hashFunction(elem);

                    std::list<uint32_t>* newList = &newTable[i];

                    int listLength = std::distance(newList->begin(),newList->end());
                    newList->push_back(elem);
                }
            }

            this->table = newTable;
            
        }

        uint32_t i = this->hashFunction(k);

        std::list<uint32_t>* list = &this->table[i];

        int listLength = std::distance(list->begin(),list->end());
        list->push_back(k);

        this->Length++;

        return std::make_tuple(i,listLength);
    }
};

class StaticTable
{
    public:
        uint32_t a, b;
        std::function<uint32_t(uint32_t)> hashFunction;

        uint32_t* table;
        uint32_t capacity;

        StaticTable(uint32_t newCapacity, LGC& rngRef) : capacity(newCapacity) {
            this->table = new uint32_t[this->capacity]{0};

            this->a = 1 + rngRef.next() % (P - 1);
            this->b = rngRef.next() % P;

            this->hashFunction = [&](uint32_t k) {
                return ((this->a * k + this->b) % P) % this->capacity;
            };

        }
};

class PerfectTable
{
    public:

        uint32_t a, b;
        std::function<uint32_t(uint32_t)> hashFunction;

        StaticTable** table;

        uint32_t capacity;

        PerfectTable(std::list<uint32_t> keys, LGC& rngRef) {
            
            uint32_t n = std::distance(keys.begin(),keys.end());
            this->capacity = 2 * (uint32_t)( n / 2 ) + 1;

            std::list<uint32_t>* buckets;

            uint32_t sumSquaredBuckets = 0;

            

            bool sumNeededFound = false;
            while (!sumNeededFound) {
                this->a = 1 + rngRef.next() % (P - 1);
                this->b = rngRef.next() % P;

                this->hashFunction = [&](uint32_t k) {
                    return ((this->a * k + this->b) % P) % this->capacity;
                };

                buckets = new std::list<uint32_t>[this->capacity];

                for (const uint32_t& k : keys) {
                    uint32_t i = hashFunction(k);
                    buckets[i].push_back(k);
                    
                }

                sumSquaredBuckets = 0;

                for (int buck = 0; buck < this->capacity; buck++) {
                    std::list<uint32_t> bucket = buckets[buck];
                    uint32_t bucketLength = std::distance(bucket.begin(), bucket.end());
                    sumSquaredBuckets += std::pow(bucketLength,2);
                }
                
                if (sumSquaredBuckets <= 4 * n) {

                    sumNeededFound = true;
                }
                
            }

            this->table = new StaticTable*[this->capacity]{nullptr};

            for (int i = 0; i < this->capacity; i++) {
                std::list<uint32_t> bucket = buckets[i];
                uint32_t bucketLength = std::distance(bucket.begin(),bucket.end());
                if (bucketLength > 0) {
                    bool collisions = false;
                    do
                    {
                        collisions = false;
                        uint32_t li = std::pow(bucketLength,2) + 1;

                        this->table[i] = new StaticTable(li, rngRef);

                        for ( const uint32_t& k : bucket ) {
                            uint32_t j = this->table[i]->hashFunction(k);
                            if (this->table[i]->table[j] != 0) {
                                collisions = true;
                                break;
                            }
                            this->table[i]->table[j] = k;
                        }
                        
                    } while (collisions);
                    
                }
            }
            
            delete[] buckets;
            

        }

        ~PerfectTable() {
            for (int i = 0; i < this->capacity; i++) {
                delete this->table[i];
            }
            delete[] this->table;
        }

        std::tuple<int,int> get(uint32_t k) {
            uint32_t i = this->hashFunction(k);
            if (this->table[i] == nullptr) {
                return std::make_tuple(-1,-1);
            }
            uint32_t j = this->table[i]->hashFunction(k);
            if (this->table[i]->table[j] != k) {
                return std::make_tuple(-1,-1);
            }
            return std::make_tuple(i,j);
        }
};

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    uint32_t S, U, M0, I, Q, Pi, Pq;

    cin >> S >> U >> M0 >> I >> Q >> Pi >> Pq;

    LGC rng(S);

    OpenTable opTable0(M0, rng);

    OpenTable* T0 = &opTable0;

    for (uint32_t t = 0; t < I; t++) {
        uint32_t k = rng.next() % U;

        std::tuple<uint32_t,int> request = T0->get(k);
        if ( std::get<1>(request) == -1 ) {
            request = T0->set(k);
        }

        if ( t % Pi == 0 ) {
            cout << "I " << k << " " << std::get<0>(request) << " " << std::get<1>(request) << '\n';
        }
    }

    std::list<uint32_t> keys;

    for (int listIdx = 0; listIdx < T0->capacity; listIdx++) {
        std::list<uint32_t> list = T0->table[listIdx];

        for ( const uint32_t& elem : list ) {
            keys.push_back(elem);
        }
    }
    
    PerfectTable T1(keys,rng);

    for (int t = 0; t < Q; t++) {
        uint32_t k = rng.next() % U;

        std::tuple<int,int> request = T1.get(k);

        if ( t % Pq == 0 ) {
            cout << "Q " << k << " " << std::get<0>(request) << " " << std::get<1>(request) << '\n';
        }
    }
    

    return 0;
}
