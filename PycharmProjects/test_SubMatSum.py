from RosesField3 import SubMatSum
import numpy as np


# Test Code
if __name__ == '__main__':
    mat = np.array([[1, 2, 3, 4, 6],
                    [5, 3, 8, 1, 2],
                    [4, 6, 7, 5, 5],
                    [2, 4, 8, 9, 4]])

    sub_mat_box = SubMatSum(mat)

    tli = 2
    tlj = 2
    rbi = 2
    rbj = 3
    print("Query1:", sub_mat_box.sum(tli, tlj, rbi, rbj))  # 38

    tli = 0
    tlj = 0
    rbi = 2
    rbj = 2
    print("Query2:", sub_mat_box.sum(tli, tlj, rbi, rbj))  # 11

    tli = 1
    tlj = 2
    rbi = 3
    rbj = 2
    print("Query3:", sub_mat_box.sum(tli, tlj, rbi, rbj))  # 38
