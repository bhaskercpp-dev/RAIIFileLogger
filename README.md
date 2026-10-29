# Modern C++ File Logger

A file-based logging system built using modern C++ features. This project demonstrates resource management, file handling, log-level filtering, thread safety, and automatic log-file maintenance.

## Features

* **RAII (Resource Acquisition Is Initialization):** Manages file resources through object lifetime.
* **File Logging:** Writes application log messages to files.
* **Log Levels:** Supports filtering messages based on severity levels.
* **Daily Log Rotation:** Creates log files based on the current date.
* **Thread Safety:** Uses `std::mutex` and `std::lock_guard` to protect shared logging operations.
* **Automatic Log Cleanup:** Removes log files older than a configured retention period.
* **Filesystem Operations:** Uses `std::filesystem` for directory creation, file inspection, and deletion.
* **Modern C++:** Uses standard library features such as `std::chrono`, `std::ofstream`, and smart resource-management techniques.

## Technologies

* C++
* C++17 or later
* Standard Template Library (STL)
* Visual Studio
* Git and GitHub

## Project Structure

```text
Modern_CPP11/
├── Modern_CPP11.cpp
├── Modern_CPP11.vcxproj
├── Modern_CPP11.vcxproj.filters
├── README.md
└── .gitignore
```

Log files and generated build artifacts are excluded from version control.

## Building the Project

### Using Visual Studio

1. Open `Modern_CPP11.vcxproj` in Visual Studio.
2. Select the desired configuration, such as `Debug` or `Release`.
3. Select the appropriate platform, such as `x64`.
4. Build the project using **Build → Build Solution**.
5. Run the application.

### Using a Developer Command Prompt

From the directory containing the project file, run:

```powershell
msbuild Modern_CPP11.vcxproj /p:Configuration=Debug /p:Platform=x64
```

This command requires MSBuild and the appropriate Visual Studio C++ build tools.

## Concepts Demonstrated

### 1. RAII

RAII ties resource management to object lifetime. The logger manages its file resource through a C++ object, helping ensure that resources are released when the object is destroyed.

### 2. Log-Level Filtering

Log levels allow applications to control which messages are written, helping separate important errors from routine diagnostic information.

### 3. Daily Log Rotation

Date-based log filenames separate log output by day, making application logs easier to organize and inspect.

### 4. Thread Safety

A mutex protects shared logger operations so that concurrent logging calls do not modify shared state simultaneously.

### 5. Automatic Log Cleanup

The logger checks log-file modification times and removes eligible old log files according to the configured retention period.

### 6. Filesystem and Time APIs

The project uses `std::filesystem` for file and directory operations and `std::chrono` for time-related functionality.

## Potential Improvements

* Add configurable log formats and output destinations.
* Support asynchronous logging for high-throughput applications.
* Add unit tests for rotation, filtering, and cleanup.
* Improve error handling for file-system failures.
* Add configurable retention policies.
* Integrate structured logging and performance benchmarks.

## Learning Objectives

This project provides practical experience with modern C++ resource management, file I/O, synchronization, filesystem operations, and maintainable application design.

It can also serve as a foundation for discussing RAII, exception safety, concurrency, and standard-library APIs during C++ technical interviews.
