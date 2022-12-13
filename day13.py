import pathlib
import functools

data = pathlib.Path("input/day13.txt").read_text()

def comp(a, b):
    if isinstance(a, list) and isinstance(b, list):
        for ea, eb in zip(a, b):
            r = comp(ea, eb)
            if r != 0:
                return r
        return len(a) - len(b)
    
    if isinstance(a, list) and isinstance(b, int):
        b = [b]
        return comp(a, b)
    if isinstance(a, int) and isinstance(b, list):
        a = [a]
        return comp(a, b)
    
    # print(f"End different value {a=} {b=}")
    return a - b

score = 0
for i, pairs in enumerate(data.split("\n\n")):
    a, b = pairs.split("\n")
    a = eval(a)
    b = eval(b)
    r = comp(a, b)
    # print(i, r)
    # print(a)
    # print(b)
    if r < 0:
        score += i + 1
print(score)

all_values = [eval(line) for line in data.split("\n") if line != ""]
all_values.append([[2]])
all_values.append([[6]])

# print("Sorted")
all_values.sort(key=functools.cmp_to_key(comp))
# print(*all_values, sep="\n")
print((1 + all_values.index([[2]])) * (1 + all_values.index([[6]])))