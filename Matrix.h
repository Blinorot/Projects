#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

template <typename T>
class Matrix {
   private:
    size_t n = 0;
    size_t m = 0;
    std::vector<std::vector<T>> data;

   public:
    Matrix(const std::vector<std::vector<T>>& v) {
        n = v.size();
        if (n != 0) {
            m = v[0].size();
        }
        data = v;
    }

    Matrix(const Matrix<T>& other) {
        n = other.n;
        m = other.m;
        data = other.data;
    }

    Matrix& operator=(const Matrix<T>& other) {
        if (this == &other) {
            return *this;
        }
        n = other.n;
        m = other.m;
        data = other.data;
        return *this;
    }

    std::pair<size_t, size_t> size() const { return {n, m}; }
    const std::vector<T>& operator[](size_t i) const { return data[i]; }
    std::vector<T>& operator[](size_t i) { return data[i]; }

    friend Matrix& operator+=(Matrix<T>& a, const Matrix<T>& b) {
        for (size_t i = 0; i < a.data.size(); ++i) {
            for (size_t j = 0; j < a.data[0].size(); ++j) {
                a[i][j] += b[i][j];
            }
        }
        return a;
    }

    friend Matrix operator+(const Matrix<T>& a, const Matrix<T>& b) {
        Matrix tmp = a;
        tmp += b;
        return tmp;
    }

    friend Matrix operator*(const Matrix<T>& a, const Matrix<T>& b) {
        assert(a.m == b.n);
        std::vector<std::vector<T>> c(a.n, std::vector<T>(b.m));
        for (size_t i = 0; i < a.n; ++i) {
            for (size_t j = 0; j < b.m; ++j) {
                for (size_t k = 0; k < a.m; ++k) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        return Matrix(c);
    }

    friend Matrix& operator*=(Matrix<T>& a, const Matrix<T>& b) {
        assert(a.m == b.n);
        Matrix c = a * b;
        a = c;
        return a;
    }

    template <typename Z>  // matrix *= scalar
    friend Matrix& operator*=(Matrix<T>& b, const Z& z) {
        for (size_t i = 0; i < b.data.size(); ++i) {
            for (size_t j = 0; j < b.data[0].size(); ++j) {
                b[i][j] *= z;
            }
        }
        return b;
    }

    template <typename Z>  // scalar * matrix
    friend Matrix operator*(const Z& z, const Matrix<T>& b) {
        Matrix tmp = b;
        tmp *= z;
        return tmp;
    }

    template <typename Z>  // matrix * scalar
    friend Matrix operator*(const Matrix<T>& b, const Z& z) {
        Matrix tmp = b;
        tmp *= z;
        return tmp;
    }

    friend std::ostream& operator<<(std::ostream& out, const Matrix<T>& m) {
        for (size_t i = 0; i < m.data.size(); ++i) {
            for (size_t j = 0; j < m.data[0].size(); ++j) {
                out << m[i][j] << (j < m.data[0].size() - 1 ? "\t" : "");
            }
            if (i < m.data.size() - 1) {
                out << '\n';
            }
        }
        return out;
    }

    Matrix& transpose() {
        std::vector<std::vector<T>> tmp;
        tmp.resize(m);
        for (auto& v : tmp) {
            v.resize(n);
        }
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                tmp[j][i] = data[i][j];
            }
        }
        *this = Matrix(tmp);
        return *this;
    }
    Matrix transposed() const {
        std::vector<std::vector<T>> tmp;
        tmp.resize(m);
        for (auto& v : tmp) {
            v.resize(n);
        }
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                tmp[j][i] = data[i][j];
            }
        }
        return Matrix(tmp);
    }
};
