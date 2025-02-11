#include <bits/stdc++.h>
using namespace std;

int optuzio[500005];
int puta_optuzen[500005];
bool rijesen[500005];

int br_mafija = 0;

void dfs(int x, bool mafija) {
    if (rijesen[x]) return;
    rijesen[x] = 1;
    br_mafija += mafija;
    int y = optuzio[x];
    if (--puta_optuzen[y] == 0 || mafija)
        dfs(y, !mafija);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        scanf("%d", optuzio + i);
        --optuzio[i];
        ++puta_optuzen[optuzio[i]];
    }
    for (int i = 0; i < n; ++i)
        if (puta_optuzen[i] == 0)
            dfs(i, true);

    for (int i = 0; i < n; ++i)
        dfs(i, false);

    cout << br_mafija << "\n";
    return 0;
}
