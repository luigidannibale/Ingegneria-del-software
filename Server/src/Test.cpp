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
            std::cout << "\033[32mTest Search Opponent, wrong time duration and increment: Test passed\033[0m" << std::endl;
        }
        else
        {
            std::cerr << "\033[31mTest Search Opponent, wrong time duration and increment: Test failed\033[0m" << std::endl;
            return;
        }
    }
    catch (nlohmann::detail::parse_error e)
    {
        std::cerr << "\033[31mTest Search Opponent, wrong time duration and increment: Test failed\033[0m" << std::endl;
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

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test Update game, no game found
    // Expected: not found.
    std::cout << "\033[34mTest Update game, no game found\033[0m" << std::endl;
    mess["codice"] = static_cast<int>(CodiceRichiesta::update_game);
    mess["input"]["game_id"] = 99999;
    mess["input"]["moves"] = "e4";
    mess["input"]["esito"] = "D";
    mess["input"]["motivo"] = "checkmate";

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    std::string channel = "game99999";
    redisManager->SubscribeToChannel(channel.c_str());
    response = redisManager->WaitResponse(true);

    try
    {
        json responseJson = json::parse(response);
        if (responseJson["codice"] == static_cast<int>(CodiceRisposta::not_found))
        {
            std::cout << "\033[32mTest Update game, no game found: Test passed\033[0m" << std::endl;
        }
        else
        {
            std::cerr << "\033[31mTest Update game, no game found: Test failed\033[0m" << std::endl;
            return;
        }
    }
    catch (nlohmann::detail::parse_error e)
    {
        std::cerr << "\033[31mTest Update game, no game found: Test failed\033[0m" << std::endl;
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test search game by id, no game found
    // Expected: not found.
    std::cout << "\033[34mTest search game by id, no game found\033[0m" << std::endl;
    mess["codice"] = static_cast<int>(CodiceRichiesta::search_game);
    mess["input"]["game_id"] = 99999;

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    channel = "game99999";
    redisManager->SubscribeToChannel(channel.c_str());
    response = redisManager->WaitResponse(true);

    try
    {
        json responseJson = json::parse(response);
        if (responseJson["codice"] == static_cast<int>(CodiceRisposta::not_found))
        {
            std::cout << "\033[32mTest search game by id, no game found: Test passed\033[0m" << std::endl;
        }
        else
        {
            std::cerr << "\033[31mTest search game by id, no game found: Test failed\033[0m" << std::endl;
            return;
        }
    }
    catch (nlohmann::detail::parse_error e)
    {
        std::cerr << "\033[31mTest search game by id, no game found: Test failed\033[0m" << std::endl;
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test list game by user, no game found
    // Expected: not found.
    std::cout << "\033[34mTest list game by user, no game found\033[0m" << std::endl;
    mess["codice"] = static_cast<int>(CodiceRichiesta::list_games);
    mess["input"]["username"] = "prova";

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    channel = "prova";
    redisManager->SubscribeToChannel(channel.c_str());
    response = redisManager->WaitResponse(true);

    try
    {
        json responseJson = json::parse(response);
        if (responseJson["codice"] == static_cast<int>(CodiceRisposta::game_found) && responseJson["input"].is_array() && responseJson["input"].empty())
        {
            std::cout << "\033[32mTest list game by user, no game found: Test passed\033[0m" << std::endl;
        }
        else
        {
            std::cerr << "\033[31mTest list game by user, no game found: Test failed\033[0m" << std::endl;
            return;
        }
    }
    catch (nlohmann::detail::parse_error e)
    {
        std::cerr << "\033[31mTest list game by user, no game found: Test failed\033[0m" << std::endl;
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Create a game, correct parameters
    // Expected: game created and correct parameters.
    std::cout << "\033[34mCreating a game\033[0m" << std::endl;
    username = "test";
    std::string username2 = "test2";
    mess["codice"] = static_cast<int>(CodiceRichiesta::search_opponent);
    mess["input"]["user"] = username;
    mess["input"]["time_duration"] = 180;
    mess["input"]["time_increment"] = 2;

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());

    std::this_thread::sleep_for(std::chrono::seconds(1));
    mess.clear();
    mess["codice"] = static_cast<int>(CodiceRichiesta::new_user);
    mess["input"]["username"] = username2;
    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());

    std::this_thread::sleep_for(std::chrono::seconds(1));
    mess.clear();
    mess["codice"] = static_cast<int>(CodiceRichiesta::search_opponent);
    mess["input"]["user"] = username2;
    mess["input"]["time_duration"] = 180;
    mess["input"]["time_increment"] = 2;

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    redisManager->SubscribeToChannel(username.c_str());
    response = redisManager->WaitResponse(true);

    int game_id;
    try
    {
        json responseJson = json::parse(response);
        if (responseJson["codice"] == static_cast<int>(CodiceRisposta::game_created))
        {
            if (responseJson["input"]["game_id"].is_number_integer() && responseJson["input"]["opponent"] == username2 && responseJson["input"]["side"] == 0)
            {
                game_id = responseJson["input"]["game_id"];
                std::cout << "\033[32mCreating a game: Correct parameters\033[0m" << std::endl;
            }
            else
            {
                std::cerr << "\033[31mCreating a game: Test failed, wrong parameters\033[0m" << std::endl;
                return;
            }
        }
        else
        {
            std::cerr << "\033[31mCreating a game: Test failed, not created\033[0m" << std::endl;
            return;
        }
    }
    catch (nlohmann::detail::parse_error e)
    {
        std::cerr << "\033[31mCreating a game: Test failed, not created\033[0m" << std::endl;
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test Update game, wrong parameters
    // Expected: no response.
    std::cout << "\033[34mTest Update game, wrong parameters\033[0m" << std::endl;
    mess["codice"] = static_cast<int>(CodiceRichiesta::update_game);
    mess["input"]["game_id"] = game_id;
    mess["input"]["moves"] = "e4";
    mess["input"]["esito"] = "Daa";
    mess["input"]["motivo"] = "checkmatead";

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    channel = "game" + std::to_string(game_id);
    redisManager->SubscribeToChannel(channel.c_str());
    response = redisManager->WaitResponse(true);

    if (response != "")
    {
        std::cerr << "\033[31mTest Update game, wrong parameters: Test failed\033[0m" << std::endl;
        return;
    }
    else if (response == "")
    {
        std::cout << "\033[32mTest Update game, wrong parameters: Test passed\033[0m" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test Update game, correct parameters
    // Expected: ok.
    std::cout << "\033[34mTest Update game, correct parameters\033[0m" << std::endl;
    mess["codice"] = static_cast<int>(CodiceRichiesta::update_game);
    mess["input"]["game_id"] = game_id;
    mess["input"]["moves"] = "e4";
    mess["input"]["esito"] = "D";
    mess["input"]["motivo"] = "checkmate";

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    redisManager->SubscribeToChannel(channel.c_str());
    response = redisManager->WaitResponse(true);

    try
    {
        json responseJson = json::parse(response);
        if (responseJson["codice"] == static_cast<int>(CodiceRisposta::ok))
        {
            std::cout << "\033[32mTest Update game, correct parameters: Test passed\033[0m" << std::endl;
        }
        else
        {
            std::cerr << "\033[31mTest Update game, correct parameters: Test failed\033[0m" << std::endl;
            return;
        }
    }
    catch (nlohmann::detail::parse_error e)
    {
        std::cerr << "\033[31mTest Update game, correct parameters: Test failed\033[0m" << std::endl;
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test search game by id, correct parameters
    // Expected: game found.
    std::cout << "\033[34mTest search game by id, correct parameters\033[0m" << std::endl;
    mess["codice"] = static_cast<int>(CodiceRichiesta::search_game);
    mess["input"]["game_id"] = game_id;

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    channel = "game" + std::to_string(game_id);
    redisManager->SubscribeToChannel(channel.c_str());
    response = redisManager->WaitResponse(true);

    try
    {
        json responseJson = json::parse(response);
        if (responseJson["codice"] == static_cast<int>(CodiceRisposta::game_found) && responseJson["input"]["ID"] == game_id)
        {
            std::cout << "\033[32mTest search game by id, correct parameters: Test passed\033[0m" << std::endl;
        }
        else
        {
            std::cerr << "\033[31mTest search game by id, correct parameters: Test failed\033[0m" << std::endl;
            return;
        }
    }
    catch (nlohmann::detail::parse_error e)
    {
        std::cerr << "\033[31mTest search game by id, correct parameters: Test failed\033[0m" << std::endl;
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test list game by user, game found
    // Expected: game found.
    std::cout << "\033[34mTest list game by user, game found\033[0m" << std::endl;
    mess["codice"] = static_cast<int>(CodiceRichiesta::list_games);
    mess["input"]["username"] = username;

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    channel = username;
    redisManager->SubscribeToChannel(channel.c_str());
    response = redisManager->WaitResponse(true);

    try
    {
        json responseJson = json::parse(response);
        if (responseJson["codice"] == static_cast<int>(CodiceRisposta::game_found) && responseJson["input"].is_array() && !responseJson["input"].empty())
        {
            std::cout << "\033[32mTest list game by user, game found: Test passed\033[0m" << std::endl;
        }
        else
        {
            std::cerr << "\033[31mTest list game by user, game found: Test failed\033[0m" << std::endl;
            return;
        }
    }
    catch (nlohmann::detail::parse_error e)
    {
        std::cerr << "\033[31mTest list game by user, game found: Test failed\033[0m" << std::endl;
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test update user, used username
    // Expected: bad request.
    std::cout << "\033[34mTest update user, used username\033[0m" << std::endl;
    mess["codice"] = static_cast<int>(CodiceRichiesta::update_user);
    mess["input"]["username"] = username;
    mess["input"]["new_username"] = username2;
    mess["input"]["nome"] = "nome";
    mess["input"]["cognome"] = "cognome";
    mess["input"]["elo"] = 0;

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    channel = username;
    redisManager->SubscribeToChannel(channel.c_str());
    response = redisManager->WaitResponse(true);

    try
    {
        json responseJson = json::parse(response);
        if (responseJson["codice"] == static_cast<int>(CodiceRisposta::bad_request))
        {
            std::cout << "\033[32mTest update user, used username: Test passed\033[0m" << std::endl;
        }
        else
        {
            std::cerr << "\033[31mTest update user, used username: Test failed\033[0m" << std::endl;
            return;
        }
    }
    catch (nlohmann::detail::parse_error e)
    {
        std::cerr << "\033[31mTest update user, used username: Test failed\033[0m" << std::endl;
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // // Test update user, correct parameters
    // // Expected: ok.
    // std::cout << "\033[34mTest update user, correct parameters\033[0m" << std::endl;
    // mess["codice"] = static_cast<int>(CodiceRichiesta::update_user);
    // mess["input"]["username"] = username2;
    // mess["input"]["new_username"] = "test3";
    // mess["input"]["nome"] = "nome";
    // mess["input"]["cognome"] = "cognome";
    // mess["input"]["elo"] = 0;

    // redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    // channel = username2;
    // redisManager->SubscribeToChannel(channel.c_str());
    // response = redisManager->WaitResponse(true);

    // try
    // {
    //     json responseJson = json::parse(response);
    //     if (responseJson["codice"] == static_cast<int>(CodiceRisposta::ok))
    //     {
    //         std::cout << "\033[32mTest update user, correct parameters: Test passed\033[0m" << std::endl;
    //     }
    //     else
    //     {
    //         std::cerr << "\033[31mTest update user, correct parameters: Test failed\033[0m" << std::endl;
    //         return;
    //     }
    // }
    // catch (nlohmann::detail::parse_error e)
    // {
    //     std::cerr << "\033[31mTest update user, correct parameters: Test failed\033[0m" << std::endl;
    //     return;
    // }

    // Test Update preference, user not found
    // Expected: not found.
    std::cout << "\033[34mTest Update preference, user not found\033[0m" << std::endl;
    mess["codice"] = static_cast<int>(CodiceRichiesta::update_userPreference);
    mess["input"]["username"] = "prova123123123";
    mess["input"]["chessboard_style"] = "brown";
    mess["input"]["pieces_style"] = "neo";

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    channel = "prova123123123";
    redisManager->SubscribeToChannel(channel.c_str());
    response = redisManager->WaitResponse(true);

    try
    {
        json responseJson = json::parse(response);
        if (responseJson["codice"] == static_cast<int>(CodiceRisposta::not_found))
        {
            std::cout << "\033[32mTest Update preference, user not found: Test passed\033[0m" << std::endl;
        }
        else
        {
            std::cerr << "\033[31mTest Update preference, user not found: Test failed\033[0m" << std::endl;
            return;
        }
    }
    catch (nlohmann::detail::parse_error e)
    {
        std::cerr << "\033[31mTest Update preference, user not found: Test failed\033[0m" << std::endl;
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test Update preference, wrong parameters
    // Expected: no response.
    std::cout << "\033[34mTest Update preference, wrong parameters\033[0m" << std::endl;
    mess["codice"] = static_cast<int>(CodiceRichiesta::update_userPreference);
    mess["input"]["username"] = username;
    mess["input"]["chessboard_style"] = "aaa";
    mess["input"]["pieces_style"] = "aaa";

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    channel = username;
    redisManager->SubscribeToChannel(channel.c_str());
    response = redisManager->WaitResponse(true);

    if (response != "")
    {
        std::cerr << "\033[31mTest Update preference, wrong parameters: Test failed\033[0m" << std::endl;
        return;
    }
    else if (response == "")
    {
        std::cout << "\033[32mTest Update preference, wrong parameters: Test passed\033[0m" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test Update preference, correct parameters
    // Expected: ok.
    std::cout << "\033[34mTest Update preference, correct parameters\033[0m" << std::endl;
    mess["codice"] = static_cast<int>(CodiceRichiesta::update_userPreference);
    mess["input"]["username"] = username;
    mess["input"]["chessboard_style"] = "black";
    mess["input"]["pieces_style"] = "neo";

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    channel = username;
    redisManager->SubscribeToChannel(channel.c_str());
    response = redisManager->WaitResponse(true);

    try
    {
        json responseJson = json::parse(response);
        if (responseJson["codice"] == static_cast<int>(CodiceRisposta::ok))
        {
            std::cout << "\033[32mTest Update preference, correct parameters: Test passed\033[0m" << std::endl;
        }
        else
        {
            std::cerr << "\033[31mTest Update preference, correct parameters: Test failed\033[0m" << std::endl;
            return;
        }
    }
    catch (nlohmann::detail::parse_error e)
    {
        std::cerr << "\033[31mTest Update preference, correct parameters: Test failed\033[0m" << std::endl;
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    redisManager->UnsubscribeFromChannel();
    mess.clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test Wrong Code variable type
    // Expected: no response
    std::cout << "\033[34mTest Wrong Code variable type\033[0m" << std::endl;
    mess["codice"] = "test";
    mess["input"]["game_id"] = game_id;

    redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, mess.dump().c_str());
    channel = "game" + std::to_string(game_id);
    redisManager->SubscribeToChannel(channel.c_str());
    response = redisManager->WaitResponse(true);

    if (response != "")
    {
        std::cerr << "\033[31mTest Wrong Code variable type: Test failed\033[0m" << std::endl;
        return;
    }
    else if (response == "")
    {
        std::cout << "\033[32mTest Wrong Code variable type: Test passed\033[0m" << std::endl;
    }
}