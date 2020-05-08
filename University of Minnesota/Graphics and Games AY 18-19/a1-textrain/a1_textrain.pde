/**
   CSci-4611 Assignment #1 Text Rain
   Deandra Bardell
**/


import processing.video.*;

// Global variables for handling video data and the input selection screen
String[] cameras;
Capture cam;
Movie mov;
PImage inputImage;
boolean inputMethodSelected = false;
PFont font;

String letters = "I solemnly swear that I am up to no good. The thing about growing up with Fred and George is that you sort of start thinking anything is possible if you have got enough nerve. Words are, in my not so humble opinion, our most inexhaustible source of magic, capable of both inflicting injury and remedying it. Soon we must all face the choice between what is right and what is easy.";

Letter[] drops = new Letter[letters.length()];
int tolerance = 128;

float dx = 40;
float dy = -120;
float lowerx = 0;
float upperx = lowerx + dx;
float lowery = -50;
float uppery = lowery + dy;
int width = 1280;
int height = 720;
int time = 0;
Boolean thresholdOn = false;
float xf = 10.;
float yf = 20.;
int ls = 30;
int us = 120;
int pushamount = 200;

void setup() {
 size(1280, 720);
 inputImage = createImage(width, height, RGB);
 font = loadFont("AdobeHeitiStd-Regular-24.vlw");
 textFont(font, 12);

 for (int i = 0; i < letters.length(); i++) {
  float x = random(lowerx, upperx);
  float y = random(uppery, lowery);
  lowerx += 20.;
  upperx = lowerx + dx;
  if (upperx > 1280) {
   lowerx = 0;
   upperx = lowerx + dx;
   lowery -= 700;
   uppery += lowery - 700;
   x = random(lowerx, upperx);
   y = random(uppery, lowery);

  }

  drops[i] = new Letter(x, y, letters.charAt(i), x, y);

  if (drops[i].l == ' ' || drops[i].l == '.' || drops[i].l == ',') {
   if (drops[i].l == ' ') {
    xf += 4;

    drops[i].setx = xf;
    drops[i].sety = yf;
    xf += 4;
   } else {
    drops[i].setx = xf;
    drops[i].sety = yf;
   }

   if (drops[i].l == '.') {

    xf = 10 - textWidth(' ') - 6;
    yf += 14;
   }
  } else {
   drops[i].setx = xf;
   drops[i].sety = yf;
   xf += textWidth(drops[i].l);
  }
 }


} // setup


