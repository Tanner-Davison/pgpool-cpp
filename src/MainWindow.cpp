#include "MainWindow.hpp"
#include "InsertDialog.hpp"
#include <QAction>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSpinBox>
#include <QSplitter>
#include <QStatusBar>
#include <QVBoxLayout>
#include <chrono>
#include <sstream>
#include <thread>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), m_isConnected(false) {
   setupUI();
   createMenuBar();

   // Set default connection parameters
   m_hostEdit->setText("localhost");
   m_portEdit->setText("5432");
   m_dbNameEdit->setText("testdb");
   m_userEdit->setText("postgres");
   m_poolSizeSpinBox->setValue(5);

   updateConnectionStatus(false);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
   auto* centralWidget = new QWidget(this);
   setCentralWidget(centralWidget);

   auto* mainLayout = new QVBoxLayout(centralWidget);

   // Connection Group
   auto* connGroup  = new QGroupBox("Database Connection", this);
   auto* connLayout = new QGridLayout(connGroup);

   connLayout->addWidget(new QLabel("Host:"), 0, 0);
   m_hostEdit = new QLineEdit(this);
   connLayout->addWidget(m_hostEdit, 0, 1);

   connLayout->addWidget(new QLabel("Port:"), 0, 2);
   m_portEdit = new QLineEdit(this);
   m_portEdit->setMaximumWidth(80);
   connLayout->addWidget(m_portEdit, 0, 3);

   connLayout->addWidget(new QLabel("Database:"), 1, 0);
   m_dbNameEdit = new QLineEdit(this);
   connLayout->addWidget(m_dbNameEdit, 1, 1);

   connLayout->addWidget(new QLabel("User:"), 1, 2);
   m_userEdit = new QLineEdit(this);
   connLayout->addWidget(m_userEdit, 1, 3);

   connLayout->addWidget(new QLabel("Password:"), 2, 0);
   m_passwordEdit = new QLineEdit(this);
   m_passwordEdit->setEchoMode(QLineEdit::Password);
   connLayout->addWidget(m_passwordEdit, 2, 1);

   connLayout->addWidget(new QLabel("Pool Size:"), 2, 2);
   m_poolSizeSpinBox = new QSpinBox(this);
   m_poolSizeSpinBox->setRange(1, 50);
   m_poolSizeSpinBox->setValue(5);
   connLayout->addWidget(m_poolSizeSpinBox, 2, 3);

   m_connectBtn = new QPushButton("Connect", this);
   connLayout->addWidget(m_connectBtn, 3, 0, 1, 2);

   m_testPoolBtn = new QPushButton("Test Connection Pool", this);
   m_testPoolBtn->setEnabled(false);
   connLayout->addWidget(m_testPoolBtn, 3, 2, 1, 2);

   mainLayout->addWidget(connGroup);

   // Query Group
   auto* queryGroup  = new QGroupBox("Query Execution", this);
   auto* queryLayout = new QVBoxLayout(queryGroup);

   auto* tableLayout = new QHBoxLayout();
   tableLayout->addWidget(new QLabel("Tables:"));
   m_tableCombo = new QComboBox(this);
   tableLayout->addWidget(m_tableCombo);
   m_refreshBtn = new QPushButton("Refresh Tables", this);
   m_refreshBtn->setEnabled(false);
   tableLayout->addWidget(m_refreshBtn);
   m_insertBtn = new QPushButton("Insert Data", this);
   m_insertBtn->setEnabled(false);
   tableLayout->addWidget(m_insertBtn);
   tableLayout->addStretch();
   queryLayout->addLayout(tableLayout);

   m_queryEdit = new QTextEdit(this);
   m_queryEdit->setPlaceholderText("Enter SQL query here...");
   m_queryEdit->setMaximumHeight(100);
   queryLayout->addWidget(m_queryEdit);

   m_executeBtn = new QPushButton("Execute Query", this);
   m_executeBtn->setEnabled(false);
   queryLayout->addWidget(m_executeBtn);

   mainLayout->addWidget(queryGroup);

   // Results and Log Splitter
   auto* splitter = new QSplitter(Qt::Vertical, this);

   // Results Table
   auto* resultsGroup  = new QGroupBox("Results", this);
   auto* resultsLayout = new QVBoxLayout(resultsGroup);
   m_resultsTable      = new QTableWidget(this);
   m_resultsTable->horizontalHeader()->setStretchLastSection(true);
   resultsLayout->addWidget(m_resultsTable);
   splitter->addWidget(resultsGroup);

   // Log Output
   auto* logGroup  = new QGroupBox("Log Output", this);
   auto* logLayout = new QVBoxLayout(logGroup);
   m_logOutput     = new QTextEdit(this);
   m_logOutput->setReadOnly(true);
   m_logOutput->setMaximumHeight(150);
   logLayout->addWidget(m_logOutput);
   splitter->addWidget(logGroup);

   mainLayout->addWidget(splitter);

   // Status Bar
   m_statusLabel = new QLabel("Disconnected", this);
   statusBar()->addPermanentWidget(m_statusLabel);

   // Connect signals
   connect(m_connectBtn, &QPushButton::clicked, this, &MainWindow::onConnectDatabase);
   connect(m_executeBtn, &QPushButton::clicked, this, &MainWindow::onExecuteQuery);
   connect(m_refreshBtn, &QPushButton::clicked, this, &MainWindow::onRefreshTables);
   connect(m_insertBtn, &QPushButton::clicked, this, &MainWindow::onInsertData);
   connect(m_testPoolBtn, &QPushButton::clicked, this, &MainWindow::onTestConnectionPool);
   connect(m_tableCombo, &QComboBox::currentTextChanged, this, &MainWindow::onTableSelectionChanged);

   setWindowTitle("PostgreSQL Connection Pool Manager");
   resize(800, 600);
}

