#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

class main_window : public QMainWindow
{
    Q_OBJECT
public:
    explicit main_window(QWidget *parent = nullptr);
    ~main_window();

    QSize sizeHint() const;

signals:

private:
    QAction* quitAction_;
};

#endif // MAIN_WINDOW_H
