#include "pdf_generator.h"
#include "../utils/dateutils.h"
#include <fstream>
#include <sstream>
#include <iomanip>

bool PdfGenerator::generateReport(const std::string& filePath,
                                   const std::vector<Transaction>& transactions,
                                   const std::vector<Category>& categories,
                                   const std::string& title,
                                   const std::string& startDate,
                                   const std::string& endDate) {
    std::string html = generateHtmlContent(transactions, categories, title, startDate, endDate);
    
    std::ofstream file(filePath);
    if (!file.is_open()) return false;
    
    file << html;
    file.close();
    return true;
}

bool PdfGenerator::generateMonthlyReport(const std::string& filePath,
                                         int year, int month,
                                         const std::vector<Transaction>& transactions,
                                         const std::vector<Category>& categories) {
    std::string title = "Monthly Report - " + DateUtils::getMonthName(month) + " " + std::to_string(year);
    std::string startDate = std::to_string(year) + "-" + std::to_string(month) + "-01";
    std::string endDate = DateUtils::getLastDayOfMonth(startDate);
    
    return generateReport(filePath, transactions, categories, title, startDate, endDate);
}

std::string PdfGenerator::formatCurrency(double amount) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << "$" << amount;
    return ss.str();
}

std::string PdfGenerator::generateHtmlContent(const std::vector<Transaction>& transactions,
                                               const std::vector<Category>& categories,
                                               const std::string& title,
                                               const std::string& startDate,
                                               const std::string& endDate) {
    Statistics stats;
    double income = stats.calculateTotalIncome(transactions);
    double expenses = stats.calculateTotalExpenses(transactions);
    double balance = stats.calculateBalance(transactions);
    
    std::stringstream ss;
    ss << "<!DOCTYPE html>\n<html>\n<head>\n"
       "<meta charset=\"UTF-8\">\n"
       "<title>" << title << "</title>\n"
       "<style>\n"
       "body { font-family: Arial, sans-serif; margin: 40px; }\n"
       "h1 { color: #2C3E50; }\n"
       ".summary { background: #ECF0F1; padding: 20px; border-radius: 5px; margin-bottom: 20px; }\n"
       ".summary-item { display: inline-block; margin-right: 30px; }\n"
       ".income { color: #27AE60; }\n"
       ".expense { color: #E74C3C; }\n"
       "table { width: 100%; border-collapse: collapse; }\n"
       "th, td { border: 1px solid #ddd; padding: 12px; text-align: left; }\n"
       "th { background: #3498DB; color: white; }\n"
       "tr:nth-child(even) { background: #f9f9f9; }\n"
       ".date-range { color: #7f8c8d; }\n"
       "</style>\n"
       "</head>\n<body>\n"
       "<h1>" << title << "</h1>\n"
       "<p class=\"date-range\">Period: " << startDate << " to " << endDate << "</p>\n"
       "<div class=\"summary\">\n"
       "  <div class=\"summary-item\"><strong>Total Income:</strong> <span class=\"income\">" << formatCurrency(income) << "</span></div>\n"
       "  <div class=\"summary-item\"><strong>Total Expenses:</strong> <span class=\"expense\">" << formatCurrency(expenses) << "</span></div>\n"
       "  <div class=\"summary-item\"><strong>Balance:</strong> <span>" << formatCurrency(balance) << "</span></div>\n"
       "</div>\n"
       "<h2>Transactions (" << transactions.size() << ")</h2>\n"
       "<table>\n"
       "<tr><th>Date</th><th>Description</th><th>Category</th><th>Type</th><th>Amount</th></tr>\n";
    
    for (const auto& t : transactions) {
        std::string typeStr = t.getType() == Transaction::Type::Income ? "Income" : "Expense";
        std::string amountClass = t.getType() == Transaction::Type::Income ? "income" : "expense";
        ss << "<tr><td>" << t.getDate() << "</td><td>" << t.getDescription() << "</td>"
           << "<td>-</td><td>" << typeStr << "</td><td class=\"" << amountClass << "\">" 
           << formatCurrency(t.getAmount()) << "</td></tr>\n";
    }
    
    ss << "</table>\n</body>\n</html>";
    return ss.str();
}