// Based on a file from Dan Keefe
import java.util.*;
import java.lang.*;


class YearlyData {
 private int year;
 private int level;
 private int num_student;
 private float hours;
 private int has_posttest;
 private float p_posttest;
 private int completed_level;
 private float level_gain;

 public YearlyData(int year) {
  this.year = year;
  this.level = 0;
  this.num_student = 0;
  this.hours = 0;
  this.has_posttest = 0;
  this.p_posttest = 0;
  this.completed_level = 0;
  this.level_gain = 0;
 }

 // Adds one row of data from the csv file
 public void addDataRow(int level, int num_student, float hours,
  int has_posttest, float p_posttest, int completed_level,
  float level_gain) {

  this.level = level;
  this.num_student = num_student;
  this.hours = hours;
  this.has_posttest = has_posttest;
  this.p_posttest = p_posttest;
  this.completed_level = completed_level;
  this.level_gain = level_gain;
 }


 public int getYear() {
  return this.year;
 }

 public int getLevel() {
  return this.level;
 }

 public int getNumStudent() {
  return this.num_student;
 }

 public float getHours() {
  return this.hours;
 }

 public int getHasPosttest() {
  return this.has_posttest;
 }

 public float getPPosttest() {
  return this.p_posttest;
 }

 public int getCompletedLevel() {
  return this.completed_level;
 }

 public float getLevelGain() {
  return this.level_gain;
 }
}


class ESLTestData {
 private HashMap < Integer, YearlyData > dataByYear;

 // Constructor just initializes internal variables
 public ESLTestData() {
  dataByYear = new HashMap < Integer, YearlyData > ();
 }

 // This is the key top-level function to call in order to load data from a csv file.
 public void loadFromFile(String fileName) {
  Table rawData = loadTable(fileName, "header");
  for (TableRow rawRow: rawData.rows()) {
   int year = rawRow.getInt("year");
   int level = rawRow.getInt("level");
   int num_student = rawRow.getInt("num_students");
   float hours = rawRow.getFloat("hours");
   int has_posttest = rawRow.getInt("has_posttest");
   float p_posttest = rawRow.getFloat("p_posttest");
   int completed_level = rawRow.getInt("completed_level");
   float level_gain = rawRow.getFloat("level_gain");

   if (!dataByYear.containsKey(year)) {
    YearlyData yd = new YearlyData(year);
    dataByYear.put(year, yd);
   }
   dataByYear.get(year).addDataRow(level, num_student, hours,
    has_posttest, p_posttest, completed_level,
    level_gain);
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

 // Provides access to the YearlyData objects that were constructed after reading the csv file.
 public YearlyData getYearlyData(int year) {
  if (dataByYear.containsKey(year)) {
   return dataByYear.get(year);
  } else {
   return null;
  }
 }
}
