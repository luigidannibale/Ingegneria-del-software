#ifndef REDISMANAGER_H
#define REDISMANAGER_H

#include <atomic>
#include <hiredis/hiredis.h>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <cstring>

class RedisManager
{
public:
    RedisManager();
    ~RedisManager();

    bool Connect();
    void Disconnect();
    bool CheckChannel(const char *channel);
    bool PublishToChannel(const char *channel, const char *message);
    bool SubscribeToChannel(const char *channel);
    bool UnsubscribeFromChannel(const char *channel = nullptr);

    std::string WaitResponse(bool timeout = true);
    void StopWaitingResponse();

    const char *SERVER_CHANNEL = "new_clients";

private:
    redisContext *c;

    redisContext *output;
    redisContext *input;

    std::atomic<bool> isCommandRunning;
    std::atomic<bool> isWaitingResponse;

    const int PORT = 6379;
    const char *HOST = "127.0.0.1";
};

#endif // REDISMANAGER_H
