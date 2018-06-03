//
//  inOut.cpp
//  CCT
//
//  Created by Кочнев Андрей Владимирович on 01/06/2018.
//  Copyright © 2018 Кочнев Андрей Владимирович. All rights reserved.
//

#include "base.h"
#include <fstream>
#include <iostream>

// Формат входных данных
// - ширина
// - высота сетки
// - количество приемников на дневной поверхности
// - количество строк ячеек
// - количество столбцов ячеек
// - значения ро в ячейках

grid_type read_grid() {
    // объявляем
    grid_type grid;
    char *path = "/Users/a.kochnev/development/CCT/CCT/CCT" "/base54.txt";
    fstream grid_file (path, ios_base::in);
    double width = 0;
    double height = 0;
    int reciever_count = 0;
    int row = 0;
    int column = 0;
    vector<double> ros;
    
    // считываем
    grid_file >> width;
    grid_file >> height;
    grid_file >> reciever_count;
    grid_file >> row;
    grid_file >> column;
    grid.rows_count = row;
    grid.column_count = column;
    for (int i = 0; i < row * column; i++) {
        double cell_ro;
        grid_file >> cell_ro;
        ros.push_back(cell_ro);
    }
    
    // формируем сетку
    
    // равномерно распределяем приемники
    vector<point2D_type> recievers_pos;
    double step = width / reciever_count;
    for(int i = 0; i < reciever_count; i++) {
        point2D_type pos;
        pos.x = i * step;
        pos.z = 0;
        
        recievers_pos.push_back(pos);
    }
    grid.recievers_pos = recievers_pos;
    
    
    //формируем ячейки
    int cell_count = row * column;
    double cell_height = height / row;
    double cell_width = width / column;
    vector<cell_type> cells;
    for(int i = 0; i < cell_count; i++) {
        cell_type cell;
        cell.ro = ros[i];
        cell.height = cell_height;
        cell.width = cell_width;
        // считаем что по y всегда единичный объем
        cell.length = 1;
        
        point2D_type center;
        center.z = -(cell_height/2 + (i / row) * cell_height);
        center.x = cell_width/2 + (i % column) * cell_width;
        
        cell.center = center;
        cells.push_back(cell);
    }
    grid.cells = cells;
    
    return grid;
}


void print_vec(vector<double> v, int size, string name, string separator) {
    cout<<name<<": "<<endl;
    for(unsigned long i = 0; i< v.size(); i++){
        if(i % size == 0) {
            cout<<endl;
        }
        if(i % size != size - 1) {
            cout<<v[i]<<separator;
        }
    }
}

double vector_mult_vector(vector<double> a, vector<double> b) {
    double result = 0;
    for (int i = 0; i < a.size(); ++i) {
        result += a[i]*b[i];
    }
    
    return result;
}

vector<double> multVecMatrix(matrix_type m, vector<double> v) {
    unsigned long size = v.size();
    vector<double> res;
    for(unsigned long i = 0; i < size; i++) {
        double tmp = vector_mult_vector(m[i], v);
        res.push_back(tmp);
    }
    
    return res;
}

double vector_length(vector<double> v_1) {
    double acc = 0;
    for (int i = 0; i < v_1.size(); i ++) {
        acc += pow(v_1[i], 2);
    }
    
    return sqrt(acc);
}

vector<double> vector_diff(vector<double> v_1, vector<double> v_2) {
    vector<double> res;
    for (int i = 0; i < v_1.size(); i++) {
        res.push_back(v_1[i] - v_2[i]);
    }
    
    return res;
}


