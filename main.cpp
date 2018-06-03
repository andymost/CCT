//
//  main.cpp
//  CCT
//
//  Created by Кочнев Андрей Владимирович on 01/06/2018.
//  Copyright © 2018 Кочнев Андрей Владимирович. All rights reserved.
//

#include "base.h"
#include <iostream>

int main(int argc, const char * argv[]) {
    grid_type grid = read_grid();
    vector<double> delta_g = delta_gamma(grid);
    matrix_type a = get_a(grid);
    a = reg_alpha(a);
    vector<double> b = get_b(delta_g, grid);
    vector<double> ro = solve_equation(a, b);
    vector<double> ro_t = ro_true(grid);
    double resid = residual(ro_t, ro);
    double phi = get_phi(ro_t, ro);
    vector<double> deltas = get_deltas(grid, ro, 0, 0, 2000);
    grid_type grid_diff = grid;
    unsigned long size = ro.size();
    for(unsigned long i = 0;i < size; i++) {
        grid_diff.cells[i].ro = ro[i];
    }
    vector<double> delta_c_g = delta_gamma(grid_diff);
    double r = 0;
    for(unsigned long i = 0;i < delta_c_g.size(); i++) {
        r += pow(delta_g[i] - delta_c_g[i], 2.0);
    }
    cout<<"R: "<< r<<endl;
//    cout<<"resid: "<<resid<<endl;
//    cout<<"phi: "<<phi<<endl;
//    print_vec(ro, grid.column_count, "ro", ";");
//    print_vec(deltas, deltas.size(), "deltas", ", ");

    vector<double> tmp = multVecMatrix(a, ro_t);
    tmp = vector_diff(tmp, b);
    double len = vector_length(tmp);
    
    return 0;
}
