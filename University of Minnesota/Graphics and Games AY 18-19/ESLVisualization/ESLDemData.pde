// Based on a file from Dan Keefe

import java.util.*;
import java.lang.*;


public class Student {
 String gender;
 int age;
 String race;
 String language;
 String ed_level;
 String work_status;

 public Student(String gender, int age, String race, String language,
  String ed_level, String work_status) {
  this.gender = gender;
  this.age = age;
  this.race = race;
  this.language = language;
  this.ed_level = ed_level;
  this.work_status = work_status;
 }


}

public class YearlyDemData {
 private int year;
 public HashMap < String, SiteData > dataBySite;

 public YearlyDemData(int year) {
  this.year = year;
  this.dataBySite = new HashMap < String, SiteData > ();
 }

 public int getYear() {
  return this.year;
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

public class SiteData {
 private String site;
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
 ArrayList < Student > students;

 public SiteData(String site) {
  this.site = site;
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
  this.students = new ArrayList < Student > ();
 }

 public void addDataRow(String gender, int age, float hours, String race,
  String language, String country, String ed_level,
  String ed_location, String work_status) {

  this.students.add(new Student(gender, age, race, language, ed_level, work_status));
  this.total_students += 1;
  this.hours += hours;

  int r = (int) Math.floor(age / this.age_step);
  int c;

  if (r < age_range_low) {
   age_range_low = r;
  } else if (r > age_range_high) {
   age_range_high = r;
  }

  if (gender.equals("Male")) {
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

 public int getTotalStudents() {
  return this.total_students;
 }

 public float getMale() {
  return (float) this.male / this.total_students;
 }

 public float getFemale() {
  return (float) this.female / this.total_students;
 }

 public float[] getGender() {
  float ret[] = new float[2];
  ret[1] = (float) this.female / (float) this.total_students;
  ret[0] = (float) this.male / (float) this.total_students;

  return (ret);
 }

 public float[] getEmployment() {
  float ret[] = new float[4];
  String[] s = {
   "Employed",
   "Employed, To Be Terminated/Separated",
   "Unemployed",
   "Not in Labor Force"
  };
  for (int i = 0; i < 4; i++) {
   if (this.work_status.containsKey(s[i])) {
    ret[i] = (float) this.work_status.get(s[i]) / this.total_students;
   } else {
    ret[i] = 0;
   }
  }

  return (ret);
 }

 public float[] getRace() {
  float ret[] = new float[7];
  String[] s = {
   "American Indian or Alaskan Native",
   "Asian",
   "Black or African American",
   "Hispanic",
   "Native Hawaiian or Other Pacific Islander",
   "Two or More Races",
   "White"
  };
  for (int i = 0; i < 7; i++) {
   if (this.race.containsKey(s[i])) {
    ret[i] = (float) this.race.get(s[i]) / this.total_students;
   } else {
    ret[i] = 0;
   }
  }

  return (ret);
 }

 public float[] getLanguage(ArrayList < String > lans) {
  float ret[] = new float[lans.size()];
  for (int i = 0; i < lans.size(); i++) {
   if (this.language.containsKey(lans.get(i))) {
    ret[i] = (float) this.language.get(lans.get(i)) / this.total_students;
   } else {
    ret[i] = 0;
   }
  }

  return (ret);
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
 private HashMap < Integer, YearlyDemData > dataByYear;

 // Constructor just initializes internal variables
 public ESLDemData() {
  dataByYear = new HashMap < Integer, YearlyDemData > ();
 }

 // This is the key top-level function to call in order to load data from a csv file.
 public void loadFromFile(String fileName) {
  Table rawData = loadTable(fileName, "header");
  for (TableRow rawRow: rawData.rows()) {
   int year = rawRow.getInt("year");
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


   if (!dataByYear.containsKey(year)) {
    YearlyDemData yd = new YearlyDemData(year);
    dataByYear.put(year, yd);
   }

   if (!dataByYear.get(year).dataBySite.containsKey(site)) {
    SiteData sd = new SiteData(site);
    dataByYear.get(year).dataBySite.put(site, sd);
   }

   if (language.equals("English") || language.equals("American Sign Language")) {} else {
    dataByYear.get(year).dataBySite.get(site).addDataRow(gender, age, hours,
     race, language,
     country, ed_level, ed_location, work_status);
   }
  }
 }


 public int[] getYears() {
  Object[] objArray = dataByYear.keySet().toArray();
  Arrays.sort(objArray);
  int[] intArray = new int[objArray.length];
  for (int i = 0; i < objArray.length; i++) {
   intArray[i] = ((Integer) objArray[i]).intValue();
  }
  return intArray;
 }


 public YearlyDemData getYearlyDemData(int year) {
  if (dataByYear.containsKey(year)) {
   return dataByYear.get(year);
  } else {
   return null;
  }
 }

}
