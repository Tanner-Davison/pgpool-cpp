#include "InsertDialog.hpp"
#include "DatabaseManager.hpp"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>

InsertDialog::InsertDialog(DatabaseManager* dbManager, QWidget* parent) : QDialog(parent), m_dbManager(dbManager) {
   setWindowTitle("Insert Data");
   setModal(true);
   setupUI();
   refreshTables();
   resize(600, 400);
}

void InsertDialog::setupUI() {
   auto* mainLayout = new QVBoxLayout(this);

   // Table selection
   auto* tableGroup  = new QGroupBox("Select Table", this);
   auto* tableLayout = new QHBoxLayout(tableGroup);

   tableLayout->addWidget(new QLabel("Table:"));
   m_tableCombo = new QComboBox(this);
   tableLayout->addWidget(m_tableCombo);
   tableLayout->addStretch();

   mainLayout->addWidget(tableGroup);

   // Data input table
   auto* dataGroup  = new QGroupBox("Data to Insert", this);
   auto* dataLayout = new QVBoxLayout(dataGroup);

   m_dataTable = new QTableWidget(this);
   m_dataTable->setRowCount(1); // Start with one row
   dataLayout->addWidget(m_dataTable);

   // Row control buttons
   auto* rowBtnLayout = new QHBoxLayout();
   m_addRowBtn        = new QPushButton("Add Row", this);
   m_removeRowBtn     = new QPushButton("Remove Row", this);
   rowBtnLayout->addWidget(m_addRowBtn);
   rowBtnLayout->addWidget(m_removeRowBtn);
   rowBtnLayout->addStretch();
   dataLayout->addLayout(rowBtnLayout);

   mainLayout->addWidget(dataGroup);

   // Dialog buttons
   auto* btnLayout = new QHBoxLayout();
   m_insertBtn     = new QPushButton("Insert", this);
   m_insertBtn->setDefault(true);
   m_cancelBtn = new QPushButton("Cancel", this);

   btnLayout->addStretch();
   btnLayout->addWidget(m_insertBtn);
   btnLayout->addWidget(m_cancelBtn);
   mainLayout->addLayout(btnLayout);

   // Connect signals
   connect(m_tableCombo, &QComboBox::currentTextChanged, this, &InsertDialog::onTableChanged);
   connect(m_addRowBtn, &QPushButton::clicked, this, &InsertDialog::onAddRow);
   connect(m_removeRowBtn, &QPushButton::clicked, this, &InsertDialog::onRemoveRow);
   connect(m_insertBtn, &QPushButton::clicked, this, &InsertDialog::onInsertData);
   connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void InsertDialog::refreshTables() {
   try {
      std::string query =
          "SELECT table_name FROM information_schema.tables "
          "WHERE table_schema = 'public' ORDER BY table_name";

      pqxx::result result = m_dbManager->query().select(query);

      m_tableCombo->clear();
      m_tableCombo->addItem("-- Select Table --");

      for (auto const& row : result) {
         m_tableCombo->addItem(QString::fromStdString(row[0].as<std::string>()));
      }
   } catch (const std::exception& e) {
      QMessageBox::warning(this, "Error", QString("Failed to fetch tables: %1").arg(e.what()));
   }
}

void InsertDialog::onTableChanged(const QString& tableName) {
   if (tableName == "-- Select Table --" || tableName.isEmpty()) {
      m_dataTable->clear();
      m_dataTable->setColumnCount(0);
      m_currentColumns.clear();
      m_columnTypes.clear();
      return;
   }

   fetchTableColumns(tableName.toStdString());
}

void InsertDialog::fetchTableColumns(const std::string& tableName) {
   try {
      // Get column information
      std::string query =
          "SELECT column_name, data_type, is_nullable, column_default "
          "FROM information_schema.columns "
          "WHERE table_schema = 'public' AND table_name = '" +
          tableName +
          "' "
          "ORDER BY ordinal_position";

      pqxx::result result = m_dbManager->query().select(query);

      m_currentColumns.clear();
      m_columnTypes.clear();
      QStringList headers;

      for (auto const& row : result) {
         std::string colName    = row[0].as<std::string>();
         std::string dataType   = row[1].as<std::string>();
         std::string isNullable = row[2].as<std::string>();
         std::string defaultVal = row[3].is_null() ? "" : row[3].as<std::string>();

         // Skip auto-generated columns
         if (!defaultVal.empty()) {
            // Skip SERIAL columns (nextval)
            if (defaultVal.find("nextval") != std::string::npos) {
               continue;
            }
            // Skip timestamp columns with defaults like CURRENT_TIMESTAMP, now(), etc.
            if (defaultVal.find("CURRENT_TIMESTAMP") != std::string::npos ||
                defaultVal.find("current_timestamp") != std::string::npos ||
                defaultVal.find("now()") != std::string::npos || defaultVal.find("timezone") != std::string::npos) {
               continue;
            }
         }

         m_currentColumns.push_back(colName);
         m_columnTypes.push_back(dataType);

         QString header = QString::fromStdString(colName);
         header += "\n(" + QString::fromStdString(dataType) + ")";
         if (isNullable == "NO" && defaultVal.empty()) {
            header += " NOT NULL";
         }
         headers << header;
      }
      // Setup the table widget
      m_dataTable->setColumnCount(m_currentColumns.size());
      m_dataTable->setHorizontalHeaderLabels(headers);
      m_dataTable->horizontalHeader()->setStretchLastSection(true);
      m_dataTable->setRowCount(1); // Reset to one row

      // Clear all cells
      for (int row = 0; row < m_dataTable->rowCount(); ++row) {
         for (int col = 0; col < m_dataTable->columnCount(); ++col) {
            m_dataTable->setItem(row, col, new QTableWidgetItem(""));
         }
      }

   } catch (const std::exception& e) {
      QMessageBox::warning(this, "Error", QString("Failed to fetch columns: %1").arg(e.what()));
   }
}

void InsertDialog::onAddRow() {
   int newRow = m_dataTable->rowCount();
   m_dataTable->insertRow(newRow);

   // Initialize cells for the new row
   for (int col = 0; col < m_dataTable->columnCount(); ++col) {
      m_dataTable->setItem(newRow, col, new QTableWidgetItem(""));
   }
}

void InsertDialog::onRemoveRow() {
   int currentRow = m_dataTable->currentRow();
   if (currentRow >= 0 && m_dataTable->rowCount() > 1) {
      m_dataTable->removeRow(currentRow);
   }
}

void InsertDialog::onInsertData() {
   QString tableName = m_tableCombo->currentText();
   if (tableName == "-- Select Table --" || tableName.isEmpty()) {
      QMessageBox::warning(this, "Warning", "Please select a table");
      return;
   }

   if (m_currentColumns.empty()) {
      QMessageBox::warning(this, "Warning", "No columns available for insertion");
      return;
   }

   try {
      int                      successCount = 0;
      std::vector<std::string> failedRows;

      // Process each row
      for (int row = 0; row < m_dataTable->rowCount(); ++row) {
         std::vector<std::string> values;
         bool                     rowHasData = false;

         // Collect values for this row
         for (int col = 0; col < m_dataTable->columnCount(); ++col) {
            QTableWidgetItem* item  = m_dataTable->item(row, col);
            QString           value = item ? item->text() : "";

            if (!value.isEmpty()) {
               rowHasData = true;
            }

            // Handle different data types
            if (value.isEmpty() || value.toUpper() == "NULL") {
               values.push_back("NULL");
            } else if (m_columnTypes[col].find("int") != std::string::npos ||
                       m_columnTypes[col].find("numeric") != std::string::npos ||
                       m_columnTypes[col].find("decimal") != std::string::npos ||
                       m_columnTypes[col].find("float") != std::string::npos ||
                       m_columnTypes[col].find("double") != std::string::npos ||
                       m_columnTypes[col].find("real") != std::string::npos) {
               // Numeric types - no quotes
               values.push_back(value.toStdString());
            } else if (m_columnTypes[col].find("bool") != std::string::npos) {
               // Boolean type
               values.push_back(value.toLower().toStdString());
            } else {
               // String/text types - add quotes
               values.push_back("'" + value.toStdString() + "'");
            }
         }

         // Skip empty rows
         if (!rowHasData) {
            continue;
         }

         // Use DataModifier's insert method
         try {
            int result = m_dbManager->data().insert(tableName.toStdString(), m_currentColumns, values);

            if (result > 0) {
               successCount++;
            }
         } catch (const std::exception& e) {
            failedRows.push_back("Row " + std::to_string(row + 1) + ": " + e.what());
         }
      }

      // Show results
      if (successCount > 0 && failedRows.empty()) {
         QMessageBox::information(this, "Success", QString("%1 row(s) inserted successfully").arg(successCount));
         emit dataInserted();
         accept();
      } else if (successCount > 0) {
         QString msg = QString("%1 row(s) inserted successfully.\n\nFailed rows:\n").arg(successCount);
         for (const auto& error : failedRows) {
            msg += QString::fromStdString(error) + "\n";
         }
         QMessageBox::warning(this, "Partial Success", msg);
      } else {
         QMessageBox::critical(this, "Error", "No rows were inserted");
      }

   } catch (const std::exception& e) {
      QMessageBox::critical(this, "Insert Error", QString("Failed to insert data: %1").arg(e.what()));
   }
}
