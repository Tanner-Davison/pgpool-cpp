#ifndef INSERTDIALOG_HPP
#define INSERTDIALOG_HPP

#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <string>
#include <vector>

class DatabaseManager;

class InsertDialog : public QDialog {
   Q_OBJECT

 public:
   explicit InsertDialog(DatabaseManager* dbManager, QWidget* parent = nullptr);

 signals:
   void dataInserted();

 private slots:
   void onTableChanged(const QString& tableName);
   void onAddRow();
   void onRemoveRow();
   void onInsertData();
   void refreshTables();

 private:
   void setupUI();
   void fetchTableColumns(const std::string& tableName);

   DatabaseManager* m_dbManager;
   QComboBox*       m_tableCombo;
   QTableWidget*    m_dataTable;
   QPushButton*     m_addRowBtn;
   QPushButton*     m_removeRowBtn;
   QPushButton*     m_insertBtn;
   QPushButton*     m_cancelBtn;

   std::vector<std::string> m_currentColumns;
   std::vector<std::string> m_columnTypes;
};

#endif // INSERTDIALOG_HPP
