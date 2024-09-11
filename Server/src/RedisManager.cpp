#include "RedisManager.hpp"

RedisManager::RedisManager()
{
    isCommandRunning.store(false);
    isWaitingResponse.store(false);
}

RedisManager::~RedisManager()
{
    // Disconnect();
}

bool RedisManager::Connect()
{
    isCommandRunning.store(true);
    input = redisConnect(INPUT_HOST, INPUT_PORT);
    if (input == NULL || input->err)
    {
        if (input)
        {
            std::cerr << "Connection input error: " << input->errstr << std::endl;
        }
        else
        {
            std::cerr << "Connection error: can't allocate redis context" << std::endl;
        }
        isCommandRunning.store(false);
        return false;
    }
    output = redisConnect(INPUT_HOST, OUTPUT_PORT);
    if (output == NULL || output->err)
    {
        if (output)
        {
            std::cerr << "Connection output error: " << output->errstr << std::endl;
        }
        else
        {
            std::cerr << "Connection error: can't allocate redis context" << std::endl;
        }
        isCommandRunning.store(false);
        return false;
    }
    isCommandRunning.store(false);
    return true;
}

void RedisManager::Disconnect()
{
    isCommandRunning.store(true);
    redisFree(input);
    redisFree(output);
    isCommandRunning.store(false);
}

bool RedisManager::PublishToChannel(const char *channel, const char *message)
{
    isCommandRunning.store(false);
    redisReply *reply = (redisReply *)redisCommand(output, "PUBLISH %s %s", channel, message);
    if (reply == NULL || reply->type != REDIS_REPLY_INTEGER)
    {
        std::cerr << "Failed to publish message to channel" << std::endl;
        std::cerr << "Errore: " << reply->str << std::endl;
        freeReplyObject(reply);
        isCommandRunning.store(false);
        return false;
    }
    freeReplyObject(reply);
    isCommandRunning.store(false);
    return true;
}

bool RedisManager::SubscribeToChannel(const char *channel)
{
    isCommandRunning.store(true);
    redisReply *reply = (redisReply *)redisCommand(input, "SUBSCRIBE %s", channel);
    if (reply == NULL || reply->type != REDIS_REPLY_ARRAY)
    {
        std::cerr << "Failed to subscribe to " << channel << std::endl;
        freeReplyObject(reply);
        isCommandRunning.store(false);
        return false;
    }
    std::cout << "Subscribed to " << channel << " channel" << std::endl;
    freeReplyObject(reply);
    isCommandRunning.store(false);
    return true;
}

bool RedisManager::UnsubscribeFromChannel(const char *channel)
{
    isCommandRunning.store(true);
    redisReply *reply = nullptr;
    if (channel == nullptr)
    {
        std::cout << "Unsubscribing from all channels" << std::endl;
        reply = (redisReply *)redisCommand(input, "UNSUBSCRIBE");
        freeReplyObject(reply);
        isCommandRunning.store(false);
        return true;
    }
    else
        reply = (redisReply *)redisCommand(input, "UNSUBSCRIBE %s", channel);
    if (reply == NULL || reply->type != REDIS_REPLY_ARRAY)
    {
        std::cerr << "Failed to unsubscribe from channel" << std::endl;
        freeReplyObject(reply);
        isCommandRunning.store(false);
        return false;
    }
    std::cout << "Unsubscribed from " << channel << " channel" << std::endl;
    freeReplyObject(reply);
    isCommandRunning.store(false);
    return true;
}

std::string RedisManager::WaitResponse(bool timeout)
{
    isCommandRunning.store(true);
    isWaitingResponse.store(true);

    struct timeval timer;
    timer.tv_sec = 0;
    timer.tv_usec = 0;
    if (timeout)
        timer.tv_sec = 2;
    redisSetTimeout(input, timer);

    while (isWaitingResponse.load())
    {
        redisReply *reply = nullptr;
        redisGetReply(input, (void **)&reply);
        if (reply == NULL)
        {
            std::cerr << "Failed to get reply from redis" << std::endl;
            freeReplyObject(reply);
            isCommandRunning.store(false);
            Connect();
            return "";
        }
        if (reply->type == REDIS_REPLY_ARRAY)
        {
            std::string response = reply->element[2]->str;
            freeReplyObject(reply);
            isCommandRunning.store(false);
            return response;
        }
        freeReplyObject(reply);
    }
    isCommandRunning.store(false);
    return "";
}