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
    bool CheckChannel(const char* channel);
    bool PublishToChannel(const char* channel, const char* message);
    bool SubscribeToChannel(const char* channel);
    bool UnsubscribeFromChannel(const char* channel = nullptr);

    std::string WaitResponse();
    void StopWaitingResponse();
private:
    redisContext *c;

    std::atomic<bool> isCommandRunning;
    std::atomic<bool> isWaitingResponse;
};

#endif //REDISMANAGER_H