void MainWindow::createMenuBar() {
   auto* fileMenu = menuBar()->addMenu("&File");

   auto* connectAction = new QAction("&Connect to Database", this);
   connectAction->setShortcut(QKeySequence("Ctrl+D"));
   connect(connectAction, &QAction::triggered, this, &MainWindow::onConnectDatabase);
   fileMenu->addAction(connectAction);

   fileMenu->addSeparator();

   auto* exitAction = new QAction("E&xit", this);
   exitAction->setShortcut(QKeySequence("Ctrl+Q"));
   connect(exitAction, &QAction::triggered, this, &QWidget::close);
   fileMenu->addAction(exitAction);

   auto* helpMenu    = menuBar()->addMenu("&Help");
   auto* aboutAction = new QAction("&About", this);
   connect(aboutAction, &QAction::triggered, [this]() {
      QMessageBox::about(this,
                         "About",
                         "PostgreSQL Connection Pool Manager\n\n"
                         "Built with Qt6 and libpqxx\n"
                         "Manages database connections efficiently");
   });
   helpMenu->addAction(aboutAction);
}

void MainWindow::onConnectDatabase() {
   try {
      if (m_isConnected) {
         // Disconnect
         m_dbManager.reset();
         updateConnectionStatus(false);
         m_logOutput->append("Disconnected from database.");
         m_tableCombo->clear();
         m_resultsTable->clear();
         return;
      }

      // Build connection parameters
      std::string host     = m_hostEdit->text().toStdString();
      std::string dbname   = m_dbNameEdit->text().toStdString();
      std::string user     = m_userEdit->text().toStdString();
      std::string password = m_passwordEdit->text().toStdString();

      // Initialize database manager with connection parameters
      m_dbManager = std::make_unique<DatabaseManager>(password,                      // password
                                                      host,                          // host
                                                      dbname,                        // dbname
                                                      user,                          // user
                                                      m_poolSizeSpinBox->value(),    // min_connections
                                                      m_poolSizeSpinBox->value() * 2 // max_connections
      );

      // Test the connection
      m_dbManager->testConnection();

      updateConnectionStatus(true);
      m_logOutput->append(
          QString("Connected to database successfully with pool size: %1").arg(m_poolSizeSpinBox->value()));

      onRefreshTables();

   } catch (const std::exception& e) {
      QMessageBox::critical(this, "Connection Error", QString("Failed to connect: %1").arg(e.what()));
      m_logOutput->append(QString("Error: %1").arg(e.what()));
   }
}

void MainWindow::onExecuteQuery() {
   if (!m_isConnected || !m_dbManager)
      return;

   QString query = m_queryEdit->toPlainText();
   if (query.isEmpty()) {
      QMessageBox::warning(this, "Warning", "Please enter a query");
      return;
   }

   try {
      auto start = std::chrono::high_resolution_clock::now();

      // Check if it's a SELECT query
      if (query.trimmed().startsWith("SELECT", Qt::CaseInsensitive)) {
         // Use QueryExecutor's select method
         pqxx::result result = m_dbManager->query().select(query.toStdString());

         auto end      = std::chrono::high_resolution_clock::now();
         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

         // Process results - using indices for column names
         std::vector<std::string> columns;
         for (size_t i = 0; i < result.columns(); ++i) {
            columns.push_back(result.column_name(i));
         }

         std::vector<std::vector<std::string>> rows;
         for (auto const& row : result) {
            std::vector<std::string> rowData;
            for (auto const& field : row) {
               rowData.push_back(field.is_null() ? "NULL" : field.as<std::string>());
            }
            rows.push_back(rowData);
         }

         displayResults(rows, columns);
         m_logOutput->append(
             QString("Query executed successfully in %1 ms. Rows returned: %2").arg(duration).arg(result.size()));
      } else if (query.trimmed().startsWith("INSERT", Qt::CaseInsensitive)) {
         // For INSERT queries, parse and use DataModifier
         // This is a simplified approach - you might want more robust parsing
         m_logOutput->append("Use the Data Modifier interface for INSERT operations");
      } else if (query.trimmed().startsWith("UPDATE", Qt::CaseInsensitive)) {
         // For UPDATE queries
         m_logOutput->append("Use the Data Modifier interface for UPDATE operations");
      } else if (query.trimmed().startsWith("CREATE TABLE", Qt::CaseInsensitive)) {
         // For CREATE TABLE queries
         m_logOutput->append("Use the Table Creator interface for CREATE TABLE operations");
      } else if (query.trimmed().startsWith("DROP TABLE", Qt::CaseInsensitive)) {
         // For DROP TABLE queries
         m_logOutput->append("Use the Table Creator interface for DROP TABLE operations");
      } else {
         // For other queries, try as SELECT
         pqxx::result result   = m_dbManager->query().select(query.toStdString());
         auto         end      = std::chrono::high_resolution_clock::now();
         auto         duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
         m_logOutput->append(QString("Query executed successfully in %1 ms").arg(duration));
      }

   } catch (const std::exception& e) {
      QMessageBox::critical(this, "Query Error", QString("Failed to execute query: %1").arg(e.what()));
      m_logOutput->append(QString("Query Error: %1").arg(e.what()));
   }
}

