#include "iceoryx_posh/popo/publisher.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"
#include "iceoryx_hoofs/posix_wrapper/signal_watcher.hpp"
#include "string_data.hpp" // Our custom data type

#include <iostream>
#include <thread> // For std::this_thread::sleep_for

constexpr char APP_NAME[] = "hello-sender";

int main()
{
    // Initialize the iceoryx runtime
    iox::runtime::PoshRuntime::initRuntime(APP_NAME);

    // Create a publisher for our StringData type
    // Service: "Chat", Instance: "HelloWorld", Event: "Message"
    iox::popo::Publisher<StringData> publisher({"Chat", "HelloWorld", "Message"});

    std::cout << APP_NAME << " started. Sending 'hello' every second." << std::endl;
    std::cout << "Press Ctrl+C to terminate." << std::endl;

    uint64_t counter = 0;

    // Loop until Ctrl+C is pressed
    while (!iox::posix::hasTerminationRequested())
    {
        StringData data_to_send;
        // Create the "hello" message with a counter
        data_to_send.message.assign("hello ");
        data_to_send.message.append(iox::cxx::string<10>(std::to_string(counter).c_str()));


        // Publish the data
        publisher.publishCopyOf(data_to_send)
            .or_else([&](auto& error) {
                std::cerr << APP_NAME << " Error: Unable to publish sample. Error code: "
                          << static_cast<uint64_t>(error) << std::endl;
            });

        std::cout << APP_NAME << " sent: '" << data_to_send.message << "'" << std::endl;
        counter++;

        // Wait for a second
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
