#include "DataModifier.hpp"
#include <iostream>
#include <stdexcept>

int DataModifier::insert(const std::string& table, const std::vector<std::string>& columns,
                         const std::vector<std::string>& values) {
   if (columns.size() != values.size()) {
      throw std::invalid_argument("Columns and values must have the same size");
   }
   auto conn_handle = pool->getConnection();
   try {
      pqxx::work  txn(*conn_handle);
      std::string query = "INSERT INTO " + txn.quote_name(table) + " (";

      for (size_t i = 0; i < columns.size(); ++i) {
         if (i > 0)
            query += ", ";
         query += txn.quote_name(columns[i]);
      }
      query += ") VALUES (";

      for (size_t i = 0; i < values.size(); ++i) {
         if (i > 0)
            query += ", ";
         query += "$" + std::to_string(i + 1);
      }
      query += ") RETURNING id";

      // convert vector to params
      pqxx::params params;
      for (const auto& value : values) {
         params.append(value);
      }
      pqxx::result result = txn.exec_params(query, params);
      txn.commit();
      return result.empty() ? -1 : result[0][0].as<int>();

   } catch (const std::exception& e) {
      std::cerr << "Error inserting data: " << e.what() << std::endl;
      throw;
   }
}

size_t DataModifier::update(const std::string& table, const std::string& set_column, const std::string& set_value,
                            const std::string& where_column, const std::string& where_value) {
   auto conn_handle = pool->getConnection();
   try {
      pqxx::work  txn(*conn_handle);
      std::string query = "UPDATE " + txn.esc(table) + " SET " + txn.quote_name(set_column) + " = " +
                          txn.quote(set_value) + " WHERE " + txn.quote_name(where_column) + " = " +
                          txn.quote(where_value);
      pqxx::result result = txn.exec(query);
      txn.commit();
      return result.affected_rows();

   } catch (const std::exception& e) {
      std::cerr << "Error updating data: " << e.what() << std::endl;
      throw;
   }
}
