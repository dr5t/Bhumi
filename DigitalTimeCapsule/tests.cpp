#include <iostream>
#include <cassert>
#include <vector>
#include <ctime>
#include "Capsule.hpp"
#include "Storage.hpp"

void testCapsuleLogic()
{
    std::time_t now = std::time(nullptr);

    Capsule past("past_id", "past message", "Memory", "Tester", now - 3600);
    assert(past.isReady() == true);
    std::cout << "Test Case 1 passed: Past capsule is ready." << std::endl;

    Capsule future("future_id", "future message", "Memory", "Tester", now + 3600);
    assert(future.isReady() == false);
    std::cout << "Test Case 2 passed: Future capsule is NOT ready." << std::endl;
}

void testStorage()
{
    Storage storage;
    std::time_t now = std::time(nullptr);
    std::vector<Capsule> testData = {
        Capsule("test1", "message 1", "Work", "Tester", now - 100),
        Capsule("test2", "message 2", "Work", "Tester", now + 100)};

    storage.saveCapsules(testData);
    std::vector<Capsule> loaded = storage.loadCapsules();

    assert(loaded.size() == 2);
    assert(loaded[0].id == "test1");
    assert(loaded[0].message == "message 1");
    assert(loaded[1].id == "test2");
    assert(loaded[1].message == "message 2");

    std::cout << "Test Case 3 passed: Storage save/load works." << std::endl;
}

int main()
{
    try
    {
        testCapsuleLogic();
        testStorage();
        std::cout << "All automated tests passed successfully!" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Tests failed: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
