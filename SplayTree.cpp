#include <bits/stdc++.h>

using namespace std;

const uint32_t A = 1664525;
const uint32_t C = 1013904223;

class LGC {
    private:
    uint32_t currentRandom;
    bool firstCall = true;
    public:
    LGC(uint32_t seed) {
        this->currentRandom = seed;
    }
    uint32_t next() {
        if (this->firstCall) {
            this->firstCall = false;
            return this->currentRandom;
        }
        this->currentRandom = (A * this->currentRandom + C);
        return this->currentRandom;
    }
};

struct Node {
    uint32_t key;

    Node* left;
    Node* right;
    Node* pair;

    Node(uint32_t newKey, Node* parent) {
        this->key = newKey;
        this->pair = parent;
        this->left = this->right = nullptr;
    }

    Node() {
        this->key = 0;
        this->pair = this->left = this->right = nullptr;
    }
};

Node* rotateLeft(Node* x) {
    Node* p = x->pair;
    Node* r = x->right;
    Node* rl = r->left;
    x->right = rl;
    if (rl != nullptr) {
        rl->pair = x;
    }
    r->left = x;
    x->pair = r;
    r->pair = p;
    if (p != nullptr && x == p->left) {
        p->left = r;
    }
    else if (p != nullptr) {
        if (x == p->right) {
            p->right = r;
        }
    }
    return r;
}

Node* rotateRight(Node* x){
    Node* p = x->pair;
    Node* l = x->left;
    Node* lr = l->right;
    x->left = lr;
    if (lr != nullptr) {
        lr->pair = x;
    }
    l->right = x;
    x->pair = l;
    l->pair = p;
    if (p != nullptr && x == p->right) {
        p->right = l;
    }
    else if (p != nullptr) {
        if (x == p->left) {
            p->left = l;
        }
    }
    return l;
};

Node* zig(Node* x) {
    Node* p = x->pair;
    if (x == p->left) {
        return rotateRight(p);
    } else {
        return rotateLeft(p);
    }
}

Node* zigzag(Node* x) {
    Node* p = x->pair;
    Node* g = p->pair;
    if (p == g->left) {
        if (x == p->left) {
            g = rotateRight(g);
            return rotateRight(p);
        } else {
            p = rotateLeft(p);
            return rotateRight(g);
        }
    } else {
        if (x == p->right) {
            g = rotateLeft(g);
            return rotateLeft(p);
        } else {
            p = rotateRight(p);
            return rotateLeft(g);
        }
    }
}

Node* splay(Node* x) {
    while (x->pair) {
        Node* g = x->pair->pair;
        if (g == nullptr) {
            x = zig(x);
        } else {
            x = zigzag(x);
        } 
    }
    return x;
}

std::tuple<Node*, uint32_t> query(Node* r, uint32_t k){
    Node* cur = r;
    uint32_t depth = 0;

    while (cur != nullptr) {
        if(k == cur->key){
            return {cur, depth};
        } else if (k < cur->key) {
            cur = cur->left;
            depth++;
        } else {
            cur = cur->right;
            depth++;
        }
    }
    return {cur, depth};
};

Node* find(Node* t, uint32_t k, bool print){
    Node* x;
    uint32_t pos; 
    std::tie(x, pos) = query(t, k);
    if (x == nullptr) {
        if (print) {
            std::cout << "Q " << k << " " << -1 << '\n';
        }
        return t;
    }
    if (print) {
        std::cout << "Q " << k << " " << pos << '\n';
    }
    t = splay(x);
    return t;
};

Node* _insert(Node* r, uint32_t key, Node* par, bool print, uint32_t depth) {
    if (r == nullptr) {
        if (print) {
            std::cout << "I " << key << " " << depth << '\n';
        }
        return new Node(key, par);
    } else if (r->key == key) {
        if (print) {
            std::cout << "I " << key << " " << -1 << '\n';
        }
        return r;
    } else if (key < r->key) {
        r->left = _insert(r->left, key, r, print, depth + 1);
    } else {
        r->right = _insert(r->right, key, r, print, depth + 1);
    }
    return r;
}

Node* insert(Node* r, uint32_t k, bool print){
    r = _insert(r, k, nullptr, print, 0);
    return r;
};

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    uint32_t S, U, B, N, I, Q, P;

    cin >> S >> U >> B >> N >> I >> Q >> P;

    LGC rng(S);

    Node* r = nullptr;
    uint32_t k;

    while(B--){
        k = rng.next() % U;
        r = insert(r, k, false);
    }
    

    for (uint32_t op = 0; op < N; op++)
    {
        uint32_t type = rng.next() % (I + Q);
        uint32_t k = rng.next() % U;
        if (type < I) {
            r = insert(r, k, op % P == 0);
        } else {
            r = find(r, k, op % P == 0);
        }
    }
    

    return 0;
}
