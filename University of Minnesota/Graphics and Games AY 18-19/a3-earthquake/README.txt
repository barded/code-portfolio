/**
   CSci-4611 Assignment 3:  Earthquake
   Deandra Bardell
**/

Summary: I did everything in the C, B, and A range.

For vertices & normals vectors I made them part of earth.h so I can morph between them. I add vertices, normals, texcoords to the vector left to right, then down.

For indices, if one square looks like
0 --------- 1
|           |
|           |
2 --------- 3

I add them in order 0 2 3 0 3 1  for each square.

In quake_app.h/.cc i created a Dots class for my earthquake markers. It holds position, magnitude, radius, and color. I have factive and ractive vectors for markers that should be shown. For each frame I use a loop to add markers for a range of dates.

I figured out the max magnitude is 9.6, so to get a range of magnitudes from 0-1 I divide by 9.6, or 9 when they're first added so it's noticeable. Each frame I decrease the magnitude. Radius of marker is multiplied by a fraction of magnitude, so both decrease together. Color goes from red to yellow. I use magnitude as the alpha value in lerp. Big quakes are red and become yellow as they shrink. When magnitude<0, I pop off that marker from the active vectors. Red is a natural 'danger' color, and yellow contrasts blue ocean so it's easy to see.

The procedure is the same for round and flat.

For morphing I have a bool for flat and a bool for transition. When the button is pressed, flat_=!flat and trans_=true.

I have vectors for transition vertices and normals. I use those and lerp to have a smooth transition between flat and round. t ranges 0-1, 1 is round, 0 is flat. When t is out of that range, trans_=false stops the morph. mesh vertices & normals are set to flat or round.

I also made the globe rotate so we can see the markers.
