#include "gtest/gtest.h"
#include "../proc_parser.h" // Adjust path as necessary if proc_parser.h is elsewhere

// Test fixture for ProcParser tests (optional, but good practice)
class ProcParserTest : public ::testing::Test {
protected:
    ProcessTimes times; // Re-usable ProcessTimes struct for tests
};

// Tests for parse_stat_line
TEST_F(ProcParserTest, ParseValidStatLine) {
    // Example stat line: PID (Name) Status ... utime stime cutime cstime ... starttime
    // Fields:          1   2      3      ... 14    15    16     17     ... 22
    // Example values:                    ... 123   456   50     60     ... 1234567
    // For simplicity, ensure all required fields up to starttime (22nd) are present.
    // Using placeholder values for fields not directly used by ProcessTimes struct.
    std::string valid_line = "1 (bash) S 0 1 1 0 -1 4202752 7020 0 0 0 123 456 50 60 20 0 1 0 1234567 268435456";
    // Add more dummy fields to ensure there are at least 22.
    for (int i = 0; i < 30; ++i) { // Ensure more than 22 fields
        valid_line += " 0";
    }
    
    ASSERT_TRUE(parse_stat_line(valid_line, times));
    EXPECT_EQ(times.utime, 123);
    EXPECT_EQ(times.stime, 456);
    EXPECT_EQ(times.cutime, 50);
    EXPECT_EQ(times.cstime, 60);
    EXPECT_EQ(times.starttime, 1234567);
}

TEST_F(ProcParserTest, ParseStatLineTooFewFields) {
    std::string short_line = "1 (bash) S 0 1 1 0 -1 4202752 7020"; // Only 10 fields
    ASSERT_FALSE(parse_stat_line(short_line, times));
}

TEST_F(ProcParserTest, ParseStatLineNonNumeric) {
    // utime (14th field) is "abc"
    std::string non_numeric_line = "1 (bash) S 0 1 1 0 -1 4202752 7020 0 0 0 abc 456 50 60 20 0 1 0 1234567 268435456";
    for (int i = 0; i < 30; ++i) {
        non_numeric_line += " 0";
    }
    ASSERT_FALSE(parse_stat_line(non_numeric_line, times));
}

// Tests for calculate_cpu_usage
TEST_F(ProcParserTest, CalculateValidCpuUsage) {
    ProcessTimes current_times;
    current_times.utime = 100;
    current_times.stime = 50;
    current_times.cutime = 10;
    current_times.cstime = 5;
    current_times.starttime = 5000; // jiffies

    double system_uptime = 100.0; // seconds
    long hz = 100;                // ticks per second

    // total_time_jiffies = 100 + 50 + 10 + 5 = 165
    // starttime_seconds = 5000 / 100 = 50.0
    // elapsed_seconds = 100.0 (system_uptime) - 50.0 (starttime_seconds) = 50.0
    // total_process_time_seconds = 165 / 100 = 1.65
    // cpu_usage = 100.0 * (1.65 / 50.0) = 3.3
    double expected_cpu_usage = 3.3;
    
    ASSERT_DOUBLE_EQ(calculate_cpu_usage(current_times, system_uptime, hz), expected_cpu_usage);
}

TEST_F(ProcParserTest, CalculateCpuUsageZeroElapsedTime) {
    ProcessTimes current_times;
    current_times.utime = 100;
    current_times.stime = 50;
    current_times.cutime = 10;
    current_times.cstime = 5;
    current_times.starttime = 10000; // jiffies, so 10000/100 = 100 seconds

    double system_uptime = 100.0; // seconds
    long hz = 100;

    // elapsed_seconds = 100.0 (system_uptime) - (10000 / 100) (starttime_seconds) = 0
    ASSERT_DOUBLE_EQ(calculate_cpu_usage(current_times, system_uptime, hz), 0.0);
}

TEST_F(ProcParserTest, CalculateCpuUsageProcessStartTimeInFuture) {
    ProcessTimes current_times;
    current_times.utime = 100;
    current_times.stime = 50;
    current_times.cutime = 10;
    current_times.cstime = 5;
    current_times.starttime = 15000; // jiffies, so 15000/100 = 150 seconds

    double system_uptime = 100.0; // seconds
    long hz = 100;

    // elapsed_seconds = 100.0 (system_uptime) - (15000 / 100) (starttime_seconds) = -50.0
    ASSERT_DOUBLE_EQ(calculate_cpu_usage(current_times, system_uptime, hz), 0.0);
}

TEST_F(ProcParserTest, CalculateCpuUsageZeroTotalTime) {
    ProcessTimes current_times;
    current_times.utime = 0;
    current_times.stime = 0;
    current_times.cutime = 0;
    current_times.cstime = 0;
    current_times.starttime = 5000; // jiffies

    double system_uptime = 100.0; // seconds
    long hz = 100;

    // total_time_jiffies = 0
    // elapsed_seconds = 100.0 - (5000/100) = 50.0
    // cpu_usage = 100.0 * ( (0/100) / 50.0) = 0.0
    ASSERT_DOUBLE_EQ(calculate_cpu_usage(current_times, system_uptime, hz), 0.0);
}

// Main function for running the tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
