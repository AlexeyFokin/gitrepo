from RosesField3 import RosesField

import random
import numpy as np
import timeit
from matplotlib import pyplot as plt


def static_init():
    global purchased_field
    global location_field
    global roses
    global costs

    global field_size
    global garden_size

    field_size = (6, 4)
    garden_size = (2, 3)

    purchased_field = np.array([
        [1, 0, 0, 0],
        [0, 0, 0, 0],
        [0, 0, 0, 0],
        [0, 0, 0, 0],
        [0, 0, 0, 1],
        [0, 0, 0, 1]
    ])

    roses = np.array([
        [100, 3, 2, 7],
        [0, 7, 0, 3],
        [2, 0, 4, 0],
        [0, 5, 0, 4],
        [3, 0, 9, 5],
        [0, 4, 0, 0]
    ])

    costs = np.array([
        [3, 7, 9, 1],
        [0, 4, 0, 2],
        [3, 5, 6, 0],
        [0, 1, 0, 7],
        [2, 0, 5, 0],
        [0, 3, 0, 0]
    ])

    location_field = costs


max_field_size = 30
max_garden_size = 3
while True:
    field_size = (random.randint(1, max_field_size),
                  random.randint(1, max_field_size))
    garden_size = (random.randint(1, max_garden_size),
                   random.randint(1, max_garden_size))
    if garden_size[0] <= field_size[0] and garden_size[1] <= field_size[1]:
        break

print(f'Generated field of size {field_size} [HxW]')
print(f'Generated garden of size {garden_size} [HxW]')


purchased_field = np.random.rand(field_size[0], field_size[1]) < 0.10  # probab
location_field = np.random.rand(field_size[0], field_size[1]) < 0.90

static_init()

y_coords, x_coords = np.where(purchased_field != 0)
purchased_squares = list(zip(y_coords, x_coords))

location_roses = dict()
location_costs = dict()

y_coords, x_coords = np.where(location_field != 0)
r = roses[roses > 0]
c = costs[costs > 0]
y_coords, x_coords = np.where(roses != 0)
for i, coord in enumerate(zip(y_coords, x_coords)):
    location_roses[coord] = r[i]  # random.randrange(1, 10)

y_coords, x_coords = np.where(costs != 0)
for i, coord in enumerate(zip(y_coords, x_coords)):
    location_costs[coord] = c[i]  # random.randrange(1, 10)



roses_field = RosesField(field_size[1], field_size[0],
                         purchased_squares,
                         location_roses,
                         location_costs,
                         garden_size[1], garden_size[0])

dt = []
r = range(3)
for i in r:
    rand1 = random.randint(1, 100)
    rand2 = random.randint(1, 100)
    rand3 = random.randint(1, 100)

    garden_size = (random.randint(1, max_garden_size),
                   random.randint(1, max_garden_size))
    print(f'Generated garden of size {garden_size} [HxW]')

    start_time = timeit.default_timer()

    # roses_field.set_garden_shape(garden_size[0], garden_size[1])
    r1 = roses_field.find_best_garden(rand1)
    r2 = roses_field.find_best_garden(rand2)
    r3 = roses_field.find_best_garden(rand3)
    r1 = 1
    r2 = 2
    r3 = 3
    rr = roses_field.find_best_garden(-10)
    dt.append(timeit.default_timer() - start_time)
    print(f'Number of possible positioning'
          f' {roses_field.no_cycles} {r1} {r2} {r3}')
    print(f'res {rr}')

plt.plot(dt)
#plt.show()
