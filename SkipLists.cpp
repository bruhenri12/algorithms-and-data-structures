#include <bits/stdc++.h>

using namespace std;

const uint32_t R = (1 << 32) - 1;
const uint32_t A = 1664525;
const uint32_t C = 1013904223;

class LGC
{
private:
    bool        firstCall = true;
    uint32_t    currentRandom;
public:
    LGC(uint32_t seed) {
        this->currentRandom = seed;
    }

    uint32_t next() {
        if (this->firstCall) {
            this->firstCall = false;
            return this->currentRandom;
        }

        this->currentRandom = (A * this->currentRandom + C) % R;
        return this->currentRandom;
    }
};

struct Node
{
    int height;

    uint32_t key;

    std::vector<Node*> next;

    Node(int newHeight) : height(newHeight) {
        for (int i = 0; i < this->height; i++)
        {
            this->next.push_back(nullptr);
        }
    }
};


class SkipList
{
private:
    LGC& rngRef;
public:
    int height;
    int findAccessed;
    Node* head;
    SkipList(LGC& rng) : rngRef(rng) {
        this->height = 1;

        this->head = new Node(this->height);
    }

    // Search

    std::vector<Node*> precursors(uint32_t key) {
        std::vector<Node*> P(this->height,nullptr);
        
        Node* cur = this->head;
        this->findAccessed = 1;

        for (int l = this->height - 1; l >= 0; --l)
        {
            while ( cur->next[l] != nullptr && cur->next[l]->key < key ) {
                this->findAccessed++;
                cur = cur->next[l];
            }
            P[l] = cur;
        }

        return P;
    }

    uint32_t find(uint32_t key) {
        std::vector<Node*> P = this->precursors(key);

        if (P[0]->next[0] != nullptr && P[0]->next[0]->key == key) {
            return P[0]->next[0]->height;
        }
        return 0;
    }

    // Insert

    int max_height() {
        return this->height + 1;
    }

    int random_height() {
        int l = 1;
        while (this->rngRef.next() % 100 < 50 && l < this->max_height()) {
            l++;
        }
        return l;
    }

    uint32_t insert(uint32_t key) {
        std::vector<Node*> P = this->precursors(key);

        if ( P[0]->next[0] != nullptr && P[0]->next[0]->key == key ) {
            return 0;
        }

        int h = this->random_height();
        if ( h > this->height ) {
            for (int i = this->height; i < h; i++)
            {
                this->head->next.push_back(nullptr);
                P.push_back(this->head);
            }
            this->height = h;
        }

        Node* N = new Node(h);

        N->key = key;

        for (int l = 0; l < h; l++)
        {
            N->next[l] = P[l]->next[l];
            P[l]->next[l] = N;
        }

        return 1;
    }

    // Delete

    int _delete(uint32_t key) {
        std::vector<Node*> P = this->precursors(key);

        if ( P[0]->next[0] == nullptr || P[0]->next[0]->key != key ) {
            return 0;
        }

        Node* N = P[0]->next[0];

        for (int l = 0; l < N->height; l++)
        {
            P[l]->next[l] = N->next[l];
        }

        while ( this->height > 1 && this->head->next[this->height-1] == nullptr )
        {
            this->height--;
        }
        
        return 1;
    }

};

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    uint32_t S, U, B, N, F, I, D, P;
    cin >> S >> U >> B >> N >> F >> I >> D >> P;

    LGC rng(S);
    SkipList L(rng);

    for (int burnIn = 0; burnIn < B; burnIn++) {
        uint32_t y = rng.next() % U;
        L.insert(y);
    }
    
    for (uint32_t oper = 0; oper < N; oper++) {
        uint32_t X = rng.next() % ( F + I + D );
        uint32_t y = rng.next() % U;
        std::ostringstream oss;
        if ( X < F ) {
            uint32_t query = L.find(y);
            oss << "F " << L.findAccessed << " " << query;
        } else if ( F <= X && X < F + I ) {
            oss << "I " << L.insert(y);
        } else if ( F + I <= X ) {
            oss << "D " << L._delete(y);
        } else {
            cout << "ERROR! > Wrong operation." << '\n';
        }

        string output = oss.str();
        if ( oper % P == 0 ) {
            cout << output << '\n';
        }
    }
    
    

    return 0;
}
