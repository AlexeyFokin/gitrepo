# class RosesField ver 3
# There is a field that contain roses [MxN] (M - row, N - column)
# Also a garden sized [OxP] is defined
# Some squares are already purchased and forbidden to be included in a garden to buy
# Find a garden with as many roses as possible, but not more than for a maximum budget

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

        self.vacant_field = np.zeros((self.field_height, self.field_width), dtype=np.int_)
        self.costs_field = np.zeros((self.field_height, self.field_width), dtype=np.int_)
        self.roses_field = np.zeros((self.field_height, self.field_width), dtype=np.int_)

        self.vacant_field[tuple(zip(*purchased_squares))] = 1
        location_costs_coord = list(location_costs.keys())
        self.costs_field[tuple(zip(*location_costs_coord))] = tuple(location_costs.values())
        location_roses_coord = list(location_roses.keys())
        self.roses_field[tuple(zip(*location_roses_coord))] = tuple(location_roses.values())

        self.garden_width = None
        self.garden_height = None
        self.gardens = OrderedDict()                # used Ordered Dictionary to facilitate sorting of dictionary

        self.set_garden_shape(garden_width, garden_height)

    class Garden:   # class is used only for grouping and addressing by name
        def __init__(self, coord, cost, roses):
            self.coord = coord
            self.cost = cost
            self.roses = roses
            self.coord_max_roses = list()

    def set_garden_shape(self, garden_width: int, garden_height: int):  # time complexity O(n^4)
        self.garden_width = garden_width
        self.garden_height = garden_height

        if(self.garden_height > self.field_height
                or self.garden_width > self.field_width):
            self.gardens = OrderedDict()
            return

        for m in range(self.field_height - self.garden_height + 1):
            for n in range(self.field_width - self.garden_width + 1):
                vacant = self.vacant_field[m:m+self.garden_height, n:n+self.garden_width].sum()
                roses = self.roses_field[m:m + self.garden_height, n:n + self.garden_width].sum()

                if vacant != 0 or roses == 0:       # if found garden with no roses - skip this garden
                    continue                        # if our garden in question overlaps with purchased squares
                cost = self.costs_field[m:m + self.garden_height, n:n + self.garden_width].sum()

                g_coord = (m, n)
                garden = self.Garden(g_coord, cost, roses)
                self.gardens[g_coord] = garden      # full set of possible garden (not purchased and with roses)

        # empty set - no garden found
        # sort founded gardens in the decreased values of costs to facilitate binary search
        self.gardens = OrderedDict(sorted(self.gardens.items(), key=lambda g: g[1].cost, reverse=True))

        # for each cost of all gardens find another garden with lower cost but with maximum roses.
        # iterate on preserved sorted order!
        # time complexity O(n^2) # TODO improve based on binary search tree
        for index, (coord, garden) in enumerate(self.gardens.items()):
            from_cost_and_lower = list(self.gardens.values())[index:]     # cut list from current cost and to lower end

            garden_with_max_roses = sorted(from_cost_and_lower, key=lambda g: g.roses, reverse=True)[0]
            self.gardens[coord].coord_max_roses = garden_with_max_roses.coord


    def find_best_garden(self, budget: float) -> Tuple:  # binary search, time complexity O(log(n))
        coord = (-1, -1)                                 # used if not found
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
