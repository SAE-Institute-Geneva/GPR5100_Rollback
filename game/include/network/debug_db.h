#pragma once
#include "network/packet_type.h"

#include <string_view>
#include <condition_variable>
#include <thread>
#include <vector>

struct sqlite3;
namespace game
{

class DebugDatabase
{
public:
    void Open(std::string_view path);
    void StorePacket(const PlayerInputPacket* inputPacket);
    void Close();
private:
    void Loop();
    void CreateTables() const;
    sqlite3* db = nullptr;
    std::atomic<bool> isOver_ = false;
    std::thread t_;
    mutable std::mutex m_;
    std::condition_variable cv_;
    std::vector<std::string> commands_;
};

}
