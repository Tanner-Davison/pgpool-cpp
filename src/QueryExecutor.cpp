#include "QueryExecutor.hpp"

/* Helpful Client Decision Tree
 *  Does the query involve ANY user input?
├── YES → Use selectPrepared() (or build safe query with escaping)
└── NO → Is it a simple SELECT * WHERE column = value?
    ├── YES → Could use either, but selectPrepared() is still safer
    └── NO → Is it complex (JOINs, multiple conditions, aggregations)?
        ├── YES → Use select() with hardcoded query
        └── NO → Use selectPrepared() for safety

 * */

pqxx::result QueryExecutor::select(const std::string& query) {
   auto conn_handle = pool->getConnection();
   try {
      pqxx::work   txn(*conn_handle);
      pqxx::result result = txn.exec(query);
      txn.commit();
      std::cout << "Query ruturned " << result.size() << " rows" << std::endl;
      return result;
   } catch (const pqxx::sql_error& e) {
      std::cerr << "SQL Error in query: " << e.what() << std::endl;
      throw;
   }
}

pqxx::result QueryExecutor::selectPrepared(const std::string& table, const std::string& condition_column,
                                           const std::string& value) {
   auto conn_handle = pool->getConnection();

   try {
      pqxx::work txn(*conn_handle);
      // Using pqxx's safe parameterization
      std::string query =
          "SELECT * FROM " + txn.esc(table) + " WHERE " + txn.quote_name(condition_column) + " = " + txn.quote(value);
      pqxx::result result = txn.exec(query);
      txn.commit();
      return result;
   } catch (const std::exception& e) {
      std::cerr << "Error in prepared select: " << e.what() << std::endl;
      throw;
   }
}
