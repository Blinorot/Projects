#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
template <typename T>
class Polynomial {
   private:
    std::vector<T> data;

    Polynomial norm() {
        while (data.size() != 1 && data[data.size() - 1] == T(0)) {
            data.pop_back();
        }
        return *this;
    }

   public:
    Polynomial(const std::vector<T>& v) : data(v) {
        while (data.size() != 1 && data[data.size() - 1] == T(0)) {
            data.pop_back();
        }
    }
    Polynomial(const T& c = T()) {
        data.resize(1);
        data[0] = c;
    }
    template <typename Iter>
    Polynomial(Iter beg, Iter end) {
        std::copy(beg, end, std::inserter(data, data.begin()));
        while (data.size() != 1 && data[data.size() - 1] == T(0)) {
            data.pop_back();
        }
    }
    Polynomial(const Polynomial& other) { data = other.data; }
    Polynomial& operator=(const Polynomial& other) {
        if (this == &other) {
            return *this;
        }
        data = other.data;
        return *this;
    }

    size_t size() const { return data.size(); }

    const T operator[](size_t i) const {
        if (i >= data.size()) {
            return 0;
        }
        return data[i];
    }

    friend bool operator==(const Polynomial& a, const Polynomial& b) {
        if (a.data.size() != b.data.size()) {
            return false;
        }
        for (size_t i = 0; i < a.data.size(); ++i) {
            if (a[i] != b[i]) {
                return false;
            }
        }
        return true;
    }
    friend bool operator!=(const Polynomial& a, const Polynomial& b) { return !(a == b); }

    friend Polynomial operator+(const Polynomial& a, const Polynomial& b) {
        auto c = a.data;
        if (a.size() < b.size()) {
            c.resize(b.size());
        }
        for (size_t i = 0; i < b.size(); ++i) {
            c[i] += b[i];
        }
        return Polynomial(c).norm();
    }
    friend Polynomial& operator+=(Polynomial& a, const Polynomial& b) {
        a = a + b;
        return a;
    }

    friend Polynomial operator-(const Polynomial& a, const Polynomial& b) {
        auto c = a.data;
        if (a.size() < b.size()) {
            c.resize(b.size());
        }
        for (size_t i = 0; i < b.size(); ++i) {
            c[i] -= b[i];
        }
        return Polynomial(c).norm();
    }
    friend Polynomial& operator-=(Polynomial& a, const Polynomial& b) {
        a = a - b;
        return a;
    }
    friend Polynomial operator*(const Polynomial& a, const Polynomial& b) {
        std::vector<T> c;
        c.resize(a.data.size() + b.data.size());
        for (size_t i = 0; i < a.size(); ++i) {
            for (size_t j = 0; j < b.size(); ++j) {
                c[i + j] += a[i] * b[j];
            }
        }
        return Polynomial(c).norm();
    }
    friend Polynomial& operator*=(Polynomial& a, const Polynomial& b) {
        a = a * b;
        return a;
    }

    int Degree() const {
        if (data.size() == 1 && data[0] == T(0)) {
            return -1;
        } else {
            return data.size() - 1;
        }
    }

