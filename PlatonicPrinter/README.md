# Platonic Solids Printer

***Platonic solids*** are geometric solids whose faces are all equally-sized regular polygons which meet at equal angles at the vertices. There are only 5 Platonic solids: cube, tetrahedron, octahedron, icosahedron, and dodecahedron.

<table width = 100%>
  <tr>
    <th width = 25%>Tetrahedron</th>
    <th width = 25%>Octahedron</th>
    <th width = 25%>Icosahedron</th>
    <th width = 25%>Dodecahedron</th>
  </tr>
  <tr>
    <td><img src=https://media1.giphy.com/media/zgwfSZ9FPRAESzk4O1/giphy.gif></td>
    <td><img src=https://media3.giphy.com/media/yf3RsqHOmlM6eVzYbV/giphy.gif></td>
    <td><img src=https://media0.giphy.com/media/b51yW3VUzUAgSZ4aHa/giphy.gif></td>
    <td><img src=https://media3.giphy.com/media/BiuElm4k1VLRvsAmEM/giphy.gif></td>
  </tr>
</table>

Using the code from the cube printer and with help from this paper (http://www.ms.uky.edu/~lee/visual05/notes/platonics.pdf), the vertices of every platonic solid can be derived from a cube. 

## Tetrahedron (4-sided)

By taking "every other" vertex of the cube (top diagonal/bottom diagonal), and connecting them you can create a tetrahedron.

<img src = https://user-images.githubusercontent.com/26773050/193640810-e6937999-9c88-4d18-b457-1bbfc6b7742e.png width = 80%>

## Octahedron (8-sided)

An octahedron can be formed by connecting the center of each of the cube's 6 faces.

<img src = https://user-images.githubusercontent.com/26773050/193640847-5055f8e0-71e9-44f3-9127-1fca237f53e7.png width = 80%>

## Icosahedron (20-sided)

Building an icosahedron from a cube is a bit trickier than the others. First, intersect 3 "golden rectangles"--rectangles whose ratio of $length/width = \phi$ (the golden ratio)--centered along the $x$, $y$, and $z$-axes. The length of the rectangles is equal to the length of the cube, and the width can be found by $length/\phi$. Take the vertices of these 3 rectangles to find the vertices of the icosahedron.

| Golden Rectangles | Connecting Vertices |
| ----------------- | ------------------- |
|![IcosahedronRectangles](https://user-images.githubusercontent.com/26773050/193671371-86efaba0-56c1-4592-873f-3a02c14437f2.png)|![IcosahedronVertices](https://user-images.githubusercontent.com/26773050/193671507-8d758bd2-5154-4b63-9013-680800565380.png)|

https://www.georgehart.com/virtual-polyhedra/ex-pr1.html

## Dodecahedron (12-sided)

Once you have an icosahedron, building a dodecahedron is relatively simple. Find the center of each of the 20 triangles that form the icosahedron by averaging their coordinates. Connect these centers to form a dodecahedron.
