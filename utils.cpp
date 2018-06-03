//
//  utils.cpp
//  CCT
//
//  Created by Кочнев Андрей Владимирович on 01/06/2018.
//  Copyright © 2018 Кочнев Андрей Владимирович. All rights reserved.
//

#include "base.h"
#include <iostream>

double euclid_distance(point2D_type p1, point2D_type p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.z - p2.z, 2));
}

double get_coord_z(point2D_type center, point2D_type point) {
    return point.z - center.z;
}

// формула 3 в методе, дельта для i-приемника, s-ячейки
double delta_gamma_i_s(point2D_type reciever_pos, cell_type cell) {
    double mes = cell.width * cell.height * cell.length;
    double r_3 = pow(euclid_distance(cell.center, reciever_pos), 3);
    double z_pos = get_coord_z(cell.center, reciever_pos);
    
    return (mes * z_pos)/(4 * M_PI * r_3);
}

// значение дельты для i-приемника
double delta_gamma_i (point2D_type reciever_pos, vector<cell_type> cells) {
    double result = 0;
    unsigned long elem_count = cells.size();
    
    for(unsigned long i = 0; i < elem_count; i++) {
        double cell_value = delta_gamma_i_s(reciever_pos, cells[i]) * cells[i].ro;
        result += cell_value;
    }
    
    return result;
}

vector<double> delta_gamma(grid_type grid) {
    vector<double> result;
    unsigned long size = grid.recievers_pos.size();
    for(unsigned int i = 0; i < size; i++) {
        double gamma = delta_gamma_i(grid.recievers_pos[i], grid.cells);
        result.push_back(gamma);
    }
    return result;
}


matrix_type get_a(grid_type grid) {
    matrix_type result;
    unsigned long cell_size = grid.cells.size();
    unsigned long reciver_size = grid.recievers_pos.size();
    
    for(unsigned long q = 0; q < cell_size; q++) {
        vector<double> tmp;
        result.push_back(tmp);
        
        for(unsigned long s = 0; s < cell_size; s++){
            double value = 0;
            for(unsigned long i = 0; i < reciver_size; i++) {
                value += delta_gamma_i_s(grid.recievers_pos[i], grid.cells[q]) * delta_gamma_i_s(grid.recievers_pos[i], grid.cells[s]);
            }
            result[q].push_back(value);
        }
    }
    
    return result;
}

vector<double> get_b(vector<double> delta_gamma, grid_type grid) {
    // delta_gamma - эксперементальные данные, должны быть получены один раз, поэтому сюда передаеются, а не выщитываются
    vector<double> result;
    
    unsigned long cell_size = grid.cells.size();
    unsigned long reciver_size = grid.recievers_pos.size();
    
    for(unsigned long s = 0; s < cell_size; s++){
        double value = 0;
        for(unsigned long i = 0; i < reciver_size; i++) {
            value += delta_gamma_i_s(grid.recievers_pos[i], grid.cells[s]) * delta_gamma[i];
        }
        result.push_back(value);
    }
    
    return result;
}

matrix_type reg_alpha(matrix_type a) {
    matrix_type result;
    double alpha = 1e-13;
    unsigned long size = a.size();
    
    for (unsigned long i = 0; i < size; i++) {
        vector<double> tmp;
        for(unsigned long j = 0; j < size; j++) {
            if(i == j) {
                tmp.push_back(a[i][j] + alpha);
            } else {
                tmp.push_back(a[i][j]);
            }
        }
        result.push_back(tmp);
    }
    
    return result;
}

double length(vector<double> v){
    double sum = 0;
    unsigned long size = v.size();
    for(unsigned long i = 0; i < size; i++) {
        sum+= pow(v[i], 2);
    }
    return sqrt(sum);
}

double residual(vector<double> t_value, vector<double> c_value) {
    vector<double> tmp;
    unsigned long size = t_value.size();
    for (unsigned long i = 0; i < size; i++) {
        tmp.push_back(t_value[i] - c_value[i]);
    }
    return length(tmp)/length(t_value);
}

vector<double> ro_true(grid_type grid) {
    vector<double> result;
    for(unsigned long i = 0; i < grid.cells.size(); i++) {
        result.push_back(grid.cells[i].ro);
    }
    return result;
}

double get_phi(vector<double> t_value, vector<double> c_value) {
    vector<double> tmp;
    double res = 0;
    unsigned long size = t_value.size();
    for (unsigned long i = 0; i < size; i++) {
        res += pow(t_value[i] - c_value[i], 2.0);
        cout<<"res: "<<res<<endl;
    }
    
    return res;
}

vector<double> get_deltas(grid_type grid, vector<double> ro, double z_level, double start, double stop) {
    vector<double> res;
    double step = (stop - start)/100;
    for(double i = 0; i < 100; i++) {
        point2D_type r_pos;
        double val;
        
        r_pos.x = i * step + start;
        r_pos.z = z_level;
        val = ro[i] * delta_gamma_i(r_pos, grid.cells);
        res.push_back(val);
    }
    
    return res;
    
}
