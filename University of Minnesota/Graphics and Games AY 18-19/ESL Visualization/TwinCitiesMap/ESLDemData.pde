// Based on a file from Dan Keefe

import java.util.*;
import java.lang.*;

public class SiteData {
 private String site;
 private PVector location;
 private int total_students;
 private int male;
 private int female;
 private int age_range_low, age_range_high, age_step;
 private float hours;
 private HashMap < Integer, Integer > age_range_f; //<range, count>
 private HashMap < Integer, Integer > age_range_m; //<range, count>
 private HashMap < String, Integer > race; // <race, count>
 private HashMap < String, Integer > language; // <language, count>
 private HashMap < String, Integer > country; // <country, count>
 private HashMap < String, Integer > ed_level; // <ed_level, count>
 private HashMap < String, Integer > ed_location; // <ed_location, count>
 private HashMap < String, Integer > work_status; // <work_status, count>

 public SiteData(String site) {
  this.site = site;
  this.location = new PVector(0, 0);
  this.total_students = 0;
  this.male = 0;
  this.female = 0;
  this.age_range_low = 100;
  this.age_range_high = 0;
  this.age_step = 5; //  range is 5 years
  this.hours = 0;
  this.age_range_f = new HashMap < Integer, Integer > ();
  this.age_range_m = new HashMap < Integer, Integer > ();
  this.race = new HashMap < String, Integer > ();
  this.language = new HashMap < String, Integer > ();
  this.country = new HashMap < String, Integer > ();
  this.ed_level = new HashMap < String, Integer > ();
  this.ed_location = new HashMap < String, Integer > ();
  this.work_status = new HashMap < String, Integer > ();
 }

 public void addDataRow(String gender, int age, float hours, String race,
  String language, String country, String ed_level,
  String ed_location, String work_status) {
  this.total_students += 1;
  this.hours += hours;

  int r = (int) Math.floor(age / this.age_step);
  int c;

  if (r < age_range_low) {
   age_range_low = r;
  } else if (r > age_range_high) {
   age_range_high = r;
  }

  if (gender == "Male") {
   this.male += 1;

   if (!this.age_range_m.containsKey(r)) {
    this.age_range_m.put(r, 0);
   }
   c = this.age_range_m.get(r);
   this.age_range_m.put(r, c + 1);
  } else {
   this.female += 1;

   if (!this.age_range_f.containsKey(r)) {
    this.age_range_f.put(r, 0);
   }
   c = this.age_range_f.get(r);
   this.age_range_f.put(r, c + 1);
  }

  if (!this.race.containsKey(race)) {
   this.race.put(race, 0);
  }
  c = this.race.get(race);
  this.race.put(race, c + 1);

  if (!this.language.containsKey(language)) {
   this.language.put(language, 0);
  }
  c = this.language.get(language);
  this.language.put(language, c + 1);

  if (!this.country.containsKey(country)) {
   this.country.put(country, 0);
  }
  c = this.country.get(country);
  this.country.put(country, c + 1);

  if (!this.ed_level.containsKey(ed_level)) {
   this.ed_level.put(ed_level, 0);
  }
  c = this.ed_level.get(ed_level);
  this.ed_level.put(ed_level, c + 1);

  if (!this.ed_location.containsKey(ed_location)) {
   this.ed_location.put(ed_location, 0);
  }
  c = this.ed_location.get(ed_location);
  this.ed_location.put(ed_location, c + 1);

  if (!this.work_status.containsKey(work_status)) {
   this.work_status.put(work_status, 0);
  }
  c = this.work_status.get(work_status);
  this.work_status.put(work_status, c + 1);
 }

 public String getSite() {
  return this.site;
 }

 public PVector getLocation() {
  return this.location;
 }

 public void addtoLocation(float x, float y) {
  this.location.x += x;
  this.location.y += y;
 }


 public int getTotalStudents() {
  return this.total_students;
 }

 public int getMale() {
  return this.male;
 }

 public int getFemale() {
  return this.female;
 }

 public float getHours() {
  return this.hours;
 }

 public int getAgeRangeMax() {
  return this.age_range_high;
 }

 public int getAgeRangeMin() {
  return this.age_range_low;
 }

 public int getAgeRangeStep() {
  return this.age_step;
 }

