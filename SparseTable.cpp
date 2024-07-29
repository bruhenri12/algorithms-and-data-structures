#include <bits/stdc++.h>

#define ui32 uint32_t
#define ll long long

using namespace std;

const ui32 R = 4294967295;
const ui32 A = 1664525;
const ui32 C = 1013904223;

// LGC RNG
class RNG {
    private:
        ui32 currentRandom;
        ui32 count = 0;
    public:
        RNG(ui32 seed) {
            this->currentRandom = seed;
        }
        ui32 next() {
            if (count == 0) {
                count++;
                return currentRandom;
            }
            currentRandom = (A*currentRandom+C)%R;
            return currentRandom;
        }
};


// Operation Functions

ll operMin(ll a, ll b) {
	return std::min(a,b);
};

ll operMax(ll a, ll b) {
	return std::max(a,b);
};

ll operSum(ll a, ll b) {
	return a+b;
};

// Sparse Table
class SparseTable
{
    public:
        typedef ll(*Mi)(ll,ll);
        ll rows, columns;
        ll** table;
        ll M;
        ll neutral;


        ll f1(ll X) {
            return X % M;
        }
        
        SparseTable(ll rows, ll columns) {
            this->rows = rows;
            this->columns = columns;
            this->table = new ll*[rows];

            for (int i = 0; i < rows; i++) {
                table[i] = new ll[columns];
            }
            
        };

        void print()
        {
            for (ll u = 0; u < rows; u++) {
                for (ll v = 0; v < columns; v++)
                    std::cout << table[u][v] << " ";
                std::cout << endl;
            }
            std::cout << endl;
        }

        void preProcess(Mi u)
        {
            ll k = 1;
            ll checkOverflow;

            for (ll i = 1; i < rows; i++) {
                for (int j = 0; j < columns; j++) {
                    checkOverflow = neutral;
                    if (j+k < columns) {
                        checkOverflow = table[i-1][j+k];
                    }
                    table[i][j] = u(table[i-1][j], checkOverflow);
                }
                k *= 2;
            }
            
        }

        void rangeQuery(ll l, ll r, Mi u) {
            ll ans = neutral;
            ll i;
            while (l < r) {
                i = floor(log2((ll)r-l));
                ans = u(ans,table[i][l]);
                l += std::pow(2,i);
            }
            
            
            std::cout << ans << '\n';
        }

        void rangeQueryOptimized(ll l, ll r, Mi u) {
            ll i = floor(log2((ll)r-l));
            std::cout << (ll)u(table[i][l],table[i][(int)(r-(1 << i))]) << '\n';
        }

        void update(ll j, ll v, Mi u) {

            ll k = 1;

            table[0][j] = v;

            for (ll i = 1; i < rows; i++) {
                for (ll l = std::max((ll)0,(ll)(j-(2*k)+1)); l <= j; l++) {
                    table[i][l] = u(table[i-1][l], table[i-1][l+k]);
                }
                k *= 2;
            }

        }
};



int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    ui32 S, N, O, Q, U;
    string F;

    ui32 testCase = 0;
    ll L, R, I, V, X;

    ll (*operF)(ll,ll);

    string input;
    while (std::cin >> S >> N >> F >> O >> Q >> U)
    {
        std::cout << "caso " << testCase << '\n';
        testCase++;

        RNG lgc(S);

        SparseTable st((ll)floor(log2(N)+1),(ll)N);
        st.M = (ll)4*N;
        
        if (F == "MIN") {
            operF = operMin;
            st.neutral = std::numeric_limits<ll>::max();
        } else if (F == "MAX") {
            operF = operMax;
            st.neutral = 0;
        } else if (F == "SUM") {
            operF = operSum;
            st.neutral = 0;
        }

        for (ll i = 0; i < N; i++) {
            st.table[0][i] = st.f1(lgc.next());
        }

        st.preProcess(operF);

        //st.print();

        for (ll i = 0; i < O; i++)
        {
            X = lgc.next();
            if ( X % (Q+U) < Q ) {
                L = lgc.next() % N;
                R = L + 1 + (lgc.next() % (N-L));
            } else {
                I = lgc.next() % N;
                V = lgc.next() % st.M;
                st.update(I,V,operF);
                L = I;
                R = N;
            }

            if (F == "SUM") {
                st.rangeQuery(L,R,operF);
            } else {
                st.rangeQueryOptimized(L,R,operF);
            }
            
        }

        std::cout << '\n';
    }

    
    return 0;
}

