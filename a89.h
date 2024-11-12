#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_a89.h"
#include <qwidget.h>
#include "Console.h"
#include <qlistwidget.h>
#include <qlineedit.h>
#include <qpushbutton.h>

class a89 : public QMainWindow
{
    Q_OBJECT

public:
    a89(QWidget *parent = nullptr);
    ~a89();

private:
    Ui::a89Class ui;
};
