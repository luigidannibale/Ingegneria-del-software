#ifndef REDISMANAGER_H
#define REDISMANAGER_H

#include <atomic>
#include <hiredis/hiredis.h>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <string>

class RedisManager {
public:
    RedisManager();
    ~RedisManager();

    bool Connect();
    void Disconnect();
    bool SubscribeToChannel(const char* channel);
    bool UnsubscribeFromChannel(const char* channel = nullptr);
    bool PublishToChannel(const char* channel, const char* message);
    std::string WaitResponse();

private:
    redisContext *input;
    redisContext *output;

    const char* INPUT_HOST = "127.0.0.1";
    const int INPUT_PORT = 6379;
    const int OUTPUT_PORT = 6380;

    std::atomic<bool> isCommandRunning;
    std::atomic<bool> isWaitingResponse;
};

#endif //REDISMANAGER_H
