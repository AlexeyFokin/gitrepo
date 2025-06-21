from manim import *

class MovingOrangeBall(Scene):
    def construct(self):
        # Create the orange ball
        ball = Circle(radius=25 / 100, color=ORANGE, fill_opacity=1)  # 50px diameter converted to Manim units (100px = 2 Manim units)
        ball.move_to(RIGHT * 1) # Start on the right side. 100px movement.

        # Create the animation
        animation = ball.animate.shift(LEFT * 2) # Move 2 Manim units left.

        # Play the animation
        self.play(animation, run_time=10)
        self.wait(1) # Add a small pause at the end

        