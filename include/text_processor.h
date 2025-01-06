#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H

#include <string>
#include <map>
#include <vector>
#include <filesystem>
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

// Function declarations
void createDirectoryIfNotExists(const fs::path &path);
std::string removePunctuation(const std::string &text);
std::string toLowerCase(const std::string &str);
std::map<std::string, std::vector<int>> trackWordOccurrences(const std::string &text);
void countWords(const std::string &cleanedContent, const fs::path &outPath);
void generateCrossReferenceTable(const std::map<std::string, std::vector<int>> &wordOccurrences, const fs::path &outPath, bool isURL);
std::map<std::string, std::vector<int>> trackDomainOccurrences(const std::string &text);
void countDomains(const std::map<std::string, std::vector<int>> &domainOccurrences, const fs::path &outPath);
void findWordsWithSto(const std::string &cleanedContent, const fs::path &outPath);

#endif