#include <bits/stdc++.h>
#define ll long long

using namespace std;

class LGC {
        bool getSeed = true;
        ll A = 1664525;
        ll C = 1013904223;
        ll R = (ll) std::pow(2, 32);
    public:
        uint32_t currentRandom;

        LGC(uint32_t seed) {
            currentRandom = seed;
        }

        uint32_t next() {
            if (getSeed) {
                getSeed = false;
                return currentRandom;
            }
            currentRandom = (A * currentRandom + C) % R;
            return currentRandom;
        }
};

class vEB {
    public:
        ll u, m;
        ll min, max;
        vEB *summary;
        vEB *clusters;

        ll high(ll x) {
            return (ll)(x/this->m);
        }

        ll low(ll x) {
            return x % (ll)(this->m);
        }

        ll index(ll h, ll l) {
            return h * this->m + l;
        }
};

void _populate(vEB *V, ll r) {
    V->u = (1 << (1 << r));
    V->m = (1 << (1 << (r-1)));
    V->min = V->u;
    V->max = -1;

    if (V->u <= 2) {
        V->summary = nullptr;
        V->clusters = nullptr;
        return;
    }

    V->summary = new vEB;
    _populate(V->summary,r-1);
    V->clusters = new vEB[V->m];

    for (int i = 0; i < V->m; i++) {
        _populate(&(V->clusters[i]),r-1);
    }
    
}

ll _insert(vEB *V,ll x) {
    if (x == V->min or x == V->max) {
        return 0;
    }

    if (V->u == 2) {
        V->min = std::min(V->min,x);
        V->max = std::max(V->max,x);
        return 1;
    }

    if (V->min == V->u) {
        V->min = V->max = x;
        return 1;
    }

    if (x < V->min) {
        std::swap(V->min,x);
    }

    V->max = std::max(V->max,x);

    ll h = V->high(x);
    ll l = V->low(x);

    if (V->clusters[h].min == V->clusters[h].u) {
        _insert(V->summary, h);
    }

    ll result = _insert(&(V->clusters[h]),l);
    return (result == 0) ? 0 : 1 + result;
}


ll _successor(vEB* V, ll x)
{
    if (x < V->min) return V->min;
    
    if (x > V->max) return V->u;
    
    if (V->u == 2) {
        if (x == 0 && V->max == 1) return 1;
        else return 2;
    }

    ll h = V->high(x);
    ll l = V->low(x);

    if (l < V->clusters[h].max) {
        ll s = _successor(&(V->clusters[h]),l);
        return V->index(h,s);
    } else {
        h = _successor(V->summary,h);
        if (h == V->summary->u) return V->u;
        else {
            ll s = V->clusters[h].min;
            return V->index(h,s);
        }
    }

}


ll _delete(vEB *V, ll x)
{
    if (V->u == 2) {
        ll result = 0;
        if (x == V->min) {
            V->min = V->max != V->min ? V->max : V->u;
            result = 1;
        }
        if (x == V->max) {
            V->max = V->min != V->u ? V->min : -1;
            result = 1;
        }
        return result;
    }

    if (x == V->min) {
        ll i = V->summary->min;
        if (i == V->summary->u) {
            V->min = V->u;
            V->max = -1;
            return 1;
        } else {
            V->min = V->index(i,V->clusters[i].min);
            x = V->min;
        }
    }

    ll h = V->high(x);
    ll l = V->low(x);

    ll result = _delete(&(V->clusters[h]),l);

    if (V->clusters[h].min == V->clusters[h].u) {
        _delete(V->summary,h);
    }

    if (x == V->max) {
        ll i = V->summary->max;
        if (i == -1) V->max = V->min;
        else V->max = V->index(i,V->clusters[i].max);
    }

    return (result == 0) ? 0 : 1 + result;
}



int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll S, M, B, N, I, F, D, P;

    cin >> S >> M >> B >> N >> I >> F >> D >> P;

    LGC RNG(S);

    vEB T;
    _populate(&T,M);

    ll U = (1<<(1<<M));

    for (int burnIn = 0; burnIn < B; burnIn++) {
        _insert(&T,RNG.next() % U);
    }
    
    for (int oper = 0; oper < N; oper++) {
        char operChar;
        ll result;
        ll operProba = RNG.next() % (I+F+D);

        if (operProba < I) {
            operChar = 'I';
            result = _insert(&T,RNG.next() % U);
        } else if ((I <= operProba) and (operProba < I+F)) {
            operChar = 'S';
            result = _successor(&T,RNG.next() % U);
        } else if ((I+F) <= operProba) {
            operChar = 'D';
            ll Y = RNG.next() % U;
            ll succY = _successor(&T,Y);
            ll tempResult = succY != U ? succY : Y;
            result = _delete(&T, tempResult);
        } else {
            continue;
        }

        if (oper % P == 0) {
            cout << operChar << " " << result << '\n';
        }

    }
    

    return 0;
}
