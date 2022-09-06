#pragma once
#include "vector"
#include "nlohmann/json.hpp"

using nlohmann::json;
using namespace std;

inline json get_untyped(const json & j, const char * property) {
    if (j.find(property) != j.end()) {
        return j.at(property).get<json>();
    }
    return json();
}

inline json get_untyped(const json & j, std::string property) {
    return get_untyped(j, property.data());
}
struct Player {
    string name;
    int highscore;
};

struct HighScoreData {
    vector<Player> players;
};

void from_json(const json & j, HighScoreData & x);
void to_json(json & j, const HighScoreData & x);

void from_json(const json & j, Player & x);
void to_json(json & j, const Player & x);

inline void from_json(const json & j, HighScoreData& x) {
    x.players = j.at("players").get<vector<Player>>();
}

inline void to_json(json & j, const HighScoreData & x) {
    j = json::object();
    j["players"] = x.players;
}

inline void from_json(const json & j, Player& x) {
    x.name = j.at("name").get<std::string>();
    x.highscore = j.at("highscore").get<int>();
}

inline void to_json(json & j, const Player & x) {
    j = json::object();
    j["name"] = x.name;
    j["highscore"] = x.highscore;
}

