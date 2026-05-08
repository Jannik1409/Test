#include "csv_importer.h"
#include "../utils/dateutils.h"
#include <fstream>
#include <algorithm>

CsvImporter::CsvImporter() {}

bool CsvImporter::import(const std::string& filePath, std::vector<Transaction>& transactions,
                         const ColumnMapping& mapping, int accountId) {
    std::ifstream file(filePath);
    if (!file.is_open()) return false;

    std::string line;
    bool firstLine = true;

    while (std::getline(file, line)) {
        if (firstLine) {
            firstLine = false;
            continue;
        }

        auto fields = splitLine(line, ',');
        int maxCol = std::max(mapping.dateCol, std::max(mapping.amountCol, mapping.descriptionCol));
        if (fields.size() <= maxCol) {
            continue;
        }

        Transaction t;
        t.setAccountId(accountId);
        t.setDate(parseDate(fields[mapping.dateCol]));
        t.setAmount(std::stod(fields[mapping.amountCol]));
        t.setDescription(fields[mapping.descriptionCol]);
        t.setType(Transaction::typeFromString(detectType(fields[mapping.amountCol])));

        if (mapping.categoryCol >= 0 && mapping.categoryCol < static_cast<int>(fields.size())) {
            // Category lookup would be done by caller
        }

        transactions.push_back(t);
    }

    file.close();
    return true;
}

std::vector<std::string> CsvImporter::getHeaders(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return {};

    std::string line;
    if (std::getline(file, line)) {
        return splitLine(line, ',');
    }
    return {};
}

bool CsvImporter::detectDelimiter(const std::string& filePath, char& delimiter) {
    std::ifstream file(filePath);
    if (!file.is_open()) return false;

    std::string line;
    if (!std::getline(file, line)) return false;

    delimiter = ',';
    if (line.find(';') != std::string::npos) delimiter = ';';
    else if (line.find('\t') != std::string::npos) delimiter = '\t';

    return true;
}

std::vector<std::string> CsvImporter::splitLine(const std::string& line, char delimiter) {
    std::vector<std::string> result;
    std::string field;
    bool inQuotes = false;

    for (char c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == delimiter && !inQuotes) {
            result.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    result.push_back(field);
    return result;
}

std::string CsvImporter::detectType(const std::string& amount) {
    try {
        double val = std::stod(amount);
        return val >= 0 ? "Income" : "Expense";
    } catch (...) {
        return "Expense";
    }
}

std::string CsvImporter::parseDate(const std::string& date) {
    // Try common formats and convert to YYYY-MM-DD
    if (date.length() == 10 && date[4] == '-' && date[7] == '-') {
        return date;
    }
    // Add more format handling as needed
    return DateUtils::currentDate();
}