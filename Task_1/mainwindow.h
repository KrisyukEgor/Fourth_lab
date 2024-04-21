#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIntValidator>
#include <QButtonGroup>
#include <QRandomGenerator>
#include <QMessageBox>

#include <QTimer>

#include "rectangle.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:

private slots:
    void on_Array_size_line_textChanged(const QString &arg1);


    void on_Sort_button_clicked();

    void on_Create_new_columns_button_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    QIntValidator* val;
    QButtonGroup* all_radio_buttons;

    QTimer* timer;
    int* current_array;
    int current_array_length;

    int time;

    void Array_input(int);
    void Array_to_scene(int);

    QList<Rectangle*> Rect_list;
    QList<QPair<int, int>> Swap_index_array;

    void QuickSort(int* &, int , int);

    void MergeSort(int* &, int , int);
    void Merge(int* &, int, int ,int);

    void Swap_array_elements(int&, int&);

    void Swap_rectangles(int, int);

    void Debug_array();

    void onTimerTimeout();

    void Set_buttons_enabled(bool);

    int count_of_iteration;
    int count_time_iteration;

};
#endif // MAINWINDOW_H
