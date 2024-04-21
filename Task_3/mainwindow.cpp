#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Input_line_textChanged(const QString &text)
{
    result.clear();

    if(text != ""){
        QString temp = text;
        QChar last_symbol = text[text.length() - 1];
        if(!last_symbol.isDigit()){
            QMessageBox* box = new QMessageBox();
            box -> setText("Введите ряд чисел");
            box -> exec();
            temp.remove(last_symbol);
            ui -> Input_line -> setText(temp);
        }
        else{
            GetMedianVector(temp);
            ui -> Output_line -> setText(result);
        }
    }
    else{

        ui -> Output_line -> setText("");
    }
}

void MainWindow::GetMedianVector(QString &text){
    if(text.length() == 0){
        return;
    }
    if(text.length() == 1){
        result += text;
        return;
    }
    else if(text.length() == 2){
        int number = text.toInt();
        int first_symbol, second_symbol;

        second_symbol = number % 10;
        number /= 10;
        first_symbol = number;

        result += QString::number((first_symbol + second_symbol) / 2);
        return;
    }
    else{
        QString first_3_symbols_of_text = text.mid(0,3);
        text.remove(0,3);

        int* array = new int[3];

        int number = first_3_symbols_of_text.toInt();

        array[2] = number % 10;
        number /= 10;

        array[1] = number % 10;
        number /= 10;

        array[0] = number;

        for(int i = 0; i < 3; ++i){
            for(int j = 0; j < 2 - i; ++j){
                if(array[j] > array[j + 1]){
                    int temp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = temp;
                }
            }
        }

        result+= QString::number(array[1]);
        GetMedianVector(text);
    }

}
