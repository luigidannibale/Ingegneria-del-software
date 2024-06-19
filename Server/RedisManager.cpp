#include "RedisManager.hpp"

RedisManager::RedisManager() {
    isCommandRunning.store(false);
    isWaitingResponse.store(false);
}


RedisManager::~RedisManager() {
    // Disconnect();
}

RedisManager::Connect() {
    isCommandRunning.store(true);
    input = redisConnect(INPUT_HOST, INPUT_PORT);
    if (input == NULL || input->err) {
        if (input) {
            std::cerr << "Connection error: " << input->errstr << std::endl;
        } else {
            std::cerr << "Connection error: can't allocate redis context" << std::endl;
        }
        isCommandRunning.store(false);
        return false;
    }
    output = redisConnect(INPUT_HOST, OUTPUT_PORT);
    if (output == NULL || output->err) {
        if (output) {
            std::cerr << "Connection error: " << output->errstr << std::endl;
        } else {
            std::cerr << "Connection error: can't allocate redis context" << std::endl;
        }
        isCommandRunning.store(false);
        return false;
    }
    isCommandRunning.store(false);
    return true;
}

RedisManager::Disconnect() {
    isCommandRunning.store(true);
    redisFree(input);
    redisFree(output);
    isCommandRunning.store(false);
}

bool RedisManager::PublishToChannel(const char* channel, const char* message) {
    isCommandRunning.store(false);
    redisReply *reply = (redisReply*)redisCommand(c, "PUBLISH %s %s", channel, message);
    if (reply == NULL || reply->type != REDIS_REPLY_INTEGER) {
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

bool RedisManager::SubscribeToChannel(const char* channel) {
    isCommandRunning.store(true);
    redisReply *reply = (redisReply*)redisCommand(input, "SUBSCRIBE %s", channel);
    if (reply == NULL || reply->type != REDIS_REPLY_ARRAY) {
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

bool RedisManager::UnsubscribeFromChannel(const char* channel) {
    isCommandRunning.store(true);
    redisReply *reply = nullptr;
    if (channel == nullptr)
        reply = (redisReply*)redisCommand(input, "UNSUBSCRIBE");
    else
        reply = (redisReply*)redisCommand(input, "UNSUBSCRIBE %s", channel);
    if (reply == NULL || reply->type != REDIS_REPLY_ARRAY) {
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

bool RedisManager::PublishToChannel(const char* channel, const char* message) {
    isCommandRunning.store(true);
    redisReply *reply = (redisReply*)redisCommand(output, "PUBLISH %s %s", channel, message);
    if (reply == NULL || reply->type != REDIS_REPLY_INTEGER) {
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

std::string RedisManager::WaitResponse() {
    isCommandRunning.store(true);
    isWaitingResponse.store(true);
    while (isWaitingResponse.load()) {
        redisReply *reply = nullptr;
        redisGetReply(input, (void**)&reply);
        if (reply == NULL) {
            std::cerr << "Failed to get reply from redis" << std::endl;
            freeReplyObject(reply);
            isCommandRunning.store(false);
            return "";
        }
        if (reply->type == REDIS_REPLY_ARRAY) {
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