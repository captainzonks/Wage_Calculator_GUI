#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "data.h"

QT_BEGIN_NAMESPACE
namespace Ui { class WageCalculator; }
QT_END_NAMESPACE

const float FEDERAL_TAX{0.153};
const float CO_STATE_TAX{0.0455};
const float FICA_STATE_INS_TAX{0.0765};

class WageCalculator : public QWidget
{
    Q_OBJECT

public:
    WageCalculator(QWidget *parent = nullptr);
    ~WageCalculator();

private slots:
    void calculate_wage();
    void calculate_wage_after_tax();
    void calculate_average_wage();
    void calculate_average_wage_post_tax();
    void wage_changed(double wage);
    void hours_changed(double hours);
    void tips_changed(double tips);

signals:
    void adjusted_wage_changed(float wage);
    void adjusted_wage_post_tax_changed(float post_tax_wage);
    void average_adjusted_wage_changed(float average_adjusted_wage);
    void average_adjusted_wage_post_tax_changed(float average_adjusted_wage_post_tax);

private:
    void initialize_data();

    Ui::WageCalculator *ui;

    class data data_;

    float adjusted_wage_{};
    float adjusted_wage_post_tax_{};
    float average_adjusted_wage_{};
    float average_adjusted_wage_post_tax_{};
    float wage_{};
    float hours_{};
    float tips_{};
    float total_hours_{};
    float total_tips_{};
};
#endif // WIDGET_H
