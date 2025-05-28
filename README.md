# Linux CPU Status for PID

This program retrieves and displays the CPU usage percentage for two specified Process IDs (PIDs) on a Linux system.

It works by reading data from the `/proc/[PID]/stat` and `/proc/uptime` files.

## Compilation

To compile the program, ensure you have `g++` installed. Then, simply run the `make` command in the root directory of the project:

```bash
make
```

This will generate an executable file named `cpu_status`.

## Usage

To run the program, execute the compiled `cpu_status` file followed by the two PIDs you want to monitor:

```bash
./cpu_status <PID1> <PID2>
```

Replace `<PID1>` and `<PID2>` with the actual process IDs.

### Example

If you want to check the CPU usage for PID 1234 and PID 5678:

```bash
./cpu_status 1234 5678
```

Expected output (will vary based on process activity and if PIDs exist):

**Scenario 1: Both PIDs are valid and processed successfully**
```
Processing PID: 1234
CPU Usage for PID 1234: 5.67%
Processing PID: 5678
CPU Usage for PID 5678: 12.34%
```

**Scenario 2: One PID is invalid or causes an error**
```
Processing PID: 99999 
Error opening /proc/99999/stat: No such file or directory
Error processing PID 99999.
Processing PID: 5678
CPU Usage for PID 5678: 12.34%
```

## Unit Tests

The project includes unit tests using the Google Test framework to ensure the core logic is working correctly. The tests cover parsing of process statistics from `/proc/[PID]/stat` format and the CPU usage calculation.

To build and run the unit tests, use the following command:

```bash
make test
```

This will compile the test runner and execute the tests. You should see output from Google Test indicating the status of the tests (pass/fail).

## Static Analysis

The project supports static analysis using Cppcheck to help identify potential issues in the source code.

To run Cppcheck, use the following command:

```bash
make cppcheck
```

This will analyze `cpu_status.cpp` and `proc_parser.cpp` and report any findings. Ensure Cppcheck is installed on your system if you wish to use this target outside of an environment where it's pre-installed.

## Cleaning Up

To remove the compiled executable, run:

```bash
make clean
```
