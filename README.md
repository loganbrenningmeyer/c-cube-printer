# c-cube-printer
Animates random cube rotation in the terminal or prints cube with user-specified size/orientation

NOTE: Only works in Linux so it can use the proper escape sequences and characters for printing. Also, will need to zoom out the terminal when running so that the cube has enough space to rotate. Zoom out more for larger cubes.

# Cube Generation

  To print a 3D cube onto a 2D terminal, a 3D cube mustfirst be built, manipulated, then projected onto a 2D plane. To do this:
  
  ## buildCube()
    - Returns an array of the 8 vertices of a cube nested in the first octant. The cube has space equal to 1 more than the max distance the cube could reach while rotating:
    - ![buildCube](https://user-images.githubusercontent.com/26773050/192668984-456cfadf-21f8-42aa-87a1-6b76b0ec5ac2.png)

# Rotation

# Printing
