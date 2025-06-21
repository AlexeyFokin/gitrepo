import unittest
from RosesField3 import RosesField


class TestRosesField(unittest.TestCase):

    def roses_field_init(self):
        purchased_squares = list()
        purchased_squares.append((0, 2))

        location_roses = dict()
        location_roses[(0, 0)] = 1
        location_roses[(0, 2)] = 10
        location_roses[(2, 0)] = 1
        location_roses[(2, 2)] = 5

        location_costs = dict()
        location_costs[(0, 1)] = 2
        location_costs[(0, 2)] = 1
        location_costs[(2, 0)] = 2
        location_costs[(2, 2)] = 3

        self.roses_field = RosesField(3, 3,
                                      purchased_squares,
                                      location_roses,
                                      location_costs,
                                      3, 1)

    def test_find_best_garden_1(self):
        self.roses_field_init()

        self.roses_field.set_garden_shape(3, 1)
        self.assertEqual(self.roses_field.find_best_garden(1), (-1, -1),
                         'Should be (-1, -1)')
        self.assertEqual(self.roses_field.find_best_garden(2), (-1, -1),
                         'Should be (-1, -1)')
        self.assertEqual(self.roses_field.find_best_garden(5), (2, 0),
                         'Should be (2, 0)')

    def test_find_best_garden_2(self):
        self.roses_field_init()

        self.roses_field.set_garden_shape(2, 2)
        self.assertEqual(self.roses_field.find_best_garden(1), (-1, -1),
                         'Should be (-1, -1)')
        self.assertEqual(self.roses_field.find_best_garden(2), (0, 0),
                         'Should be (0, 0) or (1, 0)')
        self.assertEqual(self.roses_field.find_best_garden(5), (1, 1),
                         'Should be (1, 1)')


if __name__ == '__main__':
    unittest.main()