    T operator()(const T& x) const {
        T answer = T();
        T deg = T(1);
        for (size_t i = 0; i < data.size(); ++i) {
            answer += data[i] * deg;
            deg *= x;
        }
        return answer;
    }
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }

    friend std::ostream& operator<<(std::ostream& out, const Polynomial& p) {
        bool was = false;
        if (p.size() == 0) {
            return out;
        }
        for (int i = p.data.size() - 1; i >= 0; i--) {
            if (p[i] == T(0)) {
                if (i == 0 && !was) {
                    out << T(0);
                }
                continue;
            }
            if (p[i] == T(-1)) {
                was = true;
                if (i > 0) {
                    out << "-x";
                    if (i > 1) {
                        out << "^" << i;
                    }
                } else {
                    out << T(-1);
                }
                continue;
            }
            if (p[i] < T(0)) {
                was = true;
                if (i > 0) {
                    out << p[i] << "*x";
                    if (i > 1) {
                        out << "^" << i;
                    }
                } else {
                    out << p[i];
                }
                continue;
            }
            if (was) {
                was = true;
                if (i > 0) {
                    if (p[i] == T(1)) {
                        out << "+x";
                    } else {
                        out << "+" << p[i] << "*x";
                    }
                    if (i > 1) {
                        out << "^" << i;
                    }
                } else {
                    out << "+" << p[i];
                }
                continue;
            } else {
                was = true;
                if (i > 0) {
                    if (p[i] == T(1)) {
                        out << "x";
                    } else {
                        out << p[i] << "*x";
                    }
                    if (i > 1) {
                        out << "^" << i;
                    }
                } else {
                    out << p[i];
                }
                continue;
            }
        }
        return out;
    }

   private:
    friend Polynomial Pow(const Polynomial& b, int i, std::vector<Polynomial<T>>& power) {
        if (power[i] != Polynomial()) {
            return power[i];
        }
        if (i % 2 == 0) {
            power[i] = Pow(b, i / 2, power) * Pow(b, i / 2, power);
        }
        if (i % 2 == 1) {
            power[i] = b * Pow(b, i / 2, power) * Pow(b, i / 2, power);
        }
        return power[i];
    }

   public:
    friend Polynomial operator&(const Polynomial& a, const Polynomial& b) {
        auto tmp = Polynomial();
        std::vector<Polynomial<T>> power;
        power.resize(a.size());
        power[0] = Polynomial(T(1));
        for (size_t i = 0; i < a.size(); ++i) {
            tmp += a[i] * Pow(b, i, power);
        }
        return tmp.norm();
    }

    friend Polynomial operator/(const Polynomial& a, const Polynomial& b) {
        assert(b != Polynomial());
        if (a.Degree() == -1) {
            return Polynomial();
        }
        Polynomial r = a;
        r.norm();
        Polynomial q;
        std::vector<Polynomial<T>> degs;
        degs.resize(a.size() + 1);
        degs[0] = Polynomial(1);
        std::vector<T> x_vec;
        x_vec.resize(2);
        x_vec[1] = T(1);
        auto x = Polynomial(x_vec);
        while (r.Degree() >= b.Degree()) {
            auto coef = r[r.size() - 1] / b[b.size() - 1];
            auto deg = Pow(x, r.Degree() - b.Degree(), degs);
            q += coef * deg;
            r -= coef * b * deg;
            r.norm();
        }
        return q.norm();
    }
    friend Polynomial operator%(const Polynomial& a, const Polynomial& b) {
        if (a.Degree() == -1) {
            return b;
        }
        if (b.Degree() == -1) {
            return b;
        }
        Polynomial r = a;
        r.norm();
        Polynomial q;
        std::vector<Polynomial<T>> degs;
        degs.resize(a.size() + 1);
        degs[0] = Polynomial(1);
        std::vector<T> x_vec;
        x_vec.resize(2);
        x_vec[1] = T(1);
        auto x = Polynomial(x_vec);
        while (r.Degree() >= b.Degree()) {
            auto coef = r[r.size() - 1] / b[b.size() - 1];
            auto deg = Pow(x, r.Degree() - b.Degree(), degs);
            q += coef * deg;
            r -= coef * b * deg;
            r.norm();
        }
        return r.norm();
    }

    friend Polynomial gcd(const Polynomial& a, const Polynomial& b) {
        if (b == Polynomial()) {
            return a;
        }
        return gcd(b, a % b);
    }

    friend Polynomial operator,(const Polynomial& a, const Polynomial& b) {
        auto ans = (gcd(a, b).norm()).data;
        if (ans.size() == 1 && ans[0] == T(0)) {
            return Polynomial(ans);
        }
        auto coef = ans[ans.size() - 1];
        for (size_t i = 0; i < ans.size(); ++i) {
            ans[i] /= coef;
        }
        return Polynomial(ans);
    }
};
