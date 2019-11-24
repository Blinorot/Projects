// Copyright 2019 P G

#include <iostream>
#include <vector>
using ll = long double;

struct matrix {
  unsigned m, n;
  std::vector<double> data;

  matrix(unsigned i, unsigned j) : m(i), n(j), data(i * j, 0){};

  double &operator()(unsigned i, unsigned j) { return data[i + j * m]; }
};

matrix tr(matrix &A) {
  matrix A_tr(A.n, A.m);
  for (ll i = 0; i < A.m; i++) {
    for (ll j = 0; j < A.n; j++) {
      A_tr(j, i) = A(i, j);
    }
  }
  return A_tr;
}

matrix mul(matrix &A, matrix &B) {
  matrix C(A.m, B.n);
  for (ll i = 0; i < A.m; i++) {
    for (ll j = 0; j < B.n; j++) {
      ll sum = 0;
      for (ll q = 0; q < A.n; q++) {
        sum += A(i, q) * B(q, j);
      }
      C(i, j) = sum;
    }
  }
  return C;
}

matrix H(double &a, matrix A) {
  matrix B = A;
  for (ll i = 0; i < A.m; i++) {
    for (ll j = 0; j < A.n; j++) {
      B(i, j) *= a;
    }
  }
  return B;
}

ll norm(matrix A) {
  ll sum = 0;
  for (ll i = 0; i < A.m; i++) {
    for (ll j = 0; j < A.n; j++) {
      sum += A(i, j) * A(i, j);
    }
  }
  return sum;
}

matrix sum(matrix &A, matrix B) {
  matrix C = A;
  for (ll i = 0; i < A.m; i++) {
    for (ll j = 0; j < A.n; j++) {
      C(i, j) = A(i, j) + B(i, j);
    }
  }
  return C;
}

matrix min_us(matrix &A, matrix B) {
  matrix C = A;
  for (ll i = 0; i < A.m; i++) {
    for (ll j = 0; j < A.n; j++) {
      C(i, j) = A(i, j) - B(i, j);
    }
  }
  return C;
}

int main() {
  ll n, m;

  std::cout << "Input matrice A(m X n) in (Ax = a)" << std::endl
            << "m, n, than m lines of A - one in each line" << std::endl
            << std::endl;
  std::cin >> n >> m;
  matrix A(n, m);

  for (ll i = 0; i < n; i++) {
    for (ll j = 0; j < m; j++) {
      std::cin >> A(i, j);
    }
  }
  matrix A_tr = tr(A);
  matrix B = mul(A_tr, A);
  ll norm_A = norm(A);

  std::cout << "Input a (Ах = a)" << std::endl
            << "m lines of a - one in each line" << std::endl
            << std::endl;
  matrix a(n, 1);
  for (ll i = 0; i < n; i++) {
    std::cin >> a(i, 0);
  }

  std::cout << "Input accuracy" << std::endl << std::endl;
  ll epsilon;
  std::cin >> epsilon;
  // alfa * norm_B * x = alfa * norm_A_tr * a);

  matrix ID(n, n);
  for (ll i = 0; i < n; i++) {
    ID(i, i) = 1;
  }

  double alfa = 1 / (norm_A * norm_A * 2);
  // double alfa = 0.0000000001;
  auto left_p = H(alfa, B);
  matrix b = H(alfa, mul(A_tr, a));
  matrix b_2 = min_us(ID, left_p);
  matrix x_1 = b;
  matrix x_2 = b;

  // ll check = 0;
  while (true) {
    /*
    for (ll i = 0; i < X.back().size(); i++){
        cout << X.back()[i][0] << " ";
    }
    cout << endl;
    */
    // check++; if(check>10){break;}
    x_2 = sum(b, mul(b_2, x_1));
    ll k = 0;
    // cout << x_1(0,0) << endl;
    double m = norm(min_us(b, mul(left_p, x_2)));
    if (m < alfa * alfa * epsilon * epsilon) {
      break;
    }
    x_1 = x_2;
  }
  std::cout << "Answer:" << std::endl;
  for (ll i = 0; i < x_2.m; i++) {
    std::cout << "X_" << i << " = " << x_2(i, 0) << std::endl;
  }
  std::cout << std::endl;
}
