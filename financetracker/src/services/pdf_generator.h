#ifndef PDF_GENERATOR_H
#define PDF_GENERATOR_H

#include <string>
#include <vector>
#include "../models/transaction.h"
#include "../models/category.h"
#include "../services/statistics.h"

/**
 * @class PdfGenerator
 * @brief Generate PDF reports for transactions and statistics
 */
class PdfGenerator {
public:
    static bool generateReport(const std::string& filePath,
                               const std::vector<Transaction>& transactions,
                               const std::vector<Category>& categories,
                               const std::string& title,
                               const std::string& startDate,
                               const std::string& endDate);

    static bool generateMonthlyReport(const std::string& filePath,
                                       int year, int month,
                                       const std::vector<Transaction>& transactions,
                                       const std::vector<Category>& categories);

private:
    static std::string formatCurrency(double amount);
    static std::string generateHtmlContent(const std::vector<Transaction>& transactions,
                                            const std::vector<Category>& categories,
                                            const std::string& title,
                                            const std::string& startDate,
                                            const std::string& endDate);
};

#endif // PDF_GENERATOR_H