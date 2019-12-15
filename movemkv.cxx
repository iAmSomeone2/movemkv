#include <string>
#include <iostream>
#include <vector>
#include <thread>

extern "C" {
#include <unistd.h>
}

#include "movemkv.hxx"

/**
 * Uses the name of the MKV file to determine file tree.
 * @param filePath original path of MKV file.
 * @return path to append to the output directory.
*/
static fs::path determineLoc(fs::path& filePath) {
    auto fileName = filePath.filename().string();

    std::string year = fileName.substr(0, 4);
    Month month = Month(std::stoul(fileName.substr(4, 2)));
    std::string day = fileName.substr(6, 2);

    std::string pathString = year + PATH_SEP + MONTH_STR[month] + PATH_SEP + day + PATH_SEP + fileName;
    return fs::path(pathString);
}

static void relocate(void *pathPair) {
    auto newPathPair = static_cast<InOutPair*>(pathPair);
    auto parentDir = newPathPair->outPath.parent_path();
    try {
        fs::create_directories(parentDir);
    } catch (const fs::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Make sure that you have write permissions for this directory." << std::endl;
        exit(2);
    }
    
    try {
        fs::copy_file(newPathPair->inPath.string(), newPathPair->outPath.string());
    } catch (const fs::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
    }

    newPathPair->success = true;
}

int main(int argc, char **argv) {
    fs::path inPath("./");
    fs::path outPath("./");

    int opt;
    bool hasInput = false;
    while((opt = getopt(argc, argv, OPTS)) != -1) {
        switch (opt) {
            case 'i':
                inPath = fs::path(optarg);
                hasInput = true;
                break;
            case 'o':
                outPath = fs::path(optarg);
                break;
            case ':':
                std::cerr << "option requires argument" << std::endl;
                return 1;
            case '?':
                std::cerr << "unrecognized argument" << std::endl;
                return 1;
            default:
                std::cerr << "This should never happen" << std::endl;
        }
    }

    if (!hasInput) {
        std::cerr << "Input directory or file is required" << std::endl;
        return 1;
    }

    std::vector<InOutPair*> inOutPairs;
    // Find all MKV files and determine output paths.
    std::cout << "Searching for input files..." << std::endl;
    for (auto& dir : fs::recursive_directory_iterator(inPath)) {
        auto tmpPath = outPath;
        if (dir.is_regular_file()) {
            auto filePath = dir.path();
            if (filePath.extension() == ".mkv") {
                tmpPath.append(determineLoc(filePath).string());
                inOutPairs.push_back(new InOutPair({filePath, tmpPath, false}));
            }
        }
    }

    // Move files using threads
    int relocateCount = 0;
    int numFiles = inOutPairs.size();
    std::vector<std::thread> threadPool;
    int usedThreads = 0;
    std::cout << "Trying to relocating " << numFiles <<" files..." << std::endl;
    for (auto& pathPair : inOutPairs) {
        // I'm trying to limit the number of threads so that the drives don't get overwhelmed.
        int remainingFiles = numFiles - relocateCount;
        if (relocateCount <  numFiles) {
            threadPool.emplace_back(relocate, (void *) pathPair);
            usedThreads++;
        }
        if (usedThreads == THREAD_MAX - 1 || usedThreads == remainingFiles) {
            while (!threadPool.empty()) {
                threadPool.back().join();
                threadPool.pop_back();
                if (pathPair->success) {
                    relocateCount++;
                }
            }
            usedThreads = 0;
        } 
    }

    for (auto& pair : inOutPairs) {
        delete pair;
    }

    std::cout << "Successfully relocated " << relocateCount << " of " << numFiles << " files." << std::endl;

    return 0;
}