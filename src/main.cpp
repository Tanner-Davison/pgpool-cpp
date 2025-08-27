#include "MainWindow.hpp"
#include <QApplication>
#include <iostream>

int main(int argc, char* argv[]) {
   QApplication app(argc, argv);

   // Set application metadata
   QCoreApplication::setOrganizationName("YourCompany");
   QCoreApplication::setApplicationName("PgPool Manager");
   QCoreApplication::setApplicationVersion("1.0.0");

   // Optional: Set application style
   app.setStyle("Fusion");

   // Create and show main window
   MainWindow window;
   window.show();

   return app.exec();
}
