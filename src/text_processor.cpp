#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cctype>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <set>
#include <regex>


namespace fs = std::filesystem;

// Function to create a directory if it doesn't exist
void createDirectoryIfNotExists(const fs::path &path)
{
    if (!fs::exists(path))
    {
        fs::create_directory(path);
    }
}

// Function to remove punctuation from a given text
std::string removePunctuation(const std::string &text)
{
    std::string result;
    for (char c : text)
    {
        if (!std::ispunct(c) || c == '\n')
        {
            result += c;
        }
    }
    return result;
}

// Function to convert a string to lowercase
std::string toLowerCase(const std::string &str)
{
    std::string lowerStr;
    std::transform(str.begin(), str.end(), std::back_inserter(lowerStr), ::tolower);
    return lowerStr;
}

// Function to track word occurrences and their line numbers in the text
std::map<std::string, std::vector<int>> trackWordOccurrences(const std::string &text)
{
    std::map<std::string, std::vector<int>> wordOccurrences;
    std::istringstream stream(text);
    std::string line;
    int lineNumber = 0;
    while (std::getline(stream, line))
    {
        ++lineNumber;
        std::istringstream lineStream(line);
        std::string word;
        while (lineStream >> word)
        {
            word = toLowerCase(word);
            wordOccurrences[word].push_back(lineNumber);
        }
    }
    return wordOccurrences;
}

// Function to count words and write the result to an output file
void countWords(const std::string &cleanedContent, const fs::path &outPath)
{
    std::map<std::string, int> wordCount;
    std::istringstream stream(cleanedContent);
    std::string word;
    while (stream >> word)
    {
        word = toLowerCase(word);
        ++wordCount[word];
    }

    std::vector<std::pair<std::string, int>> sortedWords(wordCount.begin(), wordCount.end());
    std::sort(sortedWords.begin(), sortedWords.end(), [](const auto &a, const auto &b)
              { return a.second > b.second; });

    std::ofstream outFile(outPath);
    if (!outFile)
    {
        throw std::runtime_error("Cannot create output file: " + outPath.string());
    }
    for (const auto &[word, count] : sortedWords)
    {
        outFile << word << ": " << count << std::endl;
    }
    outFile.close();
}

// Function to generate a cross-reference table and write it to an output file
void generateCrossReferenceTable(const std::map<std::string, std::vector<int>> &wordOccurrences, const fs::path &outPath)
{
    std::ofstream outFile(outPath);
    if (!outFile)
    {
        throw std::runtime_error("Cannot create output file: " + outPath.string());
    }

    // Sort domains alphabetically for better readability
    std::vector<std::pair<std::string, std::vector<int>>> sorted(wordOccurrences.begin(), wordOccurrences.end());
    std::sort(sorted.begin(), sorted.end());

    for (const auto &[word, lines] : sorted)
    {
        outFile << word << ": ";
        for (size_t i = 0; i < lines.size(); ++i)
        {
            outFile << lines[i];
            if (i < lines.size() - 1)
            {
                outFile << ", ";
            }
        }
        outFile << std::endl;
    }
    outFile.close();
}

// Function to extract domains and track their occurrences in the text
std::map<std::string, std::vector<int>> trackDomainOccurrences(const std::string &text)
{
    std::map<std::string, std::vector<int>> domainOccurrences;

    // Simplified regex pattern for testing
    std::regex domainRegex(R"((?:https?://)?(?:www\.)?([a-zA-Z0-9-]+(?:\.[a-zA-Z0-9-]+)*\.[a-zA-Z]{2,}))",
                           std::regex::icase);

    std::istringstream stream(text);
    std::string line;
    int lineNumber = 0;

    std::cout << "Starting domain detection...\n\n";

    while (std::getline(stream, line))
    {
        ++lineNumber;
        // std::cout << "Processing line " << lineNumber << ":\n" << line << "\n";

        std::sregex_iterator begin(line.begin(), line.end(), domainRegex), end;

        if (begin == end)
        {
            // std::cout << "No matches found in this line.\n";
        }

        for (auto it = begin; it != end; ++it)
        {
            std::smatch match = *it;
            std::string fullMatch = match[0];
            std::string domain = match[1];

            /* std::cout << "Found match:\n";
             std::cout << "  Full match: " << fullMatch << "\n";
            std::cout << "  Domain: " << domain << "\n";
            */

            domainOccurrences[domain].push_back(lineNumber);
        }
        // std::cout << "\n";
    }

    /* std::cout << "\nFinal domain occurrences:\n";
    for (const auto& [domain, lines] : domainOccurrences) {
        std::cout << domain << ": ";
        for (int line : lines) {
            std::cout << line << " ";
        }
        std::cout << "\n";
    }
    */

    return domainOccurrences;
}
// Function to count domains and write the result to an output file
void countDomains(const std::map<std::string, std::vector<int>> &domainOccurrences, const fs::path &outPath)
{
    // Create a map of domain counts
    std::map<std::string, int> domainCount;
    for (const auto &[domain, lines] : domainOccurrences)
    {
        domainCount[domain] = lines.size();
    }

    // Sort domains by count (descending) and alphabetically for ties
    std::vector<std::pair<std::string, int>> sortedDomains(domainCount.begin(), domainCount.end());
    std::sort(sortedDomains.begin(), sortedDomains.end(),
              [](const auto &a, const auto &b)
              {
                  if (a.second != b.second)
                  {
                      return a.second > b.second;
                  }
                  return a.first < b.first;
              });

    // Write to output file
    std::ofstream outFile(outPath);
    if (!outFile)
    {
        throw std::runtime_error("Cannot create output file: " + outPath.string());
    }
    for (const auto &[domain, count] : sortedDomains)
    {
        outFile << domain << ": " << count << std::endl;
    }
    outFile.close();
}