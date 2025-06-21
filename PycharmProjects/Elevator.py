import time

# Elevator Controller


class ExtFloorButton:
    """Represents a pair of UP and DOWN buttons for an elevator call"""
    def __init__(self):
        self.up_state = False
        self.down_state = False


class ElevatorController:
    """Controls entire elevator system"""

    def __init__(self, floors,
                 hw_ext_button,
                 hw_elevator,
                 algo):
        if floors < 2:
            raise ValueError('Must be 2 or more floors in the building')
        self.floors = floors
        self.hw_ext_button = hw_ext_button

        self.algo = algo

        self.ext_buttons = {}

        int_panel = {}
        for floor in range(1, floors+1):
            self.ext_buttons[floor] = ExtFloorButton()  # creating all necessary external buttons
            int_panel[floor] = IntElevButton()          # creating a panel of internal button

        # for debugging purpose
        int_panel[3].state = True
        int_panel[1].state = True
        int_panel[9].state = True
        self.ext_buttons[4].down_state = True
        self.ext_buttons[5].down_state = True

        initial_floor = 7
        self.elevator = Elevator(initial_floor, int_panel,
                                 min_floor=1, max_floor=floors,
                                 hw_elevator=hw_elevator)

        # in another thread here run buttons state update and displays update
        ###

    def run(self):
        command = self.algo.algo(self.elevator.get_current_floor(),
                                 self.elevator.get_int_panel(),
                                 self.ext_buttons
                                 )

        if command == DO_NOTHING:
            pass
        elif command == GO_UP:
            self.elevator.go_up()
        elif command == GO_DOWN:
            self.elevator.go_down()
        elif command == STOP_AND_OPEN:
            self.elevator.stop_and_open()

    def get_current_floor(self):
        return self.elevator.get_current_floor()


class IntElevButton:
    """Represents a single floor button inside an elevator"""
    def __init__(self):
        self.state = False

    def on(self):
        self.state = True

    def off(self):
        self.state = False


class Elevator:
    """Represents single car (elevator cabin)"""
    def __init__(self, current_floor, int_panel,
                 min_floor, max_floor, hw_elevator):
        self.int_panel = int_panel
        self.hw_elevator = hw_elevator
        self.current_floor = current_floor

        self.min_floor = min_floor
        self.max_floor = max_floor

        self.door_state = 'closed'

        hw_elevator.go_to_floor(current_floor)

        # start another thread for continuous internal panel (buttons) readings
        # update self.int_panel in background according to buttons state
        # update button lights according to button state

    def go_up(self):
        """move car up one floor if possible"""
        if self.current_floor >= self.max_floor:
            raise ValueError('Broken logic. The elevator already in higher position')

        self.door_state = 'closed'
        print('the door is closed')
        self.hw_elevator.close_door()

        fl = str(self.current_floor)
        print(fl + '->', end='')
        self.hw_elevator.go_up()
        self.current_floor += 1
        fl = str(self.current_floor)
        print(fl)

    def go_down(self):
        """move car down one floor if possible"""
        if self.current_floor >= self.max_floor:
            raise ValueError('Broken logic. The elevator already in higher position')

        self.door_state = 'closed'
        print('the door is closed')
        self.hw_elevator.close_door()

        fl = str(self.current_floor)
        print(fl + '->', end='')
        self.hw_elevator.go_down()
        self.current_floor -= 1
        fl = str(self.current_floor)
        print(fl)

    def stop_and_open(self):
        self.door_state = 'open'
        print('the door is opened')
        self.hw_elevator.open_door()

    def get_current_floor(self):
        return self.current_floor

    def get_int_panel(self):
        return self.int_panel


DO_NOTHING = 0
STOP_AND_OPEN = 1
GO_UP = 2
GO_DOWN = 3


