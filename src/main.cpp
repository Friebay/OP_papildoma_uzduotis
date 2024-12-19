#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <regex>
#include <set>
#include <algorithm>

// Function to count words in a file and output words that appear more than once
void countWords(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::ifstream inputFile(inputFilePath);
    std::ofstream outputFile(outputFilePath);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input file." << std::endl;
        return;
    }
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return;
    }

    std::map<std::string, int> wordCount;
    std::string line;
    std::regex wordRegex(R"(\b\w+\b)", std::regex::icase);

    while (std::getline(inputFile, line)) {
        std::sregex_iterator wordsBegin = std::sregex_iterator(line.begin(), line.end(), wordRegex);
        std::sregex_iterator wordsEnd = std::sregex_iterator();

        for (std::sregex_iterator i = wordsBegin; i != wordsEnd; ++i) {
            std::string word = (*i).str();
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            wordCount[word]++;
        }
    }

    for (const auto& pair : wordCount) {
        if (pair.second > 1) {
            outputFile << pair.first << ": " << pair.second << std::endl;
        }
    }

    inputFile.close();
    outputFile.close();
}

int main() {
    std::string inputFilePath;
    std::string outputFilePath;

    // Simple console GUI
    std::cout << "Word Counter Program" << std::endl;
    std::cout << "====================" << std::endl;
    std::cout << "Enter the path to the input file: ";
    std::getline(std::cin, inputFilePath);
    std::cout << "Enter the path to the output file: ";
    std::getline(std::cin, outputFilePath);

    // Call the function to count words
    countWords(inputFilePath, outputFilePath);

    std::cout << "Word counting completed. Check the output file for results." << std::endl;

    return 0;
}