#include "main_window.h"
#include "qapplication.h"
#include "qmenubar.h"
#include "widget.h"

main_window::main_window(QWidget *parent)
    : QMainWindow{parent}
{
    // Add central widget
    WageCalculator* wage_calculator = new WageCalculator(this);
    setCentralWidget(wage_calculator);

    // Declare quit action
    quitAction_ = new QAction("Quit");

    connect(quitAction_, &QAction::triggered, [=](){
        QApplication::quit();
    });

    // Add menus
    QMenu* fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction(quitAction_);
}

main_window::~main_window()
{
    delete quitAction_;
}

QSize main_window::sizeHint() const
{
    return QSize(800, 500);
}
