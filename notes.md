
# Current objective
- ✅ Have a circle bounce around the screen (ye olde DVD screensaver)
- ✅ Have 2 or more circles bounce around the screen, and bounce off each other.
- Ensure collision is calculated when the number of circles grows

## Current problem
When the number of circles increases, the collision detection starts to fail. It just can't check every single circle permutation so some are missed. This may be because:

 - "Tunneling": If a collision check takes too long, or if circles are moving very fast, they can pass completely through another circle between frames without the O(N2) check detecting the overlap. This is especially true for the simple instantaneous checks we are performing.

 ## Potential solution
 This is kinda tricky. Looks like the general solution is spatial partitioning, where we divide the screen up into a grid, and only check collisions between circles that share the same cell. This means we now have to ID every circle, and a grid.