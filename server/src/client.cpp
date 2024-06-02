#include <iostream>
#include <hiredis/hiredis.h>
#include <unistd.h>

void subscribe(redisContext *c, std::string client_id) {
    redisReply *reply = (redisReply*)redisCommand(c, "SUBSCRIBE %s", client_id.c_str());
    if (reply == NULL || reply->type != REDIS_REPLY_ARRAY) {
        std::cerr << "Failed to subscribe to " << client_id.c_str() << " channel" << std::endl;
        freeReplyObject(reply);
        return;
    }
    freeReplyObject(reply);
}

// int main() {
//     // Connect to Redis server
//     redisContext *c = redisConnect("127.0.0.1", 6379);
//     if (c == NULL || c->err) {
//         if (c) {
//             std::cerr << "Connection error: " << c->errstr << std::endl;
//         } else {
//             std::cerr << "Connection error: can't allocate redis context" << std::endl;
//         }
//         return 1;
//     }

//     // Generate a unique client ID (replace this with your own logic)
//     std::string client_id = "client_" + std::to_string(getpid());

//     // Publish client ID to 'new_clients' channel
//     redisReply *reply = (redisReply*)redisCommand(c, "PUBLISH new_clients %s", client_id.c_str());
//     if (reply == NULL || reply->type != REDIS_REPLY_INTEGER) {
//         std::cerr << "Failed to publish client ID" << std::endl;
//         freeReplyObject(reply);
//         return 1;
//     }
//     std::cout << "Client ID published: " << client_id << std::endl;

//     subscribe(c, client_id);

//     while (true) {
//         redisReply *reply = nullptr;
//         if (redisGetReply(c, (void**)&reply) == REDIS_OK && reply != nullptr) {
//             if (reply != NULL) {
//                 std::cout << "Received type: " << reply->type << std::endl;
//                 std::cout << "Received message: " << reply->element[2]->str << std::endl;
//             }
//             else {
//                 std::cerr << "Failed to receive message" << std::endl;
//             }
//             freeReplyObject(reply);
//             break;
//         }
//     }

//     // Disconnect from Redis server
//     redisFree(c);

//     return 0;
// }
