#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButtonLogin_clicked();

    void on_pushButton_info_clicked();

    void on_pushButton_buy_clicked();

    void on_pushButton_hist_clicked();

    void on_pushButton_turn_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
