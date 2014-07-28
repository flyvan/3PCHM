#include "gmmreg_ctrl_pts.h"

double estimate_scale(vnl_matrix<double> pts) {
  int m = pts.rows();
  int d = pts.cols();
  double scale = vnl_determinant(pts.transpose() * pts / (m * 1.0));
  for (int i = 0; i < d; ++i) {
    scale = sqrt(scale);
  }
  return scale;
}

void compute_index(unsigned long pos,
                   vnl_vector<unsigned long>& v,
                   vnl_vector<int>& index) {
  int d = v.size(); //assert index.size() = d;
  for (int i = d - 1; i >= 0; --i) {
    index[i] =  pos / v[i];
    pos = pos % v[i];
  }
}


void build_iterator(std::vector<int>& interval, vnl_matrix<int>& iterator) {
  int d = interval.size();
  vnl_vector<unsigned long> v;
  v.set_size(d);
  v[0] = 1;
  for (int i = 1; i < d; ++i) {
    v[i] = v[i - 1] * interval[i - 1];
  }
  unsigned long volume = v[d-1] * interval[d-1];
  for (unsigned long i = 0; i < volume; ++i) {
    //compute_index(i, v, iterator.get_row(i));
    unsigned long pos = i;
    for (int j = d - 1; j >= 0; --j) {
      iterator(i, j) =  pos / v[j];
      pos = pos % v[j];
    }
  }
}

void nd_grid(vnl_vector<double>& min_pos, vnl_vector<double>& max_pos,
    std::vector<int>& interval, vnl_matrix<int>& iterator,
    vnl_matrix<double>& matrix) {
  unsigned long volume = iterator.rows();
  int d = iterator.cols();
  for (unsigned long i = 0; i < volume; ++i) {
    for (int j = 0; j < d; ++j) {
      matrix(i,j) = min_pos[j] + (max_pos[j] - min_pos[j])
        * iterator(i,j) * 1.0 / (interval[j] - 1);
    }
  }
}

void ComputeCtrlPts(vnl_matrix<double>& matrix, vnl_matrix<double>& matrix_output, std::vector<int>& interval)
{
  int d = 3;
  vnl_vector<double> min_row,max_row;
  min_row.set_size(d);
  max_row.set_size(d);
  for (int i = 0; i < d; ++i) {
    min_row[i] = matrix.get_column(i).min_value();
    max_row[i] = matrix.get_column(i).max_value();
  }


  //int d = interval.size();  //assert
  unsigned long volume = 1;
  for (int i = 0; i < d; ++i) {
    volume *= interval[i];
  }
  vnl_matrix<int> iterator;
  iterator.set_size(volume, d);
  build_iterator(interval, iterator);
 // vnl_matrix<double> matrix;
  matrix_output.set_size(volume, d);
 //nd_grid(min_row, max_row, interval, iterator, matrix);
  nd_grid(min_row, max_row, interval, iterator, matrix_output);
 // matrix.print(outfile);

}
