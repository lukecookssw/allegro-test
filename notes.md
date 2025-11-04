
# Current objective
- ✅ Have a circle bounce around the screen (ye olde DVD screensaver)
- Have 2 circles bounce around the screen, and bounce off each other.


## Problem
We have 2 circles, and they are being "checked" and moved sequentially.

So, we have 2 bodies that, upon the first item checking/moving, may not be registered as colliding, but then upon the second body checking/moving, suddenly they are in collision.

To make things worse, we need to know -which- bodies are actually colliding. For instance, if there were 10 circles on screen, you'd need to know which 2 (or more!) bodies have actually collided.

To make things EVEN WORSE, we also need to figure out the angle of collision so we can bounce off in the appropriate direction (but this part should be relatively straightforward).

OKAY, so I think the sequential checking there's just not much we can do about. Shit's just sequential man, so whatever.

What we need to do though, is figure out which body/ies the collision is happening between. So how do we do that.

## Plan
This is perhaps where we arrive at "data-oriented" as opposed to "object-oriented"?
For OOP, we would normally do "array of structs":
```csharp

// pseudo code
List<Circle> allCircles;
for(int i = 0; i < allCircles.Length; i++>)
{
    Circle current = allCircles[i];
    int newXPosition = current.Position.X += current.Velocity.X;
    int newYPosition = current.Position.Y += current.Velocity.Y;

    // ignore radius for now - assume we'd have fuzzy compare to account.
    List<Circle?> collisions = allCircles
        .Where(c => c.Position.X == newXPosition && c.Position.Y == newYPosition)
        .ToList();
    if (collissions.Length == 0)
    {
        // move the fellah and continue
    }
    else
    {
        foreach(var collision in collisions)
        {
            // handle collisions
        }
    }
    
}
```
If we do that, we're somewhere around O(n)^2 territory because of the `.Where()`.

So can we do "struct of arrays" and do data-driven programming instead?

Maybe have an array of positions so we know which circle it is? Something like:

```csharp
class Circles
{
    List<int> Position_X;
    List<int> Position_y;
    List<int> Velocity_X,
    List<int> Velocity_y;
}

Circles allCircles;

for(int i = 0; i < allCircles.Position_X.Length; i++)
{
    int newPos_X = allCircles.Position_X[i] + allCircles.Velocity_X[i];
    int newPos_Y = allCircles.Position_Y[i] + allCircles.Velocity_Y[i];
    
    int colliderIndex_x = allCircles.Position_X.IndexOf(newPos_X);
    int colliderIndex_y = allCircles.Position_Y.IndexOf(newPos_Y);

    if (colliderIndex_x == -1 && colliderIndex_y == -1)
    {
        // move
    }
    else
    {
        // handle collision
    }
}

```

I'm not sure how much more performant `IndexOf` is compared to `.Where`, but it would need to be HELLA more performant, surely. Otherwise we're now at, what? O(n)^3 in that pseudo code?
I mean, there are probably perf improvements you could do by ordering the values in the array, so you get to O(log(n)), but like, still... Seems kinda weird.

From memory, the whole thing about SoA vs AoS was about maximizing L1/L2 cache hits. Having AoS meant every struct was in random locations and required separate syscalls to fetch. Or something. It's been a while.

ANYWAY, let's see if we can write the AoS version in C, just so we can get something working...


## Plan (for colliding circles)