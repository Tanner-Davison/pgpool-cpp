#include "MainWindow.hpp"
#include <QApplication>
#include <QPalette>
#include <QStyleFactory>
#include <iostream>

int main(int argc, char* argv[]) {
   QApplication app(argc, argv);

   // Set application metadata
   QCoreApplication::setOrganizationName("Davison Boys");
   QCoreApplication::setApplicationName("PgPool Manager");
   QCoreApplication::setApplicationVersion("1.0.0");

   // Set application style to Fusion for better cross-platform theming
   app.setStyle(QStyleFactory::create("Fusion"));

   // Create and apply dark theme
   QPalette darkPalette;
   darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
   darkPalette.setColor(QPalette::WindowText, Qt::white);
   darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
   darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
   darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
   darkPalette.setColor(QPalette::ToolTipText, Qt::white);
   darkPalette.setColor(QPalette::Text, Qt::white);
   darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
   darkPalette.setColor(QPalette::ButtonText, Qt::white);
   darkPalette.setColor(QPalette::BrightText, Qt::red);
   darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
   darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
   darkPalette.setColor(QPalette::HighlightedText, Qt::black);

   // Apply the palette to the application
   app.setPalette(darkPalette);

   // Optional: Add some custom styling with stylesheets
   app.setStyleSheet(R"(
        QPushButton {
            border: 1px solid #555555;
            border-radius: 4px;
            padding: 6px 12px;
            background-color: #404040;
        }
        QPushButton:hover {
            background-color: #505050;
        }
        QPushButton:pressed {
            background-color: #303030;
        }
        QPushButton:checked {
            background-color: #4CAF50;
            border: 2px solid #45a049;
            color: white;
        }
        QPushButton:checked:hover {
            background-color: #45a049;
        }
        QTableWidget {
            gridline-color: #555555;
            selection-background-color: #42a5f5;
        }
        QHeaderView::section {
            background-color: #404040;
            border: 1px solid #555555;
            padding: 4px;
        }
        QLineEdit, QComboBox {
            border: 1px solid #555555;
            border-radius: 3px;
            padding: 4px;
            background-color: #404040;
        }
        QLineEdit:focus, QComboBox:focus {
            border: 2px solid #42a5f5;
        }
    )");

   // Create and show main window
   MainWindow window;
   window.show();

   return app.exec();
}
