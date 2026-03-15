#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <thread>
#include <chrono>
#include <algorithm>
#include <sstream>
#include "Capsule.hpp"
#include "Storage.hpp"

#define RESET std::string("\033[0m")
#define NEON_CYAN std::string("\033[38;2;0;255;255m")
#define NEON_MAGENTA std::string("\033[38;2;255;0;255m")
#define NEON_LIME std::string("\033[38;2;50;255;50m")
#define NEON_YELLOW std::string("\033[38;2;255;255;0m")
#define NEON_ORANGE std::string("\033[38;2;255;165;0m")
#define NEON_RED std::string("\033[38;2;255;50;50m")
#define BOLD_WHITE std::string("\033[1;37m")
#define ADMIN_GOLD std::string("\033[38;2;255;215;0m")

const std::string MASTER_PASSWORD = "ADMIN";

void typewrite(const std::string &text, int delay_ms = 25)
{
    for (char c : text)
    {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
}

void printHeader(bool isAdmin = false)
{
    std::string title = isAdmin ? ":: [ ADMIN CONTROL PANEL ] ::" : ":: [ PRIVATE TIME VAULT ] ::";
    std::string line = "====================================================";
    std::cout << "\n";
    for (int i = 0; i < line.length(); ++i)
    {
        int r, g, b;
        if (isAdmin)
        {
            r = 255;
            g = 215 - (i * 2) % 100;
            b = 0;
        }
        else
        {
            r = 200 - (i * 2) % 100;
            g = 0;
            b = 255;
        }
        std::cout << "\033[38;2;" << r << ";" << g << ";" << b << "m" << line[i] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
    std::cout << RESET << "\n";
    typewrite(BOLD_WHITE + "       " + title + RESET + "\n", 30);
    for (int i = 0; i < line.length(); ++i)
    {
        int r, g, b;
        if (isAdmin)
        {
            r = 255;
            g = 215 - (i * 2) % 100;
            b = 0;
        }
        else
        {
            r = 200 - (i * 2) % 100;
            g = 0;
            b = 255;
        }
        std::cout << "\033[38;2;" << r << ";" << g << ";" << b << "m" << line[i] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
    std::cout << RESET << "\n"
              << std::endl;
}

std::time_t parseDateTime(const std::string &dateStr, const std::string &timeStr)
{
    struct tm tm = {0};
    std::string fullStr = dateStr + " " + timeStr;
    int hour, min;
    char ampm[3];
    if (sscanf(fullStr.c_str(), "%d-%d-%d %d:%d %s", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &hour, &min, ampm) != 6)
        return -1;

    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    tm.tm_min = min;

    std::string s_ampm = ampm;
    if (s_ampm == "PM" && hour < 12)
        hour += 12;
    if (s_ampm == "AM" && hour == 12)
        hour = 0;
    tm.tm_hour = hour;
    tm.tm_isdst = -1;

    return std::mktime(&tm);
}

std::string timeToString(std::time_t time)
{
    char buf[80];
    struct tm ts = *std::localtime(&time);
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %I:%M %p", &ts);
    return std::string(buf);
}

void adminPanel(std::vector<Capsule> &capsules, Storage &storage)
{
    std::string pass;
    typewrite(NEON_MAGENTA + "\n[SECURE] Enter Master Password: " + RESET);
    std::cin >> pass;
    if (pass != MASTER_PASSWORD)
    {
        typewrite(NEON_RED + "!! ACCESS DENIED !!\n" + RESET);
        return;
    }

    while (true)
    {
        printHeader(true);
        typewrite(ADMIN_GOLD + " [1] Edit Details\n", 15);
        typewrite(ADMIN_GOLD + " [2] Delete Specific\n", 15);
        typewrite(ADMIN_GOLD + " [3] Detailed Registry\n", 15);
        typewrite(NEON_RED + " [4] WIPE DATABASE\n", 15);
        typewrite(BOLD_WHITE + " [5] Return to Menu\n", 15);
        std::cout << RESET << "\n";
        typewrite(">>> Admin Selection: ", 20);

        int choice;
        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 1)
        {
            std::string id;
            typewrite("\nTarget UID: ", 20);
            std::cin.ignore();
            std::getline(std::cin, id);
            auto it = std::find_if(capsules.begin(), capsules.end(), [&](const Capsule &c)
                                   { return c.id == id; });
            if (it != capsules.end())
            {
                typewrite(NEON_MAGENTA + "Modifying Record [" + it->id + "]\n" + RESET, 15);
                typewrite("1.Msg 2.Time 3.Pwd 4.ID 5.Cat 6.Aut\nSelection: ", 15);
                int f;
                std::cin >> f;
                std::cin.ignore();
                if (f == 1)
                {
                    typewrite("New Msg: ", 10);
                    std::getline(std::cin, it->message);
                }
                else if (f == 2)
                {
                    std::string d, t;
                    typewrite("Date (YYYY-MM-DD): ", 10);
                    std::getline(std::cin, d);
                    typewrite("Time (HH:MM AM/PM): ", 10);
                    std::getline(std::cin, t);
                    it->unlockDate = parseDateTime(d, t);
                }
                else if (f == 3)
                {
                    typewrite("New Pwd: ", 10);
                    std::getline(std::cin, it->password);
                }
                else if (f == 4)
                {
                    typewrite("New ID: ", 10);
                    std::getline(std::cin, it->id);
                }
                else if (f == 5)
                {
                    typewrite("New Cat: ", 10);
                    std::getline(std::cin, it->category);
                }
                else if (f == 6)
                {
                    typewrite("New Aut: ", 10);
                    std::getline(std::cin, it->author);
                }

                storage.saveCapsules(capsules);
                typewrite(NEON_LIME + "\n[SYSTEM] Registry updated successfully.\n" + RESET, 20);
            }
            else
                typewrite(NEON_RED + "\n[FAILURE] UID not found.\n" + RESET, 20);
        }
        else if (choice == 3)
        {
            typewrite(NEON_YELLOW + "\n--- MASTER STORAGE READOUT ---\n" + RESET, 25);
            if (capsules.empty())
            {
                typewrite("[EMPTY] Zero records found.\n", 20);
            }
            else
            {
                for (const auto &c : capsules)
                {
                    std::cout << BOLD_WHITE << "ID: " << NEON_CYAN << c.id << RESET << std::endl;
                    std::cout << "  Author:   " << c.author << std::endl;
                    std::cout << "  Category: " << c.category << std::endl;
                    std::cout << "  Hidden:   " << (c.isReady() ? NEON_LIME + "UNLOCKED" : NEON_RED + c.getStatusString() + " REMAINING") << RESET << std::endl;
                    std::cout << "  Timeline: Born [" << timeToString(c.creationDate) << "] | Wake [" << timeToString(c.unlockDate) << "]" << std::endl;
                    std::cout << "  Password: " << NEON_YELLOW << c.password << RESET << std::endl;
                    std::cout << "  Payload:  " << NEON_MAGENTA << c.message << RESET << std::endl;
                    std::cout << "------------------------------------------" << std::endl;
                }
            }
            std::cout << "\n";
            typewrite("Backtrack to menu (Enter)...", 10);
            std::cin.ignore();
            std::cin.get();
        }
        else if (choice == 4)
        {
            std::string confirm;
            std::cout << "Type 'CONFIRM' to wipe: ";
            std::cin >> confirm;
            if (confirm == "CONFIRM")
            {
                capsules.clear();
                storage.saveCapsules(capsules);
                std::cout << "Wiped.\n";
            }
        }
        else if (choice == 5)
            break;
    }
}

int main()
{
    Storage storage;
    std::vector<Capsule> capsules = storage.loadCapsules();
    int choice;

    std::vector<Capsule *> opened;
    for (auto &c : capsules)
        if (c.isOpened)
            opened.push_back(&c);
    if (!opened.empty())
    {
        srand(time(0));
        Capsule *memory = opened[rand() % opened.size()];
        printHeader();
        typewrite(std::string(NEON_MAGENTA) + "✨ NOSTALGIC MEMORY ✨\n", 20);
        typewrite("From " + memory->author + " [" + timeToString(memory->creationDate) + "]:\n", 15);
        typewrite(BOLD_WHITE + "\"" + memory->message + "\"\n" + RESET, 10);
        typewrite("\nPress Enter to enter the Vault...", 10);
        std::cin.get();
    }

    while (true)
    {
        printHeader();
        typewrite(NEON_CYAN + " [1] New Entry       " + NEON_LIME + " [2] My History\n", 10);
        typewrite(NEON_YELLOW + " [3] Unlock Message  " + ADMIN_GOLD + " [4] Admin Access\n", 10);
        typewrite(NEON_MAGENTA + " [5] Vault Stats     " + NEON_ORANGE + " [6] Search Vault\n", 10);
        typewrite(NEON_ORANGE + " [7] Export Data     " + BOLD_WHITE + " [8] Shutdown\n", 10);
        std::cout << RESET << "\n";
        typewrite(">>> Select Option: ", 20);

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 1)
        {
            std::string id, msg, cat, aut, pwd, dateStr, timeStr, mood;
            int prio;
            char isGoalChar;

            typewrite(NEON_CYAN + "\n[ID] Choose Unique ID: " + RESET, 20);
            std::cin.ignore();
            std::getline(std::cin, id);
            auto exists = std::find_if(capsules.begin(), capsules.end(), [&](const Capsule &c)
                                       { return c.id == id; });
            if (exists != capsules.end())
            {
                typewrite(NEON_RED + "!! ID ALREADY TAKEN !!\n" + RESET, 20);
                continue;
            }

            typewrite(NEON_CYAN + "[PWD] Privacy Password: " + RESET, 20);
            std::getline(std::cin, pwd);
            if (pwd.empty())
            {
                typewrite(NEON_RED + "!! PASSWORD REQUIRED !!\n" + RESET, 20);
                continue;
            }

            typewrite(NEON_CYAN + "[WHO] Creator Name: " + RESET, 20);
            std::getline(std::cin, aut);
            typewrite(NEON_CYAN + "[WHY] Purpose/Reason: " + RESET, 20);
            std::getline(std::cin, cat);
            typewrite(NEON_CYAN + "[WHEN] Date (YYYY-MM-DD): " + RESET, 20);
            std::getline(std::cin, dateStr);
            typewrite(NEON_CYAN + "[TIME] Time (HH:MM AM/PM): " + RESET, 20);
            std::getline(std::cin, timeStr);

            typewrite(NEON_CYAN + "[VIBE] Mood (Happy/Reflective/Sad/Excited): " + RESET, 20);
            std::getline(std::cin, mood);
            typewrite(NEON_CYAN + "[LEVEL] Priority (1:Low, 2:Med, 3:High): " + RESET, 20);
            std::cin >> prio;
            if (prio < 1)
                prio = 1;
            if (prio > 3)
                prio = 3;

            typewrite(NEON_CYAN + "[GOAL] Is this a life goal? (y/n): " + RESET, 20);
            std::cin >> isGoalChar;
            std::cin.ignore();

            typewrite(NEON_CYAN + "[DATA] Message to Bury: " + RESET, 20);
            std::getline(std::cin, msg);

            std::time_t unlock = parseDateTime(dateStr, timeStr);
            if (unlock == -1)
                typewrite(NEON_RED + "!! INVALID TIME FORMAT !!\n" + RESET, 20);
            else
            {
                capsules.emplace_back(id, msg, cat, aut, unlock, pwd, prio, mood, (isGoalChar == 'y' || isGoalChar == 'Y'));
                storage.saveCapsules(capsules);
                typewrite(NEON_LIME + "\n[OK] Entry secured with Advanced Metadata. Message buried.\n" + RESET, 30);
            }
        }
        else if (choice == 2)
        {
            std::string id, pwd;
            typewrite("\n[AUTH] Enter ID: ", 20);
            std::cin.ignore();
            std::getline(std::cin, id);
            typewrite("[AUTH] Enter Password: ", 20);
            std::getline(std::cin, pwd);

            auto it = std::find_if(capsules.begin(), capsules.end(), [&](const Capsule &c)
                                   { return c.id == id && c.password == pwd; });
            if (it != capsules.end())
            {
                printHeader();
                std::string prioColor = (it->priority == 3) ? NEON_RED : (it->priority == 2 ? NEON_YELLOW : NEON_CYAN);
                std::string prioLabel = (it->priority == 3) ? "HIGH" : (it->priority == 2 ? "MEDIUM" : "LOW");

                typewrite(NEON_MAGENTA + " OWNER:   " + it->author + "\n" + RESET, 20);
                typewrite(NEON_CYAN + " MOOD:    " + it->mood + (it->isGoal ? " [GOAL]" : "") + "\n" + RESET, 20);
                typewrite(prioColor + " PRIO:    " + prioLabel + "\n" + RESET, 20);
                typewrite(NEON_CYAN + " PURPOSE: " + it->category + "\n" + RESET, 20);
                typewrite(NEON_YELLOW + " OPENS:   " + timeToString(it->unlockDate) + "\n" + RESET, 20);
                typewrite(" STATUS:  " + it->getStatusString() + "\n", 20);
            }
            else
                typewrite(NEON_RED + "!! ACCESS DENIED !!\n" + RESET, 20);
            std::cout << "\n";
            typewrite("Press Enter to return...", 10);
            std::cin.get();
        }
        else if (choice == 3)
        {
            std::string id, pwd;
            typewrite("\n[UNLOCK] Enter ID: ", 20);
            std::cin.ignore();
            std::getline(std::cin, id);
            typewrite("[UNLOCK] Enter Password: ", 20);
            std::getline(std::cin, pwd);

            auto it = std::find_if(capsules.begin(), capsules.end(), [&](const Capsule &c)
                                   { return c.id == id && c.password == pwd; });
            if (it != capsules.end())
            {
                if (it->isReady())
                {
                    typewrite(NEON_LIME + "\n--- IDENTITY VERIFIED. REVEALING RECORD ---\n" + RESET, 40);
                    typewrite(BOLD_WHITE + "\"" + it->message + "\"\n" + RESET, 35);
                    it->isOpened = true;

                    if (it->isGoal && !it->isAchieved)
                    {
                        typewrite(NEON_YELLOW + "\n[GOAL TRACKER] Did you achieve this goal? (y/n): " + RESET, 20);
                        char ach;
                        std::cin >> ach;
                        if (ach == 'y' || ach == 'Y')
                        {
                            it->isAchieved = true;
                            typewrite(NEON_LIME + "Congratulations! Goal marked as ACHIEVED.\n" + RESET, 30);
                        }
                        else
                            typewrite(NEON_ORANGE + "Keep pushing! You'll get there next time.\n" + RESET, 30);
                    }
                    storage.saveCapsules(capsules);
                }
                else
                {
                    typewrite(NEON_RED + "!! TEMPORAL LOCK ACTIVE !!\n" + RESET, 20);
                    typewrite(it->getStatusString() + "\n", 20);
                }
            }
            else
                typewrite(NEON_RED + "!! AUTHENTICATION FAILED !!\n" + RESET, 20);
            std::cout << "\n";
            typewrite("Press Enter to return...", 10);
            std::cin.get();
        }
        else if (choice == 4)
            adminPanel(capsules, storage);
        else if (choice == 5)
        { // Statistics
            int total = capsules.size(), locked = 0, openedCount = 0, goals = 0, achieved = 0;
            for (const auto &c : capsules)
            {
                if (c.isOpened)
                    openedCount++;
                else
                    locked++;
                if (c.isGoal)
                {
                    goals++;
                    if (c.isAchieved)
                        achieved++;
                }
            }
            typewrite(NEON_MAGENTA + "\n--- VAULT STATISTICS & AUDIT ---\n" + RESET, 25);
            typewrite("Total Capsules:  " + std::to_string(total) + "\n", 15);
            typewrite(NEON_RED + "Locked Status:   " + std::to_string(locked) + "\n" + RESET, 15);
            typewrite(NEON_LIME + "Opened Memories: " + std::to_string(openedCount) + "\n" + RESET, 15);
            typewrite(NEON_YELLOW + "Life Goals:      " + std::to_string(goals) + " (" + std::to_string(achieved) + " Met)\n" + RESET, 15);
            std::cout << "\n";
            typewrite("Press Enter...", 10);
            std::cin.ignore();
            std::cin.get();
        }
        else if (choice == 6)
        {
            std::string q;
            typewrite(NEON_ORANGE + "\nEnter Keyword/Mood/Author: " + RESET, 20);
            std::cin.ignore();
            std::getline(std::cin, q);
            std::transform(q.begin(), q.end(), q.begin(), ::tolower);

            typewrite("\n--- SEARCH RESULTS ---\n", 20);
            bool found = false;
            for (const auto &c : capsules)
            {
                std::string id = c.id, aut = c.author, cat = c.category, mood = c.mood;
                std::transform(id.begin(), id.end(), id.begin(), ::tolower);
                std::transform(aut.begin(), aut.end(), aut.begin(), ::tolower);
                std::transform(cat.begin(), cat.end(), cat.begin(), ::tolower);
                std::transform(mood.begin(), mood.end(), mood.begin(), ::tolower);

                if (id.find(q) != std::string::npos || aut.find(q) != std::string::npos ||
                    cat.find(q) != std::string::npos || mood.find(q) != std::string::npos)
                {
                    std::cout << BOLD_WHITE << "[" << c.id << "]" << RESET << " By " << c.author
                              << " | Mood: " << c.mood << " | " << c.getStatusString() << std::endl;
                    found = true;
                }
            }
            if (!found)
                typewrite("No matches found.\n", 20);
            std::cout << "\n";
            typewrite("Press Enter...", 10);
            std::cin.get();
        }
        else if (choice == 7)
        {
            std::ofstream out("vault_export.txt");
            out << "--- DIGITAL TIME VAULT EXPORT ---\n\n";
            for (const auto &c : capsules)
            {
                out << "ID: " << c.id << "\nAuthor: " << c.author << "\nCategory: " << c.category
                    << "\nMood: " << c.mood << "\nPriority: " << c.priority
                    << "\nCreated: " << timeToString(c.creationDate)
                    << "\nUnlocks: " << timeToString(c.unlockDate)
                    << "\nStatus: " << (c.isOpened ? "OPENED" : "LOCKED") << "\n";
                if (c.isOpened)
                    out << "Message: " << c.message << "\n";
                out << "----------------------------------\n";
            }
            typewrite(NEON_LIME + "\n[SUCCESS] Vault exported to 'vault_export.txt'.\n" + RESET, 30);
            std::cout << "\n";
            typewrite("Press Enter...", 10);
            std::cin.ignore();
            std::cin.get();
        }
        else if (choice == 8)
        {
            typewrite(BOLD_WHITE + "\nShutting down Digital Vault... System Offline.\n" + RESET, 30);
            break;
        }
    }
    return 0;
}
