// PrestoCPPSTUFFmain.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../SL_PrestoStuff/helloworld.h"
#include "../SL_PrestoStuff/Utility/Multithread/DataStructure/LocklessQueue.h"


int main()
{
    printHelloWorld();
#pragma region LocklessQueue

    LocklessQueue<int> queue;

    // Producer thread
    std::thread producer([&queue]() {
        for (int i = 0; i < 10; ++i) {
            queue.enqueue(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        });

    // Consumer thread
    std::thread consumer([&queue]() {
        int value;
        for (int i = 0; i < 10; ++i) {
            while (!queue.dequeue(value)) {
                std::this_thread::yield(); // Wait until there is data to dequeue
            }
            std::cout << "Dequeued: " << value << std::endl;
        }
        });

    producer.join();
    consumer.join();



#pragma endregion LocklessQueue
}

