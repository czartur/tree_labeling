#include <bits/stdc++.h>
using namespace std;

#define db(x) cerr << #x << " == " << x << endl
#define dbs(x) cerr << x << endl
#define st first
#define nd second
#define pb push_back
#define mp make_pair
#define cl(x,y) memset(x, y, sizeof(x))
#define endl '\n'

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<int, pii> piii;
typedef vector<int> vi;

const int inf = 0x3f3f3f3f;
const ll linf = 0x3f3f3f3f3f3f3f3f;
const int mod = 1e9+7;
const int N = 5e5+5;

vector <int> adj[N];
queue <int> level[N];
int label[N], mimic[N], height[N];
int n, l, h_max, root;

int getlabel(string s){
    int ans = 0;
    if(s[0] == '$') return ans;
    for(auto c : s)
        ans += 1<<(c-'A');
    return ans;
}

int getcount(int n){
    int ans = 0;
    while(n){
        ans += n&1;
        n = n>>1;
    }
    return ans;
}

void bfs(int root){
    queue<pair<int,int>> q;
    q.push({root,1});

    while(!q.empty()){
        int u = q.front().first;
        int h = q.front().second;
        q.pop();

        height[u] = h; 
        level[h].push(u);
        h_max = max(h_max, h);

        for(auto v : adj[u]){
            if(height[v]) continue;
            q.push({v, h+1});
        }
    }
}

int solve(){
    // itera nas alturas
    int ans = 0;
    for(int cur_h = h_max - 1; cur_h > 0; cur_h--){
        while(!level[cur_h].empty()){ // itera no nivel
            int u = level[cur_h].front();
            level[cur_h].pop();
            if((int) adj[u].size() == 1) continue; // folha

            // NAGAI PINTA
            for(int p=0; (1<<p) <= 'Z'-'A'; p++){ // itera nas letras
                int l0 = 0, l1 = 0;                
                for(auto v : adj[u]) {
                    if(height[v] == cur_h - 1) continue; // pai de u
                    if((1<<p) & mimic[v]) continue; // v é 2 para a letra ~ 'p'

                    // incrementa l0 ou l1
                    ((1<<p) & label[v]) ? l1++ : l0++;
                }
                
                // pinta de 1 ou 0 na letra
                if(l1 > l0) label[u] += (1<<p);
                if(l1 == l0) mimic[u] += (1<<p);
            }
            
            // NAGAI CONTA
            for(auto v : adj[u]){
                if(height[v] == cur_h - 1) continue;
                
                // calculate edge value : if cor(v) = 2 --> 0, else --> xor(u,v)  
                ans += getcount((~mimic[v])&(label[u]^label[v]));
            }
        }
    }
    return ans;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> l;
    for(int i=0; i<n-1; i++){
        int u, v;
        cin >> u >> v;
        adj[u].pb(v);
        adj[v].pb(u);
    }

    while(l--){
        int u; cin >> u;
        string s; cin >> s;
        label[u] = getlabel(s);
    }

    root = 1; 
    // force start at non leaf (if n > l)
    if(n>l) while((int) adj[root].size() == 1) root++;
    
    bfs(root);
    
    cout << solve() << endl;
}