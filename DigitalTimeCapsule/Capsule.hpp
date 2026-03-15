#ifndef CAPSULE_HPP
#define CAPSULE_HPP

#include <string>
#include <ctime>
#include <cmath>

struct Capsule
{
    std::string id;
    std::string message;
    std::string category;
    std::string author;
    std::string password;
    std::time_t unlockDate;
    std::time_t creationDate;
    bool isOpened;

    int priority;
    std::string mood;
    bool isGoal;
    bool isAchieved;

    Capsule() : unlockDate(0), creationDate(0), isOpened(false), priority(2), isGoal(false), isAchieved(false) {}

    Capsule(std::string id, std::string msg, std::string cat, std::string aut, std::time_t unlock, std::string pwd = "",
            int prio = 2, std::string m = "Neutral", bool goal = false)
        : id(id), message(msg), category(cat), author(aut), unlockDate(unlock), password(pwd),
          isOpened(false), priority(prio), mood(m), isGoal(goal), isAchieved(false)
    {
        creationDate = std::time(nullptr);
    }

    bool isReady() const
    {
        return std::time(nullptr) >= unlockDate;
    }

    bool hasPassword() const
    {
        return !password.empty();
    }

    long long getSecondsRemaining() const
    {
        std::time_t now = std::time(nullptr);
        if (now >= unlockDate)
            return 0;
        return static_cast<long long>(std::difftime(unlockDate, now));
    }

    std::string getStatusString() const
    {
        if (isOpened)
            return "OPENED";
        long long diff = getSecondsRemaining();
        if (diff <= 0)
            return "READY TO REVEAL";

        long long days = diff / (24 * 3600);
        long long hours = (diff % (24 * 3600)) / 3600;
        long long mins = (diff % 3600) / 60;

        if (days > 0)
            return "Opens in " + std::to_string(days) + " days (" + std::to_string(hours) + "h remaining)";
        if (hours > 0)
            return "Opens in " + std::to_string(hours) + "h " + std::to_string(mins) + "m";
        return "CRITICAL: Opens in " + std::to_string(mins) + "m " + std::to_string(diff % 60) + "s!";
    }
};

#endif
