#include <iostream>
#include <fstream>
#include "../include/text_processor.h"

int main()
{
    char continueProgram;
    do
    {
        try
        {
            // Create output directory if it doesn't exist
            fs::path outputDir = "output";
            createDirectoryIfNotExists(outputDir);

            // User menu for choosing the operation
            std::cout << "Pasirinkite norima funkcija:\n";
            std::cout << "1. Skaiciuoti zodzius tekste\n";
            std::cout << "2. Generuoti cross-reference lentele zodziams\n";
            std::cout << "3. Skaiciuoti domenus tekste\n";
            std::cout << "4. Generuoti cross-reference lentele domenams\n";
            std::cout << "5. Atlikti visas operacijas su pasirinktu failu\n";
            std::cout << "6. Rasti zodzius su 'sto'\n";
            int choice;
            std::cin >> choice;

            // User menu for choosing the data file
            std::cout << "Pasirinkite teksto faila:\n";
            std::cout << "1. nuorodos_testavimui.txt\n";
            std::cout << "2. papartynu_saule_90531_zodziai.txt\n";
            std::cout << "3. tekstas_angliskas.txt\n";
            std::cout << "4. tekstas_su_nuorodomis.txt\n";

            int fileChoice;
            std::cin >> fileChoice;

            std::string fileName;
            if (fileChoice == 1)
            {
                fileName = "nuorodos_testavimui.txt";
            }
            else if (fileChoice == 2)
            {
                fileName = "papartynu_saule_90531_zodziai.txt";
            }
            else if (fileChoice == 3)
            {
                fileName = "tekstas_angliskas.txt";
            }
            else if (fileChoice == 4)
            {
                fileName = "tekstas_su_nuorodomis.txt";
            }
            else
            {
                std::cerr << "Netinkamas failas!" << std::endl;
                return 1;
            }

            fs::path dataDir = "data";
            fs::path filePath = dataDir / fileName;

            // Read input file
            std::ifstream inFile(filePath);
            if (!inFile)
            {
                throw std::runtime_error("Negalime atidaryti duomenu failo: " + filePath.string());
            }

            std::string content((std::istreambuf_iterator<char>(inFile)),
                                std::istreambuf_iterator<char>());
            inFile.close();

            if (choice == 1)
            {
                // Remove punctuation from the content
                std::string cleanedContent = removePunctuation(content);

                // Count words and write to output file
                fs::path outPath = outputDir / ("zodziu_skaicius_" + fileName);
                countWords(cleanedContent, outPath);
            }
            else if (choice == 2)
            {
                // Remove punctuation from the content
                std::string cleanedContent = removePunctuation(content);

                // Track word occurrences
                std::map<std::string, std::vector<int>> wordOccurrences = trackWordOccurrences(cleanedContent);

                // Generate cross-reference table and write to output file
                fs::path outPath = outputDir / ("cross_reference_" + fileName);
                generateCrossReferenceTable(wordOccurrences, outPath, false);
            }
            else if (choice == 6)
            {
                // Generate cross-reference table and write to output file
                fs::path outPath = outputDir / ("words_with_sto_" + fileName);

                // Save the unique words with 'sto' to the output file
                findWordsWithSto(content, outPath);
            }
            else if (choice == 3 || choice == 4)
            { // Handle both domain options
                // Track domain occurrences without removing punctuation
                std::map<std::string, std::vector<int>> domainOccurrences = trackDomainOccurrences(content);

                if (choice == 3)
                {
                    // Count domain occurrences
                    fs::path outPath = outputDir / ("URL_skaicius_" + fileName);
                    countDomains(domainOccurrences, outPath);
                }
                else
                {
                    // Generate cross-reference table for domains
                    fs::path outPath = outputDir / ("domain_cross_reference_" + fileName);
                    generateCrossReferenceTable(domainOccurrences, outPath, true);
                }
            }
            else if (choice == 5)
            {
                // Remove punctuation from the content
                std::string cleanedContent = removePunctuation(content);

                // Count words and write to output file
                fs::path wordCountPath = outputDir / ("zodziu_skaicius_" + fileName);
                countWords(cleanedContent, wordCountPath);

                // Track word occurrences
                std::map<std::string, std::vector<int>> wordOccurrences = trackWordOccurrences(cleanedContent);

                // Generate cross-reference table for words and write to output file
                fs::path wordXrefPath = outputDir / ("cross_reference_" + fileName);
                generateCrossReferenceTable(wordOccurrences, wordXrefPath, false);

                // Track domain occurrences without removing punctuation
                std::map<std::string, std::vector<int>> domainOccurrences = trackDomainOccurrences(content);

                // Count domain occurrences
                fs::path domainCountPath = outputDir / ("URL_skaicius_" + fileName);
                countDomains(domainOccurrences, domainCountPath);

                // Generate cross-reference table for domains
                fs::path domainXrefPath = outputDir / ("URL_cross_reference_" + fileName);
                generateCrossReferenceTable(domainOccurrences, domainXrefPath, true);
            }
            else
            {
                std::cerr << "Netinkamas pasirinkimas!" << std::endl;
                return 1;
            }

            std::cout << "Atlikta: " << fileName << std::endl;
            std::cout << "Apdorota! Informacija issaugota i 'output' aplankala." << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Klaida: " << e.what() << std::endl;
        }

        // Clear input buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "\nAr norite testi? (t/n): ";
        std::cin >> continueProgram;

    } while (tolower(continueProgram) == 't');

    return 0;
}