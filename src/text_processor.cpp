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
        if (!std::ispunct(c) && c != '–' || c == '\n')
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
        throw std::runtime_error("Negalima sukurti isvesties failo: " + outPath.string());
    }
    for (const auto &[word, count] : sortedWords)
    {
        if (count > 1)
        {
            outFile << word << ": " << count << std::endl;
        }
    }
    outFile.close();
}
// Function to generate a cross-reference table and write it to an output file
void generateCrossReferenceTable(const std::map<std::string, std::vector<int>> &wordOccurrences, const fs::path &outPath, bool isURL = false)
{
    std::ofstream outFile(outPath);
    if (!outFile)
    {
        throw std::runtime_error("Negalima sukurti isvesties failo: " + outPath.string());
    }

    // Sort entries alphabetically for better readability
    std::vector<std::pair<std::string, std::vector<int>>> sorted(wordOccurrences.begin(), wordOccurrences.end());
    std::sort(sorted.begin(), sorted.end());

    for (const auto &[word, lines] : sorted)
    {
        if ((isURL && lines.size() > 0) || (!isURL && lines.size() > 1))
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
    }
    outFile.close();
}
#include <fstream>
#include <set>

// Function to read TLDs from a file
std::set<std::string> readTLDs(const std::string &filePath)
{
    std::set<std::string> tlds;
    std::ifstream file(filePath);
    std::string line;
    while (std::getline(file, line))
    {
        tlds.insert(line);
    }
    return tlds;
}

// Function to track domain occurrences and their line numbers in the text
std::map<std::string, std::vector<int>> trackDomainOccurrences(const std::string &text)
{
    std::map<std::string, std::vector<int>> domainOccurrences;

    // Read TLDs from file
    std::set<std::string> tlds = readTLDs("../data/tlds.txt");

    // Debug output for cleaned text
    std::istringstream debugStream(text);
    std::string debugLine;
    int debugLineNum = 0;

    // Existing regex patterns
    std::regex urlRegex(
        R"((https?:\/\/)?((www\.)?([a-zA-Z0-9]([a-zA-Z0-9-]*[a-zA-Z0-9])?\.)+[a-zA-Z]{2,}|localhost|\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})(:\d+)?(\/[a-zA-Z0-9\-._~:/?#\[\]@!$&'()*+,;=]*)?)",
        std::regex::icase);

    std::regex fileExtensionRegex(
        R"(\.(exe|txt|doc|docx|xls|xlsx|zip|rar|7z|tar|gz|bin|iso|dmg|apk|bat|cmd|css|dll|jar|js|msi|ps1|py|sh|sql|sys|vb|xml|pdf)$)",
        std::regex::icase);

    // New dot check regex
    std::regex hasDotRegex(R"(\.)");

    std::istringstream stream(text);
    std::string line;
    int lineNumber = 0;

    while (std::getline(stream, line))
    {
        ++lineNumber;

        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

        // Debug output
        // std::cout << "Processing line " << lineNumber << ": [" << line << "]\n";

        std::sregex_iterator begin(line.begin(), line.end(), urlRegex), end;

        for (auto it = begin; it != end; ++it)
        {
            std::smatch match = *it;
            std::string url = match[0];

            // Debug output
            std::cout << "Found potential URL: " << url << "\n";

            if (std::regex_search(url, fileExtensionRegex))
            {
                std::cout << "URL rejected due to file extension: " << url << "\n";
            }
            else if (!std::regex_search(url, hasDotRegex))
            {
                std::cout << "URL rejected due to missing dot: " << url << "\n";
            }
            else
            {
                // Check if URL contains a valid TLD
                bool hasValidTLD = false;
                for (const auto &tld : tlds)
                {
                    if (url.find(tld) != std::string::npos)
                    {
                        hasValidTLD = true;
                        break;
                    }
                }

                if (hasValidTLD)
                {
                    std::cout << "URL accepted: " << url << "\n";
                    domainOccurrences[url].push_back(lineNumber);
                }
                else
                {
                    std::cout << "URL rejected due to invalid TLD: " << url << "\n";
                }
            }
        }
    }

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
        throw std::runtime_error("Negalima sukurti isvesties failo: " + outPath.string());
    }
    for (const auto &[domain, count] : sortedDomains)
    {
        if (count > 0)
        {
            {
                outFile << domain << ": " << count << std::endl;
            }
        }
    }
    outFile.close();
}