void draw() {
 // When the program first starts, draw a menu of different options for which camera to use for input
 // The input method is selected by pressing a key 0-9 on the keyboard
 fill(255, 0, 0);

 if (!inputMethodSelected) {
  cameras = Capture.list();
  int y = 40;
  text("O: Offline mode, test with TextRainInput.mov movie file instead of live camera feed.", 20, y);
  y += 40;
  for (int i = 0; i < min(9, cameras.length); i++) {
   text(i + 1 + ": " + cameras[i], 20, y);
   y += 40;
  }
  return;
 }


 // This part of the draw loop gets called after the input selection screen, during normal execution of the program.


 // STEP 1.  Load an image, either from a movie file or from a live camera feed. Store the result in the inputImage variable

 if ((cam != null) && (cam.available())) {
  cam.read();

  inputImage.copy(cam, 0, 0, cam.width, cam.height, 0, 0, inputImage.width, inputImage.height);
  if (thresholdOn) {
   inputImage.filter(THRESHOLD, (float) tolerance / 255);
  } else {
   inputImage.filter(GRAY);
  }
  inputImage.filter(BLUR, 2);
  pushMatrix();
  scale(-1, 1);
  image(inputImage, -inputImage.width, 0);
  popMatrix();
 } else if ((mov != null) && (mov.available())) {
  mov.read();
  inputImage.copy(mov, 0, 0, mov.width, mov.height, 0, 0, inputImage.width, inputImage.height);
  if (thresholdOn) {
   inputImage.filter(THRESHOLD, (float) tolerance / 255);
  } else {
   inputImage.filter(GRAY);
  }
  pushMatrix();
  image(inputImage, 0, 0);
  popMatrix();
 }

 // Fill in your code to implement the rest of TextRain here..

 if (inputMethodSelected) {

  for (int i = 0; i < drops.length; i++) {
   text(drops[i].l, drops[i].x, drops[i].y);

   if (drops[i].y > height + 100) {
    drops[i].y = random(-500, -10);
    drops[i].speed = random(ls, us);
    drops[i].x = random(round(drops[i].initx - 10), round(drops[i].initx + 10));
   }

   if (abs(drops[i].x - drops[i].initx) > pushamount) {
    drops[i].pushed = true;
   }

   if (drops[i].y > 0) {
    drops[i].y = (drops[i].y + drops[i].speed * 1.1 * (float)(millis() - time) / 1000.0); // gravity. and falls based on time elapsed.
   } else {
    drops[i].y = (drops[i].y + drops[i].speed * 1.0 * (float)(millis() - time) / 1000.0);
   }

   if (drops[i].speed == 0) {
    drops[i].speed = drops[i].initspeed;
   }

   color below, above;
   color left, right;

   for (int j = 0; j < 10; j++) { // makes more robust. letter stops on 1px line
    below = get(round(drops[i].x), round(drops[i].y + j));
    left = get(round(drops[i].x - 5), round(drops[i].y + j));
    right = get(round(drops[i].x + 5), round(drops[i].y + j));


    if (j >= 0 && j < 6 && round(brightness(below)) < tolerance && drops[i].y < 710 && drops[i].y > 0) {
     drops[i].y -= 10;

     if (j >= 0 && j < 6 && round(brightness(right)) > tolerance && round(brightness(left)) < tolerance && drops[i].y < 710 && drops[i].y > 0) {
      drops[i].x += 10;
     }
     if (j >= 0 && j < 6 && round(brightness(right)) < tolerance && round(brightness(left)) > tolerance && drops[i].y < 710 && drops[i].y > 0) {
      drops[i].x -= 10;
     }

    } else if (j >= 6 && j < 10 && round(brightness(below)) < tolerance && drops[i].y < 710 && drops[i].y > 0) {
     drops[i].speed = 0;
    }

   }

  }

  fill(0, 63, 91);
  int x = 5;
  text("tolerance: " + tolerance, 10, 700);
  for (int i = 0; i < drops.length; i++) {
   if (drops[i].pushed || drops[i].l == ' ') {
    text(drops[i].l, drops[i].setx, drops[i].sety);

   }
  }

  time = millis();

 }

} //    draw



void keyPressed() {

 if (!inputMethodSelected) {
  // If we haven't yet selected the input method, then check for 0 to 9 keypresses to select from the input menu
  if ((key >= '0') && (key <= '9')) {
   int input = key - '0';
   if (input == 0) {
    println("Offline mode selected.");
    mov = new Movie(this, "TextRainInput.mov");
    mov.loop();
    inputMethodSelected = true;
   } else if ((input >= 1) && (input <= 9)) {
    println("Camera " + input + " selected.");
    // The camera can be initialized directly using an element from the array returned by list():
    cam = new Capture(this, cameras[input - 1]);
    cam.start();
    inputMethodSelected = true;
   }
  }
  return;
 }


 // This part of the keyPressed routine gets called after the input selection screen during normal execution of the program
 // Fill in your code to handle keypresses here..

 if (key == CODED) {
  if (keyCode == UP) {
   tolerance++;
  } else if (keyCode == DOWN) {
   tolerance--;
  }
 } else if (key == ' ') {
  thresholdOn = !thresholdOn;

 }


}

class Letter {

 float speed = random(ls, us);
 float x, y, initx, inity;
 float setx, sety;
 char l;
 Boolean pushed = false;
 float initspeed = speed;

 Letter(float x, float y, char l, float initx, float inity) {
  this.x = x;
  this.y = y;
  this.l = l;
  this.initx = initx;
  this.inity = inity;
 }



}
