#include <bits/stdc++.h>
using namespace std;

constexpr const int N = 5e4 + 5;
constexpr const int MAXN = N * 30 * 2;
constexpr const int MAX = 1e8;
constexpr const int MIN = 0;
namespace fhq {
    int sz[MAXN], v[MAXN], rnd[MAXN], son[MAXN][2], cnt;
    std::mt19937 rand(time(0));
    #define ls son[p][0]
    #define rs son[p][1]
    int newNode(int val) {
        v[++cnt] = val;
        rnd[cnt] = rand();
        sz[cnt] = 1;
        return cnt;
    }
    void up(int p) {
        sz[p] = sz[ls] + sz[rs] + 1;
    }
    void split(int p, int k, int& x, int& y) {
        if (!p) {
            x = y = 0;
            return;
        }
        if (v[p] <= k) {
            x = p;
            split(rs, k, rs, y);
        } else {
            y = p;
            split(ls, k, x, ls);
        }
        up(p);
    }
    int merge(int A, int B) {
        if (!A || !B) return A | B;
        if (rnd[A] > rnd[B]) {
            son[A][1] = merge(son[A][1], B);
            up(A);
            return A;
        } else {
            son[B][0] = merge(A, son[B][0]);
            up(B);
            return B;
        }
    }
    int insert(int root, int a) {
        int x, y;
        split(root, a, x, y);
        return merge(merge(x, newNode(a)), y);
    }
    int del(int root, int a) {
        int x, y, z;
        split(root, a, x, z);
        split(x, a - 1, x, y);
        y = merge(son[y][0], son[y][1]);
        return merge(merge(x, y), z);
    }
    // start with zero
    int rank(int& root, int l, int r, int a = MAX) {
        int res = 0;
        int x, y, z;
        split(root, r, x, z);
        split(x, l - 1, x, y);
        if (a == MAX) {
            int res = sz[y];
            root = merge(merge(x, y), z);
            return res;
        }
        int p = y;
        while (p) {
            if (v[p] < x) {
                res += sz[ls] + 1;
                p = rs;
            } else {
                p = ls;
            }
        }
        root = merge(merge(x, y), z);
        return res;
    }

    #undef ls
    #undef rs
}

namespace seg {
    int son[MAXN][2], rt[MAXN], cnt, root;
    #define ls son[p][0]
    #define rs son[p][1]
    #define MID int mid = l + r >> 1

    void update(int l, int r, int pos, int d, int op, int &p) {
        if (!p) {
            p = ++cnt;
        }
        if (op == 1) {
            rt[p] = fhq::insert(rt[p], pos);
        } else {
            rt[p] = fhq::del(rt[p], pos);
        }
        if (l == r) {
            return;
        }
        MID;
        if (d <= mid) update(l, mid, pos, d, op, ls);
        else update(mid + 1, r, pos, d, op, rs);
    }

    int rank(int l, int r, int L, int R, int d, int p) {
        if (!p) return 0;
        if (l == r) {
            return 0;
        }
        MID;
        if (d <= mid) return rank(l, mid, L, R, d, ls);
        else return fhq::rank(rt[ls], L, R) + rank(mid + 1, r, L, R, d, rs);
    }

    int kth(int l, int r, int L, int R, int k, int p) {
        if (l == r) {
            return l;
        }
        MID;
        int tmp = fhq::rank(rt[ls], L, R);
        // cout << l << " " << r << " " << L << " " << R << " cnt: " << tmp << " sz: " << fhq::sz[rt[ls]] << endl;
        if (tmp < k) return kth(mid + 1, r, L, R, k - tmp, rs);
        else return kth(l, mid, L, R, k, ls);
    }

    void build(int a[], int n) {
        for (int i = 1; i <= n; i++) {
            update(MIN, MAX, i, a[i], 1, root);
        }
    }

    int pre(int l, int r, int L, int R, int d, int p) {
        if (!p) return -1;
        if (!fhq::rank(rt[p], L, R)) return -1;
        if (l == r) {
            return l;
        }
        MID;
        if (d <= mid + 1) return pre(l, mid, L, R, d, ls);
        else {
            int res = pre(mid + 1, r, L, R, d, rs);
            if (res == -1) {
                res = pre(l, mid, L, R, d, ls);
            }
            return res;
        }
    }

    int nxt(int l, int r, int L, int R, int d, int p) {
        if (!p) return -1;
        if (!fhq::rank(rt[p], L, R)) return -1;
        if (l == r) {
            return l;
        }
        MID;
        // cout << l << " " << r << " " << d << " " << " tmp " << fhq::rank(rt[p], L, R) << endl;
        if (d >= mid) return nxt(mid + 1, r, L, R, d, rs);
        else {
            int res = nxt(l, mid, L, R, d, ls);
            if (res == -1) {
                res = nxt(mid + 1, r, L, R, d, rs);
            }
            return res;
        }
    }
    

    #undef ls
    #undef rs
    #undef MID
}
int a[N];
#define endl '\n'
int main () {
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++) cin >> a[i];
    seg::build(a, n);
    while (m--) {
        int opt, l, r, pos, k;
        cin >> opt;
        // ++count;
        if (opt == 1) {
            cin >> l >> r >> k;
            cout << seg::rank(MIN, MAX, l, r, k, seg::root) + 1 << endl;
        } else if (opt == 2) {
            cin >> l >> r >> k;
            cout << seg::kth(MIN, MAX, l, r, k, seg::root) << endl;
        } else if (opt == 3) {
            cin >> pos >> k;
            seg::update(MIN, MAX, pos, a[pos], -1, seg::root);
            seg::update(MIN, MAX, pos, a[pos] = k, 1, seg::root);
        } else if (opt == 4) {
            cin >> l >> r >> k;
            int res = seg::pre(MIN, MAX, l, r, k, seg::root);
            cout << (res == -1 ? -2147483647 : res) << endl;
        } else if (opt == 5) {
            cin >> l >> r >> k;
            int res = seg::nxt(MIN, MAX, l, r, k, seg::root);
            cout << (res == -1 ? 2147483647 : res) << endl;
        }
    }
}