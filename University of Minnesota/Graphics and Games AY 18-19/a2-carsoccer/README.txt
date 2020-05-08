/**
   CSci-4611 Assignment 2:  Car Soccer
   Deandra Bardell
**/

Summary: I did everything in the C, B, and A range plus some wizard work.

At the bottom of car_soccer.cc I draw the boundary in dark red and white goals with DrawLines.

On start up I give each component of balls velocity a random number between -20 and 20. In launch_ball() it's -200 and 200. Seeing the ball fly around super fast is cool.

My gravity is a Vector3 in car_soccer.h with only a y value. Then, it's easy to update velocity by vnew=v+gravity*dt. Right after this I have a if statement that detects if the ball is nearly stopped, then don't do a calculation with gravity and set ball velocity.y to 0. This way the ball doesn't bounce up and down when it's supposed to be motionless.

In the car class I added variables for speed, angle, drag, and velocity. I have permission from Prof Keefe to change how car driving works because I can only type one key at a time.

drag slows the car down every frame, speed*=drag. velocity is a unit vector in the angle direction times speed. up and down arrows increase or decrease speed. left and right arrows change the angle independent of up & down. This way I can "aim & fire" the car.

The ball collides wth walls and cars like the directions say to do. Since my cars don't go very fast for control reasons, I add the entire velocity of the car to the ball instead of a fraction of it. bouncedamp is defined in car_soccer.h, and it takes away velocity from the ball when it hits something.

When a car hits a wall, it just stops. Then I can "aim & fire" again at the ball.

Goals reset the cars and relaunch the ball.

Wizard Work:
I added another car, controlled with wsad keys. I put arrows on the car too so its easy to see forward direction. The cars don't collide on purpose so players can't trap each other.

The enter key sets the ball on the ground motionless. This way, if you want to hit the ball, there's no need to wait for it to stop. It was also a good way to debug collisions.
