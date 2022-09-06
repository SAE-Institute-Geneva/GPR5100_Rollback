#include "network/debug_db.h"
#include "utils/log.h"
#include "utils/conversion.h"


#include <sqlite3.h>
#include <fmt/format.h>

#include <filesystem>

namespace fs = std::filesystem;


namespace game
{

static int callback([[maybe_unused]] void* NotUsed, int argc, char** argv, char** azColName)
{
    for (int i = 0; i < argc; i++) {
        core::LogDebug(fmt::format("{} = {}", azColName[i], argv[i] ? argv[i] : "NULL"));
    }
    return 0;
}

void DebugDatabase::Open(std::string_view path)
{
    if(fs::exists(path))
    {
        fs::remove(path);
    }
    const auto rc = sqlite3_open(path.data(), &db);
    if (rc != SQLITE_OK) 
    {
        core::LogError(fmt::format("Can't open database: {}\n", sqlite3_errmsg(db)));
        sqlite3_close(db);
        db = nullptr;
    }
    CreateTables();
}

void DebugDatabase::StorePacket(const PlayerInputPacket* inputPacket) const
{
    const PlayerNumber playerNumber = inputPacket->playerNumber;
    const auto frame = core::ConvertFromBinary<Frame>(inputPacket->currentFrame);
    const PlayerInput input = inputPacket->inputs[0];

    const auto query = fmt::format("INSERT INTO inputs (player_number, frame, up, down, left, right, shoot) VALUES({}, {}, {}, {}, {}, {},  {});",
                                   playerNumber,
                                   frame,
                                   (input & PlayerInputEnum::UP) == PlayerInputEnum::UP,
                                   (input & PlayerInputEnum::DOWN) == PlayerInputEnum::DOWN,
                                   (input & PlayerInputEnum::LEFT) == PlayerInputEnum::LEFT,
                                   (input & PlayerInputEnum::RIGHT) == PlayerInputEnum::RIGHT,
                                   (input & PlayerInputEnum::SHOOT) == PlayerInputEnum::SHOOT);


    /* Execute SQL statement */

    char* zErrMsg = nullptr;
    const auto rc = sqlite3_exec(db, query.c_str(), callback, nullptr, &zErrMsg);

    if (rc != SQLITE_OK) {
        core::LogError(fmt::format("SQL error with storing input: {}", zErrMsg));
        sqlite3_free(zErrMsg);
    }
}

void DebugDatabase::Close()
{
    if(db != nullptr)
    {
        sqlite3_close(db);
        db = nullptr;
    }
}

void DebugDatabase::CreateTables() const
{
        //playerNumber, frame, up, down, left, right, shoot
    
    const auto createInputTable = "CREATE TABLE inputs ("\
        "input_id INTEGER PRIMARY KEY,"\
        "player_number INTEGER NOT NULL,"\
        "frame INTEGER NOT NULL,"\
        "up INTEGER NOT NULL,"\
        "down INTEGER NOT NULL,"\
        "left INTEGER NOT NULL,"\
        "right INTEGER NOT NULL,"\
        "shoot INTEGER NOT NULL);";
    
    /* Execute SQL statement */

    char* zErrMsg = nullptr;
    const auto rc = sqlite3_exec(db, createInputTable, callback, nullptr, &zErrMsg);

    if (rc != SQLITE_OK) {
        core::LogError(fmt::format("SQL error while creating table: {}", zErrMsg));
        sqlite3_free(zErrMsg);
    }
}
}