class DispatchAlgo:
    """
    One of the possible dispatch algorithm - LOOK algorithm

        The algorithm analyses the button state, current floor position,
        previous direction and gives control command:
        2 - go_up, 3 - go_down, 1 - stop_and_open_door, 0 - do_nothing
    """
    def __init__(self):
        self.dir = None

    def algo(self, cur_floor, int_panel, ext_buttons):

        input_d = (cur_floor, int_panel, ext_buttons)
        # all calls are served, we may reset direction flag
        # the elevator is in a free state now (ready).
        if self.dir == 'up' and not self.is_exist_calls_upper(input_d) \
                or self.dir == 'down' and not self.is_exist_calls_lower(input_d):
            self.dir = None

        # conditions to stop and open the door
        if int_panel[cur_floor].state:
            int_panel[cur_floor].off()  # turn off internal button state
            return STOP_AND_OPEN

        # we reached highest or lowest call from outside.
        # open door independently of directions the caller wants to go
        if (ext_buttons[cur_floor].up_state or ext_buttons[cur_floor].up_state) and \
                self.dir is None:
            ext_buttons[cur_floor].up_state = False  # turn off external button UP and DOWN
            ext_buttons[cur_floor].up_state = False
            return STOP_AND_OPEN

        # stops only if outside caller wants to go in the same direction
        if self.dir == 'up' and ext_buttons[cur_floor].up_state:
            ext_buttons[cur_floor].up_state = False
            return STOP_AND_OPEN

        if self.dir == 'down' and ext_buttons[cur_floor].down_state:
            ext_buttons[cur_floor].down_state = False
            return STOP_AND_OPEN

        # check direction to go next
        if self.dir in ['up', None] and self.is_exist_calls_upper(input_d):
            self.dir = 'up'         # here direction may change
            return GO_UP

        if self.dir in ['down', None] and self.is_exist_calls_lower(input_d):
            self.dir = 'down'       # here direction may change
            return GO_DOWN

        return DO_NOTHING

    @staticmethod
    def is_exist_calls_upper(input_data):
        """
        Checks is there exist call above current floor:
            doesn't matter internal buttons pressed or external UL or DOWN
        """
        cur_floor, int_panel, ext_buttons = input_data
        for floor, button in int_panel.items():
            if button.state and floor > cur_floor:
                return True
        for floor, button in ext_buttons.items():
            if (button.up_state or button.down_state) and floor > cur_floor:
                return True
        return False

    @staticmethod
    def is_exist_calls_lower(input_data):
        """
        Checks is there exist call below current floor:
            doesn't matter internal buttons pressed or external UL or DOWN
        """
        cur_floor, int_panel, ext_buttons = input_data
        for floor, button in int_panel.items():
            if button.state and floor < cur_floor:
                return True
        for floor, button in ext_buttons.items():
            if (button.up_state or button.up_state) and floor < cur_floor:
                return True
        return False


class HwElevator:
    """HW handle for elevator electronics"""
    def close_door(self):
        pass

    def open_door(self):
        pass

    def go_up(self):
        pass

    def go_down(self):
        pass

    def go_to_floor(self, floor):
        pass

    def read_panel_state(self):
        pass

    def turn_on(self, floor_no):
        pass

    def turn_off(self, floor_no):
        pass


class HwExtButton:
    """HW handle for distributed external buttons up and down"""
    def read(self):
        """read all button state"""
        pass


def main():

    no_of_floors = 10

    hw_ext_button = HwExtButton()       # hw handle for reading external UP's and DOWN's
    hw_elevator = HwElevator()          # hw handle for controlling elevator

    # chose appropriate dispatch algorithm
    dispatch_algo = DispatchAlgo()

    ec = ElevatorController(floors=no_of_floors,
                            hw_ext_button=hw_ext_button,
                            hw_elevator=hw_elevator,
                            algo=dispatch_algo)

    # main control loop for elevator control
    print('Initially the elevator is at ' + str(ec.get_current_floor()) + ' floor')
    while True:
        ec.run()
        time.sleep(0.5)           # elevator's slow movement emulation
        if ec.get_current_floor() == 6:         # for debugging purpose
            ec.ext_buttons[10].down_state = True
            print('passenger on floor 10 press down')


if __name__ == "__main__":
    main()
