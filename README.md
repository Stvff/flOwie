# flOwie
A simple, funky, horrible fluid simulator, made possible by OLC's Pixel Game Engine! Not meant for usefulness.\
The big O is in the name because it scales based on size of grid, instead of amount of particles, which is something my earlier attempts at fluid simulations did not have, so I made the O big as a nod to this different type of complexity scaling (or maybe I did it just because I thought it looked funny UwU).

Each frame, the calculation in the image is preformed, and the result is drawn on the screen by the OLC PGE. Velocities in the x direction are green, velocities in the y direction are red, and pressure is blue.
![The algorithm used](/The_technique.png "The algorithm used")\
Incrementing of pressure is based on [this page](https://www.flow3d.com/resources/cfd-101/numerical-issues/numerical-treatment-of-incompressible-flow/), specifically:\
*J. Chorin, “A Numerical Method for Solving Incompressible Viscous Flow Problems,” J. Comp. Physics, 2, 12 (1967)*
