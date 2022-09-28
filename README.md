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
![buildCube](https://user-images.githubusercontent.com/26773050/192680347-df1fdb52-b95a-479b-bfd4-ac8c48254336.png)  

## rotateCube  

Given the 8 vertices found by buildCube, rotateCube rotates each vertex around the cube's center x-axis, y-axis, and z-axis by xTheta, yTheta, and zTheta degrees, respectively.  

When rotating from a single axis (x, y, or z), the change in coordinates is essentially the same as a rotation with 2D polar coordinates, treating the two axes that are not being rotated as x and y in 2D.  
  - Recall in polar coordinates $x = r\cos(\theta)$ and $y = r\sin(\theta)$

The figures below represent a 45 degree rotation of the green vertex around the x-axis:  
![rotateCube3D](https://user-images.githubusercontent.com/26773050/192688442-e96a949f-91aa-4ab4-b4bb-8aa4fecedd8b.png)  
Represented in 2D polar coordinates:  
![rotateCube2D](https://user-images.githubusercontent.com/26773050/192685974-0708a092-5956-4700-8436-358ff8c5b32e.png)

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

# Printing
