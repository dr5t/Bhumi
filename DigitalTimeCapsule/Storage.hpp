#ifndef STORAGE_HPP
#define STORAGE_HPP

#include "Capsule.hpp"
#include <vector>
#include <fstream>
#include <sstream>

class Storage
{
private:
    const std::string filename = "capsules.dat";

public:
    void saveCapsules(const std::vector<Capsule> &capsules)
    {
        std::ofstream file(filename);
        for (const auto &c : capsules)
        {
            file << c.id << "\t"
                 << c.unlockDate << "\t"
                 << c.creationDate << "\t"
                 << c.category << "\t"
                 << c.author << "\t"
                 << (c.password.empty() ? "NONE" : c.password) << "\t"
                 << (c.isOpened ? "1" : "0") << "\t"
                 << c.priority << "\t"
                 << c.mood << "\t"
                 << (c.isGoal ? "1" : "0") << "\t"
                 << (c.isAchieved ? "1" : "0") << "\t"
                 << c.message << "\n";
        }
    }

    std::vector<Capsule> loadCapsules()
    {
        std::vector<Capsule> capsules;
        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty())
                continue;
            std::stringstream ss(line);
            std::vector<std::string> tokens;
            std::string token;
            while (std::getline(ss, token, '\t'))
            {
                tokens.push_back(token);
            }

            if (tokens.size() < 8)
                continue;
            Capsule c;
            try
            {
                c.id = tokens[0];
                c.unlockDate = std::stoll(tokens[1]);
                c.creationDate = std::stoll(tokens[2]);
                c.category = tokens[3];
                c.author = tokens[4];
                c.password = (tokens[5] == "NONE" ? "" : tokens[5]);
                c.isOpened = (tokens[6] == "1");

                if (tokens.size() >= 12)
                {
                    c.priority = std::stoi(tokens[7]);
                    c.mood = tokens[8];
                    c.isGoal = (tokens[9] == "1");
                    c.isAchieved = (tokens[10] == "1");
                    c.message = tokens[11];
                }
                else
                {
                    c.priority = 2;
                    c.mood = "Neutral";
                    c.isGoal = false;
                    c.isAchieved = false;
                    c.message = tokens[7];
                }
                capsules.push_back(c);
            }
            catch (...)
            {
                continue;
            }
        }
        return capsules;
    }
};

#endif
