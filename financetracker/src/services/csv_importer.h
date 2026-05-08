#ifndef CSV_IMPORTER_H
#define CSV_IMPORTER_H

#include <string>
#include <vector>
#include "../models/transaction.h"

/**
 * @class CsvImporter
 * @brief Import transactions from CSV files
 */
class CsvImporter {
public:
    struct ColumnMapping {
        int dateCol;
        int amountCol;
        int descriptionCol;
        int categoryCol;
        int typeCol;
    };

    CsvImporter();

    bool import(const std::string& filePath, std::vector<Transaction>& transactions,
                const ColumnMapping& mapping, int accountId);
    std::vector<std::string> getHeaders(const std::string& filePath);
    bool detectDelimiter(const std::string& filePath, char& delimiter);

private:
    std::vector<std::string> splitLine(const std::string& line, char delimiter);
    std::string detectType(const std::string& amount);
    std::string parseDate(const std::string& date);
};

#endif // CSV_IMPORTER_H