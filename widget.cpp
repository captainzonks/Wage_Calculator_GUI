#include "widget.h"
#include "./ui_widget.h"

#include <fstream>
#include <sys/stat.h>

WageCalculator::WageCalculator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WageCalculator)
{
    ui->setupUi(this);

    initialize_data();

    ui->hourly_wage_spinbox->setValue(wage_);

    connect(ui->hourly_wage_spinbox, &QDoubleSpinBox::valueChanged, this, &WageCalculator::wage_changed);
    connect(ui->hours_worked_spinbox, &QDoubleSpinBox::valueChanged, this, &WageCalculator::hours_changed);
    connect(ui->tips_earned_spinbox, &QDoubleSpinBox::valueChanged, this, &WageCalculator::tips_changed);

    connect(this, &WageCalculator::adjusted_wage_changed, this, [=](){
        QString new_value = QString("$%1").arg(adjusted_wage_, 4, 'f', 2);
        ui->adjusted_wage_value->setText(new_value);
    });
    connect(this, &WageCalculator::adjusted_wage_post_tax_changed, this, [=]() {
        QString new_value = QString("$%1").arg(adjusted_wage_post_tax_, 4, 'f', 2);
        ui->after_tax_value->setText(new_value);
    });
    connect(this, &WageCalculator::average_adjusted_wage_changed, this, [=]() {
        QString new_value = QString("$%1").arg(average_adjusted_wage_, 4, 'f', 2);
        ui->average_wage_value->setText(new_value);
    });
    connect(this, &WageCalculator::average_adjusted_wage_post_tax_changed, this, [=]() {
        QString new_value = QString("$%1").arg(average_adjusted_wage_post_tax_, 4, 'f', 2);
        ui->average_after_tax_value->setText(new_value);
    });

    calculate_average_wage();

    // save data / overwrite file
    connect(ui->save_button, &QPushButton::clicked, this, [=](){
        data_.set_wage(wage_);
        data_.add_hours(hours_);
        data_.add_tips(tips_);

        data_.set_today_hours(hours_);
        data_.set_today_tips(tips_);

        std::fstream file("wage_data", std::fstream::in | std::fstream::out);

        file << data_;

        file.close();

        initialize_data();
        calculate_average_wage();
    });
}

WageCalculator::~WageCalculator()
{
    delete ui;
}

void WageCalculator::calculate_wage()
{
    adjusted_wage_ = wage_ + tips_ / hours_;
    calculate_wage_after_tax();
    emit adjusted_wage_changed(adjusted_wage_);
}

void WageCalculator::calculate_wage_after_tax()
{
    adjusted_wage_post_tax_ = adjusted_wage_ -
            (adjusted_wage_ * FEDERAL_TAX) -
            (adjusted_wage_ * CO_STATE_TAX) -
            (adjusted_wage_ * FICA_STATE_INS_TAX);
    emit adjusted_wage_post_tax_changed(adjusted_wage_post_tax_);
}

void WageCalculator::calculate_average_wage()
{
    average_adjusted_wage_ = wage_ + total_tips_ / total_hours_;
    calculate_average_wage_post_tax();
    emit average_adjusted_wage_changed(average_adjusted_wage_);
}

void WageCalculator::calculate_average_wage_post_tax()
{
    average_adjusted_wage_post_tax_ = average_adjusted_wage_ -
            (average_adjusted_wage_ * FEDERAL_TAX) -
            (average_adjusted_wage_ * CO_STATE_TAX) -
            (average_adjusted_wage_ * FICA_STATE_INS_TAX);
    emit average_adjusted_wage_post_tax_changed(average_adjusted_wage_post_tax_);
}

void WageCalculator::wage_changed(double wage)
{
    this->wage_ = wage;
    calculate_wage();
}

void WageCalculator::hours_changed(double hours)
{
    this->hours_ = hours;
    calculate_wage();
}

void WageCalculator::tips_changed(double tips)
{
    this->tips_ = tips;
    calculate_wage();
}

void WageCalculator::initialize_data()
{
    bool exists{ false };

    // Check if file exists, if not, ask to initialize wage
    struct stat buffer{};
    if (stat("wage_data", &buffer)) {
    } else {
        exists = true;
        qDebug() << "File exists";
    }

    std::fstream file;

    file.open("wage_data", std::fstream::in | std::fstream::out | std::fstream::app);

    // Retrieve data from the file
    if (file) {
        if (exists) {
            file >> data_;
            qDebug() << "Imported data.";
        }
    } else {
        qDebug() << "Error opening file";
        file.close();
    }

    file.close();

    wage_ = data_.get_wage();
    hours_ = ui->hours_worked_spinbox->value();
    tips_ = ui->tips_earned_spinbox->value();
    total_hours_ = data_.get_total_hours();
    total_tips_ = data_.get_total_tips();
}
