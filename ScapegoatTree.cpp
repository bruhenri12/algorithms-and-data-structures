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
        this->currentRandom = A * this->currentRandom + C;
        return this->currentRandom;
    }
};

struct Node {
    uint32_t key;
    Node* left;
    Node* right;

    Node(uint32_t key) {
        this->key = key;
        this->left = this->right = nullptr;
    }
    Node() {
        this->key = 0;
        this->left = this->right = nullptr;
    }
};

uint32_t odist = 0;

struct SGTree{
    Node* root;
    float alpha;
    uint32_t depth, size;

    SGTree(float alpha) : alpha(alpha) {
        this->root = nullptr;
        this->depth = this->size = 0;
    };
};

float count(struct Node* root) {
    if (root == nullptr) {
        return 0;
    }
    return 1 + count(root->left) + count(root->right);
}

Node* flatten(Node* root, Node* head){
    if (root == nullptr) {
        return head;
    }
    root->right = flatten(root->right, head);
    return flatten(root->left, root);
}

Node* build(Node* head, uint32_t n){
    if (n == 0) {
        head->left = nullptr; return head;
    }
    uint32_t nl = std::ceil((n-1.0)/2.0);
    uint32_t nd = (n-1)/2;
    Node* mid = build(head, nl);
    Node* handle = build(mid->right, nd);
    mid->right = handle->left;
    handle->left = mid;
    return handle;
}

Node* rebuild(Node* root, uint32_t n){
    Node* dummy = new Node();
    Node* head = flatten(root, dummy);
    return build(head, n)->left;
};


std::tuple<Node*, Node*, int> query(Node *r, int k) {
        Node* pair;
        Node* cur = r;
        uint32_t depth = 0;
        while (cur != nullptr) {
            if (k == cur->key) {
                return {pair, cur, depth};
            } else if (k < cur->key) {
                pair = cur;
                cur = cur->left;
                depth++;
            } else {
                pair = cur;
                cur = cur->right;
                depth++;
            }
        }
        return {pair, cur, depth};
    };

uint32_t Find(Node* r, uint32_t k, bool print) {
    Node* s;
    Node* l;
    uint32_t p;
    std::tie(s, l, p) = query(r, k);
    if (l == nullptr) {
        if (print) {
            std::cout << "Q " << k << " " << -1 << '\n';
        }
        return -1;
    } else {
        if (print) {
            std::cout << "Q " << k << " " << p << '\n';
        }
        return p;
    }
};


std::tuple<Node*, uint32_t, uint32_t, bool, uint32_t> SGIns(Node* root, uint32_t depth, uint32_t n, float alpha, uint32_t k) {
    uint32_t add, nse, nre, dist, nroot;
    bool chg;
    if(root == nullptr){
        Node* x = new Node(k);
        chg = depth > std::floor(log2(n + 1)/log2(1/alpha));
        odist = depth;
        return {x, 1, 1, chg, 0};
    } else if (k == root->key) {
        return {root, 0, -1, false, -1};
    } else if ( k < root->key ) {
        std::tie(root->left, add, nse, chg, dist) = SGIns(root->left, depth+1, n, alpha, k);
        nroot = chg ? 1 + nse + count(root->right) : -1;
    } else {
        std::tie(root->right, add, nre, chg, dist) = SGIns(root->right, depth+1, n, alpha, k);
        nroot = chg ? 1 + nre + count(root->left) : -1;  
    }
    
    dist++;
    if (!chg) {
        return {root, add, nroot,chg, -1};
    }

    if ( chg && dist > std::floor( log2(nroot) / log2(1.0/alpha) ) ) {
        return { rebuild(root, nroot), add, nroot, false, -1};
    } 
    return {root, add, nroot, chg, dist};
}


void SGInsert(SGTree& tree, uint32_t key, bool print) {
    uint32_t added, depth, distToNewLeaf;
    bool needRebuild;

    std::tie(tree.root, added, depth, needRebuild, distToNewLeaf) = SGIns(tree.root, 0, tree.size, tree.alpha, key); 
    tree.size += added;
    if (print) {
        int Lo = ((added == 0) ? -1 : odist);
        int Lf = ((added == 0) ? -1 : Find(tree.root, key, false));
        std::cout << "I " << key << " " << Lo << " " << Lf << '\n';
    }
}

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    uint32_t S, U, B, N, I, Q, P;
    float A;

    cin >> S >> U >> A >> B >> N >> I >> Q >> P;

    LGC rng(S);

    SGTree T(A);

    while (B--) {
        SGInsert(T, rng.next() % U, false);
    }
    
    uint32_t k;
    for (uint32_t op = 0; op < N; op++)
    {
        uint32_t type = rng.next() % (I + Q);
        uint32_t k = rng.next() % U;
        if (type < I) {
            SGInsert(T, k, op % P == 0);
        } else {
            Find(T.root, k, op % P == 0);
        }
    }

    return 0;
}
