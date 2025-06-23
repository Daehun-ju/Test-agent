#include "iceoryx_posh/popo/subscriber.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"
#include "iceoryx_hoofs/posix_wrapper/signal_watcher.hpp"
#include "string_data.hpp" // Our custom data type

#include <iostream>
#include <thread> // For std::this_thread::sleep_for

constexpr char APP_NAME[] = "hello-receiver-a";

int main()
{
    // Initialize the iceoryx runtime
    iox::runtime::PoshRuntime::initRuntime(APP_NAME);

    // Create a subscriber for our StringData type
    // It must match the publisher's Service, Instance, and Event
    iox::popo::Subscriber<StringData> subscriber({"Chat", "HelloWorld", "Message"});

    std::cout << APP_NAME << " started. Waiting for messages." << std::endl;
    std::cout << "Press Ctrl+C to terminate." << std::endl;

    // Loop until Ctrl+C is pressed
    while (!iox::posix::hasTerminationRequested())
    {
        subscriber.take()
            .and_then([&](auto& sample) {
                // Data is valid, access it
                std::cout << APP_NAME << " received: '" << sample->message << "'" << std::endl;
                // The sample is automatically released when it goes out of scope (RAII)
            })
            .or_else([](auto& result) {
                // Only print an error if it's not simply "no new data"
                if (result != iox::popo::ChunkReceiveResult::NO_CHUNK_AVAILABLE)
                {
                    std::cerr << APP_NAME << " Error: Failed to take sample. Error code: "
                              << static_cast<uint64_t>(result) << std::endl;
                }
            });

        // Sleep for a short duration to avoid busy polling
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
