/**
   CSci-4611 Assignment 5:  Art Render
   Deandra Bardell
**/

Summary: I did everything in the C, B, and A range.

I don't feel like there's a lot to talk about since there wasn't a lot to do.

I've already done shading and lighting for 5607, so I'm pretty comfortable with light and material properties. I've never used GLSL, so I've had to learn a bit about it.

I went with Blinn Phong for specular highlights. For these objects, I thought it looked a little better.

For artsy I was a little confused whether the texture took the place of I as well, but the directions seem to tell us to keep I. My objects look like the examples.

In outline.vert I had to play around with multiplying by model_view_matrix with the vertex. At first I had very thin lines. After trial and error I realized for e I had to do model_view_matrix*vertex because the vertex wasn't in camera coordinates. Other than that, the directions were helpful and pretty straightforward. I thought the outline would be more complicated to create. Thanks for making it manageable.   
