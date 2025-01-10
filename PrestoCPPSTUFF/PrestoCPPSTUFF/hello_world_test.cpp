#include "pch.h"
#include <string>
#include "../SL_PrestoStuff/helloworld.h"

TEST(HelloWorldTest, ReturnsCorrectString) {
    std::string result = getHelloWorld();
    EXPECT_EQ(result, "Hello, World!");  // Test the return value
}