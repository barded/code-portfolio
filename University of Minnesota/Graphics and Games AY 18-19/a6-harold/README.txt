/**
   CSci-4611 Assignment 6:  Harold
   Deandra Bardell
**/

Summary: I did everything in the C, B, and A range.

I don't feel like there's a lot to talk about since there wasn't a lot to do that was our creative choice.

For the sky, I just followed the handout closely. At first it was confusing that Mesh is Point3  but ScreenPtHitsSky wants Point2. And then I made sky_point a pointer and the program kept seg faulting. After fixing those I got sky working.

Step 1 of ground was fine. We've done a lot with cross and dot products.

Step 2 was hardest. I thought I had to use ScreenPtHitsPlane from Billboards, which didn't seem to work. Then I used the ray code part from ScreenPtHitsGround and IntersectPlane, and that seemed to solve the issue.

Step 3 I followed the handout. I was glad hfunc was written for us. This part was just following the math formulas, pretty easy.

I made a mountain with peaks and valleys. Fun project!
