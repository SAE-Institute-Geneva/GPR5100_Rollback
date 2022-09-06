#pragma once
#include <string_view>
#include "network/packet_type.h"

struct sqlite3;
namespace game
{

class DebugDatabase
{
public:
    void Open(std::string_view path);
    void StorePacket(const PlayerInputPacket* inputPacket) const;
    void Close();
private:
    void CreateTables() const;
    sqlite3* db = nullptr;
};

}
