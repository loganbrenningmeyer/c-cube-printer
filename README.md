# c-cube-printer
Animates random cube rotation in the terminal or prints cube with user-specified size/orientation

**NOTE: Only works in Linux so it can use the proper escape sequences and characters for printing. Also, will need to zoom out the terminal when running so that the cube has enough space to rotate. Zoom out more for larger cubes.**

# Cube Generation  

To print a 3D cube onto a 2D terminal, a 3D cube must first be constructed, then rotated, then projected onto a 2D plane. To do this:
  1. Build the cube as 8 vertices in 3D space
  2. Apply rotations to the points
  3. Project the vertices onto a 2D plane
  4. Calculate each coordinate of the lines connecting the vertices
  5. Print the coordinates to the terminal  
  
## buildCube  
    
Returns an array of the 8 vertices of a cube nested in the first octant with the specified length. The cube has space on all sides equal to 1 more than the max distance the cube could reach while rotating. Each cube is numbered 0-7 in the order that it is stored in the array. This ordering will be important to remember later when drawing lines between the proper vertices after rotation.     
  
<img src = "https://user-images.githubusercontent.com/26773050/192688832-fb0e6fed-8487-41c5-9693-0683503cc562.png" width = 80%>

## rotateCube  

Given the 8 vertices found by buildCube, rotateCube rotates each vertex around the cube's center x-axis, y-axis, and z-axis by xTheta, yTheta, and zTheta degrees, respectively.  

When rotating from a single axis (x, y, or z), the change in coordinates is essentially the same as a rotation with 2D polar coordinates, treating the two axes that are not being rotated as x and y in 2D.  
  - Recall in polar coordinates $x = r\cos(\theta)$ and $y = r\sin(\theta)$

The figures below represent a 45 degree rotation of the green vertex around the x-axis:  
  
<img src = "https://user-images.githubusercontent.com/26773050/192688704-69374cc3-1ae7-45a5-a623-ec885241ec7e.png" width = 80%>  
  
Represented in 2D polar coordinates:  

<img src = "https://user-images.githubusercontent.com/26773050/192688715-295ad6ca-2ff4-4ad4-a88f-a7ac845e9680.png" width = 80%>   

Calculating adjacent and opposite:
  - $adjacent = x - origin$
  - $opposite = y - origin$
  - adjacent written in code as **alphaX** and opposite written as **alphaY**

Calculating radius:
  - Using the Pythagorean theorem:
    - $radius = \sqrt{adjacent^2 + opposite^2}$

Solving for $\alpha$, the vertex's original angle of rotation:  
  - $\tan(\alpha) = y/x$
  - $\alpha = \arctan(y/x)$
    - If the point lies in Quadrant I relative to the origin, leave $\alpha$ as is
    - If the point is in Quadrant II or III, add $\pi$
    - If the point is in Quadrant IV, add $2\pi$

Finally, finding $(x', y')$:
  - Add angle shift $\theta$ to $\alpha$ and correct by the origin
    - $x' = r\cos(\alpha + \theta) + origin$
    - $y' = r\sin(\alpha + \theta) + origin$

This point, $(x', y')$, represents the coordinates of the axes not being rotated. Because rotation is parallel to its axis, the coordinate for the axis of rotation does not change. Therefore, in the instance of the green vertex being rotated 45 degrees around the x-axis, it's new coordinates are $(x, x', y')$
  - Relative x & y in 2D depending on axis of rotation:
    - X-Rotation: x-axis = y, y-axis = z
      - asdlf
    - Y-Rotation: x-axis = z, y-axis = x
    - Z-Rotation: x-axis = x, y-axis = y

# Printing
