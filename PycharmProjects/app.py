# for statement with two indices

import itertools

l1 = [1, 2, 3, 4, 5, 6, 7]
l2 = ["A", "B", "C", "D", "E", "F"]

for i, j in zip(l1, l2):
#for i, j in itertools.zip_longest(l1, l2):
    print(i, j)
