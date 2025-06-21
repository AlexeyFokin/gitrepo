# class RosesField ver 3
#
# There is a grid field [MxN] (M - row, N - column) that contains roses
# Also a garden sized [OxP] is defined on that field
# Some squares are already purchased and forbidden
# to be included in a garden to buy
# Find a garden with as many roses as possible,
# but not more than for a maximum budget
#
# time complexity
# __init__()            O(n^2)     n - max(field height, field width)
# set_garden_shape()    O(n^2)     n - number of possible suitable garden
# find_best_garden()    O(log(n))  n - number of possible suitable garden

import numpy as np
from collections import OrderedDict
from typing import Dict, List, Tuple


class RosesField:
    def __init__(self,
                 field_width: int, field_height: int,
                 purchased_squares: List[Tuple[int, int]],
                 location_roses: Dict[Tuple[int, int], int],
                 location_costs: Dict[Tuple[int, int], float],
                 garden_width: int, garden_height: int):
        self.field_width = field_width
        self.field_height = field_height

        self.vacant_field = np.zeros((self.field_height, self.field_width),
                                     dtype=np.int_)
        self.costs_field = np.zeros((self.field_height, self.field_width),
                                    dtype=np.int_)
        self.roses_field = np.zeros((self.field_height, self.field_width),
                                    dtype=np.int_)

        # convert coordinates and clip to the boundary of field
        purchased_squares = self.clip_coord(purchased_squares)
        self.vacant_field[purchased_squares] = 1

        location_costs_coord = self.clip_coord(location_costs.keys())
        self.costs_field[location_costs_coord] = \
            tuple(location_costs.values())

        location_roses_coord = self.clip_coord(location_roses.keys())
        self.roses_field[location_roses_coord] = \
            tuple(location_roses.values())

        # pre-processing for summation
        self.vacant_field_int = SubMatSum(self.vacant_field)
        self.costs_field_int = SubMatSum(self.costs_field)
        self.roses_field_int = SubMatSum(self.roses_field)

        self.garden_width = None
        self.garden_height = None

        # Ordered Dictionary to facilitate sorting of dictionary
        self.gardens = OrderedDict()
        self.no_cycles = None  # for debugging

        self.set_garden_shape(garden_width, garden_height)  # init garden

    class Garden:   # class is used only for grouping and addressing by name
        def __init__(self, coord, cost, roses):
            self.coord = coord
            self.cost = cost
            self.roses = roses
            self.coord_max_roses = list()

    def set_garden_shape(self, garden_width: int, garden_height: int):
        # time complexity O(n^2)
        self.garden_width = garden_width
        self.garden_height = garden_height

        self.no_cycles = 0
        self.gardens = OrderedDict()

        if(self.garden_height > self.field_height
                or self.garden_width > self.field_width):
            return

        for m in range(self.field_height - self.garden_height + 1):
            for n in range(self.field_width - self.garden_width + 1):
                vacant = self.vacant_field_int.sum(m, n,
                                                   self.garden_height,
                                                   self.garden_width)

                roses = self.roses_field_int.sum(m, n,
                                                 self.garden_height,
                                                 self.garden_width)

                if vacant != 0 or roses == 0:  # garden with no roses - skip
                    continue                   # overlaps with purchased - skip

                cost = self.costs_field_int.sum(m, n,
                                                self.garden_height,
                                                self.garden_width)

                g_coord = (m, n)

                garden = self.Garden(g_coord, cost, roses)
                self.gardens[g_coord] = garden  # full set of possible garden

                self.no_cycles += 1

        # empty set - no garden found
        # sort founded gardens in the decreased values of costs
        # to facilitate binary search O(n*log(n))
        self.gardens = OrderedDict(sorted(self.gardens.items(),
                                          key=lambda g: g[1].cost,
                                          reverse=True))

        # for each cost of all gardens find another garden with lower cost
        # but with maximum roses.
        # reverse iteration on preserved sorted order!
        # time complexity O(n)

        max_roses = -1
        max_coord = list()
        for coord, garden in reversed(list(self.gardens.items())):
            if garden.roses >= max_roses:
                max_coord = coord
                max_roses = garden.roses

            self.gardens[coord].coord_max_roses = max_coord

    # binary search, time complexity O(log(n))
    def find_best_garden(self, budget: float) -> Tuple:
        coord = (-1, -1)                                 # if not found
        garden_keys = list(self.gardens.keys())
        low = 0
        high = len(self.gardens) - 1

        while low <= high:
            mid = (high + low) // 2

            if self.gardens[garden_keys[mid]].cost > budget:
                low = mid + 1
            else:
                coord = self.gardens[garden_keys[mid]].coord_max_roses
                high = mid - 1

        return coord

    def clip_coord(self, coord_set):
        coord_set = list(zip(*coord_set))             # reorder coordinate set
        coord_set = np.clip(coord_set,
                            np.array([[0], [0]]),
                            np.array([[self.field_height-1], [self.field_width-1]]))
        return tuple(coord_set)

# class for matrix pre-calculation (2D integration)
# to facilitate deriving a sum of arbitrary box with time complexity O(1)
# based on
# https://www.techiedelight.com/calculate-sum-elements-sub-matrix-constant-time
class SubMatSum:
    def __init__(self, mat):
        # pre-process 2D matrix
        self.mat_p = np.cumsum(np.cumsum(mat, axis=1), axis=0)

    # summation of all element that in square coord=(Y,X), size=(MxN)
    def sum(self, y: int, x: int, m: int, n: int) -> float:

        # summa - mat_p[r][s] - mat_p[r][q-1] - mat_p[p-1][s] + mat_p[p-1][q-1]
        summa = self.mat_p[(y+m-1, x+n-1)]

        if x-1 >= 0:
            summa -= self.mat_p[(y+m-1, x-1)]

        if y-1 >= 0:
            summa -= self.mat_p[(y-1, x+n-1)]

        if y-1 >= 0 and x-1 >= 0:
            summa += self.mat_p[(y-1, x-1)]

        return summa
