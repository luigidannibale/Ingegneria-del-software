#include "RedisManager.hpp"

RedisManager::RedisManager()
{
    isCommandRunning.store(false);
    isWaitingResponse.store(false);
}

RedisManager::~RedisManager()
{
}

bool RedisManager::Connect()
{
    isCommandRunning.store(true);
    input = redisConnect(HOST, PORT);
    if (input == NULL || input->err)
    {
        if (input)
        {
            std::cerr << "Connection error: " << input->errstr << std::endl;
        }
        else
        {
            std::cerr << "Connection error: can't allocate redis context" << std::endl;
        }
        isCommandRunning.store(false);
        return false;
    }
    output = redisConnect(HOST, PORT);
    if (output == NULL || output->err)
    {
        if (output)
        {
            std::cerr << "Connection error: " << output->errstr << std::endl;
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
    redisFree(output);
    redisFree(input);
    isCommandRunning.store(false);
}

bool RedisManager::CheckChannel(const char *channel)
{
    isCommandRunning.store(true);
    redisReply *reply = (redisReply *)redisCommand(output, "PUBSUB NUMSUB %s", channel);
    if (reply == NULL || reply->type != REDIS_REPLY_ARRAY || reply->elements != 2)
    {
        std::cerr << "Failed to check " << channel << " channel" << std::endl;
        freeReplyObject(reply);
        isCommandRunning.store(false);
        return false;
    }
    int numSubscribers = reply->element[1]->integer;
    freeReplyObject(reply);

    if (numSubscribers < 0)
    {
        std::cout << "No one is on the " << channel << " channel" << std::endl;
        isCommandRunning.store(false);
        return false;
    }
    isCommandRunning.store(false);
    return true;
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
        std::cerr << "Failed to subscribe to new_clients channel" << std::endl;
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
        reply = (redisReply *)redisCommand(input, "UNSUBSCRIBE");
    else
        reply = (redisReply *)redisCommand(input, "UNSUBSCRIBE %s", channel);
    if (reply == NULL || reply->type != REDIS_REPLY_ARRAY)
    {
        std::cerr << "Failed to unsubscribe from new_clients channel" << std::endl;
        freeReplyObject(reply);
        isCommandRunning.store(false);
        return false;
    }
    std::cout << "Unsubscribed from " << channel << " channel" << std::endl;
    freeReplyObject(reply);
    isCommandRunning.store(false);
    return true;
}

std::string RedisManager::WaitResponse()
{
    isCommandRunning.store(true);
    isWaitingResponse.store(true);
    while (isWaitingResponse.load())
    {
        redisReply *reply = nullptr;
        if (redisGetReply(input, (void **)&reply) == REDIS_OK && reply != nullptr)
        {
            if (reply != NULL)
            {
                std::cout << "Received message: " << reply->element[2]->str << std::endl;
                isWaitingResponse.store(false);
                return reply->element[2]->str;
            }
            else
            {
                std::cerr << "Failed to receive message" << std::endl;
            }
            freeReplyObject(reply);
            break;
        }
        std::cout << "non entra nell'if" << std::endl;
    }
    std::cout << "Quitting" << std::endl;
    isCommandRunning.store(false);
    isWaitingResponse.store(false);
    return "";
}

void RedisManager::StopWaitingResponse()
{
    isWaitingResponse.store(false);
}