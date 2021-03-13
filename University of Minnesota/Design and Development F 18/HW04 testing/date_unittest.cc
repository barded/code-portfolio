// deandra bardell
// Note: I tried to use the simple test fixture for all, but didn't work for PrintDate.


#include "gtest/gtest.h"

#include <iostream>
#include <math.h>
#include <ctime>     // Used to get current date
#include <string>
#include <cstdio>    // Used for sprintf, converting int to string
#include <iomanip>   // Used to print leading zeroes for month, day

#include "date.h"


class DateTest : public ::testing::Test {
 public:
  void SetUp( ) {
    // code here will execute just before the test ensues
	first_day = Date(2018, 9, 4);
	last_day = Date(2018, 12, 11);


  }
 protected:
  Date first_day;          // first day of classes
  Date last_day;           // last day of classes
};



TEST_F(DateTest, PrintDateTests) {
  Date y2k(1999, 12, 31);              // y2k
  Date ind_day(1776, 7, 4);            // US Independence
  Date best_holiday(2018, 10, 31);     // Halloween

  std::string expected_out_1 = "1999-12-31\n";
  std::string expected_out_2 = "1776-07-04\n";
  std::string expected_out_3 = "2018-10-31\n";


  testing::internal::CaptureStdout();
  y2k.PrintDate(true);
  std::string output1 = testing::internal::GetCapturedStdout();

  testing::internal::CaptureStdout(); // You must do this EVERY TIME, or else it will segfault
  ind_day.PrintDate(true);
  std::string output2 = testing::internal::GetCapturedStdout();

  testing::internal::CaptureStdout();
  best_holiday.PrintDate(true);
  std::string output3 = testing::internal::GetCapturedStdout();


  EXPECT_EQ(output1, expected_out_1);
  EXPECT_EQ(output2, expected_out_2);
  EXPECT_EQ(output3, expected_out_3);


}


TEST_F(DateTest, PrintUsDateTests) {
  Date y2k(1999, 12, 31);              // y2k
  Date ind_day(1776, 7, 4);            // US Independence
  Date best_holiday(2018, 10, 31);     // Halloween

  std::string expected_out_1 = "12-31-1999\n";
  std::string expected_out_2 = "07-04-1776\n";
  std::string expected_out_3 = "10-31-2018\n";


  testing::internal::CaptureStdout();
  y2k.PrintUsDate(true);
  std::string output1 = testing::internal::GetCapturedStdout();

  testing::internal::CaptureStdout();
  ind_day.PrintUsDate(true);
  std::string output2 = testing::internal::GetCapturedStdout();

  testing::internal::CaptureStdout();
  best_holiday.PrintUsDate(true);
  std::string output3 = testing::internal::GetCapturedStdout();


  EXPECT_EQ(output1, expected_out_1);
  EXPECT_EQ(output2, expected_out_2);
  EXPECT_EQ(output3, expected_out_3);
}

TEST_F(DateTest, PrintDateTestsWithoutNewline) {
  Date y2k(1999, 12, 31);              // y2k
  Date ind_day(1776, 7, 4);            // US Independence
  Date best_holiday(2018, 10, 31);     // Halloween


  std::string expected_out_1 = "1999-12-31";
  std::string expected_out_2 = "1776-07-04";
  std::string expected_out_3 = "2018-10-31";


  testing::internal::CaptureStdout();
  y2k.PrintDate(false);
  std::string output1 = testing::internal::GetCapturedStdout();

  testing::internal::CaptureStdout();
  ind_day.PrintDate(false);
  std::string output2 = testing::internal::GetCapturedStdout();

  testing::internal::CaptureStdout();
  best_holiday.PrintDate(false);
  std::string output3 = testing::internal::GetCapturedStdout();


  EXPECT_EQ(output1, expected_out_1);
  EXPECT_EQ(output2, expected_out_2);
  EXPECT_EQ(output3, expected_out_3);
}

TEST_F(DateTest, PrintUsDateTestsWithoutNewline) {
  Date y2k(1999, 12, 31);              // y2k
  Date ind_day(1776, 7, 4);            // US Independence
  Date best_holiday(2018, 10, 31);     // Halloween

  std::string expected_out_1 = "12-31-1999";
  std::string expected_out_2 = "07-04-1776";
  std::string expected_out_3 = "10-31-2018";


  testing::internal::CaptureStdout();
  y2k.PrintUsDate(false);
  std::string output1 = testing::internal::GetCapturedStdout();

  testing::internal::CaptureStdout();
  ind_day.PrintUsDate(false);
  std::string output2 = testing::internal::GetCapturedStdout();

  testing::internal::CaptureStdout();
  best_holiday.PrintUsDate(false);
  std::string output3 = testing::internal::GetCapturedStdout();


  EXPECT_EQ(output1, expected_out_1);
  EXPECT_EQ(output2, expected_out_2);
  EXPECT_EQ(output3, expected_out_3);
}

TEST_F(DateTest, GetUsDate) {
  Date y2k(1999, 12, 31);              // y2k
  Date ind_day(1776, 7, 4);            // US Independence
  Date best_holiday(2018, 10, 31);     // Halloween

  EXPECT_EQ(y2k.GetUsDate(), "12-31-1999") << "GetUsDate problem";
  EXPECT_EQ(ind_day.GetUsDate(), "07-04-1776") << "GetUsDate problem";
  EXPECT_EQ(best_holiday.GetUsDate(), "10-31-2018") << "GetUsDate problem";
}

