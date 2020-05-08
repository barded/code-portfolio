// --------------------- Sketch-wide variables ----------------------
PImage img;
PFont titleFont, smallFont, hb16, hb14;

Chartctl[] chart = new Chartctl[2];

ESLDemData demData;
ESLTestData testData;

color [] lancolors = {#CE632D, #3170B1, #5DBCD2, #45A07B, #FD4474, #C183A6, #7F042F, #301C02 }; // 8
color [] linecolors = {#5C92CC,#7EBDC4,#64af56, #998610, #DB9F5A, #878272,#424240 }; // 7

String[] greek = {
 "µ",
 "ξ",
 "Ω",
 "Σ",
 "Θ",
 "β",
 "π"
};

String[] races = {
 "American Indian or Alaskan Native",
 "Asian",
 "Black or African American",
 "Hispanic",
 "Native Hawaiian or Other Pacific Islander",
 "Two or More Races",
 "White"
};

HashMap < String, PVector > sites_loc;
HashMap < String, Button > sites_button;

ArrayList < Button > onoff = new ArrayList < Button > ();
ArrayList < Button > onoff_copy = new ArrayList < Button > ();

ArrayList < Button > lonoff = new ArrayList < Button > ();
ArrayList < Button > lonoff_copy = new ArrayList < Button > ();

ArrayList < Button > chartyears = new ArrayList < Button > ();
ArrayList < Button > chartyears_copy = new ArrayList < Button > ();


Button[] mapyears = new Button[5];

float hmin = 1000000, hmax = 0, sizeofpts = 7;
int headerHeight = 150;
int halfwidth, halfheight;
int curr_chart = 0;
boolean on_chart0 = true, two_charts = false, showchart = false, showlinechart = false, samelinecolors = true, showpercents = false;

int year = 2014;

void setup() {
 // min width is 1280
 size(1280, 700, P2D); // sara 1280  680
 halfwidth = width / 2;
 halfheight = height / 2;
 // img = loadImage("MSP_satellitemap-1.png");
 img = loadImage("MSP_roadmap_edit2.png");

 smooth();
 // titleFont = loadFont("Arial-Black-48.vlw");

 titleFont = loadFont("Helvetica-22.vlw");
 smallFont = loadFont("Helvetica-12.vlw");
 hb16 = loadFont("Helvetica-Bold-16.vlw");
 hb14 = loadFont("Helvetica-Bold-14.vlw");

 // midFont = loadFont("AdobeFanHeitiStd-Bold-16.vlw");
 textFont(smallFont);

 demData = new ESLDemData();
 demData.loadFromFile("demographics_by_year.csv");

 testData = new ESLTestData();
 testData.loadFromFile("testdata.csv");

 sites_loc = new HashMap < String, PVector > ();
 sites_loc.put("Holy Rosary Church", new PVector(246 * width / 1080, 275 * (height - headerHeight) / 575 + headerHeight));
 sites_loc.put("MLC Main Site", new PVector(403 * width / 1080, 262 * (height - headerHeight) / 575 + headerHeight));
 sites_loc.put("Arlington Hills Lutheran Church", new PVector(791 * width / 1080, 230 * (height - headerHeight) / 575 + headerHeight));
 sites_loc.put("Lake Street", new PVector(294 * width / 1080, 314 * (height - headerHeight) / 575 + headerHeight));
 sites_loc.put("Midway", new PVector(462 * width / 1080, 271 * (height - headerHeight) / 575 + headerHeight));
 sites_loc.put("Gustavus Adolphus Lutheran Church - Northeast", new PVector(283 * width / 1080, 12 * (height - headerHeight) / 575 + headerHeight));
 sites_loc.put("Sumner Library - Northside", new PVector(101 * width / 1080, 155 * (height - headerHeight) / 575 + headerHeight));
 sites_loc.put("Rondo Library", new PVector(622 * width / 1080, 288 * (height - headerHeight) / 575 + headerHeight));

 sites_button = new HashMap < String, Button > ();
 sites_button.put("Holy Rosary Church", new Button(246 * width / 1080, 275 * (height - headerHeight) / 575 + headerHeight));
 sites_button.put("MLC Main Site", new Button(403 * width / 1080, 262 * (height - headerHeight) / 575 + headerHeight));
 sites_button.put("Arlington Hills Lutheran Church", new Button(791 * width / 1080, 230 * (height - headerHeight) / 575 + headerHeight));
 sites_button.put("Lake Street", new Button(294 * width / 1080, 314 * (height - headerHeight) / 575 + headerHeight));
 sites_button.put("Midway", new Button(462 * width / 1080, 271 * (height - headerHeight) / 575 + headerHeight));
 sites_button.put("Gustavus Adolphus Lutheran Church - Northeast", new Button(283 * width / 1080, 12 * (height - headerHeight) / 575 + headerHeight));
 sites_button.put("Sumner Library - Northside", new Button(101 * width / 1080, 155 * (height - headerHeight) / 575 + headerHeight));
 sites_button.put("Rondo Library", new Button(622 * width / 1080, 288 * (height - headerHeight) / 575 + headerHeight));


 YearlyDemData ydd = demData.getYearlyDemData(year);

 String[] sites = ydd.getSites();
 for (int i = 0; i < sites.length; i++) {
  SiteData s = ydd.getSiteData(sites[i]);
  float h = s.getHours();

  if (h < hmin) {
   hmin = h;
  }

  if (h > hmax) {
   hmax = h;
  }
 }

 chart[0] = new Chartctl();
 chart[1] = new Chartctl();

 Button b = new Button(0, 0, 67);

 for (int i = 0; i < 5; i++) {
  mapyears[i] = new Button(width - 15 - b.w, headerHeight + 70 + i * (b.h + 5), 2014 + i);
 }

} // set up

void draw() {
 fill(0);
 textFont(titleFont);
 textAlign(CENTER, CENTER);
 background(255);
 // image(img, 0, 0);
 beginShape();
 texture(img);
 // vertex( x, y, u, v) where u and v are the texture coordinates in pixels
 vertex(0, headerHeight, 0, 0);
 vertex(width, headerHeight, img.width, 0);
 vertex(width, height, img.width, img.height);
 vertex(0, height, 0, img.height);
 endShape();



 ellipseMode(CENTER);

 text("English as a Second Language (ESL) in the Twin Cities", halfwidth, 20);
 textFont(smallFont);
 text("Demographics of each site from 2014-2018", halfwidth, 60);
 textFont(hb14);
 text("Click a site to learn more", halfwidth, 60 + textAscent() * 2);
 textFont(smallFont);
 text("Resources and data provided by the Minnesota Literacy Council", halfwidth, 60 + textAscent() * 6);

 // ellipse(403,262,10,10); // MLC Main Site
 // fill(50,150,200,127);
 // ellipse(246,275,10,10); // Holy Rosary Church
 // ellipse(791,230,39,39); // Arlington Hills Lutheran Church
 // ellipse(294,314,71,71); // Lake Street
 // ellipse(462,271,10,10); // Midway
 // ellipse(283,12,22,22); // Gustavus Adolphus Lutheran Church - Northeast
 // ellipse(101,155,78,78); // Sumner Library - Northside
 // ellipse(622,288,10,10); // Rondo Library
 textAlign(LEFT);

 YearlyDemData ydd = demData.getYearlyDemData(chart[curr_chart].year);

 if (!showchart && !showlinechart) {
  String[] sites = ydd.getSites();
  for (int i = sites.length - 1; i >= 0; i--) {
   if (sites[i].equals("MLC Main Site") && (chart[curr_chart].year == 2017 || chart[curr_chart].year == 2018)) {
    continue;
   }

   SiteData s = ydd.getSiteData(sites[i]);
   PVector p = sites_loc.get(sites[i]);
   float h = s.getHours();
   fill(150, 150, 200, 127);
   float diam = ((h - hmin) * (100 - 10)) / (hmax - hmin) + 10;
   // float diam = 50;

   circle(p.x, p.y, diam);
   fill(0);
   textFont(hb14);

   Button b = sites_button.get(sites[i]);
   if (b.w != diam) {
    b.w = diam;
    sites_button.put(sites[i], b);
   }

   if (sites[i].equals("Holy Rosary Church") || sites[i].equals("MLC Main Site")) {
    textAlign(RIGHT);
    text(sites[i], p.x - diam * .6, p.y);
    textAlign(LEFT);
   } else
    text(sites[i], p.x + diam * .6, p.y);

   textFont(smallFont);

   if (sqrt(abs((p.x - mouseX) * (p.x - mouseX) + (p.y - mouseY) * (p.y - mouseY))) < diam / 2) {
    fill(255);
    rect(mouseX, mouseY, 350, 40);
    fill(0);
    text(sites[i], mouseX + 10, mouseY + 14);
    text("Contact hours = " + h + ", or about " + h / 24 + " days.", mouseX + 25, mouseY + 30);

   }
  }
  drawKey();
 }


 if (showchart) {
  if (!two_charts) {
   drawDemGraph(width - 20, height - 20, 10, 10, sizeofpts, chart[curr_chart], false);
  } else {
   drawDemGraph(width - 20, halfheight - 15, 10, 10, sizeofpts, chart[0], curr_chart == 0);
   drawDemGraph(width - 20, halfheight - 15, 10, halfheight + 5, sizeofpts, chart[1], curr_chart == 1);
  }
 }

 if (showchart || showlinechart) {
  fill(200, 0, 0);
  rect(width - 40, 0, 40, 40);
  fill(0);
  textAlign(CENTER, CENTER);
  textFont(titleFont);
  text("X", width - 20, 20);
 }

 if (!showchart) {
  for (int i = 0; i < 5; i++) {
   if (mapyears[i].yr != chart[curr_chart].year) {
    mapyears[i].on = false;
    mapyears[i].drawYbtn();
   } else {
    mapyears[i].on = true;
    mapyears[i].drawYbtn();
   }
  }
 }
}


void drawKey() {
 fill(255);
 rect(width - 150, height - 200, 145, 175);
 fill(0);
 textAlign(CENTER);
 textFont(hb14);
 text("Key", width - 70, height - 180);
 textAlign(LEFT);
 textFont(smallFont);
 fill(150, 0, 0);
 // text("Year = "+chart[curr_chart].year,width-135,height-160);
 fill(0);
 text("Roughly size of ", width - 110, height - 130);
 text("center by number", width - 110, height - 115);
 text("of contact hours", width - 110, height - 100);
 fill(150, 150, 200, 127);
 circle(width - 130, height - 130, 18);
 fill(0);
 text("*Contact hours means ", width - 145, height - 75);
 text("combined learning hours", width - 145, height - 60);
 text("of all students per year.", width - 145, height - 45);
}


void drawDemGraph(int w, int h, int x, int y, float sizeofpts, Chartctl chart, boolean current) {
 fill(255);
 rect(x, y, w, h, 3);

 if (two_charts && onoff.size() == 4) {
  // nothing
 } else {
  onoff.clear();
  lonoff.clear();
  chartyears.clear();
 }

 if (chart.site.equals("MLC Main Site") && (chart.year == 2017 || chart.year == 2018)) {
  fill(0);
  textFont(titleFont);
  text("No data available", x + w / 2, y + h / 2);
  return;
 }

 SiteData site = demData.getYearlyDemData(chart.year).getSiteData(chart.site);

 Button b = new Button(0, 0, 67);
 float ydown = 50, xright = 60, footersize = 120, rightsize = 5;

 if (!chart.key) {
  footersize = 10;
 }


 textAlign(LEFT);
 fill(150, 0, 0);
 textFont(hb14);
 text(chart.site + ": " + site.getTotalStudents() + " students ", x + xright + 8, y + ydown - textAscent() * 3);


 ArrayList < Student > students = new ArrayList < Student > (site.students);
 randomSeed(students.size());

 if (!chart.lancalc) {
  analyzelanguages(students, chart.num_lans, chart.lans);
  chart.lancalc = true;
 }

 int[][] counts = new int[][] { //      5  8
  {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0
  }, {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0
  }, {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0
  }, {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0
  }, {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0
  }
 };

 float row[] = new float[] {
  0,
  0,
  0,
  0,
  0,
  0
 };
 float col[] = new float[] {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
 };

 if (current) {
  dcir(x + 10, y + 20, color(0, 200, 0), 10);
 }

 fill(0);

 if (chart.line < 0) {
  textFont(hb14);

  for (int i = 0; i < 5; i++) {
   chartyears.add(new Button(x + xright + w / 2 + 120 + i * (b.w + 5), y + ydown - textAscent() * 4.5, 2014 + i));
   if (2014 + i == chart.year) {
    chartyears.get(chartyears.size() - 1).on = true;
    chartyears.get(chartyears.size() - 1).drawYbtn();
   } else {
    chartyears.get(chartyears.size() - 1).on = false;
    chartyears.get(chartyears.size() - 1).drawYbtn();
   }
  }

  float xdist = (float)(w - xright - rightsize) / 8;
  float ydist = (float)(h - ydown - footersize) / 5;


  for (int i = 0; i < 9; i++) {
   col[i] = x + xright + i * xdist;
   line(col[i], y + ydown, col[i], y + ydown + ydist * 5);
  }

  for (int i = 0; i < 6; i++) {
   row[i] = y + ydown + i * ydist;
   line(x + xright, row[i], x + xright + xdist * 8, row[i]);
  }

  float tx, ty;
  pushStyle();
  textAlign(CENTER, CENTER);
  textFont(hb14);

  tx = x + xright - textAscent() * 3;
  ty = y + ydown + 3 * ydist - ydist / 2;
  pushMatrix();
  translate(tx, ty);
  rotate(-HALF_PI);
  translate(-tx, -ty);
  text("Age", tx, ty);
  popMatrix();

  textFont(smallFont);

  tx = x + xright - textAscent();
  ty = y + ydown + 5 * ydist - ydist / 2;
  pushMatrix();
  translate(tx, ty);
  rotate(-HALF_PI);
  translate(-tx, -ty);
  text("<18", tx, ty);
  popMatrix();

  tx = x + xright - textAscent();
  ty = y + ydown + 4 * ydist - ydist / 2;
  pushMatrix();
  translate(tx, ty);
  rotate(-HALF_PI);
  translate(-tx, -ty);
  text("18-24", tx, ty);
  popMatrix();

  tx = x + xright - textAscent();
  ty = y + ydown + 3 * ydist - ydist / 2;
  pushMatrix();
  translate(tx, ty);
  rotate(-HALF_PI);
  translate(-tx, -ty);
  text("25-44", tx, ty);
  popMatrix();

  tx = x + xright - textAscent();
  ty = y + ydown + 2 * ydist - ydist / 2;
  pushMatrix();
  translate(tx, ty);
  rotate(-HALF_PI);
  translate(-tx, -ty);
  text("45-55", tx, ty);
  popMatrix();

  tx = x + xright - textAscent();
  ty = y + ydown + 1 * ydist - ydist / 2;
  pushMatrix();
  translate(tx, ty);
  rotate(-HALF_PI);
  translate(-tx, -ty);
  text("55<", tx, ty);
  popMatrix();

  text("No Schooling", x + xright + 1 * xdist - xdist / 2, y + ydown - textAscent());
  text("Grade 1-5", x + xright + 2 * xdist - xdist / 2, y + ydown - textAscent());
  text("Grade 6-9", x + xright + 3 * xdist - xdist / 2, y + ydown - textAscent());
  text("Grade 9-12 (no diploma)", x + xright + 4 * xdist - xdist / 2, y + ydown - textAscent());
  text("Graduated Secondary", x + xright + 5 * xdist - xdist / 2, y + ydown - textAscent());
  text("Some Postsecondary", x + xright + 6 * xdist - xdist / 2, y + ydown - textAscent());
  text("Graduated Postsecondary", x + xright + 7 * xdist - xdist / 2, y + ydown - textAscent());
  text("Unknown/Unreported", x + xright + 8 * xdist - xdist / 2, y + ydown - textAscent());

  textFont(hb14);
  text("Highest Education Level", x + xright + 8 * xdist / 2, y + ydown - textAscent() * 3);
  fill(150, 0, 0);

  popStyle();

  for (int i = 0; i < students.size(); i++) {
   int r = 0, c = 0;
   float lerpval = 0;
   Student s = students.get(i);
   String edl = s.ed_level;

   if (s.age < 18) {
    r = 4;
    lerpval = lerp(row[r + 1] - sizeofpts, row[r] + sizeofpts, (float)(s.age - 10) / (18 - 10));
   } else if (s.age < 25) {
    r = 3;
    lerpval = lerp(row[r + 1] - sizeofpts, row[r] + sizeofpts, (float)(s.age - 18) / (24 - 18));
   } else if (s.age < 45) {
    r = 2;
    lerpval = lerp(row[r + 1] - sizeofpts, row[r] + sizeofpts, (float)(s.age - 25) / (44 - 25));
   } else if (s.age < 56) {
    r = 1;
    lerpval = lerp(row[r + 1] - sizeofpts, row[r] + sizeofpts, (float)(s.age - 45) / (55 - 45));
   } else {
    r = 0;
    lerpval = lerp(row[r + 1] - sizeofpts, row[r] + sizeofpts, (float)(s.age - 56) / (94 - 56));
   }

   if (edl.equals("No Schooling")) {
    c = 0;
   } else if (edl.equals("Grades 1-5")) {
    c = 1;
   } else if (edl.equals("Grades 6-8")) {
    c = 2;
   } else if (edl.equals("Grades 9-12 (no diploma)")) {
    c = 3;
   } else if (edl.equals("Secondary School Diploma or alternate degree") || edl.equals("Secondary School Equivalent")) {
    c = 4;
   } else if (edl.equals("Some Postsecondary education (no degree)")) {
    c = 5;
   } else if (edl.equals("Postsecondary or professional degree")) {
    c = 6;
   } else if (edl.equals("Unknown") || edl.equals("Unreported")) {
    c = 7;
   }

   ArrayList < PVector > points = new ArrayList < PVector > ();

   float xloc = random(col[c] + sizeofpts, col[c + 1] - sizeofpts);
   int tryp = 0;
   while (tryp < 4) {
    for (int u = 0; u < points.size(); u++) {
     PVector tep = points.get(u);
     if (abs(xloc - tep.x) < sizeofpts * 1.5 && abs(lerpval - tep.y) < sizeofpts * 1.5) {
      xloc = random(col[c] + sizeofpts, col[c + 1] - sizeofpts);
     }
    }
    tryp++;
   }

   points.add(new PVector(xloc, lerpval));

   color clr = color(80);

   if (chart.language) {
    String sl = s.language;
    for (int m = 0; m < chart.lans.size(); m++) {
     if (sl.equals(chart.lans.get(m))) {
      clr = lancolors[m];
      m = chart.lans.size();
     } else {
      clr = lancolors[chart.num_lans];
     }
    }
   }

   if (chart.gender) {
    if (s.gender.equals("Male")) dmale(xloc, lerpval, clr, sizeofpts);
    if (s.gender.equals("Female")) dfemale(xloc, lerpval, clr, sizeofpts);
   }

   if (chart.employment) {
    if (s.work_status.equals("Unemployed")) dsq(xloc, lerpval, clr, sizeofpts);
    if (s.work_status.equals("Employed, To Be Terminated/Separated")) moon(xloc, lerpval, clr, sizeofpts);
    if (s.work_status.equals("Employed")) dcir(xloc, lerpval, clr, sizeofpts);
    if (s.work_status.equals("Not in Labor Force")) tri(xloc, lerpval, clr, sizeofpts);
   }

   if (chart.race) {
    String sr = s.race;
    for (int m = 0; m < races.length; m++) {
     if (sr.equals(races[m])) {
      dgreek(xloc, lerpval, clr, sizeofpts, greek[m], chart.employment);
      m = races.length;
     }
    }
   }

   if (!chart.employment && !chart.gender && !chart.race)
    donut(xloc, lerpval, clr, sizeofpts);

   counts[r][c]++;

  } //  students draw loop

  if (showpercents) {
   pushStyle();
   textAlign(LEFT, CENTER);
   textFont(smallFont);
   for (int rc = 0; rc < 5; rc++) {
    for (int cc = 0; cc < 8; cc++) {
     fill(255);
     if (counts[rc][cc] != 0) {
      rect(col[cc], row[rc], 90, 15);
      fill(0);
      text((float) counts[rc][cc] / site.getTotalStudents() * 100 + "%", col[cc] + 7.5, row[rc] + 7.5);
     }
    }
   }

   popStyle();
  }

 } else { // line graph
  float xdist = (float)(w - xright - rightsize) / 6;
  float ydist = (float)(h - ydown - footersize) / 5;


  for (int i = 0; i < 7; i++) {
   col[i] = x + xright + i * xdist;
   if (i == 0 || i == 6) {
    fill(0);
   } else {
    fill(150);
   }

   line(col[i], y + ydown, col[i], y + ydown + ydist * 5);
  }

  for (int i = 0; i < 6; i++) {
   row[i] = y + ydown + i * ydist;
   if (i == 0 || i == 6) {
    fill(0);
   } else {
    fill(150);
   }

   line(x + xright, row[i], x + xright + xdist * 6, row[i]);
  }
  fill(0);

  float tx, ty;
  pushStyle();
  textAlign(LEFT, CENTER);
  textFont(hb14);

  tx = x + xright - textAscent() * 3;
  ty = y + ydown + 3 * ydist - ydist / 2;
  pushMatrix();
  translate(tx, ty);
  rotate(-HALF_PI);
  translate(-tx, -ty);
  text("Percent", tx, ty);
  popMatrix();

  textFont(smallFont);

  tx = x + xright - textAscent();
  ty = row[5];
  pushMatrix();
  translate(tx, ty);
  rotate(-HALF_PI);
  translate(-tx, -ty);
  text("0%", tx, ty);
  popMatrix();

  tx = x + xright - textAscent();
  ty = row[4];
  pushMatrix();
  translate(tx, ty);
  rotate(-HALF_PI);
  translate(-tx, -ty);
  text("20%", tx, ty);
  popMatrix();

  tx = x + xright - textAscent();
  ty = row[3];
  pushMatrix();
  translate(tx, ty);
  rotate(-HALF_PI);
  translate(-tx, -ty);
  text("40%", tx, ty);
  popMatrix();

  tx = x + xright - textAscent();
  ty = row[2];
  pushMatrix();
  translate(tx, ty);
  rotate(-HALF_PI);
  translate(-tx, -ty);
  text("60%", tx, ty);
  popMatrix();

  tx = x + xright - textAscent();
  ty = row[1];
  pushMatrix();
  translate(tx, ty);
  rotate(-HALF_PI);
  translate(-tx, -ty);
  text("80%", tx, ty);
  popMatrix();

  textAlign(RIGHT);
  tx = x + xright - textAscent();
  ty = row[0];
  pushMatrix();
  translate(tx, ty);
  rotate(-HALF_PI);
  translate(-tx, -ty);
  text("100%", tx, ty);
  popMatrix();

  textAlign(CENTER);
  text("2014", col[1], y + ydown - textAscent());
  text("2015", col[2], y + ydown - textAscent());
  text("2016", col[3], y + ydown - textAscent());
  text("2017", col[4], y + ydown - textAscent());
  text("2018", col[5], y + ydown - textAscent());
  textFont(hb14);
  text("Year", col[3], y + ydown - textAscent() * 3);

  popStyle();

  pushStyle();
  if (chart.line == 0) {
   float[] ps = demData.getYearlyDemData(2014).getSiteData(chart.site).getEmployment();

   for (int yr = 2015; yr <= 2018; yr++) {
    SiteData tsite = demData.getYearlyDemData(yr).getSiteData(chart.site);
    float[] p2 = tsite.getEmployment();

    if (yr != 2014) {
     strokeWeight(sizeofpts / 2);
     for (int i = 0; i < ps.length; i++) {
      stroke(linecolors[i]);
      line(col[yr - 2014], lerp(row[5], row[0], ps[i]), col[yr - 2013], lerp(row[5], row[0], p2[i]));
      if (i == 0) {
       dcir(col[yr - 2014], lerp(row[5], row[0], ps[i]), linecolors[i], sizeofpts * 1.5);
       dcir(col[yr - 2013], lerp(row[5], row[0], p2[i]), linecolors[i], sizeofpts * 1.5);
      } else if (i == 1) {
       moon(col[yr - 2014], lerp(row[5], row[0], ps[i]), linecolors[i], sizeofpts * 1.5);
       moon(col[yr - 2013], lerp(row[5], row[0], p2[i]), linecolors[i], sizeofpts * 1.5);
      } else if (i == 2) {
       dsq(col[yr - 2014], lerp(row[5], row[0], ps[i]), linecolors[i], sizeofpts * 1.5);
       dsq(col[yr - 2013], lerp(row[5], row[0], p2[i]), linecolors[i], sizeofpts * 1.5);
      } else {
       tri(col[yr - 2014], lerp(row[5], row[0], ps[i]), linecolors[i], sizeofpts * 1.5);
       tri(col[yr - 2013], lerp(row[5], row[0], p2[i]), linecolors[i], sizeofpts * 1.5);
      }
      ps[i] = p2[i];
     }
    }
   }
  } else if (chart.line == 1) {
   float[] ps = demData.getYearlyDemData(2014).getSiteData(chart.site).getGender();

   for (int yr = 2015; yr <= 2018; yr++) {
    SiteData tsite = demData.getYearlyDemData(yr).getSiteData(chart.site);
    float[] p2 = tsite.getGender();

    if (yr != 2014) {
     strokeWeight(sizeofpts / 2);
     for (int i = 0; i < ps.length; i++) {
      stroke(linecolors[i]);
      line(col[yr - 2014], lerp(row[5], row[0], ps[i]), col[yr - 2013], lerp(row[5], row[0], p2[i]));
      if (i == 0) {
       dmale(col[yr - 2014], lerp(row[5], row[0], ps[i]), linecolors[i], sizeofpts * 1.5);
       dmale(col[yr - 2013], lerp(row[5], row[0], p2[i]), linecolors[i], sizeofpts * 1.5);
      } else {
       dfemale(col[yr - 2014], lerp(row[5], row[0], ps[i]), linecolors[i], sizeofpts * 1.5);
       dfemale(col[yr - 2013], lerp(row[5], row[0], p2[i]), linecolors[i], sizeofpts * 1.5);
      }
      ps[i] = p2[i];
     }
    }
   }
  } else if (chart.line == 2) {
   for (int i = 0; i < 5; i++) {
    chartyears.add(new Button(x + xright + w / 2 + 120 + i * (b.w + 5), y + ydown - textAscent() * 4.5, 2014 + i));
    if (2014 + i == chart.year) {
     chartyears.get(chartyears.size() - 1).on = true;
     chartyears.get(chartyears.size() - 1).drawYbtn();
    } else {
     chartyears.get(chartyears.size() - 1).on = false;
     chartyears.get(chartyears.size() - 1).drawYbtn();
    }
   }

   float[] ps = demData.getYearlyDemData(2014).getSiteData(chart.site).getLanguage(chart.lans);

   for (int yr = 2015; yr <= 2018; yr++) {
    SiteData tsite = demData.getYearlyDemData(yr).getSiteData(chart.site);
    float[] p2 = tsite.getLanguage(chart.lans);

    if (yr != 2014) {
     strokeWeight(sizeofpts / 2);
     for (int i = 0; i < ps.length; i++) {
      if (samelinecolors) {
       stroke(linecolors[i]);
       line(col[yr - 2014], lerp(row[5], row[0], ps[i]), col[yr - 2013], lerp(row[5], row[0], p2[i]));
       dcir(col[yr - 2014], lerp(row[5], row[0], ps[i]), linecolors[i], sizeofpts * 1.5);
       dcir(col[yr - 2013], lerp(row[5], row[0], p2[i]), linecolors[i], sizeofpts * 1.5);
      } else {
       stroke(lancolors[i]);
       line(col[yr - 2014], lerp(row[5], row[0], ps[i]), col[yr - 2013], lerp(row[5], row[0], p2[i]));
       dcir(col[yr - 2014], lerp(row[5], row[0], ps[i]), lancolors[i], sizeofpts * 1.5);
       dcir(col[yr - 2013], lerp(row[5], row[0], p2[i]), lancolors[i], sizeofpts * 1.5);
      }
      ps[i] = p2[i];
     }
    }
   }
  } else if (chart.line == 3) {
   float[] ps = demData.getYearlyDemData(2014).getSiteData(chart.site).getRace();

   for (int yr = 2015; yr <= 2018; yr++) {
    SiteData tsite = demData.getYearlyDemData(yr).getSiteData(chart.site);
    float[] p2 = tsite.getRace();

    if (yr != 2014) {
     strokeWeight(sizeofpts / 2);
     for (int i = 0; i < ps.length; i++) {
      stroke(linecolors[i]);
      line(col[yr - 2014], lerp(row[5], row[0], ps[i]), col[yr - 2013], lerp(row[5], row[0], p2[i]));
      dgreek(col[yr - 2014], lerp(row[5], row[0], ps[i]), linecolors[i], sizeofpts, greek[i], true);
      dgreek(col[yr - 2013], lerp(row[5], row[0], p2[i]), linecolors[i], sizeofpts, greek[i], true);
      // dcir(col[yr-2014],lerp(row[5],row[0],ps[i]),lancolors[i],sizeofpts*2);
      // dcir(col[yr-2013],lerp(row[5],row[0],p2[i]),lancolors[i],sizeofpts*2);
      ps[i] = p2[i];
     }
    }
   }
  }



  popStyle();

 }



 //     KEY CODE

 if (chart.key) {
  float topx = halfwidth - 567, topy = y + h - 90;
  float temp;

  pushStyle();
  textFont(hb14);
  fill(0);
  text("Employment", topx, topy);

  onoff.add(new Button(topx + 90, topy - textAscent() - 4, chart.employment));
  onoff.get(onoff.size() - 1).drawRbtn();

  lonoff.add(new Button());
  lonoff.get(lonoff.size() - 1).x = topx + 90 + 3 + 35;
  lonoff.get(lonoff.size() - 1).y = topy - textAscent() - 4;
  lonoff.get(lonoff.size() - 1).on = (chart.line == 0);
  lonoff.get(lonoff.size() - 1).drawLbtn();

  color cs = color(0);

  if (chart.employment) {
   temp = textAscent();
   textFont(smallFont);
   if (chart.line == 0) {
    cs = linecolors[0];
   }

   dcir(topx + 8, topy + temp, cs, 8);
   fill(0);
   text("Employed", topx + 16, topy + temp + 5);
   temp = textAscent();
   if (chart.line == 0) {
    cs = linecolors[1];
   }

   moon(topx + 8, topy + temp + 16, cs, 8);
   fill(0);
   text("Employed, To Be Terminated/Separated", topx + 16, topy + temp + 22);

   if (chart.line == 0) {
    cs = linecolors[2];
   }

   dsq(topx + 8, topy + temp + 32, cs, 8);
   fill(0);
   text("Unemployed", topx + 16, topy + temp + 37);
   if (chart.line == 0) {
    cs = linecolors[3];
   }
   tri(topx + 8, topy + temp + 48, cs, 8);
   fill(0);
   text("Not in Labor Force", topx + 16, topy + temp + 54);
  }
  noFill();
  rect(topx - 4, topy - 20, 260, 90);

  topx += 265;
  textFont(hb14);
  fill(0);

  text("Gender", topx, topy);
  onoff.add(new Button(topx + 60, topy - textAscent() - 4, chart.gender));
  onoff.get(onoff.size() - 1).drawRbtn();
  lonoff.add(new Button());
  lonoff.get(lonoff.size() - 1).x = topx + 60 + 3 + 35;
  lonoff.get(lonoff.size() - 1).y = topy - textAscent() - 4;
  lonoff.get(lonoff.size() - 1).on = (chart.line == 1);
  lonoff.get(lonoff.size() - 1).drawLbtn();

  if (chart.gender) {
   temp = textAscent();
   textFont(smallFont);
   if (chart.line == 1) {
    cs = linecolors[0];
   }
   dmale(topx + 2, topy + temp + 5, cs, 8);
   fill(0);
   text("Male", topx + 16, topy + temp + 5);
   temp = textAscent();
   if (chart.line == 1) {
    cs = linecolors[1];
   }

   dfemale(topx + 6, topy + temp + 10, cs, 8);
   fill(0);
   text("Female", topx + 16, topy + temp + 22);
  }
  noFill();
  rect(topx - 4, topy - 20, 130, 90);

  topx += 135;
  textFont(hb14);
  fill(0);

  text("Top Languages", topx, topy);
  onoff.add(new Button(topx + 115, topy - textAscent() - 4, chart.language));
  onoff.get(onoff.size() - 1).drawRbtn();

  lonoff.add(new Button());
  lonoff.get(lonoff.size() - 1).x = topx + 115 + 3 + 35;
  lonoff.get(lonoff.size() - 1).y = topy - textAscent() - 4;
  lonoff.get(lonoff.size() - 1).on = (chart.line == 2);
  lonoff.get(lonoff.size() - 1).drawLbtn();

  if (chart.language) {
   temp = textAscent();
   textFont(smallFont);


   if (chart.line == 2) {
    if (samelinecolors) {
     cs = linecolors[0];
    } else {
     cs = lancolors[0];
    }
    dcir(topx + 8, topy + temp, cs, 8);
    fill(0);
    text(chart.lans.get(0), topx + 16, topy + temp + 4);

    temp = textAscent();
    for (int i = 1; i < chart.lans.size(); i++) {
     if (samelinecolors) {
      cs = linecolors[i];
     } else {
      cs = lancolors[i];
     }
     if (i < 4) {
      dcir(topx + 8, topy + temp + 15 * i, cs, 8);
      fill(0);
      if (i == chart.lans.size()) {
       text("Other", topx + 16, topy + 14 + 15 * i);
      } else {
       text(chart.lans.get(i), topx + 16, topy + 14 + 15 * i);
      }
     } else {
      dcir(topx + 98, topy + temp + 15 * (i - 4), cs, 8);
      fill(0);
      if (i == chart.lans.size()) {
       text("Other", topx + 106, topy + 14 + 15 * (i - 4));
      } else {
       text(chart.lans.get(i), topx + 106, topy + 14 + 15 * (i - 4));
      }
     }
    }
   } else {
    dcir(topx + 8, topy + temp, lancolors[0], 8);
    fill(0);
    text(chart.lans.get(0), topx + 16, topy + temp + 4);

    temp = textAscent();
    for (int i = 1; i <= chart.lans.size(); i++) {
     if (i < 4) {
      dcir(topx + 8, topy + temp + 15 * i, lancolors[i], 8);
      fill(0);
      if (i == chart.lans.size()) {
       text("Other", topx + 16, topy + 14 + 15 * i);
      } else {
       text(chart.lans.get(i), topx + 16, topy + 14 + 15 * i);
      }
     } else {
      dcir(topx + 98, topy + temp + 15 * (i - 4), lancolors[i], 8);
      fill(0);
      if (i == chart.lans.size()) {
       text("Other", topx + 106, topy + 14 + 15 * (i - 4));
      } else {
       text(chart.lans.get(i), topx + 106, topy + 14 + 15 * (i - 4));
      }
     }
    }
   }
  }

  noFill();
  rect(topx - 4, topy - 20, 200, 90);

  topx += 205;
  textFont(hb14);
  fill(0);

  text("Race", topx, topy);
  onoff.add(new Button(topx + 40, topy - textAscent() - 4, chart.race));
  onoff.get(onoff.size() - 1).drawRbtn();

  lonoff.add(new Button());
  lonoff.get(lonoff.size() - 1).x = topx + 40 + 3 + 35;
  lonoff.get(lonoff.size() - 1).y = topy - textAscent() - 4;
  lonoff.get(lonoff.size() - 1).on = (chart.line == 3);
  lonoff.get(lonoff.size() - 1).drawLbtn();

  if (chart.race) {
   temp = textAscent();
   textFont(smallFont);
   fill(0);
   if (chart.line == 3) {
    fill(linecolors[0]);
   }
   text(greek[0], topx + 8, topy + temp);
   fill(0);
   text(races[0], topx + 19, topy + temp + 2);

   temp = textAscent();

   for (int i = 1; i < 7; i++) {
    if (i < 4) {
     if (chart.line == 3) {
      fill(linecolors[i]);
     }
     text(greek[i], topx + 8, topy + temp + 16 * i);
     fill(0);
     text(races[i], topx + 19, topy + 12 + 16 * i);
    } else {
     if (chart.line == 3) {
      fill(linecolors[i]);
     }
     text(greek[i], topx + 238, topy + temp + 2 + 16 * (i - 4));
     fill(0);
     text(races[i], topx + 249, topy + 12 + 16 * (i - 4));
    }
   }
  }
  noFill();
  rect(topx - 4, topy - 20, 530, 90);
  popStyle();

  if (two_charts && onoff.size() == 4) {
   // nothing
  } else {
   onoff_copy.clear();
   lonoff_copy.clear();
   chartyears_copy.clear();
   onoff_copy.addAll(onoff);
   lonoff_copy.addAll(lonoff);
   chartyears_copy.addAll(chartyears);
  }

 }


} // drawDemGraph


void donut(float x, float y, color c, float d) {
 pushStyle();
 noFill();
 strokeWeight(d / 2);
 stroke(c);
 circle(x, y, d);
 popStyle();
}

void moon(float x, float y, color c, float d) {
 pushStyle();
 fill(c);
 strokeWeight(.5);
 ellipseMode(CENTER);
 arc(x, y, d, d, -.15, PI + QUARTER_PI, PIE);
 popStyle();
}

void dcir(float x, float y, color c, float d) {
 pushStyle();
 fill(c);
 strokeWeight(.5);
 circle(x, y, d);
 popStyle();
}

void tri(float x, float y, color c, float d) {
 pushStyle();
 fill(c);
 strokeWeight(.5);
 float r = d / 2;
 triangle(x - r, y + r, x, y - r, x + r, y + r);
 popStyle();
}

void dsq(float x, float y, color c, float d) {
 pushStyle();
 fill(c);
 strokeWeight(.5);
 rectMode(CENTER);
 square(x, y, d);
 popStyle();
}

void dmale(float x, float y, color c, float d) {
 pushStyle();
 float r = d / 2, b = d * .707, k = r * .6;
 noFill();
 strokeWeight(k);
 stroke(c);
 x += k;
 y -= k;
 line(x, y, x + b, y - b);
 line(x + b, y - b, x + b - r, y - b);
 line(x + b, y - b, x + b, y - b + r);
 popStyle();
}

void dfemale(float x, float y, color c, float d) {
 pushStyle();
 float r = d / 2, b = d * .707, k = r * .6;
 noFill();
 strokeWeight(k);
 stroke(c);
 y += k;
 line(x, y, x, y + d);
 line(x - b * .5, y + b, x + b * .5, y + b);
 popStyle();
}

void dgreek(float x, float y, color c, float d, String l, boolean e) {
 pushStyle();
 textFont(smallFont);
 float r = d / 2, b = d * .707;
 fill(c);
 textSize(d * 1.8);
 if (e)
  text(l, x - d, y - b);
 else text(l, x, y);
 popStyle();
}


int[] analyzelanguages(ArrayList < Student > students, int n, ArrayList < String > str) {
 str.clear();
 HashMap < String, Integer > lan = new HashMap < String, Integer > ();
 int[] count = new int[n];

 for (int i = 0; i < students.size(); i++) {
  Student s = students.get(i);
  if (!lan.containsKey(s.language)) {
   lan.put(s.language, 0);
  }

  int c = lan.get(s.language);
  lan.put(s.language, c + 1);
 }

 List < Map.Entry < String, Integer > > l =
  new LinkedList < Map.Entry < String, Integer > > (lan.entrySet());

 Collections.sort(l, new Comparator < Map.Entry < String, Integer > > () {
  public int compare(Map.Entry < String, Integer > a,
   Map.Entry < String, Integer > b) {
   return (a.getValue()).compareTo(b.getValue());
  }
 });

 Collections.reverse(l);

 for (int i = 0; i < n && i < lan.size(); i++) {
  Map.Entry < String, Integer > temp = l.get(i);
  str.add(temp.getKey());
  count[i] = temp.getValue();
 }

 return count;

}

void keyPressed() {
 if (key == '4') {
  chart[curr_chart].year = 2014;
  chart[curr_chart].lancalc = false;
 }

 if (key == '5') {
  chart[curr_chart].year = 2015;
  chart[curr_chart].lancalc = false;
 }

 if (key == '6') {
  chart[curr_chart].year = 2016;
  chart[curr_chart].lancalc = false;
 }

 if (key == '7') {
  chart[curr_chart].year = 2017;
  chart[curr_chart].lancalc = false;
 }

 if (key == '8') {
  chart[curr_chart].year = 2018;
  chart[curr_chart].lancalc = false;
 }

 if (key == 'e') {
  chart[curr_chart].employment = !chart[curr_chart].employment;
 }

 if (key == 'E') {
  chart[curr_chart].setline(0);
 }

 if (key == 'g') {
  chart[curr_chart].gender = !chart[curr_chart].gender;
 }

 if (key == 'G') {
  chart[curr_chart].setline(1);
 }

 if (key == 'l') {
  chart[curr_chart].language = !chart[curr_chart].language;
 }

 if (key == 'L') {
  chart[curr_chart].setline(2);
 }

 if (key == 'r') {
  chart[curr_chart].race = !chart[curr_chart].race;
 }

 if (key == 'R') {
  chart[curr_chart].setline(3);
 }

 if (key == 'w') {
  chart[curr_chart].work_status = !chart[curr_chart].work_status;
 }

 if (key == 'k') {
  chart[curr_chart].key = !chart[curr_chart].key;
 }

 if (key == 'x') {
  two_charts = !two_charts;
 }

 if (key == 'c') {
  showchart = !showchart;
 }

 if (key == 'a') {
  samelinecolors = !samelinecolors;
 }

 if (key == 'p') {
  showpercents = !showpercents;
 }

 if (key == ' ') {
  if (curr_chart == 0) {
   curr_chart = 1;
  } else {
   curr_chart = 0;
  }
 }

 if (key == '!') {
  chart[curr_chart].site = "Arlington Hills Lutheran Church";
  chart[curr_chart].lancalc = false;
 }

 if (key == '@') {
  chart[curr_chart].site = "Gustavus Adolphus Lutheran Church - Northeast";
  chart[curr_chart].lancalc = false;
 }

 if (key == '#') {
  chart[curr_chart].site = "Holy Rosary Church";
  chart[curr_chart].lancalc = false;
 }

 if (key == '$') {
  chart[curr_chart].site = "Lake Street";
  chart[curr_chart].lancalc = false;
 }

 if (key == '%') {
  chart[curr_chart].site = "Midway";
  chart[curr_chart].lancalc = false;
 }

 if (key == '^') {
  chart[curr_chart].site = "MLC Main Site";
  chart[curr_chart].lancalc = false;
 }

 if (key == '&') {
  chart[curr_chart].site = "Sumner Library - Northside";
  chart[curr_chart].lancalc = false;
 }

 if (key == '*') {
  chart[curr_chart].site = "Rondo Library";
  chart[curr_chart].lancalc = false;
 }

 if (key == CODED) {

  if (keyCode == UP) {
   sizeofpts += .1;
  } else if (keyCode == DOWN) {
   sizeofpts -= .1;
  }

  if (keyCode == LEFT) {
   if (chart[curr_chart].num_lans > 1) {
    chart[curr_chart].num_lans -= 1;
    chart[curr_chart].lancalc = false;
   }
  }

  if (keyCode == RIGHT) {
   if (chart[curr_chart].num_lans < 7) {
    chart[curr_chart].num_lans += 1;
    chart[curr_chart].lancalc = false;
   }
  }

 }
}

void mousePressed() {
 if (!showchart) {
  YearlyDemData ydd = demData.getYearlyDemData(chart[curr_chart].year);
  String[] sites = ydd.getSites();
  for (int i = sites.length - 1; i >= 0; i--) {
   Button b = sites_button.get(sites[i]);
   if (b.cclicked()) {
    chart[curr_chart].site = sites[i];
    showchart = true;
    return;
   }
  }
 }

 if (mouseX >= width - 40 && mouseY <= 40) {
  if (showlinechart) {
   showlinechart = false;
   return;
  } else if (showchart) {
   showchart = false;
   chart[curr_chart].reset();
   return;
  }
 }

 if (showchart) {
  for (int i = 0; i < chartyears_copy.size(); i++) {
   Button b = chartyears_copy.get(i);
   if (b.rclicked()) {
    if (!two_charts) {
     chart[curr_chart].year = b.yr;
     return;
    } else if (i < 5) {
     chart[1].year = b.yr;
     return;
    } else {
     chart[0].year = b.yr;
     return;
    }
   }
  }

  for (int i = 0; i < lonoff_copy.size(); i++) {
   Button b = lonoff_copy.get(i);
   if (b.rclicked()) {
    if (!two_charts) {
     chart[curr_chart].setline(i);
     return;
    } else if (i < 4) {
     chart[1].setline(i);
     return;
    } else {
     i -= 4;
     chart[0].setline(i);
     return;
    }
   }

  }

  for (int i = 0; i < onoff_copy.size(); i++) {
   Button b = onoff_copy.get(i);
   if (b.rclicked()) {
    if (!two_charts) {
     if (i == 0) {
      chart[curr_chart].employment = !chart[curr_chart].employment;
      return;
     }
     if (i == 1) {
      chart[curr_chart].gender = !chart[curr_chart].gender;
      return;
     }
     if (i == 2) {
      chart[curr_chart].language = !chart[curr_chart].language;
      return;
     }
     if (i == 3) {
      chart[curr_chart].race = !chart[curr_chart].race;
      return;
     }
    } else {
     if (i == 0) {
      chart[1].employment = !chart[1].employment;
      return;
     }
     if (i == 1) {
      chart[1].gender = !chart[1].gender;
      return;
     }
     if (i == 2) {
      chart[1].language = !chart[1].language;
      return;
     }
     if (i == 3) {
      chart[1].race = !chart[1].race;
      return;
     }
     if (i == 4) {
      chart[0].employment = !chart[0].employment;
      return;
     }
     if (i == 5) {
      chart[0].gender = !chart[0].gender;
      return;
     }
     if (i == 6) {
      chart[0].language = !chart[0].language;
      return;
     }
     if (i == 7) {
      chart[0].race = !chart[0].race;
      return;
     }
    }
   }
  }
 }

 for (int i = 0; i < 5; i++) {
  Button b = mapyears[i];
  if (!showchart && b.rclicked()) {
   chart[curr_chart].year = i + 2014;
   return;
  }
 }
}


class Chartctl {
 public int year;
 public String site;
 public boolean employment;
 public boolean gender;
 public boolean language;
 public boolean race;
 public boolean work_status;
 public boolean lancalc;
 public boolean key;
 public ArrayList < String > lans;
 public int num_lans;
 public int line;

 public Chartctl() {
  this.year = 2014;
  this.site = "Arlington Hills Lutheran Church";
  this.employment = false;
  this.gender = false;
  this.language = false;
  this.race = false;
  this.work_status = false;
  this.lancalc = false;
  this.key = true;
  this.lans = new ArrayList < String > ();
  this.num_lans = 3;
  this.line = -1;
 }

 public void reset() {
  this.employment = false;
  this.gender = false;
  this.language = false;
  this.race = false;
  this.work_status = false;
  this.lancalc = false;
  this.key = true;
  this.num_lans = 3;
  this.line = -1;
 }

 void setline(int j) {
  if (this.line == j) {
   this.line = -1;
  } else {
   this.line = j;
  }

  this.employment = false;
  this.gender = false;
  this.language = false;
  this.race = false;

  if (this.line == 0) {
   this.employment = true;
   this.gender = false;
   this.language = false;
   this.race = false;
  } else if (this.line == 1) {
   this.employment = false;
   this.gender = true;
   this.language = false;
   this.race = false;
  } else if (this.line == 2) {
   this.employment = false;
   this.gender = false;
   this.language = true;
   this.race = false;
  } else if (this.line == 3) {
   this.employment = false;
   this.gender = false;
   this.language = false;
   this.race = true;
  }
 }

}


class Button {
 public float x;
 public float y;
 public float w;
 public float h;
 public boolean on;
 public int yr;

 public Button() {
  this.x = 0;
  this.y = 0;
  this.w = 15;
  this.h = 15;
  this.on = false;
  this.yr = 2014;
 }

 public Button(float x, float y, float w) {
  this.x = x;
  this.y = y;
  this.w = w;
  this.h = 0;
  this.on = false;
  this.yr = 2014;
 }

 public Button(float x, float y) {
  this.x = x;
  this.y = y;
  this.w = 35;
  this.h = 15;
  this.on = false;
  this.yr = 2014;
 }

 public Button(float x, float y, boolean b) {
  this.x = x;
  this.y = y;
  this.w = 35;
  this.h = 15;
  this.on = b;
  this.yr = 2014;
 }

 public Button(float x, float y, int yr) {
  this.x = x;
  this.y = y;
  this.w = 55;
  this.h = 25;
  this.on = false;
  this.yr = yr;
 }

 public void drawRbtn() {
  pushStyle();
  textAlign(CENTER, CENTER);
  fill(0);
  textFont(smallFont);
  if (this.on) {
   fill(226, 244, 225);
   rect(this.x, this.y, this.w, this.h);
   fill(0);
   text("ON", this.x + this.w / 2, this.y + this.h / 2);
  } else {
   fill(255);
   rect(this.x, this.y, this.w, this.h);
   fill(0);
   text("OFF", this.x + this.w / 2, this.y + this.h / 2);
  }
  popStyle();
 }

 public void drawLbtn() {
  pushStyle();
  textAlign(CENTER, CENTER);
  fill(0);
  textFont(smallFont);
  if (this.on) {
   fill(226, 244, 225);
   rect(this.x, this.y, this.w, this.h);
   fill(0);
  } else {
   fill(255);
   rect(this.x, this.y, this.w, this.h);
  }
  stroke(3);
  line(this.x + 5, this.y + this.h - 5, this.x + this.w - 5, this.y + 5);
  popStyle();
 }

 public void drawYbtn() {
  pushStyle();
  textAlign(CENTER, CENTER);
  fill(0);
  textFont(smallFont);
  if (this.on) {
   color c = #ffffd1;
   fill(c);
   rect(this.x, this.y, this.w, this.h);
   fill(0);
   text(this.yr, this.x + this.w / 2, this.y + this.h / 2);
  } else {
   fill(255);
   rect(this.x, this.y, this.w, this.h);
   fill(0);
   text(this.yr, this.x + this.w / 2, this.y + this.h / 2);
  }
  popStyle();
 }

 public boolean cclicked() {
  return sqrt(abs((this.x - mouseX) * (this.x - mouseX) + (this.y - mouseY) * (this.y - mouseY))) < this.w / 2;
 }

 public boolean rclicked() {
  return (mouseX >= this.x && mouseX <= this.x + this.w && mouseY >= this.y && mouseY <= this.y + this.h);
 }


}
