# Iceoryx "Hello World" String IPC Example

This example demonstrates inter-process communication using Eclipse iceoryx, where a sender process publishes a "hello" string (with a counter), and two receiver processes (A and B) subscribe to and print this message.

## Prerequisites

1.  **Eclipse iceoryx Installation:**
    You must have Eclipse iceoryx installed and built on your system. Please follow the official installation guide: [https://iceoryx.io/latest/getting-started/installation/](https://iceoryx.io/latest/getting-started/installation/)

2.  **`iox-roudi` Daemon:**
    The iceoryx routing daemon (`iox-roudi`) must be running before you start any of the applications. You can typically run it by executing:
    ```bash
    # Source your iceoryx build environment if necessary
    # e.g., source /path/to/your/iceoryx_install/bin/iceoryx_env.sh
    iox-roudi
    ```
    Keep this running in a separate terminal.

## Code Files

*   `string_data.hpp`: Defines the `StringData` structure used for messages.
*   `sender.cpp`: The publisher application that sends "hello <counter>".
*   `receiver_a.cpp`: The first subscriber application.
*   `receiver_b.cpp`: The second subscriber application.

## Compilation

You can use the following `CMakeLists.txt` to build the example. Place it in the same directory as the `.cpp` and `.hpp` files.

```cmake
cmake_minimum_required(VERSION 3.16)
project(IceoryxHelloStringExample CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the iceoryx package
# This assumes iceoryx has been installed to a location CMake can find it
# (e.g., CMAKE_PREFIX_PATH is set, or it's in a standard system path)
find_package(iceoryx_posh REQUIRED)
find_package(iceoryx_hoofs REQUIRED)

# Add the sender executable
add_executable(sender sender.cpp)
target_link_libraries(sender PRIVATE iceoryx_posh::iceoryx_posh iceoryx_hoofs::iceoryx_hoofs)

# Add receiver_a executable
add_executable(receiver_a receiver_a.cpp)
target_link_libraries(receiver_a PRIVATE iceoryx_posh::iceoryx_posh iceoryx_hoofs::iceoryx_hoofs)

# Add receiver_b executable
add_executable(receiver_b receiver_b.cpp)
target_link_libraries(receiver_b PRIVATE iceoryx_posh::iceoryx_posh iceoryx_hoofs::iceoryx_hoofs)

# Optional: Add install rules if you want to install the executables
# install(TARGETS sender receiver_a receiver_b DESTINATION bin)
```

**Build Steps:**

1.  Create a build directory:
    ```bash
    mkdir build
    cd build
    ```
2.  Run CMake and build:
    ```bash
    cmake ..
    make
    ```
    (If iceoryx is installed in a custom location, you might need to specify `CMAKE_PREFIX_PATH`:
    `cmake -DCMAKE_PREFIX_PATH=/path/to/your/iceoryx_install ..`)

## Execution

1.  **Start `iox-roudi`:** Ensure `iox-roudi` is running in a terminal (see Prerequisites).

2.  **Open three new terminals.**

3.  **In the first new terminal, run the sender:**
    ```bash
    ./build/sender
    ```
    You should see output like:
    `hello-sender started. Sending 'hello' every second.`
    `Press Ctrl+C to terminate.`
    `hello-sender sent: 'hello 0'`
    `hello-sender sent: 'hello 1'`
    `...`

4.  **In the second new terminal, run receiver A:**
    ```bash
    ./build/receiver_a
    ```
    You should see output like:
    `hello-receiver-a started. Waiting for messages.`
    `Press Ctrl+C to terminate.`
    `hello-receiver-a received: 'hello 0'`
    `hello-receiver-a received: 'hello 1'`
    `...`

5.  **In the third new terminal, run receiver B:**
    ```bash
    ./build/receiver_b
    ```
    You should see output like:
    `hello-receiver-b started. Waiting for messages.`
    `Press Ctrl+C to terminate.`
    `hello-receiver-b received: 'hello 0'`
    `hello-receiver-b received: 'hello 1'`
    `...`

All applications can be terminated by pressing `Ctrl+C` in their respective terminals.
