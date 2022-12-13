import pathlib
import functools

data = pathlib.Path("input/day13.txt").read_text()

cont = object()

def comp(a, b):
    if isinstance(a, list) and isinstance(b, list):
        if len(a) == len(b) == 0:
            return cont
        if len(a) == 0 and len(b) > 0:
            # print("End because b longer", a, b)
            return False
        if len(a) >= 0 and len(b) == 0:
            # print("End because a longer")
            return True

        r = comp(a[0], b[0])
        if r is not cont:
            return r
        return comp(a[1:], b[1:])
    
    if isinstance(a, list) and isinstance(b, int):
        b = [b]
        return comp(a, b)
    if isinstance(a, int) and isinstance(b, list):
        a = [a]
        return comp(a, b)
    
    # print("Did not match", a, b)
    if a == b:
        return cont
    # print(f"End different value {a=} {b=}")
    return a > b

score = 0
for i, pairs in enumerate(data.split("\n\n")):
    a, b = pairs.split("\n")
    a = eval(a)
    b = eval(b)
    r = comp(a, b)
    print(i, r)
    print(a)
    print(b)
    if not r:
        score += i + 1
print(score)

all_values = [eval(line) for line in data.split("\n") if line != ""]
all_values.append([[2]])
all_values.append([[6]])

print("Sorted")
all_values.sort(key=functools.cmp_to_key(lambda a, b:1 if comp(a, b) else -1))
print(*all_values, sep="\n")
print((1 + all_values.index([[2]])) * (1 + all_values.index([[6]])))