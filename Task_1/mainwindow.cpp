#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene();
    ui -> graphicsView -> setScene(scene);

    all_radio_buttons = new QButtonGroup();
    all_radio_buttons -> addButton(ui -> Heap_sort_button);
    all_radio_buttons -> addButton(ui -> Merge_sort_button);
    all_radio_buttons -> addButton(ui -> Quick_sort_button);

    val = new QIntValidator(0,99);

    ui -> Array_size_line -> setValidator(val);

    scene -> setBackgroundBrush(Qt::black);

    time = 40;
    current_array_length = 0;
    count_of_iteration = 0;
    count_time_iteration = 0;

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Array_size_line_textChanged(const QString &arg1)
{
    QString text = ui -> Array_size_line -> text();

    scene -> clear();
    Rect_list.clear();

    if(text != "") {

        if(text.toInt() > 99){
            ui -> Array_size_line -> setText(QString::number(99));
        }

        else if(text.toInt() == 0){
            ui -> Array_size_line -> setText(QString::number(1));
        }

        ui -> Sort_button -> setEnabled(true);
        ui -> Create_new_columns_button -> setEnabled(true);

        current_array_length = ui -> Array_size_line -> text().toInt();
        current_array = new int[current_array_length];

        scene -> clear();
        Array_input(current_array_length);


        Array_to_scene(current_array_length);

    }

    else{

        ui -> Sort_button -> setEnabled(false);
    }
}


void MainWindow::Array_input(int current_array_length){

    for(int i = 0; i < current_array_length; ++i){
        current_array[i] = QRandomGenerator::global() -> bounded(5,100);
    }
}
void MainWindow::Array_to_scene(int current_array_length){

    Rectangle* rect;

    Rect_list.clear();

    qreal min_pos = 0;
    qreal max_pos = 500;
    qreal d;

    if(current_array_length == 1){
        d = 10;
    }
    else{
        d = (max_pos - min_pos) / (current_array_length - 1);
    }
    if(d > 10){
        d = 10;
    }
    qreal current_pos = 0;

    QBrush brush(QColor(255, 255, 255));

    for(int i = 0; i < current_array_length; ++i){
        qreal height = current_array[i] * 3;

        rect = new Rectangle(current_pos, -height, d, height, nullptr);

        rect -> setBrush(brush);

        current_pos += d;

        Rect_list.append(rect);

        scene -> addItem(rect);

    }
}

void MainWindow::on_Sort_button_clicked()
{
    Set_buttons_enabled(false);

    if(ui -> Heap_sort_button -> isChecked()){

    }
    else if(ui -> Merge_sort_button -> isChecked()){

        MergeSort(current_array, 0, current_array_length - 1);

        QTimer* timer_to_next_swap = new QTimer(this);
        timer_to_next_swap -> setInterval(2 * time);

        for(int i = 0; i < Swap_index_array.length(); ++i){
            qDebug() << "pair" << Swap_index_array[i].first << Swap_index_array[i].second;
        }

        int index = 0;
        connect (timer_to_next_swap, &QTimer::timeout, this , [=]() mutable{
            qDebug() << index;

            if(index < Swap_index_array.length()){
                int first = Swap_index_array[index].first;
                int second = Swap_index_array[index].second;

                if(first != second){
                    Swap_rectangles(first, second);

                }
                index++;
            }
            else{

                timer_to_next_swap -> stop();
            }
        });

        timer_to_next_swap -> start();
    }
    else if(ui -> Quick_sort_button -> isChecked()){

        QuickSort(current_array, 0 , current_array_length - 1);

        QTimer* timer_to_enable = new QTimer(this);

        int time_to_enable = current_array_length;
        if(current_array_length > 90){
            time_to_enable = 90;
        }
        timer_to_enable -> setInterval(2 * time * (time_to_enable));

        connect (timer_to_enable, &QTimer::timeout, this , [=]() mutable{

            Set_buttons_enabled(true);

            timer_to_enable -> stop();
        });
        timer_to_enable -> start();


    }
    else{
        QMessageBox* box = new QMessageBox();
        box -> setText("Выберите сортировку");

        Set_buttons_enabled(true);

        box -> exec();
    }
}

void MainWindow::QuickSort(int* &array, int min_index, int max_index){
    int left_index = min_index;
    int right_index = max_index;

    int pivot = array[(min_index + max_index) / 2];

    int pivot_index = (left_index + right_index) / 2;

    count_of_iteration++;

    Rect_list[pivot_index] -> SetColor(QColor(13, 227, 71));

    QTimer* timer_to_next_generation = new QTimer(this);

    timer_to_next_generation -> setInterval(2 * time);

    connect(timer_to_next_generation, &QTimer::timeout, this, [=] () mutable {

        while(array[left_index] < pivot){
            left_index++;

        }

        while(array[right_index] > pivot){
            right_index--;
        }

        if(left_index <= right_index){

            if(left_index != right_index){

                Swap_rectangles(left_index, right_index);

                Swap_array_elements(array[left_index], array[right_index]);

            }

            left_index++;
            right_index--;

        }

        else{
            timer_to_next_generation -> stop();

            Rect_list[pivot_index] -> SetColor(QColor(255, 255, 255));


            if(min_index < right_index){
                QuickSort(array, min_index, right_index);
            }

            if(max_index > left_index){
                QuickSort(array, left_index, max_index);
            }

        }


    });
    timer_to_next_generation -> start();

}

