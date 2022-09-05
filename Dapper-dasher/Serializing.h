//  To parse this JSON data, first install
//
//      Boost     http://www.boost.org
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Artist data = nlohmann::json::parse(jsonString);
//     Album data = nlohmann::json::parse(jsonString);
//     Track data = nlohmann::json::parse(jsonString);

#pragma once
#include "vector"
#include "nlohmann/json.hpp"

struct Player;
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

struct HighScoreData {
    vector<Player> players;
};

struct Player {
    string name;
    int highscore;
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

