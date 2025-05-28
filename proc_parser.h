/**
 * @file proc_parser.h
 * @brief Declares utilities for parsing process statistics from /proc filesystem
 *        and calculating CPU usage.
 */
#ifndef PROC_PARSER_H
#define PROC_PARSER_H

#include <string> // For std::string

/**
 * @brief Stores timing information for a process.
 * All time values are in jiffies (clock ticks).
 */
struct ProcessTimes {
    long utime;      /**< Amount of time that this process has been scheduled in user mode. */
    long stime;      /**< Amount of time that this process has been scheduled in kernel mode. */
    long cutime;     /**< Amount of time that this process's waited-for children have been scheduled in user mode. */
    long cstime;     /**< Amount of time that this process's waited-for children have been scheduled in kernel mode. */
    long starttime;  /**< The time the process started after system boot. */
};

/**
 * @brief Parses a single line from a /proc/[PID]/stat file to extract process times.
 * 
 * The function expects a string containing space-separated values as read from
 * the stat file. It extracts specific fields corresponding to utime, stime,
 * cutime, cstime, and starttime.
 * 
 * @param line The string line read from the stat file.
 * @param times Output parameter. A ProcessTimes struct that will be populated with the extracted values.
 * @return true if parsing was successful and all required fields were found and valid.
 * @return false if the line is malformed, fields are missing, or values are invalid.
 */
bool parse_stat_line(const std::string& line, ProcessTimes& times);

/**
 * @brief Calculates the CPU usage percentage for a process.
 * 
 * CPU usage is calculated based on the total time spent by the process and its children,
 * relative to the elapsed time since the process started.
 * 
 * @param times A ProcessTimes struct containing the timing information for the process.
 * @param system_uptime The total system uptime in seconds (from /proc/uptime).
 * @param hz The number of clock ticks per second (obtained via sysconf(_SC_CLK_TCK)).
 * @return The calculated CPU usage as a percentage (e.g., 12.3 for 12.3%). 
 *         Returns 0.0 if the elapsed time for the process is zero or negative to prevent division by zero.
 */
double calculate_cpu_usage(const ProcessTimes& times, double system_uptime, long hz);

#endif // PROC_PARSER_H
