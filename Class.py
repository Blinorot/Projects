import copy


def gcd(m, n):
    a, b = copy.copy(abs(m)), copy.copy(abs(n))
    while b:
        a, b = b, a % b
    return a


class Q():  # rational numbers

    def __init__(self, m, n=1):
        assert (n != 0), "division on zero"
        g = gcd(m, n)
        m //= g
        n //= g
        if n < 0:
            if m < 0:
                n, m = abs(n), abs(m)
            else:
                m *= -1
                n = abs(n)
        self.data = [m, n]

    def __getitem__(self, key):
        return self.data[key]

    def __add__(self, other):
        return Q(self[0] * other[1] + self[1] * other[0], self[1] * other[1])

    def __mul__(self, other):
        return Q(self[0] * other[0], self[1] * other[1])

    def __floordiv__(self, other):
        assert (other[0] != 0), "division on zero"
        return Q(self[0] * other[1], self[1] * other[0])


class Polynomial():

    def __init__(self, n):
        self.data = [Q(0)] * (n + 1)
        self.deg = n

    def One(self):
        result = Polynomial(0)
        result.data[0] = Q(1)
        return result

    def H(self, alpha):
        result = Polynomial(self.deg)
        for i in range(self.deg + 1):
            result[i] = alpha * self[i]
        return result

    def Monom(self):  # х ^ 1
        result = Polynomial(1)
        result[1] = Q(1)
        return result

    def __setitem__(self, key, val):
        self.data[key] = val

    def __getitem__(self, key):
        return self.data[key]

    def __mul__(self, other):
        result = Polynomial(self.deg + other.deg)
        for i in range(self.deg + 1):
            for j in range(other.deg + 1):
                result[i + j] += self[i] * other[j]
        return result

    def __add__(self, other):
        result = Polynomial(max(self.deg, other.deg))
        for i in range(self.deg + 1):
            result[i] += self[i]
        for j in range(other.deg + 1):
            result[j] += other[j]
        return result


class Permutation():

    def Sign(self):
        mass = self.data
        amount = 0
        for i in range(len(mass)):
            for j in range(i + 1, len(mass)):
                if mass[i] > mass[j]:
                    amount += 1
        if amount % 2 == 0:
            return 1
        return -1

    def __init__(self, mass):
        self.data = mass
        self.sign = self.Sign()

    def __getitem__(self, key):
        return self.data[key]

    def __mul__(self, per):  # self * per
        result = [0] * len(self)
        for i in range(len(self)):
            result[i] = self.data[per.data[i] - 1]
        result = Permutation(result)
        return result

    def __pow__(self, k):
        result = copy.copy(self)
        for _ in range(k - 1):
            result *= self
        return result

    def __len__(self):
        return len(self.data)

    def __eq__(self, per):
        return self.data == per.data


def Generator(per):
    if len(per) == 0:
        return [[]]
    prev = Generator(per[:-1])
    result = []
    for p in prev:
        for i in range(len(per)):
            result.append(p[:i] + [per[-1]] + p[i:])
    return result


class Matrix(object):

    def __init__(self, n, m):  # indexes from 0
        self.data = [0] * n * m
        self.lin = n
        self.col = m

    def __getitem__(self, key):  # key is a pair (f, s)
        assert (self.lin > key[0]), "out of lines"
        assert (self.col > key[1]), "out of columns"
        return self.data[key[0] * self.lin + key[1]]

    def __setitem__(self, key, val):
        assert (self.lin > key[0]), "out of lines"
        assert (self.col > key[1]), "out of columns"
        self.data[key[0] * self.lin + key[1]] = val

    def Tr(self):  # transpose
        new_self = Matrix(self.col, self.lin)
        for i in range(self.lin):
            for j in range(self.col):
                new_self[(j, i)] = self[(i, j)]
        return new_self

    def No_i_j(self, i, j):  # matice without i-line and j-column
        new_self = Matrix(self.lin - 1, self.col - 1)
        for a in range(self.lin):
            for b in range(self.col):
                if a < i:
                    if b < j:
                        new_self[(a, b)] = self[(a, b)]
                    if b > j:
                        new_self[(a, b - 1)] = self[(a, b)]
                elif a > i:
                    if b < j:
                        new_self[(a - 1, b)] = self[(a, b)]
                    if b > j:
                        new_self[(a - 1, b - 1)] = self[(a, b)]
        return new_self

    def Det(self):
        assert (self.col == self.lin), "det undefined"
        n = self.lin
        det = Polynomial(0)
        perList = [Permutation(elem) for elem in
                   Generator([i for i in range(n)])]
        for p in perList:
            s = Polynomial(0).One().H(Q(p.sign))
            for i in range(n):
                s *= self[(i, p[i])]
            det += s
        return det

    def Inv(self):  # search for inversed matrice
        assert (self.col == self.lin), "inversed doesnt exist"
        inv_self = Matrix(self.lin, self.col)
        det = self.Det()[0]
        for i in range(self.lin):
            for j in range(self.col):
                inv_self[(i, j)] = Q((-1) ** (i + j))
                inv_self[(i, j)] *= self.No_i_j(i, j).Det()[0]
                inv_self[(i, j)] = inv_self[(i, j)] // det

        inv_self = inv_self.Tr()
        return inv_self

    def H(self, alpha):
        result = Matrix(self.lin, self.col)
        for i in range(self.lin):
            for j in range(self.col):
                result[(i, j)] = self[(i, j)] * alpha
        return result

    def __add__(self, other):
        assert(self.lin == other.lin and self.col == other.col), "sum undef"
        result = Matrix(self.lin, self.col)
        for i in range(self.lin):
            for j in range(self.col):
                result[(i, j)] = self[(i, j)] + other[(i, j)]
        return result

    def __mul__(self, other):
        assert (self.col == other.lin), "mul undefined"
        result = Matrix(self.lin, other.col)
        for i in range(self.lin):
            for j in range(other.col):
                summury = Q(0)
                for k in range(self.col):
                    summury += self[(i, k)] * other[(k, j)]
                result[(i, j)] = summury
        return result
