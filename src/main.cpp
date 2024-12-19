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
void createDirectoryIfNotExists(const fs::path& path) {
    if (!fs::exists(path)) {
        fs::create_directory(path);
    }
}

// Function to remove punctuation from a given text
std::string removePunctuation(const std::string& text) {
    std::string result;
    for (char c : text) {
        if (!std::ispunct(c) || c == '\n') {
            result += c;
        }
    }
    return result;
}

// Function to convert a string to lowercase
std::string toLowerCase(const std::string& str) {
    std::string lowerStr;
    std::transform(str.begin(), str.end(), std::back_inserter(lowerStr), ::tolower);
    return lowerStr;
}

// Function to track word occurrences and their line numbers in the text
std::map<std::string, std::vector<int>> trackWordOccurrences(const std::string& text) {
    std::map<std::string, std::vector<int>> wordOccurrences;
    std::istringstream stream(text);
    std::string line;
    int lineNumber = 0;
    while (std::getline(stream, line)) {
        ++lineNumber;
        std::istringstream lineStream(line);
        std::string word;
        while (lineStream >> word) {
            word = toLowerCase(word);
            wordOccurrences[word].push_back(lineNumber);
        }
    }
    return wordOccurrences;
}

// Function to count words and write the result to an output file
void countWords(const std::string& cleanedContent, const fs::path& outPath) {
    std::map<std::string, int> wordCount;
    std::istringstream stream(cleanedContent);
    std::string word;
    while (stream >> word) {
        word = toLowerCase(word);
        ++wordCount[word];
    }

    std::vector<std::pair<std::string, int>> sortedWords(wordCount.begin(), wordCount.end());
    std::sort(sortedWords.begin(), sortedWords.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    std::ofstream outFile(outPath);
    if (!outFile) {
        throw std::runtime_error("Cannot create output file: " + outPath.string());
    }
    for (const auto& [word, count] : sortedWords) {
        outFile << word << ": " << count << std::endl;
    }
    outFile.close();
}

// Function to generate a cross-reference table and write it to an output file
void generateCrossReferenceTable(const std::map<std::string, std::vector<int>>& wordOccurrences, const fs::path& outPath) {
    std::ofstream outFile(outPath);
    if (!outFile) {
        throw std::runtime_error("Cannot create output file: " + outPath.string());
    }

    // Sort domains alphabetically for better readability
    std::vector<std::pair<std::string, std::vector<int>>> sorted(wordOccurrences.begin(), wordOccurrences.end());
    std::sort(sorted.begin(), sorted.end());

    for (const auto& [word, lines] : sorted) {
        outFile << word << ": ";
        for (size_t i = 0; i < lines.size(); ++i) {
            outFile << lines[i];
            if (i < lines.size() - 1) {
                outFile << ", ";
            }
        }
        outFile << std::endl;
    }
    outFile.close();
}

// Function to extract domains and track their occurrences in the text
std::map<std::string, std::vector<int>> trackDomainOccurrences(const std::string& text) {
    std::map<std::string, std::vector<int>> domainOccurrences;
    
    // Simplified regex pattern for testing
    std::regex domainRegex(R"((?:https?://)?(?:www\.)?([a-zA-Z0-9-]+(?:\.[a-zA-Z0-9-]+)*\.[a-zA-Z]{2,}))", 
        std::regex::icase);
    
    std::istringstream stream(text);
    std::string line;
    int lineNumber = 0;
    
    std::cout << "Starting domain detection...\n\n";
    
    while (std::getline(stream, line)) {
        ++lineNumber;
        // std::cout << "Processing line " << lineNumber << ":\n" << line << "\n";
        
        std::sregex_iterator begin(line.begin(), line.end(), domainRegex), end;
        
        if (begin == end) {
            // std::cout << "No matches found in this line.\n";
        }
        
        for (auto it = begin; it != end; ++it) {
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
void countDomains(const std::map<std::string, std::vector<int>>& domainOccurrences, const fs::path& outPath) {
    // Create a map of domain counts
    std::map<std::string, int> domainCount;
    for (const auto& [domain, lines] : domainOccurrences) {
        domainCount[domain] = lines.size();
    }

    // Sort domains by count (descending) and alphabetically for ties
    std::vector<std::pair<std::string, int>> sortedDomains(domainCount.begin(), domainCount.end());
    std::sort(sortedDomains.begin(), sortedDomains.end(),
        [](const auto& a, const auto& b) {
            if (a.second != b.second) {
                return a.second > b.second;
            }
            return a.first < b.first;
        });

    // Write to output file
    std::ofstream outFile(outPath);
    if (!outFile) {
        throw std::runtime_error("Cannot create output file: " + outPath.string());
    }
    for (const auto& [domain, count] : sortedDomains) {
        outFile << domain << ": " << count << std::endl;
    }
    outFile.close();
}

int main() {
    try {
        // Create output directory if it doesn't exist
        fs::path outputDir = "output";
        createDirectoryIfNotExists(outputDir);

        // User menu for choosing the operation
        std::cout << "Choose an option:\n";
        std::cout << "1. Count words\n";
        std::cout << "2. Generate cross-reference table\n";
        std::cout << "3. Extract and track internet domains\n";
        std::cout << "4. Count domain occurrences\n";  // New option
        int choice;
        std::cin >> choice;

        // User menu for choosing the data file
        std::cout << "Choose a data file:\n";
        std::cout << "1. text_1.txt\n";
        std::cout << "2. text_2.txt\n";
        int fileChoice;
        std::cin >> fileChoice;

        std::string fileName;
        if (fileChoice == 1) {
            fileName = "text_1.txt";
        } else if (fileChoice == 2) {
            fileName = "text_2.txt";
        } else {
            std::cerr << "Invalid file choice!" << std::endl;
            return 1;
        }

        fs::path dataDir = "data";
        fs::path filePath = dataDir / fileName;

        // Read input file
        std::ifstream inFile(filePath);
        if (!inFile) {
            throw std::runtime_error("Cannot open input file: " + filePath.string());
        }

        std::string content((std::istreambuf_iterator<char>(inFile)),
                            std::istreambuf_iterator<char>());
        inFile.close();

        if (choice == 1) {
            // Remove punctuation from the content
            std::string cleanedContent = removePunctuation(content);

            // Count words and write to output file
            fs::path outPath = outputDir / ("word_count_" + fileName);
            countWords(cleanedContent, outPath);
        } else if (choice == 2) {
            // Remove punctuation from the content
            std::string cleanedContent = removePunctuation(content);

            // Track word occurrences
            std::map<std::string, std::vector<int>> wordOccurrences = trackWordOccurrences(cleanedContent);

            // Generate cross-reference table and write to output file
            fs::path outPath = outputDir / ("xref_" + fileName);
            generateCrossReferenceTable(wordOccurrences, outPath);
        } else if (choice == 3 || choice == 4) {  // Handle both domain options
            // Track domain occurrences without removing punctuation
            std::map<std::string, std::vector<int>> domainOccurrences = trackDomainOccurrences(content);

            if (choice == 3) {
                // Generate cross-reference table for domains
                fs::path outPath = outputDir / ("domain_xref_" + fileName);
                generateCrossReferenceTable(domainOccurrences, outPath);
            } else {
                // Count domain occurrences
                fs::path outPath = outputDir / ("domain_count_" + fileName);
                countDomains(domainOccurrences, outPath);
            }
        } else {
            std::cerr << "Invalid choice!" << std::endl;
            return 1;
        }

        std::cout << "Processed: " << fileName << std::endl;
        std::cout << "All files processed successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}