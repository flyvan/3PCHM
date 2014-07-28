#ifndef vnl_gmmreg_ctrl_pts_h_
#define vnl_gmmreg_ctrl_pts_h_

#include <fstream>
#include <iostream>
#include <vector>

#include <vnl/algo/vnl_determinant.h>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>

using std::cerr;
using std::cout;
using std::endl;

double estimate_scale(vnl_matrix<double> pts);

void compute_index(unsigned long pos,
                   vnl_vector<unsigned long>& v,
                   vnl_vector<int>& index);

void build_iterator(std::vector<int>& interval, vnl_matrix<int>& iterator) ;

void nd_grid(vnl_vector<double>& min_pos, vnl_vector<double>& max_pos,
    std::vector<int>& interval, vnl_matrix<int>& iterator,
    vnl_matrix<double>& matrix);

void ComputeCtrlPts(vnl_matrix<double>& matrix, vnl_matrix<double>& matrix_output, std::vector<int>& interval);
#endif 