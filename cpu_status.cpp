/**
 * @file cpu_status.cpp
 * @brief Main program to retrieve and display CPU usage for specified PIDs.
 *
 * This program takes one or more Process IDs (PIDs) as command-line arguments,
 * reads their statistics from the /proc filesystem, calculates their CPU usage
 * relative to system uptime, and prints the usage percentage for each PID.
 */
#include <iostream>
#include <fstream>
#include <string>
// #include <vector> // No longer needed here
// #include <sstream> // No longer needed here
#include <unistd.h> // For sysconf
#include <stdio.h>  // For perror
#include "proc_parser.h" // Include the new header

/**
 * @brief Reads the stat file for a given PID and parses it to populate ProcessTimes.
 * 
 * Constructs the path to /proc/[PID]/stat, reads the first line, and then
 * calls parse_stat_line to extract timing information.
 * 
 * @param pid The process ID (as a string) to get stats for.
 * @param[out] times Output parameter. A ProcessTimes struct to be populated.
 * @return true if the stat file was read and parsed successfully.
 * @return false if the stat file cannot be opened, read, or if parsing fails.
 */
bool get_proc_stat(const std::string& pid, ProcessTimes& times) {
    std::string stat_path = "/proc/" + pid + "/stat";
    std::ifstream stat_file(stat_path);
    if (!stat_file.is_open()) {
        perror(("Error opening " + stat_path).c_str());
        return false;
    }

    std::string line;
    std::getline(stat_file, line);
    stat_file.close();

    if (line.empty()) {
        std::cerr << "Error: Stat file is empty for PID " << pid << std::endl;
        return false;
    }

    if (!parse_stat_line(line, times)) {
        // parse_stat_line prints its own detailed error via std::cerr
        // std::cerr << "Failed to parse stat line for PID " << pid << std::endl; // Redundant
        return false;
    }

    return true;
}

/**
 * @brief Reads the system uptime from /proc/uptime.
 * 
 * The /proc/uptime file contains two numbers: the uptime of the system (seconds),
 * and the amount of time spent in idle process (seconds). This function reads
 * the first value.
 * 
 * @param[out] uptime The system uptime in seconds.
 * @return true if the uptime was successfully read.
 * @return false if /proc/uptime could not be opened or read.
 */
bool get_system_uptime(double& uptime) {
    std::ifstream uptime_file("/proc/uptime");
    if (!uptime_file.is_open()) {
        perror("Error opening /proc/uptime");
        return false;
    }

    uptime_file >> uptime;
    if (uptime_file.fail()) { // Check for read failure
        perror("Error reading from /proc/uptime");
        uptime_file.close();
        return false;
    }
    uptime_file.close();
    return true;
}

/**
 * @brief Main entry point for the CPU status monitoring program.
 * 
 * Parses command-line arguments for PIDs, retrieves system uptime and clock ticks (HZ).
 * For each PID, it fetches process statistics, calculates CPU usage, and prints it.
 * 
 * @param argc Argument count. Expected to be 3 (program name + 2 PIDs).
 * @param argv Argument vector. argv[1] and argv[2] should be the PIDs.
 * @return 0 if at least one PID was processed successfully.
 * @return 1 if there's an error in arguments, system calls fail, or no PIDs could be processed.
 */
int main(int argc, const char* const argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <PID1> <PID2>" << std::endl;
        return 1;
    }

    double system_uptime_val; // Renamed to avoid conflict with function name
    if (!get_system_uptime(system_uptime_val)) {
        // Error message already printed by get_system_uptime or perror
        return 1; // Exit if system uptime cannot be read
    }

    long hz = sysconf(_SC_CLK_TCK);
    if (hz <= 0) {
        perror("Error getting sysconf(_SC_CLK_TCK)");
        std::cerr << "Warning: sysconf(_SC_CLK_TCK) failed or returned invalid value. Using default HZ=100." << std::endl;
        hz = 100; // Default HZ if sysconf fails
    }

    bool any_success = false;

    for (int i = 1; i <= 2; ++i) {
        std::string pid_str = argv[i];
        std::cout << "Processing PID: " << pid_str << std::endl;

        ProcessTimes proc_times;
        if (!get_proc_stat(pid_str, proc_times)) {
            // get_proc_stat or parse_stat_line would have printed a specific error
            std::cerr << "Failed to get process stats for PID " << pid_str << "." << std::endl;
            continue; // Skip to the next PID
        }

        double cpu_usage = calculate_cpu_usage(proc_times, system_uptime_val, hz);

        if (cpu_usage == 0.0) {
            // Check if it's due to elapsed_seconds being zero or negative to provide a more specific message.
            double starttime_check_seconds = static_cast<double>(proc_times.starttime) / hz;
            if (system_uptime_val - starttime_check_seconds <= 0) {
                 std::cerr << "Warning for PID " << pid_str << ": Process elapsed time is zero or negative. CPU usage reported as 0.00%." << std::endl;
            }
        }
        
        std::cout.precision(2);
        std::cout << "CPU Usage for PID " << pid_str << ": " << std::fixed << cpu_usage << "%" << std::endl;
        any_success = true;
    }

    return any_success ? 0 : 1; // Return 0 if at least one PID was processed successfully
}
