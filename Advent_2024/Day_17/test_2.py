l = []
import re


def func(x):
    return [
        *map(int, re.findall(r"-?\d+", x)),
    ]
    return str((x))


with open(f"input.txt") as f:
    for x in f.read().split("\n"):
        if x != "":
            l.extend(func(x))

a, b, c, *prog = l

# a = 2024
# prog = [0,1,5,4,3,0]


def run(prog, a, b, c):
    def combo(num):
        if num <= 3:
            return num
        if num == 4:
            return a
        if num == 5:
            return b
        if num == 6:
            return c

    o = []
    ip = 0
    while ip < len(prog):
        instr = prog[ip]
        operand = prog[ip + 1]
        if instr == 0:
            a = a // (2 ** combo(operand))
        if instr == 1:
            b = b ^ operand
        if instr == 2:
            b = combo(operand) % 8
        if instr == 3:
            if a != 0:
                ip = operand - 2
        if instr == 4:
            b = b ^ c
        if instr == 5:
            o.append(combo(operand) % 8)
        if instr == 6:
            b = a // (2 ** combo(operand))
        if instr == 7:
            c = a // (2 ** combo(operand))
        ip += 2
    return o


print(*run(prog, a, b, c), sep=",")


def rec(n, a):
    if n == -1:
        return a
    a <<= 3
    for x in range(8):
        result = run(prog, a + x, 0, 0)
        if result == prog[n:]:
            print(result, prog[n:])
            s = rec(n - 1, a + x)
            if s != -1:
                return s
    return -1


print(rec(len(prog) - 1, 0))
