#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "DatabaseManager.hpp"
#include <QComboBox>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <memory>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QSpinBox;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
   Q_OBJECT

 public:
   explicit MainWindow(QWidget* parent = nullptr);
   ~MainWindow();

 private slots:
   void onConnectDatabase();
   void onExecuteQuery();
   void onRefreshTables();
   void onTableSelectionChanged();
   void onTestConnectionPool();
   void onInsertData();
   void updateConnectionStatus(bool connected);

 private:
   void setupUI();
   void createMenuBar();
   void displayResults(const std::vector<std::vector<std::string>>& results, const std::vector<std::string>& columns);

   // UI Elements
   QLineEdit* m_hostEdit;
   QLineEdit* m_portEdit;
   QLineEdit* m_dbNameEdit;
   QLineEdit* m_userEdit;
   QLineEdit* m_passwordEdit;
   QSpinBox*  m_poolSizeSpinBox;

   QPushButton* m_connectBtn;
   QPushButton* m_executeBtn;
   QPushButton* m_refreshBtn;
   QPushButton* m_testPoolBtn;
   QPushButton* m_insertBtn;

   QComboBox*    m_tableCombo;
   QTextEdit*    m_queryEdit;
   QTableWidget* m_resultsTable;
   QTextEdit*    m_logOutput;
   QLabel*       m_statusLabel;

   // Database components
   std::unique_ptr<DatabaseManager> m_dbManager;
   bool                             m_isConnected;
};

#endif // MAINWINDOW_HPP
