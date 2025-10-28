
# Current objective
- Have a circle bounce around the screen (ye olde DVD screensaver)

## Problem
Every frame, before the circle moves, we need to figure out whether it's reached (or will reach) the edge of the screen.

Let's say the dimensions of the area are 10x10, so the center is at position [5,5]
If the circle has a radius of, say, 2, and it moves at random speed in both x and y axes, then...

Frame 1: current position = [0,0]. moving at [3,2]
Frame 2: current position = [3,2]. moving at [3,2]
Frame 3: current position = [6,4]. moving at [3,2]
Frame 4: current position = [9,6]. ...etc.
Frame 5: current position = [12,8]

So, the problem is actually at frame 3. Because the circle has a radius of 2, we have to "look" 2 units ahead of the circle's position. So at Frame 3, the center is at x=6, but the edge of the circle is actually at x=8.
This means that, when the circle moves to the next frame, it'll be out-of-bounds.

So how do we solve this, conceptually? If the circle was to move ONE unit every frame, then that's easy. But it doesn't. And because we are literally between single frames here, there's no option to say "hey, next frame, only move 2 units across X", because then the circle would be slowing down for that frame, and speeding back up again next frame. Maybe that's what you're meant to do, but it doesn't sound right to me.

Instead, I guess we can just go with a "you're -close enough- to the boundary that you would cross it next frame, so I'm going to call this a collision now".

### Plan
- We have a circle moving on a vector.
- The circle has a known radius.

so before every frame, we need to determine the following:
1. Will the circle go beyond the boundary?
2. If so, do we need to know -which- boundary specifically (x vs y)?
3. we need to calculate a new vector as it will bounce/reflect off

#### Calculating new vector.
Let's just say it's moving diagonally downwards, and we'll blow out the X value of the boundary for the sake of simplicity.
So it's moving at [5,5]. Perfectly diagonal. Then it collides with the bottom border.
that would be the Y value we now need to invert, so it would become [5,-5]. I think it's that easy. Same for when it inevitably collides with the X boundary - we just inverse the X velocity.
