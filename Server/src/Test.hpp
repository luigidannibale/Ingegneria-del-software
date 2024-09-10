#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

#include "RedisManager.hpp"
#include "Server.hpp"

class Test
{
public:
    Test();
    ~Test();

private:
    void RunTests();

    RedisManager *redisManager;
};

#endif // TEST_HPP