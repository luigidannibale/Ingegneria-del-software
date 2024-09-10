#include "Test.hpp"

Test::Test()
{
    std::cout << "Test constructor" << std::endl;

    std::thread serverThread([]()
                             { Server server = Server(); });
    serverThread.detach();

    // Sleeps 1 second
    std::this_thread::sleep_for(std::chrono::seconds(1));

    redisManager = new RedisManager();

    if (!redisManager->Connect())
    {
        std::cerr << "Error connecting to Redis" << std::endl;
        return;
    }

    RunTests();
}

Test::~Test()
{
}

void Test::RunTests()
{
    std::cout << "Running tests" << std::endl;

    std::string test = "test";
    json mess;
    std::string username;
    std::string response;

    // Test Register, correct parameters
    // Expected: user created.
    std::cout << "\033[34mTest Register, correct parameters\033[0m" << std::endl;
    username = "test";
    mess["codice"] = static_cast<int>(CodiceRichiesta::new_user);
    mess["input"]["username"] = username;

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    redisManager->SubscribeToChannel(username.c_str());
    response = redisManager->WaitResponse(true);

    try
    {
        json responseJson = json::parse(response);
        if (responseJson["codice"] == static_cast<int>(CodiceRisposta::user_created))
        {
            std::cout << "\033[32mTest Register, correct parameters: Test passed\033[0m" << std::endl;
        }
        else
        {
            std::cerr << "\033[31mTest Register, correct parameters: Test failed\033[0m" << std::endl;
            return;
        }
    }
    catch (nlohmann::detail::parse_error e)
    {
        std::cerr << "\033[31mTest Register, correct parameters: Test failed\033[0m" << std::endl;
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test Quit Search Opponent, not in queue user
    // Expected: not found.
    std::cout << "\033[34mTest Quit Search Opponent, not in queue user\033[0m" << std::endl;
    username = "test";
    mess["codice"] = static_cast<int>(CodiceRichiesta::quit_search_opponent);
    mess["input"]["user"] = username;
    mess["input"]["time_duration"] = 180;
    mess["input"]["time_increment"] = 2;

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    redisManager->SubscribeToChannel(username.c_str());
    response = redisManager->WaitResponse(true);

    try
    {
        json responseJson = json::parse(response);
        if (responseJson["codice"] == static_cast<int>(CodiceRisposta::not_found))
        {
            std::cout << "\033[32mTest Quit Search Opponent, not in queue user: Test passed\033[0m" << std::endl;
        }
        else
        {
            std::cerr << "\033[31mTest Quit Search Opponent, not in queue user: Test failed\033[0m" << std::endl;
            return;
        }
    }
    catch (nlohmann::detail::parse_error e)
    {
        std::cerr << "\033[31mTest Quit Search Opponent, not in queue user: Test failed\033[0m" << std::endl;
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test Search Opponent, unregistered user
    // Expected: not found.
    std::cout << "\033[34mTest Search Opponent, unregistered user\033[0m" << std::endl;
    username = "293423842389";
    mess["codice"] = static_cast<int>(CodiceRichiesta::search_opponent);
    mess["input"]["user"] = username;
    mess["input"]["time_duration"] = 180;
    mess["input"]["time_increment"] = 2;

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    redisManager->SubscribeToChannel(username.c_str());
    response = redisManager->WaitResponse(true);
    try
    {
        json responseJson = json::parse(response);
        if (responseJson["codice"] == static_cast<int>(CodiceRisposta::not_found))
        {
            std::cout << "\033[32mTest Search Opponent, unregistered user: Test passed\033[0m" << std::endl;
        }
        else
        {
            std::cerr << "\033[31mTest Search Opponent, unregistered user: Test failed\033[0m" << std::endl;
            return;
        }
    }
    catch (nlohmann::detail::parse_error e)
    {
        std::cerr << "\033[31mTest Search Opponent, unregistered user: Test failed\033[0m" << std::endl;
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test Search Opponent, wrong time duration and increment
    // Expected: bad request.
    std::cout << "\033[34mTest Search Opponent, wrong time duration and increment\033[0m" << std::endl;
    username = test;
    mess["codice"] = static_cast<int>(CodiceRichiesta::search_opponent);
    mess["input"]["user"] = username;
    mess["input"]["time_duration"] = -1;
    mess["input"]["time_increment"] = -1;

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    redisManager->SubscribeToChannel(username.c_str());
    response = redisManager->WaitResponse(true);
    try
    {
        json responseJson = json::parse(response);
        if (responseJson["codice"] == static_cast<int>(CodiceRisposta::bad_request))
        {
            std::cout << "\033[32mTest Search Opponent, unregistered user: Test passed\033[0m" << std::endl;
        }
        else
        {
            std::cerr << "\033[31mTest Search Opponent, unregistered user: Test failed\033[0m" << std::endl;
            return;
        }
    }
    catch (nlohmann::detail::parse_error e)
    {
        std::cerr << "\033[31mTest Search Opponent, unregistered user: Test failed\033[0m" << std::endl;
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test Search Opponent, correct parameters
    // Expected: ok.
    std::cout << "\033[34mTest Search Opponent, correct parameters\033[0m" << std::endl;
    username = "test";
    mess["codice"] = static_cast<int>(CodiceRichiesta::search_opponent);
    mess["input"]["user"] = username;
    mess["input"]["time_duration"] = 180;
    mess["input"]["time_increment"] = 2;

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    redisManager->SubscribeToChannel(username.c_str());
    response = redisManager->WaitResponse(true);

    try
    {
        json responseJson = json::parse(response);
        if (responseJson["codice"] == static_cast<int>(CodiceRisposta::ok))
        {
            std::cout << "\033[32mTest Search Opponent, correct parameters: Test passed\033[0m" << std::endl;
        }
        else
        {
            std::cerr << "\033[31mTest Search Opponent, correct parameters: Test failed\033[0m" << std::endl;
            return;
        }
    }
    catch (nlohmann::detail::parse_error e)
    {
        std::cerr << "\033[31mTest Search Opponent, correct parameters: Test failed\033[0m" << std::endl;
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test Quit Search Opponent, correct parameters
    // Expected: ok.
    std::cout << "\033[34mTest Quit Search Opponent, correct parameters\033[0m" << std::endl;
    username = "test";
    mess["codice"] = static_cast<int>(CodiceRichiesta::quit_search_opponent);
    mess["input"]["user"] = username;
    mess["input"]["time_duration"] = 180;
    mess["input"]["time_increment"] = 2;

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    redisManager->SubscribeToChannel(username.c_str());
    response = redisManager->WaitResponse(true);

    try
    {
        json responseJson = json::parse(response);
        if (responseJson["codice"] == static_cast<int>(CodiceRisposta::ok))
        {
            std::cout << "\033[32mTest Quit Search Opponent, correct parameters: Test passed\033[0m" << std::endl;
        }
        else
        {
            std::cerr << "\033[31mTest Quit Search Opponent, correct parameters: Test failed\033[0m" << std::endl;
            return;
        }
    }
    catch (nlohmann::detail::parse_error e)
    {
        std::cerr << "\033[31mTest Quit Search Opponent, correct parameters: Test failed\033[0m" << std::endl;
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test Search Opponent, wrong parameters name
    // Expected: no response.
    std::cout << "\033[34mTest Search Opponent, wrong parameters name\033[0m" << std::endl;
    mess["codice"] = static_cast<int>(CodiceRichiesta::search_opponent);
    mess["input"]["something"] = test;
    mess["input"]["something_else"] = 999;

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    redisManager->SubscribeToChannel(test.c_str());
    response = redisManager->WaitResponse(true);

    if (response != "")
    {
        std::cerr << "\033[31mTest Search Opponent, wrong parameters name: Test failed\033[0m" << std::endl;
        return;
    }
    else if (response == "")
    {
        std::cout << "\033[32mTest Search Opponent, wrong parameters name: Test passed\033[0m" << std::endl;
    }
}