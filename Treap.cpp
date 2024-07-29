#include <bits/stdc++.h>

using namespace std;

class LGC
{
private:
    bool firstCall = true;

    uint32_t A = 1664525U;
    uint32_t C = 1013904223U;
    uint32_t R = 4294967295U; // 2^32

    uint32_t currentRandom;
public:
    LGC(uint32_t newSeed) {
        this->currentRandom = newSeed;
    }

    uint32_t next() {
        if (this->firstCall)
        {
            this->firstCall = false;
            return this->currentRandom;
        }
        this->currentRandom = ( this->A * this->currentRandom + this->C ) % R;
        return this->currentRandom;
    }
};

// TREAP

struct Node {
    uint32_t key, priority;
    Node *left, *right;
    
    Node(uint32_t key, LGC& rng) {
        this->key = key;
        this->priority = rng.next();
        this->left = this->right = nullptr;
    }

    ~Node() {
        this->left = this->right = nullptr;
    }
};


int treapQuery(Node *root, uint32_t key){
    Node *cur = root;
    int depth = 0;
    while (cur != nullptr) {
        if (key == cur->key) {
            return depth;
        } else if (key < cur->key) {
            cur = cur->left;
            depth++;
        } else {
            cur = cur->right;
            depth++;
        }
    }
    return -1;
};

std::tuple<Node*, Node*> split(Node *root, uint32_t key){ 
    if (root == nullptr) {
        return {nullptr, nullptr};
    } else if (key == root->key) {
        return {root->left, root->right};
    } else if (key < root->key) {
        Node* lower = nullptr;
        std::tie(lower, root->left) = split(root->left, key);
        return {lower, root};
    } else {
        Node* greater = nullptr;
        std::tie(root->right, greater) = split(root->right, key);
        return {root, greater};
    }
};

Node* NodeInsert(Node *root, Node*& x) {
    if (root == nullptr) {
        return x;
    } else if(x->priority > root->priority) {
        std::tie(x->left, x->right) = split(root, x->key);
        return x;
    } else if (x->key < root->key) {
        root->left = NodeInsert(root->left, x);
        return root;
    } else {
        root->right = NodeInsert(root->right, x);
        return root;
    }
};

int treapInsert(Node*& root, uint32_t key, LGC& rng) {
    int depth = treapQuery(root, key);
    if (depth != -1) {
        return -1;
    }
    Node* X = new Node(key, rng);
    root = NodeInsert(root, X);
    depth = treapQuery(root, key);
    return depth;
}

Node* TreapMerge(Node*& rootx, Node*& rooty){
    if (rootx == nullptr) {
        return rooty;
    } else if (rooty == nullptr) {
        return rootx;
    } else if (rootx->priority > rooty->priority) {
        rootx->right = TreapMerge(rootx->right, rooty);
        return rootx;
    } else {
        rooty->left = TreapMerge(rootx, rooty->left);
        return rooty;
    }
};

Node* DeleteNode(Node*& root, int key){
    if (root == nullptr) {
        return nullptr;
    } else if (key == root->key) {
        return TreapMerge(root->left,root->right);
    } else if (key < root->key){
        root->left = DeleteNode(root->left, key);
        return root;
    } else {
        root->right = DeleteNode(root->right, key);
        return root;
    }
};

int treapDelete(Node*& root, uint32_t key){
    uint32_t depth = treapQuery(root, key);
    if (depth == -1) {
        return -1;
    }
    root = DeleteNode(root, key);
    return depth;
}

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    uint32_t S, U, B, N, I, D, Q, P;
    cin >> S >> U >> B >> N >> I >> D >> Q >> P;

    LGC rng(S);

    Node* root = nullptr;

    for (int burnIn = 0; burnIn < B; burnIn++)
    {
        treapInsert(root, rng.next() % U, rng);
    }
    
    for (int operation = 0; operation < N; operation++)
    {
        uint32_t X = rng.next() % (I + D + Q);
        uint32_t K = rng.next() % U; 
        char OP = ' ';
        int depth;
        if (X < I) {
            OP = 'I';
            depth = treapInsert(root, K, rng);
        } else if (X < I + D) {
            OP = 'D';
            depth = treapDelete(root, K);
        } else {
            OP = 'Q';
            depth = treapQuery(root, K);
        }
        

        if (operation % P == 0) {
            cout << OP << " " << K << " " << depth << '\n';
        }
        
    }
    

    return 0;
}
