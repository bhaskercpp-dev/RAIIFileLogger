
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <mutex>
#include <thread>
using namespace std;

enum class Loglevel
{
    DEBUG,  //0
    INFO,   //1
    WARNING,//2
    ERROR   //3
};

std::string logLevelToString(Loglevel level)
{
    switch (level)
    {
    case Loglevel::DEBUG: return "DEBUG";
    case Loglevel::INFO: return "INFO";
    case Loglevel::WARNING: return "WARNING";
    case Loglevel::ERROR: return "ERROR";
    default: return "UNKNOWN";
    }
}

bool Loglevel_Compare(Loglevel messageLevel, Loglevel minimumLevel)
{
    return static_cast<int>(messageLevel) >= static_cast<int>(minimumLevel);
}


class Logger
{
private:
    Loglevel minimumLevel = Loglevel::INFO;
    std::ofstream logFile;
    std::filesystem::path logDirectory;
    std::string currentDate;
    std::mutex logMutex;

    // Get the current date as YYYY-MM-DD
    std::string getCurrentDate()
    {
        const auto now = std::chrono::system_clock::now();

        const std::time_t currentTime =
            std::chrono::system_clock::to_time_t(now);

        std::tm localTime{};

        if (localtime_s(&localTime, &currentTime) != 0)
        {
            throw std::runtime_error("Failed to get current date");
        }

        std::ostringstream dateStream;

        dateStream << std::put_time(&localTime, "%Y-%m-%d");

        return dateStream.str();
    }

    // Open the log file for the current date
    void rotateLogIfNeeded()
    {
        const std::string today = getCurrentDate();
        //const std::string today = "2026-10-29";
        // Keep using the current file if the date has not changed
        if (today == currentDate && logFile.is_open())
        {
            return;
        }

        // Close the previous day's file
        if (logFile.is_open())
        {
            logFile.close();
        }

        currentDate = today;

        // Example: logs/app_2026-10-27.log
        const std::string filename = "app_" + currentDate + ".log";

        const auto fullPath = logDirectory / filename;

        logFile.open(fullPath, std::ios::app);

        if(!logFile.is_open())
        {
            throw std::runtime_error(
                "Failed to open log file: " + fullPath.string());
        }
    }
    void cleanupOldLogs()
    {
        namespace fs = std::filesystem;
        std::cout << "Cleanup directory: "
            << fs::absolute(logDirectory).string()
            << '\n';

        std::cout << "Directory exists: "
            << fs::exists(logDirectory)
            << '\n';
        // Get the current time
        auto now = fs::file_time_type::clock::now();

        // Maximum log age: 7 days
        auto maxAge = std::chrono::hours(24 * 7);

        // Visit each entry in the log directory
        for (const auto& entry : fs::directory_iterator(logDirectory))
        {
            // Check whether it is a .log file
            if (entry.is_regular_file() &&
                entry.path().extension() == ".log")
            {
                // Get the file's last-modified time
                auto fileTime = entry.last_write_time();

                // Delete files older than 7 days
                if (now - fileTime > maxAge)
                {
                    fs::remove(entry.path());

                    std::cout << "Deleted old log: "
                        << entry.path().filename().string()
                        << '\n';
                }
            }
        }
    }
public:
    explicit Logger(
        const std::filesystem::path& directory = "logs", Loglevel level = Loglevel::INFO)
        : logDirectory(directory)
    {
        // Create the directory if it does not exist
        std::filesystem::create_directories(logDirectory);

        cleanupOldLogs();

        // Open today's log file
        rotateLogIfNeeded();

        std::cout << "Logger initialized\n";
    }

    void log(Loglevel level,
        const std::string& message)
    {
        // Protect shared logger state
        std::lock_guard<std::mutex> lock(logMutex);

        if (!Loglevel_Compare(level,minimumLevel))
        {
            return;
        }

        // Check whether the date has changed
        rotateLogIfNeeded();

        const auto now = std::chrono::system_clock::now();

        const std::time_t currentTime =
            std::chrono::system_clock::to_time_t(now);

        std::tm localTime{};

        if (localtime_s(&localTime, &currentTime) != 0)
        {
            throw std::runtime_error("Failed to get current time");
        }

        std::ostringstream timestamp;

        timestamp << std::put_time(
            &localTime, "%Y-%m-%d %H:%M:%S");

       /* logFile << "[" << timestamp.str() << "] "
            << "[" << static_cast<int> (level) << "] "
            << message << '\n';*/
        logFile << "[" << timestamp.str() << "] "
            << "[" << logLevelToString(level) << "] "
            << message << '\n';

        if (!logFile)
        {
            throw std::runtime_error("Failed to write to log file");
        }
    }
    
   

    // Logger cannot be copied because it owns a stream and mutex
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // std::ofstream automatically closes when destroyed
    ~Logger() = default;
};


int main()
{
  /*  try
    {
        Logger logger("logs", Loglevel::WARNING);

        logger.log(Loglevel::INFO, "Server started");
        logger.log(Loglevel::DEBUG, "Client connected");
        logger.log(Loglevel::WARNING, "Request is taking too long");
        logger.log(Loglevel::ERROR, "Database connection failed");

        std::cout << "Log messages written successfully\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }*/

    try
    {
        Logger logger("E:/CPP_Practice/Modern_CPP11/Modern_CPP11/logs/test_logs",
            Loglevel::INFO);

        thread t1([&logger]()
        {
            for (int i = 0;i < 100;i++)
            {
                logger.log(Loglevel::INFO, "MESSAGE FROM THREAD #1" + std::to_string(i));
            }
        });

        thread t2([&logger]()
            {
                for (int i = 0; i < 100; i++)
                {
                    logger.log(Loglevel::ERROR, "MESSAGE FROM THREAD #2" + std::to_string(i));
                }
            });

        t1.join();
        t2.join();

        std::cout << "All the threads completed successfully\n";
    }

    catch(const exception& e)
    {
        cerr << "Error" << e.what() << '\n';
    }

    std::cout << "Program finished\n";

    return 0;
}