TEST_F(DateTest, GetDate) {
  Date y2k(1999, 12, 31);              // y2k
  Date ind_day(1776, 7, 4);            // US Independence
  Date best_holiday(2018, 10, 31);     // Halloween

  EXPECT_EQ(y2k.GetDate(), "1999-12-31") << "GetDate problem";
  EXPECT_EQ(ind_day.GetDate(), "1776-07-04") << "GetDate problem";
  EXPECT_EQ(best_holiday.GetDate(), "2018-10-31") << "GetDate problem";
}

/**
  *
  * Note, this is the only provided test case which actually uses our test fixture
  *
  * However, it is illegal to mix TEST() and TEST_F() in the same test case (file).
  *
  */

TEST_F(DateTest, DaysBetweenTests) {
  Date feb2816(2016,2,29);
  Date feb2815(2015,2,28);

  EXPECT_EQ(first_day.GetUsDate(), "09-04-2018") << "First day of class not setup properly";
  EXPECT_EQ(last_day.GetDate(), "2018-12-11") << "Last day of class not setup properly";
  EXPECT_EQ(first_day.DaysBetween(last_day), 98) << "Days between is not calculated properly";
  EXPECT_EQ(feb2816.GetDate(),"2016-02-29") << "feb 29 2016 not setup properly";
  EXPECT_EQ(feb2815.GetDate(),"2015-02-28") << "feb 28 2015 not setup properly";
  EXPECT_EQ(feb2815.DaysBetween(feb2816), 366) << "Days between is not calculated properly for leap year";
}


TEST_F(DateTest,DefaultConstructor){
  std::time_t t = std::time(0);
  std::tm* now = std::localtime(&t);
  int todayyear = now->tm_year + 1900;
  int todaymonth = now->tm_mon + 1;
  int todayday = now->tm_mday;
  Date today(todayyear,todaymonth,todayday);
  Date todayempty;


  testing::internal::CaptureStdout();
  todayempty.PrintDate(true);
  std::string output1 = testing::internal::GetCapturedStdout();

  testing::internal::CaptureStdout();
  today.PrintDate(true);
  std::string expected_out_1 = testing::internal::GetCapturedStdout();

  testing::internal::CaptureStdout();
  todayempty.PrintUsDate(false);
  std::string output2 = testing::internal::GetCapturedStdout();

  testing::internal::CaptureStdout();
  today.PrintUsDate(false);
  std::string expected_out_2 = testing::internal::GetCapturedStdout();

  EXPECT_EQ(output1, expected_out_1) << "current date problem (via PrintDate)";
  EXPECT_EQ(output2, expected_out_2)  << "current date problem (via PrintUsDate)";

}



TEST_F(DateTest,Epoch){
  Date ep0(0);
  Date epn1000(-1000);
  Date ep90000(90000);
  Date bigep(1538697600);

  std::string expected_out_3 = "01-02-1970";
  std::string expected_out_4 = "2018-10-05";

  testing::internal::CaptureStdout();
  ep90000.PrintUsDate(false);
  std::string output3 = testing::internal::GetCapturedStdout();

  testing::internal::CaptureStdout();
  bigep.PrintDate(false);
  std::string output4 = testing::internal::GetCapturedStdout();

  EXPECT_EQ(ep0.GetDate(), "1970-01-01")<< "epoch 0 problem (via GetDate)";;
  EXPECT_EQ(epn1000.GetUsDate(), "12-31-1969")<< "epoch -1000 problem (via GetUsDate)";;
  EXPECT_EQ(output3, expected_out_3)<< "epoch 90,000 problem (via PrintDate)";;
  EXPECT_EQ(output4, expected_out_4)<< "epoch big problem (via PrintDate)";;

}


TEST_F(DateTest, Addition){
  Date feb2816(2016,2,28);
  Date dec312000(2000,12,31);
  Date jan51972(1972,1,5);

  EXPECT_EQ((feb2816+1).GetUsDate(), "02-29-2016")<< "Leap year addition issue";
  EXPECT_EQ((feb2816+2).GetUsDate(), "03-01-2016")<< "Leap year addition issue, march 1";
  EXPECT_EQ((dec312000+1).GetDate(), "2001-01-01")<<"new year addition issue";
  EXPECT_EQ((jan51972+32099).GetUsDate(), "11-23-2059")<<"big number addition issue";

}

TEST_F(DateTest,Subtraction){
  Date jan112000(2000,1,1);
  Date mar12016(2016,3,1);
  Date mar2004(2004,3,1);

  EXPECT_EQ((jan112000-1).GetDate(), "1999-12-31")<< "jan 1 2000 subtraction issue";
  EXPECT_EQ((mar12016-6426).GetDate(), "1998-07-28")<<"big number subtraction issue";
  EXPECT_EQ((mar2004-1).GetUsDate(), "02-29-2004")<< "Leap year subtraction issue";
  EXPECT_EQ((mar2004-367).GetUsDate(), "02-28-2003")<< "Leap year subtraction issue, 1 year";


}
