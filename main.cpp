#include <iostream>
#include <vector>
#include <tuple>
using namespace std;

string A, B;
struct Polovina {
    char znak;
    int i, j;
    Polovina(char znak, int i, int j) : znak(znak), i(i), j(j) {}
};

pair<int, Polovina> iterativne4(int startA, int konecA, int startB, int konecB, int polovina=1) {
    int delkaA = konecA - startA + 1;
    int delkaB = konecB - startB + 1;
    vector<pair<int, Polovina>> predchozi(delkaB, make_pair(0, Polovina('0', 0, 0)));
    for (int i = 0; i < delkaA; ++i) {
        vector<pair<int, Polovina>> aktualni(delkaB, make_pair(0, Polovina('0', 0, 0)));
        for (int j = 0; j < delkaB; ++j) {
            int maximum = 0;
            int val1 = 0, val2 = 0, val3 = 0;
            Polovina polovina1('0', 0, 0);
            Polovina nova_polovina('0', 0, 0);
            if (j > 0) {
                tie(val1, polovina1) = predchozi[j-1];
                tie(val2, ignore) = aktualni[j-1];
                tie(val3, ignore) = predchozi[j];
            }
            if (A[i+startA] == B[j+startB]) {
                maximum = val1 + 1;
                nova_polovina = polovina1;
                if (maximum == polovina) {
                    nova_polovina = Polovina(A[i+startA], i+startA, j+startB);
                }
            }
            if (maximum < val3) {
                maximum = val3;
                nova_polovina = predchozi[j].second;
            }
            if (maximum < val2) {
                maximum = val2;
                nova_polovina = aktualni[j-1].second;
            }
            aktualni[j] = make_pair(maximum, nova_polovina);
        }
        predchozi = aktualni;
    }
    return predchozi[delkaB - 1];
}

struct Interval {
    int startA, konecA, startB, konecB, startCesty, konecCesty;
    Interval(int startA, int konecA, int startB, int konecB, int startCesty, int konecCesty)
            : startA(startA), konecA(konecA), startB(startB), konecB(konecB), startCesty(startCesty), konecCesty(konecCesty) {}
};

vector<char> nalezni_cestu() {
    pair<int, Polovina> result = iterativne4(0, A.size()-1, 0, B.size()-1);
    int delka_cesty = result.first;
    if (delka_cesty == 0) {
        return vector<char>();
    }
    vector<char> cesta(delka_cesty, 0);
    vector<Interval> intervaly { Interval(0, A.size()-1, 0, B.size()-1, 1, delka_cesty) };
    while (!intervaly.empty()) {
        vector<Interval> nove_intervaly;
        for (const auto& interval : intervaly) {
            int pozice_v_ceste = (interval.startCesty + interval.konecCesty) / 2;
            if (cesta[pozice_v_ceste-1] != 0) {
                continue;
            }
            int polovina = pozice_v_ceste - interval.startCesty + 1;
            Polovina stred = iterativne4(interval.startA, interval.konecA, interval.startB, interval.konecB, polovina).second;
            cesta[pozice_v_ceste-1] = stred.znak;
            nove_intervaly.emplace_back(interval.startA, stred.i, interval.startB, stred.j, interval.startCesty, pozice_v_ceste);
            nove_intervaly.emplace_back(stred.i, interval.konecA, stred.j, interval.konecB, pozice_v_ceste, interval.konecCesty);
        }
        intervaly = nove_intervaly;
    }
    char posledni = iterativne4(0, A.size()-1, 0, B.size()-1, delka_cesty).second.znak;
    cesta[delka_cesty - 1] = posledni;
    return cesta;
}

int main() {
    int N;
    cin >> N;
    cin >> A;
    cin >> N;
    cin >> B;
    vector<char> C = nalezni_cestu();
    for (char i : C)
        cout << i << endl;
    vector<char> vystup;
    short u1 = 0, u2 = 0, u3 = 0;
    vector<char> pridaniA, pridaniB;
    short pocetZmen = A.size() + B.size() - 2*C.size();
    while (u1 < A.size() && u2 < B.size()) {
        char val1 = A[u1];
        char val2 = B[u2];
        char val3 = C[u3];

        if (val1 != val3) {
            pridaniA.push_back(val1);
            ++u1;
        } else if (val2 != val3) {
            pridaniB.push_back(val2);
            ++u2;
        } else {
            if (!pridaniA.empty()) {
                vystup.push_back('{');
                vystup.push_back('-');
                vystup.insert(vystup.end(), pridaniA.begin(), pridaniA.end());
                vystup.push_back('}');
            }
            if (!pridaniB.empty()) {
                vystup.push_back('{');
                vystup.push_back('+');
                vystup.insert(vystup.end(), pridaniB.begin(), pridaniB.end());
                vystup.push_back('}');
            }
            vystup.push_back(val1);
            ++u1;
            ++u2;
            ++u3;
            pridaniA.clear();
            pridaniB.clear();
        }
    }
    vystup.pop_back();
    string output(vystup.begin(), vystup.end());
    cout << pocetZmen << endl;
    cout << output << endl;

    return 0;
}