void MainWindow::MergeSort(int* &array, int left, int right) {

    if (left < right) {
        int middle = (right + left) / 2;

        MergeSort(array, left, middle);
        MergeSort(array, middle + 1, right);

        Merge(array, left, middle, right);
    }

}
void MainWindow::Merge(int* &array, int left, int middle, int right) {

    int left_array_size = middle - left + 1;
    int right_array_size = right - middle;

    int* left_array = new int[left_array_size];
    int* right_array = new int[right_array_size];

    for (int i = 0; i < left_array_size; i++)
        left_array[i] = array[left + i];
    for (int i = 0; i < right_array_size; i++)
        right_array[i] = array[middle + 1 + i];

    int left_array_index = 0;
    int right_array_index = 0;
    int array_current_index = left;

    while (left_array_index < left_array_size && right_array_index < right_array_size) {

        if (left_array[left_array_index] <= right_array[right_array_index]) {

            if(array[array_current_index] != left_array[left_array_index]){

                qDebug() << "left";
                qDebug() << array[array_current_index] << array_current_index;
                qDebug() << left_array[left_array_index] << left_array_index << '\n';

                array[array_current_index] = left_array[left_array_index];

                QPair<int , int> pair_index;
                pair_index.first = array_current_index;
                pair_index.second = left_array_index;

                Swap_index_array.append(pair_index);
            }

            left_array_index++;
        }
        else {

            if(array[array_current_index] != right_array[right_array_index]){

                qDebug() << "right";
                qDebug() << array[array_current_index] << array_current_index;
                qDebug() << right_array[left_array_index] << right_array_index << right_array_index + middle << '\n';

                array[array_current_index] = right_array[right_array_index];

                QPair<int , int> pair_index;
                pair_index.first = array_current_index;
                pair_index.second = right_array_index + middle;

                Swap_index_array.append(pair_index);
            }

            right_array_index++;
        }
        array_current_index++;
    }

    QPair<int , int> pair_index;

    while (left_array_index < left_array_size) {

        if(array[array_current_index] != left_array[left_array_index]){

            array[array_current_index] = left_array[left_array_index];

//            pair_index.first = array_current_index;
//            pair_index.second = left_array_index;

//            Swap_index_array.append(pair_index);
        }

        left_array_index++;
        array_current_index++;
    }

    while (right_array_index < right_array_size) {

        if(array[array_current_index] != right_array[right_array_index]){

            array[array_current_index] = right_array[right_array_index];

//            pair_index.first = array_current_index;
//            pair_index.second = middle + right_array_index;

//            Swap_index_array.append(pair_index);
        }

        right_array_index++;
        array_current_index++;
    }

    Debug_array();

    delete[] left_array;
    delete[] right_array;

}


void MainWindow::Swap_array_elements(int& a, int& b){
    int temp = a;
    a = b;
    b = temp;
}

void MainWindow::Swap_rectangles(int first_index, int second_index){

    QTimer* timer_to_delete = new QTimer(this);

    timer_to_delete -> setInterval(time);

    qreal first_x = Rect_list[first_index] -> GetX();
    qreal second_x = Rect_list[second_index] -> GetX();

    Rectangle* first_rect = new Rectangle(second_x, Rect_list[first_index] -> GetY(), Rect_list[first_index] -> GetWidth(), Rect_list[first_index] -> GetHeight(), nullptr);
    Rectangle* second_rect = new Rectangle(first_x, Rect_list[second_index] -> GetY(), Rect_list[second_index] -> GetWidth(), Rect_list[second_index] -> GetHeight(), nullptr);


    Rect_list[first_index] -> SetColor(QColor(5, 219, 247));
    Rect_list[second_index] -> SetColor(QColor(5, 219, 247));

    connect (timer_to_delete, &QTimer::timeout, this , [=]() mutable{

        scene -> removeItem(Rect_list[first_index]);
        scene -> removeItem(Rect_list[second_index]);

        Rect_list[first_index] = second_rect;
        Rect_list[second_index] = first_rect;

        timer_to_delete -> stop();
    });
    timer_to_delete -> start();

    QTimer* timer_to_add = new QTimer(this);
    timer_to_add -> setInterval(time);

    connect (timer_to_add, &QTimer::timeout, this , [=]() mutable{

        scene -> addItem(Rect_list[first_index]);
        scene -> addItem(Rect_list[second_index]);

        first_rect -> SetColor(QColor(255, 255, 255));
        second_rect -> SetColor(QColor(255, 255, 255));

        timer_to_add -> stop();
    });
    timer_to_add -> start();




}
void MainWindow::Debug_array(){
    qDebug() << "===========================";
    for(int i = 0; i < current_array_length; ++i){
        qDebug() << current_array[i];
    }
    qDebug() << "===========================";
}

void MainWindow::Set_buttons_enabled(bool mode){
    if(mode){
        ui -> Sort_button -> setEnabled(true);
        ui -> Create_new_columns_button -> setEnabled(true);
        ui -> Array_size_line -> setReadOnly(false);
    }
    else{
        ui -> Sort_button -> setEnabled(false);
        ui -> Create_new_columns_button -> setEnabled(false);
        ui -> Array_size_line -> setReadOnly(true);
    }
}

void MainWindow::on_Create_new_columns_button_clicked()
{
    Array_input(current_array_length);
    scene -> clear();
    Array_to_scene(current_array_length);
}

