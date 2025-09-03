#include "MainWindow.hpp"
#include <QApplication>
#include <QPalette>
#include <QStyleFactory>
#include <iostream>

int main(int argc, char* argv[]) {
   QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
   QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
   QApplication app(argc, argv);
   QCoreApplication::setOrganizationName("Davison Boys");
   QCoreApplication::setApplicationName("PgPool Manager");
   QCoreApplication::setApplicationVersion("1.0.0");

   QFont font = app.font();
   font.setPointSize(12);
   app.setFont(font);
   app.setStyle(QStyleFactory::create("Fusion"));

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

   app.setPalette(darkPalette);

   app.setStyleSheet(R"(
    QPushButton {
        background-color: #2d2d2d;
        border: 1px solid #3d3d3d;
        border-radius: 6px;
        padding: 8px 16px;
        color: #e0e0e0;
        font-weight: bold;
        font-size: 11pt;
    }
    QPushButton:hover {
        background-color: #00d4aa;
        color: #1a1a1a;
        border: 1px solid #00ffcc;
    }
    QPushButton:pressed {
        background-color: #00a080;
    }
    QPushButton:disabled {
        background-color: #1a1a1a;
        color: #4d4d4d;
        border: 1px solid #2d2d2d;
    }
    QLineEdit, QSpinBox {
        background-color: #1a1a1a;
        border: 1px solid #3d3d3d;
        border-radius: 4px;
        padding: 6px;
        color: #e0e0e0;
        font-size: 11pt;
    }
    QLineEdit:focus, QSpinBox:focus {
        border: 2px solid #00d4aa;
    }
    QComboBox {
        background-color: #1a1a1a;
        border: 1px solid #3d3d3d;
        border-radius: 4px;
        padding: 6px;
        color: #e0e0e0;
        font-size: 11pt;
    }
    QComboBox:hover {
        border: 1px solid #00d4aa;
    }
    QComboBox::drop-down {
        border: none;
        padding-right: 4px;
    }
    QComboBox::down-arrow {
        image: none;
        border-left: 4px solid transparent;
        border-right: 4px solid transparent;
        border-top: 6px solid #00d4aa;
        margin-right: 8px;
    }
    QLabel {
        color: #a0a0a0;
        font-size: 11pt;
    }
)");

   MainWindow window;
   window.show();

   return app.exec();
}
