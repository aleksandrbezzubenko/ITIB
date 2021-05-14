import random
import numpy as np

class MyClass:
    def __init__(self, init_symbols, width, height):
        self.len = len(init_symbols[0])
        self.weights_matrix = np.zeros((self.len, self.len), dtype=int, order='C')
        self._init_weights_matrix(init_symbols)
        self.width = width
        self.height = height

    def _init_weights_matrix(self, init_symbols):
        self.len = len(init_symbols[0])
        for i in range(self.len):
            for j in range(i + 1, self.len):
                for k in range(len(init_symbols)):
                    self.weights_matrix[i][j] += init_symbols[k][i] * init_symbols[k][j]
                self.weights_matrix[j][i] = self.weights_matrix[i][j]
        for i in self.weights_matrix:
            print(i)

    def print_num(self, num, res):
        for i in range(self.height):
            s = ""
            for j in range(self.width):
                if num[i + j * self.height] == 1:
                    s += " ⬛"
                else:
                    s += "  "
            s += " ----- "
            for j in range(self.width):
                if res[i + j * self.height] == 1:
                    s += " ⬛"
                else:
                    s += "  "
            print(s)

    def output_result_synchronously(self, vec):
        res = np.dot(vec, self.weights_matrix)

        def find_net(i, j):
            if j == 0:
                return vec[i]
            return int(j / np.abs(j))

        return [find_net(i, j) for i, j in enumerate(res)]

    def final_conclusion(self, vec):
        while True:
            res = self.output_result_synchronously(vec)

            flag = True

            for i, j in zip(res, vec):
                if j != i:
                    flag = False

            if flag:
                break

            vec = res

        return vec


def get_new_vec_10_per(symbol):
    for i in range(int(len(symbol) / 10)):
        symbol[random.randint(0, len(symbol) - 1)] *= -1
    return symbol


def main():
    width = 5
    height = 7
    symbols = [[-1, -1, 1, 1, 1, 1, -1,
                -1, 1, -1, 1, -1, -1, 1,
                1, -1, -1, 1, -1, -1, 1,
                1, -1, -1, 1, -1, -1, 1,
                -1, -1, -1, -1, 1, 1, -1, ],

               [1, -1, -1, -1, -1, 1, -1,
                1, -1, -1, -1, 1, -1, -1,
                1, -1, -1, 1, -1, -1, -1,
                1, -1, 1, -1, -1, -1, -1,
                1, 1, -1, -1, -1, -1, -1],

               [-1, -1, -1, 1, 1, -1, -1,
                -1, -1, 1, -1, 1, -1, -1,
                -1, 1, -1, -1, 1, -1, -1,
                1, 1, 1, 1, 1, 1, 1,
                -1, -1, -1, -1, 1, -1, -1]
               ]

    lab = MyClass(symbols, width, height)
    print("Целевой тест")
    for i in symbols:
        lab.print_num(i, lab.final_conclusion(i))
        print("\n\n")

    print("Искаженный тест")
    for i in symbols:
        vec = get_new_vec_10_per(i)
        lab.print_num(vec, lab.final_conclusion(vec))
        print("\n\n")


main()
