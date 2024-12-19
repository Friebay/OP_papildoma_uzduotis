#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cctype>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>

namespace fs = std::filesystem;

void createDirectoryIfNotExists(const fs::path& path) {
    if (!fs::exists(path)) {
        fs::create_directory(path);
    }
}

std::string removePunctuation(const std::string& text) {
    std::string result;
    for (char c : text) {
        if (!std::ispunct(c) || c == '\n') {
            result += c;
        }
    }
    return result;
}

std::map<std::string, int> countWords(const std::string& text) {
    std::map<std::string, int> wordCount;
    std::istringstream stream(text);
    std::string word;
    while (stream >> word) {
        ++wordCount[word];
    }
    return wordCount;
}

bool compareByValue(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
    return a.second > b.second;
}

int main() {
    try {
        // Create output directory if it doesn't exist
        fs::path outputDir = "output";
        createDirectoryIfNotExists(outputDir);

        // Read and process each file from data directory
        fs::path dataDir = "data";
        for (const auto& entry : fs::directory_iterator(dataDir)) {
            if (entry.path().extension() == ".txt") {
                // Read input file
                std::ifstream inFile(entry.path());
                if (!inFile) {
                    throw std::runtime_error("Cannot open input file: " + entry.path().string());
                }

                std::string content((std::istreambuf_iterator<char>(inFile)),
                                  std::istreambuf_iterator<char>());
                inFile.close();

                // Remove punctuation
                std::string cleanedContent = removePunctuation(content);

                // Count words
                std::map<std::string, int> wordCount = countWords(cleanedContent);

                // Sort words by frequency
                std::vector<std::pair<std::string, int>> sortedWords(wordCount.begin(), wordCount.end());
                std::sort(sortedWords.begin(), sortedWords.end(), compareByValue);

                // Write to output file
                fs::path outPath = outputDir / ("cleaned_" + entry.path().filename().string());
                std::ofstream outFile(outPath);
                if (!outFile) {
                    throw std::runtime_error("Cannot create output file: " + outPath.string());
                }
                for (const auto& [word, count] : sortedWords) {
                    outFile << word << ": " << count << std::endl;
                }
                outFile.close();

                std::cout << "Processed: " << entry.path().filename() << " -> " 
                         << outPath.filename() << std::endl;
            }
        }

        std::cout << "All files processed successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}