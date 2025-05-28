#include "proc_parser.h"
#include <vector>
#include <sstream>
#include <iostream> // Required for std::cerr

// Parses a single line from a /proc/[PID]/stat file.
bool parse_stat_line(const std::string& line, ProcessTimes& times) {
    std::istringstream iss(line);
    std::string token;
    std::vector<std::string> tokens;
    while (iss >> token) {
        tokens.push_back(token);
    }

    // /proc/[PID]/stat has at least 22 fields.
    // We need fields up to 22nd (index 21).
    if (tokens.size() < 22) {
        std::cerr << "Error: Not enough fields in stat line. Expected at least 22, got " << tokens.size() << "." << std::endl;
        return false;
    }

    try {
        // Field indices are 0-based. According to man proc(5):
        // (14) utime %lu
        // (15) stime %lu
        // (16) cutime %ld
        // (17) cstime %ld
        // (22) starttime %llu (or %lu prior to Linux 2.6)
        times.utime = std::stol(tokens[13]);
        times.stime = std::stol(tokens[14]);
        times.cutime = std::stol(tokens[15]);
        times.cstime = std::stol(tokens[16]);
        times.starttime = std::stol(tokens[21]); // Using stol for consistency, should be sufficient.
    } catch (const std::out_of_range& oor) {
        std::cerr << "Error parsing stat line (out_of_range): " << oor.what() << std::endl;
        return false;
    } catch (const std::invalid_argument& ia) {
        std::cerr << "Error parsing stat line (invalid_argument): " << ia.what() << std::endl;
        return false;
    }
    return true;
}

// Calculates CPU usage percentage.
double calculate_cpu_usage(const ProcessTimes& times, double system_uptime, long hz) {
    if (hz <= 0) { // Should be caught earlier, but good for robustness
        std::cerr << "Error: HZ is zero or negative." << std::endl;
        return 0.0;
    }

    long total_time_jiffies = times.utime + times.stime + times.cutime + times.cstime;
    double starttime_seconds = static_cast<double>(times.starttime) / hz;
    double elapsed_seconds = system_uptime - starttime_seconds;

    if (elapsed_seconds <= 0) {
        // This can happen if the process has just started or due to clock sync issues.
        // Or if starttime_seconds is greater than system_uptime, which implies an issue.
        // std::cerr << "Warning: Elapsed time for process is zero or negative. PID might have just started or there's a clock issue." << std::endl;
        return 0.0; // Return 0% CPU usage in this case
    }

    double total_process_time_seconds = static_cast<double>(total_time_jiffies) / hz;
    double cpu_usage = 100.0 * (total_process_time_seconds / elapsed_seconds);

    return cpu_usage;
}
