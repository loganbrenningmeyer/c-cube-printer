# Spinning Cube

***Go to the PlatonicPrinter folder for printing other Platonic solids***

Animates random cube rotation in the terminal or prints cube with user-specified size/orientation  
  - If compiled with infiniteCube.c: Animates infinitely rotating cube until the user terminates the program (CTRL+C)

![Spinning Cube](https://media.giphy.com/media/fJ0fxIbhiA8cfdUcJS/giphy.gif)

**NOTE: Only works in Linux so it can use the proper escape sequences and characters for printing. Also, will need to zoom out the terminal when running so that the cube has enough space to rotate. Zoom out more for larger cubes.**

To print a 3D cube onto a 2D terminal, a 3D cube must first be constructed, then rotated, then projected onto a 2D plane. To do this:  
  1. Build the cube as 8 vertices in 3D space  
  2. Apply rotations to the points  
  3. Project the vertices onto a 2D plane  
  4. Calculate each coordinate of the lines connecting the vertices  
  5. Print the coordinates to the terminal  

# Cube Generation  
  
## buildCube  
    
Returns an array of the 8 vertices of a cube nested in the first octant with the specified length. The cube has space on all sides equal to 1 more than the max distance the cube could reach while rotating. Each cube is numbered 0-7 in the order that it is stored in the array. This ordering will be important to remember later when drawing lines between the proper vertices after rotation.     
  
<img src = "https://user-images.githubusercontent.com/26773050/192688832-fb0e6fed-8487-41c5-9693-0683503cc562.png" width = 80%>

## rotateCube  

Given the 8 vertices found by buildCube, rotateCube rotates each vertex around the cube's center $x$-axis, $y$-axis, and $z$-axis by $xTheta,$ $yTheta,$ and $zTheta$ degrees, respectively.  

When rotating from a single axis $x$, $y$, or $z$, the change in coordinates is essentially the same as a rotation with 2D polar coordinates, treating the two axes that are not being rotated as $x$ and $y$ in 2D.  
  - Recall in polar coordinates $x = r\cos(\theta)$ and $y = r\sin(\theta)$

The figures below represent a 45 degree rotation of the green vertex around the $x$-axis:  
  
<img src = "https://user-images.githubusercontent.com/26773050/192688704-69374cc3-1ae7-45a5-a623-ec885241ec7e.png" width = 80%>  
  
Represented in 2D polar coordinates:  

<img src = "https://user-images.githubusercontent.com/26773050/192688715-295ad6ca-2ff4-4ad4-a88f-a7ac845e9680.png" width = 80%>   

Calculating $adjacent$ and $opposite$:
  - $adjacent = x - origin$
  - $opposite = y - origin$
  - adjacent written in code as **alphaX** and opposite written as **alphaY**

Calculating $radius$:
  - Using the Pythagorean theorem:
    - $radius = \sqrt{adjacent^2 + opposite^2}$
  - $radius$ must be calculated for each vertex as different vertices may be different distances from the axis of rotation if a rotation for another axis has already been applied.

Calculating $\alpha$, the vertex's original angle of rotation:  
  - $\tan(\alpha) = opposite/adjacent$
  - $\alpha = \arctan(opposite/adjacent)$
    - If the point lies in Quadrant I relative to the origin, leave $\alpha$ as is
    - If the point is in Quadrant II or III, add $\pi$
    - If the point is in Quadrant IV, add $2\pi$

Finally, finding $(x', y')$:
  - Add angle shift $\theta$ to $\alpha$ and correct by the origin
    - $x' = r\cos(\alpha + \theta) + origin$
    - $y' = r\sin(\alpha + \theta) + origin$

This point, $(x', y')$, represents the coordinates of the axes not being rotated. Because rotation is parallel to its axis, the coordinate for the axis of rotation does not change. Therefore, in the instance of the green vertex being rotated 45 degrees around the $x$-axis, it's new coordinates are $(x, x', y')$
  - Relative $x$ & $y$ in 2D depending on axis of rotation:
    - $X$-Rotation: $x$-axis = $y$, $y$-axis = $z$
      - $(x', y', z') = (x, x', y')$
    - $Y$-Rotation: $x$-axis = $z$, $y$-axis = $x$
      - $(x', y', z') = (y', y, x')$
    - $Z$-Rotation: $x$-axis = $x$, $y$-axis = $y$
      - $(x', y', z') = (x', y', z)$

# Cube Projection/Plotting

## getVerticesCoordinates

With the now obtained 3D coordinates of the vertices of the cube, getVerticesCoordinates projects them onto a 2D plane and returns an int array of the 2D vertices' coordinates.

To project the vertices onto a plane, a "camera" is placed at the point whose $y$ and $z$-values are centered with the cube and whose $x$-value is located $1.5\*length$ units past where the space surrounding the cube ends. In addition, a plane perpendicular to the $x$-axis, $screen(xS, yS)$, is placed between the cube and the camera at the $x$-value of the outer edge of the cube's space $(xS = xC - 1.5\*length)$. For each vertex, the equation of the line between the vertex and the camera is calculated. The 2D projection of the vertex is then found by finding the point of intersection of the line and $screen$.

| Lines/Intersections in 3D | 2D Projection |
| ------------------------- | ------------- |
| ![](https://user-images.githubusercontent.com/26773050/192704553-cb0a48a8-2764-47ae-8c77-d0e21ade62b5.png) | ![](https://user-images.githubusercontent.com/26773050/192706152-092c3b61-4410-4302-addb-999f93499ad7.png) |

Calculating intersections of $screen$ and the lines connecting vertices $(x, y, z)$ and the camera $(xC, yC, zC)$:
  - Line equation: $(xC, yC, zC) = (x, y, z) + t&lta, b, c&gt$
    - $a = xC - x$
    - $b = yC - y$
    - $c = zC - z$
  - Need to find constant $t$ for the intersection of the line and the screen.
    - $xS = xC - length\*1.5$
    - $xS = x + ta$
      - $screen$ $x$-coordinate must be somewhere along the line connecting the vertex and camera
    - $x + ta = xC - length\*1.5$
    - $t = (xC - length\*1.5 - x)/a$
  - Now, the points are solvable:
    - $screen(xS, yS) = (y + tb, z + tc)$
      - When looking at the cube in the negative $x$ direction, $y$ represents $x$-axis and $z$ represents $y$-axis
      - Multiply $y$-value by 0.8 to get rid of the warping effect from non-square pixels (██) 
        - In the terminal, pixels are taller than they are wide. Through testing, 0.8 was found to be the most accurate **yScale** value to maintain a cube
  - Multiply all coordinate values by 10 then round to the nearest integer
     - Prepares coordinates for Bresenham's algorithm by converting to int and minimizes loss of data

## getLineCoordinates

Now that we have the vertices, the next step is to connect them with lines. To do so, <a href = "https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/">Bresenham's Line Generation Algorithm</a> can calculate each coordinate between two points in a line using only int arithmetic. In short, the algorithm tracks the slope error between the the generated line and the true line, incrementing as needed to stay aligned. 

Bresenham's line algorithm in action:

<a href = "https://www.middle-engine.com/blog/posts/2020/07/28/bresenhams-line-algorithm">![Bresenham Line Algorithm](https://www.middle-engine.com/images/2020-07-28-bresenhams-line-algorithm/03_bresenham-12x12-example.gif)</a>

Recall earlier when I said the ordering of the vertices would be important later? Now is that time. To draw each of the 12 edges of the cube, you must create a line between each of the four vertices on top of the cube, between each of the four vertices on the bottom, and for each of the four columns connecting the top and bottom. By referring to the order of the vertices in the screen array when the cube was initially built, lines can always be drawn between the proper vertices regardless of orientation. 

Using Bresenham's algorithm, getLineCoordinates finds every coordinate of all 12 lines and adds it to **coordsArray**. This creates an unordered array of all of the cube's points. Vertices and intersections are included multiple times because they are connected to multiple lines. This is taken care of later, however, when printing to the terminal.
  - **coordsArray** is a 2D int array[1000][2]. Has enough space for up to 1000 coordinates if the cube is large, and each coordinate holds 2 values, $x$ and $y$.

To prepare the coordinates for printing, **coordsArray** is quicksorted so that coordinates are sorted descending by $y$-value. In addition, coordinates with matching $y$-values are sorted ascending by $x$-value. This way, when the array is read from beginning to end it gives coordinates from top to bottom, left to right.

Sample **coordsArray**:

`int coordsArray[1000][2] = {{5, 10}, {6, 10}, {7, 10}, {8, 10}, {9, 10}, {10, 10}, {5, 9}, {10, 9}, {5, 8}, {10, 8}...}`

## printToTerminal

Lastly, printing the coordinates to the terminal. Because the array is already sorted, printing isn't too difficult. There are only a few rules:
  - For $(x,y)$:
    - If the point is a repeat of the previous point in the array (vertices), continue to the next point
    - If the point is the first on its row, print $x-1$ spaces before it
    - If the point is the last on its row, print a new line
    - Otherwise, print $x-xPrev-1$ spaces before it
      - Print amount of space between current point and previous point
  - To keep the cube from bobbing up and down and sticking to the top of the terminal, print $(2 \* space + length)\*10 - maxY)$ new lines above the cube
    - Adds space above the cube equal to the distance between the maximum y-value of the cube and the edge of the space taken up by the cube
    - Needs $\*10$ to account for $\*10$ scaling when converting vertices from floats to ints by multiplying by 10 and rounding in getVerticesCoordinates
    - Just as printing $x-1$ spaces to the left of new rows prints $x$-values properly relative to their true coordinates, printing these new lines situates the $y$-values so the cube doesn't stray from its center

  
