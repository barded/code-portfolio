// --------------------- Sketch-wide variables ----------------------
PImage img;
PFont titleFont, smallFont;

ESLDemData demData;
ESLTestData testData;

float hmin = 10000000, hmax = 0;
int headerHeight;
int width = 1080, height = 800, halfwidth = width / 2;

void setup() {
 size(1080, 800);
 headerHeight = height - 575;
 // img = loadImage("MSP_satellitemap-1.png");
 img = loadImage("MSP_roadmap_edit.png");

 smooth();

 titleFont = loadFont("Helvetica-22.vlw");
 smallFont = loadFont("Helvetica-12.vlw");
 // midFont = loadFont("AdobeFanHeitiStd-Bold-16.vlw");
 textFont(smallFont);

 demData = new ESLDemData();
 demData.loadFromFile("demographics.csv");

 testData = new ESLTestData();
 testData.loadFromFile("testdata.csv");

 String[] sites = demData.getSites();
 for (int i = 0; i < sites.length; i++) {
  SiteData s = demData.getSiteData(sites[i]);
  float h = s.getHours();
  s.addtoLocation(0, headerHeight);

  if (h < hmin) {
   hmin = h;
  }

  if (h > hmax) {
   hmax = h;
  }
 }
}

void draw() {
 fill(0);
 textFont(titleFont);
 textAlign(CENTER, CENTER);
 background(255);
 image(img, 0, headerHeight);
 ellipseMode(CENTER);

 text("English as a Second Language (ESL) Classes in the Twin Cities", halfwidth, 70);
 textFont(smallFont);
 text("Demographics of each site from 2014-2018", halfwidth, 100);
 text("Click a site to learn more", halfwidth, 100 + textAscent() * 2);
 text("Resources and data provided by the Minnesota Literacy Council", halfwidth, 100 + textAscent() * 6);



 // ellipse(403,262,10,10); // MLC Main Site
 // fill(50,150,200,127);
 // ellipse(246,275,10,10); // Holy Rosary Church
 // ellipse(791,230,39,39); // Arlington Hills Lutheran Church
 // ellipse(294,314,71,71); // Lake Street
 // ellipse(462,271,10,10); // Midway
 // ellipse(283,12,22,22); // Gustavus Adolphus Lutheran Church - Northeast
 // ellipse(101,155,78,78); // Sumner Library - Northside
 // ellipse(622,288,10,10); // Rondo
 textFont(smallFont);
 textAlign(LEFT);
 String[] sites = demData.getSites();

 for (int i = 0; i < sites.length; i++) {
  SiteData s = demData.getSiteData(sites[i]);
  PVector p = s.getLocation();
  float h = s.getHours();
  fill(150, 150, 200, 127);
  float diam = ((h - hmin) * (100 - 10)) / (hmax - hmin) + 10;
  circle(p.x, p.y, diam);
  fill(0);
  if (sites[i].equals("Holy Rosary Church") || sites[i].equals("MLC Main Site")) {
   textAlign(RIGHT);
   text(sites[i], p.x - diam * .6, p.y);
   textAlign(LEFT);
  } else
   text(sites[i], p.x + diam * .6, p.y);

  if (sqrt(abs((p.x - mouseX) * (p.x - mouseX) + (p.y - mouseY) * (p.y - mouseY))) < diam / 2) {
   fill(255);
   rect(mouseX, mouseY, 190, 40);
   fill(0);
   text(sites[i], mouseX + 10, mouseY + 14);
   text("Contact hours = " + (int) diam * 6, mouseX + 25, mouseY + 30);

  }
 }


 fill(255);
 rect(width - 150, height - 200, 145, 175);
 fill(0);
 textAlign(CENTER);
 text("Key", width - 70, height - 180);
 textAlign(LEFT);
 text("Roughly size of ", width - 110, height - 130);
 text("center by number", width - 110, height - 115);
 text("of contact hours", width - 110, height - 100);
 fill(150, 150, 200, 127);
 circle(width - 130, height - 130, 10);

}

// Circles with rad 10 have <=10,000 contact hours in May 2017 - April 2018.

// https://processing.org/examples/loaddisplayimage.html
//https://processing.org/tutorials/drawing/

void drawAgeGraph(String site, int x, int y) {

 SiteData curr = demData.getSiteData(site);
 int step = curr.getAgeRangeStep();
 int ymin = curr.getAgeRangeMin();
 int ymax = curr.getAgeRangeMax();

 fill(0);


}
