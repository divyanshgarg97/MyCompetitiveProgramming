/*************************************************************************
 *                                                                       *
 *                    XXI Olimpiada Informatyczna                        *
 *                                                                       *
 *   Zadanie:              Turystyka                                     *
 *   Autor:                Marcin Andrychowicz                           *
 *   Zlozonosc czasowa:    O(4^t * (n+m))                                *
 *                         (t - dlugosc najdluzszej sciezki w grafie)    *
 *   Opis:                 Rozwiazanie wolne                             *
 *                                                                       *
 *************************************************************************/

#include <cstdio>
#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;
typedef long long LL;
#define REP(i,n) for(int i=0; i<(n); i++)
#define FORE(i,x) for(__typeof((x).begin()) i=(x).begin(); i != (x).end(); ++i)

const int MAXT = 10;
const int MAXN = 20000;
const int INF = 1000000000;
const int MASKS = 1<<(MAXT);

int n,m,c[MAXN+7]; // wartosci z wejscia
vector<int> G[MAXN+7]; // graf
int t; // rozmiar obecnie trzymanego zbioru
int act[MASKS+7][MASKS+7]; // wartosci dynamika w obecnym kroku
int nxt[MASKS+7][MASKS+7]; // nowe wartosci dynamika
int where[MAXN+7]; // na jakiej pozycji na strosie snajduje sie dany wierzcholek, -1 jesli nie ma
int res; // wynik
bool vis[MAXN+7]; // tablica odwiedzonech wierzcholkow dla DFSa

inline void mini(int& a,int b) {
    a = min(a,b);
}

// dodanie lub usuniecie elementu ze zbioru
void change(int v, bool add) {
//	cout << "change " << v << " " << add << endl;
    // przygotowanie
    int one = (1<<(add ? t : t-1));
    int newt = t + (add ? 1 : -1);
    REP(X,1<<newt) REP(Y,1<<newt)
    nxt[X][Y] = INF;
    // wykonanie operacji
    if(add) {
        int nei_mask = 0;
        FORE(w, G[v])
        if(where[*w] != -1)
            nei_mask |= (1 << where[*w]);
//		cout << "nei " << nei_mask << endl;
        where[v] = t;
        REP(X,1<<t) REP(Y,1<<t) {
            bool covered = (bool)(X&nei_mask);
            mini(nxt[X][Y|(covered ? one : 0)], act[X][Y]); // nie bierzemy v
            mini(nxt[X|one][Y|one|nei_mask], act[X][Y] + c[v]); // bierzemy v
        }
    } else {
        assert(where[v] == t-1);
        where[v] = -1;
        REP(X,1<<t) REP(Y,1<<t) {
            if(Y&one)
                mini(nxt[X&(~one)][Y&(~one)], act[X][Y]);
        }
    }
    // kopiowanie nowych wartosci
    t = newt;
    assert(t <= MAXT);
    REP(X,1<<newt) REP(Y,1<<newt)
    act[X][Y] = nxt[X][Y];
}

void dfs(int v) {
    vis[v] = true;
    change(v, true);
    FORE(w, G[v])
    if(!vis[*w])
        dfs(*w);
    change(v, false);
}

int main() {
    std::ios_base::sync_with_stdio(0);
    // wczytanie wejscia
    cin >> n >> m;
    for(int i=1; i<=n; i++)
        cin >> c[i];
    while(m--) {
        int a,b;
        cin >> a >> b;
        G[a].push_back(b);
        G[b].push_back(a);
    }
    // rozwiazanie
    for(int i=1; i<=n; i++)
        where[i] = -1;
    for(int i=1; i<=n; i++)
        if(!vis[i]) {
            act[0][0] = 0;
            dfs(i);
            assert(t == 0);
            res += act[0][0];
        }
    // wypisanie wyniku
    cout << res << endl;
}