void MainWindow::onRefreshTables() {
   if (!m_isConnected || !m_dbManager)
      return;

   try {
      // Use QueryExecutor to get table list from information_schema
      std::string query =
          "SELECT table_name FROM information_schema.tables "
          "WHERE table_schema = 'public' ORDER BY table_name";

      pqxx::result result = m_dbManager->query().select(query);

      m_tableCombo->clear();
      m_tableCombo->addItem("-- Select Table --");

      for (auto const& row : result) {
         m_tableCombo->addItem(QString::fromStdString(row[0].as<std::string>()));
      }

      m_logOutput->append(QString("Found %1 tables").arg(result.size()));

   } catch (const std::exception& e) {
      m_logOutput->append(QString("Error refreshing tables: %1").arg(e.what()));
   }
}

void MainWindow::onTableSelectionChanged() {
   if (m_tableCombo->currentIndex() <= 0)
      return;

   QString tableName = m_tableCombo->currentText();
   QString query     = QString("SELECT * FROM %1 LIMIT 100").arg(tableName);
   m_queryEdit->setPlainText(query);
}

void MainWindow::onTestConnectionPool() {
   if (!m_isConnected || !m_dbManager)
      return;

   m_logOutput->append("Starting connection pool test...");

   try {
      // Use DatabaseManager's pool stats
      m_dbManager->printPoolStats();

      // Test the connection
      m_dbManager->testConnection();

      m_logOutput->append("Connection pool test completed successfully!");

   } catch (const std::exception& e) {
      m_logOutput->append(QString("Pool test error: %1").arg(e.what()));
   }
}

void MainWindow::updateConnectionStatus(bool connected) {
   m_isConnected = connected;
   m_connectBtn->setText(connected ? "Disconnect" : "Connect");
   m_executeBtn->setEnabled(connected);
   m_refreshBtn->setEnabled(connected);
   m_insertBtn->setEnabled(connected);
   m_testPoolBtn->setEnabled(connected);
   m_statusLabel->setText(connected ? "Connected" : "Disconnected");

   // Update UI state
   m_hostEdit->setEnabled(!connected);
   m_portEdit->setEnabled(!connected);
   m_dbNameEdit->setEnabled(!connected);
   m_userEdit->setEnabled(!connected);
   m_passwordEdit->setEnabled(!connected);
   m_poolSizeSpinBox->setEnabled(!connected);
}

void MainWindow::displayResults(const std::vector<std::vector<std::string>>& results,
                                const std::vector<std::string>&              columns) {
   m_resultsTable->clear();
   m_resultsTable->setColumnCount(columns.size());
   m_resultsTable->setRowCount(results.size());

   // Set headers
   QStringList headers;
   for (const auto& col : columns) {
      headers << QString::fromStdString(col);
   }
   m_resultsTable->setHorizontalHeaderLabels(headers);

   // Populate data
   for (size_t row = 0; row < results.size(); ++row) {
      for (size_t col = 0; col < results[row].size(); ++col) {
         auto* item = new QTableWidgetItem(QString::fromStdString(results[row][col]));
         m_resultsTable->setItem(row, col, item);
      }
   }

   m_resultsTable->resizeColumnsToContents();
}

void MainWindow::onInsertData() {
   if (!m_isConnected || !m_dbManager)
      return;

   InsertDialog dialog(m_dbManager.get(), this);

   connect(&dialog, &InsertDialog::dataInserted, [this]() {
      onRefreshTables();
      m_logOutput->append("Data inserted successfully");
   });

   dialog.exec();
}
#include "moc_MainWindow.cpp"
