#include <iostream>
#include <fstream>
#include "../include/text_processor.h"

int main()
{
    try
    {
        // Create output directory if it doesn't exist
        fs::path outputDir = "output";
        createDirectoryIfNotExists(outputDir);

        // User menu for choosing the operation
        std::cout << "Choose an option:\n";
        std::cout << "1. Count words\n";
        std::cout << "2. Generate cross-reference table\n";
        std::cout << "3. Count domain occurrences\n";
        std::cout << "4. Extract and track internet domains\n";
        int choice;
        std::cin >> choice;

        // User menu for choosing the data file
        std::cout << "Choose a data file:\n";
        std::cout << "1. text_1.txt\n";
        std::cout << "2. text_2.txt\n";
        int fileChoice;
        std::cin >> fileChoice;

        std::string fileName;
        if (fileChoice == 1)
        {
            fileName = "text_1.txt";
        }
        else if (fileChoice == 2)
        {
            fileName = "text_2.txt";
        }
        else
        {
            std::cerr << "Invalid file choice!" << std::endl;
            return 1;
        }

        fs::path dataDir = "data";
        fs::path filePath = dataDir / fileName;

        // Read input file
        std::ifstream inFile(filePath);
        if (!inFile)
        {
            throw std::runtime_error("Cannot open input file: " + filePath.string());
        }

        std::string content((std::istreambuf_iterator<char>(inFile)),
                            std::istreambuf_iterator<char>());
        inFile.close();

        if (choice == 1)
        {
            // Remove punctuation from the content
            std::string cleanedContent = removePunctuation(content);

            // Count words and write to output file
            fs::path outPath = outputDir / ("word_count_" + fileName);
            countWords(cleanedContent, outPath);
        }
        else if (choice == 2)
        {
            // Remove punctuation from the content
            std::string cleanedContent = removePunctuation(content);

            // Track word occurrences
            std::map<std::string, std::vector<int>> wordOccurrences = trackWordOccurrences(cleanedContent);

            // Generate cross-reference table and write to output file
            fs::path outPath = outputDir / ("xref_" + fileName);
            generateCrossReferenceTable(wordOccurrences, outPath);
        }
        else if (choice == 3 || choice == 4)
        { // Handle both domain options
            // Track domain occurrences without removing punctuation
            std::map<std::string, std::vector<int>> domainOccurrences = trackDomainOccurrences(content);

            if (choice == 3)
            {

                // Count domain occurrences
                fs::path outPath = outputDir / ("domain_count_" + fileName);
                countDomains(domainOccurrences, outPath);
            }
            else
            {
                // Generate cross-reference table for domains
                fs::path outPath = outputDir / ("domain_xref_" + fileName);
                generateCrossReferenceTable(domainOccurrences, outPath);
            }
        }
        else
        {
            std::cerr << "Invalid choice!" << std::endl;
            return 1;
        }

        std::cout << "Processed: " << fileName << std::endl;
        std::cout << "All files processed successfully!" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}