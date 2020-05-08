/**
   CSci-4611 Assignment #1 Text Rain
   Deandra Bardell
**/

Summary: I did everything in the C, B, and A range plus some wizard work.

My letters are quotes from Harry Potter. I put them all in drops[] array, even punctuation to give a little more variety.

I created a class Letter so each character can have it's own attributes, like x, y, speed, char, etc. The class is at the end of the code. Speed is randomly chosen in a certain interval (ls,us).

In setup I give each drop an initial x, y. For x, it's random on interval (lowerx, lowerx+dx). Every letter I move the interval right by 20. This way characters can overlap but more likely they remain in order, so catching a word may be possible.

For y, the interval is (lowery,uppery) initially.  When upperx>1280, I reset the x interval to (0,dx). Every time upperx>1280, I move the y interval up by 700. This way, adjacent characters are more likely to drop together.

The setx sety attributes are for wizard work.

I mirror and blur the camera input, but not video input. I use filter to get grayscale or threshold based on boolean thresholdOn.  Drops are red.

For displaying and moving drops, if they fall past the end of the screen, I give drops a new speed, initial y, and initial x thats not too far from their original setting. This way they more or less stay in order.

To prevent drops from going through thin lines, I check the color of pixels 0-10 below the drop. If a dark pixel is 0-6 below the drop, I move the drop up. If it's dark between 6-10 below, I set speed to 0, so the next frame the drop won't move. Then I reset the speed.

The drops move based on time. I use y'=y+speed*(millis()-time)/1000 to get pixels per second movement.

up or down arrows adjust darkness tolerance. space toggles 'debug' mode.


Wizard Work:

My drops flow left and right like rain should. I do this by checking under a drop to the left and right. If left is black and right is white, drop moves right. Same idea for left.

I display the tolerance in the bottom left after all the drop calculations so drops won't stop.

I account for gravity by multiplying speed by 1.1 if y>0. This way if they start offscreen, they won't have a super fast speed by the time they appear. I chose slower overall so it's easier to interact with drops.

Back to setx sety. I wanted a way to eventually see what the drops spell out, and to show it on top. When a drop gets pushed left or right far enough, 'pushed' boolean attribute gets set to true, telling my last bit of code in draw to display that character in its (setx, sety) position. The idea is to get a user to approach the camera and start interacting, with the challenge of figuring out how to display the characters in the top left. In a public installment I'd probably have the entire program reset after m minutes so user after user can try it.