 public int[] getAgesF() {
  List < Integer > list = new ArrayList < Integer > (this.age_range_f.keySet());
  int[] r = new int[list.size()];
  for (int i = 0; i < list.size(); i++) {
   r[i] = list.get(i);
  }

  return r;
 }

 public int[] getAgesM() {
  List < Integer > list = new ArrayList < Integer > (this.age_range_m.keySet());
  int[] r = new int[list.size()];
  for (int i = 0; i < list.size(); i++) {
   r[i] = list.get(i);
  }

  return r;
 }

 public int getAgeCountM(int key) {
  if (this.age_range_m.containsKey(key)) {
   return this.age_range_m.get(key);
  } else {
   return 0;
  }
 }

 public int getAgeCountF(int key) {
  if (this.age_range_f.containsKey(key)) {
   return this.age_range_f.get(key);
  } else {
   return 0;
  }
 }

 public String[] getRaces() {
  List < String > list = new ArrayList < String > (this.race.keySet());
  String[] r = new String[list.size()];
  for (int i = 0; i < list.size(); i++) {
   r[i] = list.get(i);
  }

  return r;
 }

 public String[] getLanguages() {
  List < String > list = new ArrayList < String > (this.language.keySet());
  String[] r = new String[list.size()];
  for (int i = 0; i < list.size(); i++) {
   r[i] = list.get(i);
  }

  return r;
 }

 public String[] getCountries() {
  List < String > list = new ArrayList < String > (this.country.keySet());
  String[] r = new String[list.size()];
  for (int i = 0; i < list.size(); i++) {
   r[i] = list.get(i);
  }

  return r;
 }

 public String[] getEdLevels() {
  List < String > list = new ArrayList < String > (this.ed_level.keySet());
  String[] r = new String[list.size()];
  for (int i = 0; i < list.size(); i++) {
   r[i] = list.get(i);
  }

  return r;
 }

 public String[] getEdLocations() {
  List < String > list = new ArrayList < String > (this.ed_location.keySet());
  String[] r = new String[list.size()];
  for (int i = 0; i < list.size(); i++) {
   r[i] = list.get(i);
  }

  return r;
 }

 public String[] getWorkStatus() {
  List < String > list = new ArrayList < String > (this.work_status.keySet());
  String[] r = new String[list.size()];
  for (int i = 0; i < list.size(); i++) {
   r[i] = list.get(i);
  }

  return r;
 }

}

public class ESLDemData {
 private HashMap < String, SiteData > dataBySite;

 // Constructor just initializes internal variables
 public ESLDemData() {
  dataBySite = new HashMap < String, SiteData > ();
 }

 // This is the key top-level function to call in order to load data from a csv file.
 public void loadFromFile(String fileName) {
  Table rawData = loadTable(fileName, "header");
  for (TableRow rawRow: rawData.rows()) {
   String site = rawRow.getString("site");
   String gender = rawRow.getString("gender");
   int age = rawRow.getInt("age");
   float hours = rawRow.getFloat("hours");
   String race = rawRow.getString("race");
   String language = rawRow.getString("language");
   String country = rawRow.getString("country");
   String ed_level = rawRow.getString("ed_level");
   String ed_location = rawRow.getString("ed_location");
   String work_status = rawRow.getString("work_status");
   Float x = rawRow.getFloat("x_loc");
   Float y = rawRow.getFloat("y_loc");



   if (!dataBySite.containsKey(site)) {
    SiteData sd = new SiteData(site);
    dataBySite.put(site, sd);
   }
   dataBySite.get(site).addDataRow(gender, age, hours, race, language,
    country, ed_level, ed_location, work_status);

   if (!x.isNaN() && !y.isNaN()) {
    dataBySite.get(site).addtoLocation(x, y);
   }
  }
 }


 public String[] getSites() {
  Object[] objArray = dataBySite.keySet().toArray();
  Arrays.sort(objArray);
  String[] strArray = new String[objArray.length];
  for (int i = 0; i < objArray.length; i++) {
   strArray[i] = String.valueOf(objArray[i]);
  }
  return strArray;
 }

 public SiteData getSiteData(String site) {
  if (dataBySite.containsKey(site)) {
   return dataBySite.get(site);
  } else {
   return null;
  }
 }

}
