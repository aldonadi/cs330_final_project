# CS330 - Computational Graphics and Visualization
SNHU CS330 Module 8 Journal

## Design approach
I came to this class with almost no computer graphics programming experience.
I spent a lot of time reading OpenGL docs and referring to other OpenGL
projects and YouTube videos. I took a very iterative approach, using a git
repository allow me to non-destructively explore changes and features that I
could easily roll back. A clean, modular design was another goal of mine. I
would plan out classes, responsibilities, and an API before starting to code in
earnest. For example, I did this when incorporating an ImGui UI for adjusting
object properties live while running.

To design the 3D scene itself, I first took several photographs of the
real-world scene. I made sure to include all needed angles such as the
reference perspective, orthographic views from the left and right, a
front-facing view, and a top-down view. I then added and placed the objects
using the UI I developed to position, scale, and rotate each object to match
the reference photo. I then chose textures that were similar to the reference.
For the (obviously copyrighted) book covers and pages, I used a locally-hosted
SDXL AI model to generate suitable textures. I then chose materials to assign
to the objects, to allow lighting to interact differently with, for example,
glassy plastic surfaces and matte flooring.

Even with my live-adjustment system, building the scene took many, many
iterations to get right. One drawback I had in this class was not having taken
Linear Algebra yet, which I believe would have helped me more precisely
position objects on top of each other, for example.

I think further pursuit of this field of programming would allow me to create
interesting and helpful visualizations of complex problems or simulations. For
example, at my work, we have a large steam drum that is shaped like a cylinder
on its side. A year or so ago, I overfilled and nearly overpressurized it while
filling because I did not appreciate how the change in drum level increases
rapidly for the same flow as you fill up past the middle (widest) part of the
sideways cylinder. Using my computer graphics knowledge and some calculus for
the drum level equation, I could write a simulation or indication program to
visualize how water level changes for a constant mass flow rate of water. This
could help provide a training environment for future operators.
