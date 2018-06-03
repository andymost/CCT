//
//  base.h
//  CCT
//
//  Created by Кочнев Андрей Владимирович on 01/06/2018.
//  Copyright © 2018 Кочнев Андрей Владимирович. All rights reserved.
//

#ifndef base_h
#define base_h

#include <cstdio>
#include <vector>
#include <cmath>

using namespace std;

typedef struct {
    double x;
    double z;
} point2D_type;

typedef struct {
    double ro;
    point2D_type center;
    double width;
    double height;
    double length;
} cell_type;

typedef struct {
    vector<cell_type> cells;
    vector<point2D_type> recievers_pos;
    int rows_count;
    int column_count;
} grid_type;

typedef vector<vector<double>> matrix_type;

grid_type read_grid();
vector<double> delta_gamma(grid_type grid);
matrix_type get_a(grid_type grid);
vector<double> get_b(vector<double> delta_gamma, grid_type grid);
vector<double> solve_equation(matrix_type a, vector<double> b);
matrix_type reg_alpha(matrix_type a);
double residual(vector<double> t_value, vector<double> c_value);
double get_phi(vector<double> t_value, vector<double> c_value);
vector<double> ro_true(grid_type grid);
void print_vec(vector<double> v, int s, string name, string separator);
vector<double> get_deltas(grid_type grid, vector<double> ro, double z_level, double start, double stop);
vector<double> multVecMatrix(matrix_type m, vector<double> v);
vector<double> vector_diff(vector<double> v_1, vector<double> v_2);
double vector_length(vector<double> v_1);
#endif /* base_h */
