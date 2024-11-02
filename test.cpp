#include <vector>
#include "Vector.h"
#include <cassert>
#include <iostream>

using std::vector;

template <class T>
void compare(const my_vector<T>& my, const vector<T>& v) {
    assert(my.empty() == v.empty());
    assert(my.size() == v.size());
    for (int i = 0; i < my.size(); ++i) {
        assert(my[i] == v[i]);
    }
    if (!my.empty()) {
        assert(my.front() == v.front());
        assert(my.back() == v.back());
    }
}

void test_constructor_copy_swap_clear() {
    my_vector<int> my[3] = {my_vector<int>(), my_vector<int>(5, 1337), {1, 2, 3, 4, 5, 6, 7}};
    vector<int> v[3] = {vector<int>(), vector<int>(5, 1337), {1, 2, 3, 4, 5, 6, 7}};

    for (int i = 0; i < 3; ++i)
        compare(my[i], v[i]);

    my_vector<int> my_copy = my[2];
    vector<int> v_copy = v[2];
    compare(my_copy, v_copy);

    my_vector<int> my_move = std::move(my_copy);
    compare(my_move, v_copy);

    my_vector<int> my_a = {1, 3, 3, 7};
    my_vector<int> my_b;
    my_b = my_a;
    vector<int> b = {1, 3, 3, 7};
    compare(my_b, b);

    my_vector<int> my_c;
    my_c = std::move(my_b);
    compare(my_c, b);

    my_c.clear();
    b.clear();
    compare(my_c, b);

    my_vector <int> my_d = {1, 2, 3};
    my_vector <int> my_e = {4, 5, 6};
    vector <int> d = {4, 5, 6};
    vector <int> e = {1, 2, 3};
    my_d.swap(my_e);
    compare(my_d, d);
    compare(my_e, e);

}

void test_resize_reserve_assign_shrink() {
    my_vector<int> my_a;
    my_a.resize(3);
    vector <int> a;
    a.resize(3);

    compare(my_a, a);

    my_a = {1, 2, 3, 4, 5};
    a = {1, 2, 3, 4, 5};
    my_a.resize(10);
    a.resize(10);
    compare(my_a, a);

    my_a.resize(3);
    a.resize(3);
    compare(my_a, a);

    my_a.reserve(6);
    assert(my_a.capacity() == 6);
    my_a.shrink_to_fit();
    assert(my_a.capacity() == 3);
}

void test_push_pop() {
    const int n = 10;
    int x[n] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int y[n] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    my_vector<int> my_a;
    vector <int> a;
    for (int i = 0; i < n; ++i) {
        my_a.push_back(x[i]);
        a.push_back(x[i]);
        compare(my_a, a);
    }
    for (int i = 0; i < 10; ++i) {
        my_a.pop_back();
        a.pop_back();
        compare(my_a, a);
    }

    my_vector<std::pair <int, int>> my_b;
    vector<std::pair <int, int>> b;
    for (int i = 0; i < 10; ++i) {
        my_b.emplace_back(x[i], y[i]);
        b.emplace_back(x[i], y[i]);
        compare(my_b, b);
    }
    for (int i = 0; i < 10; ++i) {
        my_b.pop_back();
        b.pop_back();
        compare(my_b, b);
    }
}

void test_insert_emplace_erase() {
    const int n = 10;
    int pos[n] = {0, 0, 1, 0, 3, 2, 6, 4, 2, 8};
    int x[n] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int y[n] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    my_vector <int> my_a;
    vector <int> a;
    for (int i = 0; i < n; ++i) {
        my_a.insert(my_a.begin() + pos[i], x[i]);
        a.insert(a.begin() + pos[i], x[i]);
        compare(my_a, a);
    }
    for (int i = n - 1; i >= 0; --i) {
        my_a.erase(my_a.begin() + pos[i]);
        a.erase(a.begin() + pos[i]);
        compare(my_a, a);
    }

    my_vector <std::pair <int, int>> my_b;
    vector <std::pair <int, int>> b;
    for (int i = 0; i < n; ++i) {
        my_b.emplace(my_b.begin() + pos[i], x[i], y[i]);
        b.emplace(b.begin() + pos[i], x[i], y[i]);
        compare(my_b, b);
    }
    for (int i = n - 1; i >= 0; --i) {
        my_b.erase(my_b.begin() + pos[i]);
        b.erase(b.begin() + pos[i]);
        compare(my_b, b);
    }
}

void test_iterators() {
    my_vector <int> my_a = {1, 2, 3, 4, 5, 6, 7};
    vector <int> a = {1, 2, 3, 4, 5, 6, 7};

    for (int i = 0; i <= 7; ++i) {
        for (int j = 0; j <= 7; ++j) {
            auto it_i = my_a.begin() + i;
            auto it_j = my_a.end() - j;

            if (i < 6) {
                assert(*it_i == a[i]);
            }
            if (j > 1) {
                assert(*it_j == a[7 - j]);
            }

            if (i == 7 - j) {
                assert(it_i == it_j && *it_i == *it_j);
            }
            if (i < 7 - j) {
                assert(it_i < it_j);
            }
            if (i > 7 - j) {
                assert(it_i > it_j);
            }
        }
    }

    const my_vector <int> const_my_a = {1, 2, 3, 4, 5, 6, 7};
    for (int i = 0; i <= 7; ++i) {
        for (int j = 0; j <= 7; ++j) {
            auto it_i = const_my_a.begin() + i;
            auto it_j = const_my_a.end() - j;

            if (i < 6) {
                assert(*it_i == a[i]);
            }
            if (j > 1) {
                assert(*it_j == a[7 - j]);
            }

            if (i == 7 - j) {
                assert(it_i == it_j && *it_i == *it_j);
            }
            if (i < 7 - j) {
                assert(it_i < it_j);
            }
            if (i > 7 - j) {
                assert(it_i > it_j);
            }
        }
    }
}

int main() {

    test_constructor_copy_swap_clear();
    test_resize_reserve_assign_shrink();
    test_push_pop();
    test_insert_emplace_erase();
    test_iterators();

    std::cout << "All tests passed" << std::endl;
    return 0;
}