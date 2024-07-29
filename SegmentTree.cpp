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

uint32_t _max(uint32_t a, uint32_t b) {
    return std::max(a, b);
}

uint32_t _min(uint32_t a, uint32_t b) {
    return std::min(a, b);
}

uint32_t _sum(uint32_t a, uint32_t b) {
    return a + b;
}

class SegmentTree
{
private:
    std::vector<uint32_t>* T;
    std::function<uint32_t(uint32_t,uint32_t)>* func;

    uint32_t funcNeutral;
public:
    SegmentTree(std::vector<uint32_t> values, std::function< uint32_t(uint32_t, uint32_t) > func, uint32_t neutral) {
        this->func = &func;
        this->funcNeutral = neutral;
        this->T = new std::vector<uint32_t>(2*values.size());
        for (uint32_t i = 0; i < values.size(); i++) {
            (*this->T)[i+values.size()] =  values[i];
        }
        for (int i = values.size() - 1; i > 0; i--) {
            (*this->T)[i] = func((*this->T)[2 * i], (*this->T)[2*i+1]);
        }
    }

    void update(uint32_t n, uint32_t i, uint32_t val){
        i = n + i; 
        (*this->T)[i] = val;

        i = i/2;
        while (i > 0) {
            (*this->T)[i] = (*this->func)((*this->T)[2*i], (*this->T)[2*i+1]);
            i = i/2;
        }
    }

    uint32_t query(uint32_t n, uint32_t l, uint32_t r){
        uint32_t left = n + l, right = n+r;
        uint32_t ans = this->funcNeutral;
        while(left < right){
            if(left%2){ 
                ans = (*this->func)(ans, (*this->T)[left]);
                left++;
            }
            if(right%2){
                right--;
                ans = (*this->func)(ans, (*this->T)[right]);
            }
            left /= 2;
            right /= 2;
        }
        return ans;
    }
};

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    uint32_t S, N, W, Q, U, P;
    string F;

    int caso = 0;
    while(cin >> S >> N >> F >> W >> Q >> U >> P){
        std::cout << "caso " << caso << '\n';

        LGC rng(S);


        uint32_t M = 4*N;

        std::vector<uint32_t> values;
        for (int i = 0; i < N; i++) {
            values.push_back(rng.next()%M);
        }

        std::function<uint32_t(uint32_t,uint32_t)> func;
        uint32_t neutral;
        if (F == "MAX") {
            func = _max;
            neutral = 0;
        } else if (F == "MIN") {
            func = _min;
            neutral = 4294967295U;
        } else {
            func = _sum;
            neutral = 0;
        }

        SegmentTree T(values, func, neutral);

        for (int operation = 0; operation < W; operation++)
        {
            uint32_t OP = rng.next() % (Q + U);
            uint32_t result;
            if (OP < Q) {
                uint32_t l = rng.next() % N;
                uint32_t r = l + 1 + (rng.next() % (N - l));

                result = T.query(N, l, r);
            } else {
                uint32_t i = rng.next() % N;
                uint32_t v = rng.next() % M;

                T.update(N, i, v);
                result = T.query(N, i, N);
            }

            if (operation % P == 0) {
                cout << result << '\n';
            }
            
        }

        std::cout << '\n'; 
        caso++;
    }

    return 0;
}
