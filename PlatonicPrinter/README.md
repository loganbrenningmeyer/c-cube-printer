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

<img src = https://user-images.githubusercontent.com/26773050/193676149-53011335-aac0-419a-aa8d-65e78f3cf599.png width = 80%>

## Octahedron (8-sided)

An octahedron can be formed by connecting the centers of each of the cube's 6 faces.

<img src = https://user-images.githubusercontent.com/26773050/193700009-e89c6307-4cf9-4106-adeb-699f6994c40a.png width = 80%>

## Icosahedron (20-sided)

Building an icosahedron from a cube is a bit trickier than the others. First, intersect 3 "golden rectangles"—rectangles whose ratio of $length/width = \phi$ (the golden ratio)—centered along the $x$, $y$, and $z$-axes. The length of the rectangles is equal to the length of the cube, and the width can be found by $length/\phi$. Take the vertices of these 3 rectangles to find the vertices of the icosahedron. (Reference: https://www.georgehart.com/virtual-polyhedra/ex-pr1.html)

<table>
  <tr>
    <th width = 50%>
      Golden Rectangles
    </th>
    <th width = 50%>
      Connecting Vertices
    </th>
  </tr>
  <tr>
    <td>
      <img src = https://user-images.githubusercontent.com/26773050/193698542-8719f7c6-37a1-4cb7-bd2a-34b1d831c90c.png>
    </td>
    <td>
      <img src = https://user-images.githubusercontent.com/26773050/193677960-531bed7a-89b0-414e-b33c-b0c4bad9016b.png>
    </td>
  </tr>
</table>

## Dodecahedron (12-sided)

Once you have an icosahedron, building a dodecahedron is relatively simple. Find the center of each of the 20 triangles that form the icosahedron by averaging the triangle's vertices' coordinates. Connect these centers to form a dodecahedron.

<table>
  <tr>
    <th width = 50%>
      Find Centers of Icosahedron
    </th>
    <th width = 50%>
      Connecting Vertices
    </th>
  </tr>
  <tr>
    <td>
      <img src = https://user-images.githubusercontent.com/26773050/193698104-0fdfafbf-1d79-439b-b4e1-8e40045f9b6c.png>
    </td>
    <td>
      <img src = https://user-images.githubusercontent.com/26773050/193698124-8ac40974-8941-4881-a856-771f2de56211.png>
    </td>
  </tr>
</table>
