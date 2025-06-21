# Find piece of garden with as many roses as possible, but not more than for a maximum budget

import numpy as np


class RosesField:
    def __init__(self, field_width, field_height, purchased_squares, location_roses, location_costs,
                 garden_width, garden_height):
        self.field_width = field_width
        self.field_height = field_height

        self.vacant_field = np.zeros((self.field_height, self.field_width), dtype=np.int_)
        self.costs_field = np.zeros((self.field_height, self.field_width), dtype=np.int_)
        self.roses_field = np.zeros((self.field_height, self.field_width), dtype=np.int_)

        for purchased in purchased_squares:
            self.vacant_field[purchased] = 1

        for key, value in location_costs.items():
            self.costs_field[key] = value

        for key, value in location_roses.items():
            self.roses_field[key] = value

        self.garden_width = None
        self.garden_height = None
        self.costs = None

        self.set_garden_shape(garden_width, garden_height)

    def set_garden_shape(self, garden_width, garden_height):
        self.garden_width = garden_width
        self.garden_height = garden_height

        costs = list()
        # check that solution is not exist
        for m in range(self.field_height - self.garden_height + 1):
            for n in range(self.field_width - self.garden_width + 1):
                vacant = self.vacant_field[m:m+self.garden_height, n:n+self.garden_width].sum()
                if vacant != 0:           # if our garden overlaps with purchased squares
                    continue

                roses = self.roses_field[m:m + self.garden_height, n:n + self.garden_width].sum()

                if vacant != 0 or roses == 0:       # if found garden with no roses - skip this garden
                    continue
                cost = self.costs_field[m:m + self.garden_height, n:n + self.garden_width].sum()

                # print(f'({m}, {n}) cost {cost}  #roses {roses}')
                costs.append([cost, roses, (m, n)])

        self.costs = sorted(costs, key=lambda x: x[0], reverse=True)
        self.costs = [item + [idx] for idx, item in enumerate(self.costs)]  # add index for tracking after sorting

        # build for each budged lookup table of indices of maximum roses for no more for that budget
        for index, (budget, roses, coord, index_l) in enumerate(self.costs):
            # index of the line that contains maximum number of roses
            max_index = sorted(self.costs[index:], key=lambda x: x[1], reverse=True)[0][3]
            self.costs[index].append(max_index)

    def find_best_garden(self, budget):
        coord = (-1, -1)                                    # if not found
        bugdet_list = [b for b, *_ in self.costs]
        for index, budget_in_q in enumerate(bugdet_list):
            if budget_in_q <= budget:
                index_to_max_roses = self.costs[index][4]
                coord = self.costs[index_to_max_roses][2]
                break
        #print(coord)
        return coord
