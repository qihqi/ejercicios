// Seq tree supports 2 operations:
// 1. Modify one element
// 2. get sum of ranges
#include <vector>
#include <iostream>

struct SeqTree {

    SeqTree(const std::vector<int>& original)
    : n(original.size()), t(2*original.size())
    {
        std::cout << (t.size()) << std::endl;
        for (int i = 0; i < n ; i++) {
            t[i + n] = original[i];
        }
        for (int i  = n - 1; i > 0; --i) {
            t[i] = t[i << 1] + t[(i << 1) | 1];
        }
    }

    void modify(int p, int value) {
        int start = p + n;
        t[start] = value; // set init value
        for (; p > 1; p >>= 1) { // move up to parent
            t[p>>1] = t[p] + t[p^1]; // parent equals sum of children
        }
    }

    int query(int l, int r) {
        int res = 0;
        for (l += n, r += n; l < r; l>>=1, r>>=1) {
            if (l&1) res += t[l++];
            if (r&1) res += t[--r];
        }
        return res;
    }

    int n; // array size
    std::vector<int> t; // content of the tree, of size 2*n
};

template<typename T, typename COMP>
// COMP = function <T, T> -> T
struct SeqTree2 {

    SeqTree(const std::vector<T>& original, T init_val, COMP combiner)
    : n(original.size()), t(2*original.size()), init_val(init_val), combiner(combiner)
    {
        for (int i = 0; i < n ; i++) {
            t[i + n] = original[i];
        }
        for (int i  = n - 1; i > 0; --i) {
            t[i] = combiner(t[i << 1],  t[(i << 1) | 1]);
        }
    }

    void modify(int p, T value) {
        p = p + n;
        t[p] = value; // set init value
        for (; p > 1; p >>= 1) { // move up to parent
            t[p>>1] = combiner(t[p], t[p^1]); // parent equals sum of children
        }
    }

    T query(int l, int r) {
        auto res = init_val;
        for (l += n, r += n; l < r; l>>=1, r>>=1) {
            if (l&1) res = combiner(res, t[l++]);
            if (r&1) res = combiner(res, t[--r]);
        }
        return res;
    }

    int n; // array size
    std::vector<T> t; // content of the tree, of size 2*n
    T init_val;
    COMP combiner;
};

using namespace std;
int main() {

    std::vector<int> source;
    for (int i = 0; i < 100; i++) {
        source.push_back(i);
    }
    SeqTree tree(source);
    tree.modify(0, 20);
    cout << tree.query(0, 10) << endl;

}