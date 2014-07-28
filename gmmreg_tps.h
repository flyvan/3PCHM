#ifndef gmmreg_tps_h
#define gmmreg_tps_h

#include <vector>

#include "gmmreg_base.h"
#include "gmmreg_tps_func.h"
#include "gmmreg_ctrl_pts.h"

#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

#include <vcl_iostream.h>
#include <vnl/algo/vnl_lbfgs.h>
#include <vnl/algo/vnl_qr.h>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_trace.h>

#include "gmmreg_utils.h"

class gmmreg_tps: public gmmreg_base {
 public:
  gmmreg_tps() {
  }
  vnl_matrix<double> after_tps, basis, param_all;
  virtual ~gmmreg_tps() {
    delete func;
  }
    int iterid;

 protected:
  gmmreg_tps_func *func;

 private:
  vnl_matrix<double> param_affine, param_tps;
  //vnl_matrix<double> after_tps, basis, param_all;
 

  void start_registration(vnl_vector<double>&, int n_max_func_evals);
  void set_init_affine();
  void set_init_tps();
  void set_param(vnl_vector<double>& x0);
  void set_affine_and_tps(const vnl_vector<double>&);
  void set_init_params();

  void prepare_input();
  void prepare_basis_kernel();
  void prepare_basis();
  void prepare_param_gradient(bool);
  void perform_transform(const vnl_vector<double>&);
  double bending_energy();
  void compute_gradient(double lambda, const vnl_matrix<double>& gradient,
      vnl_matrix<double>& grad_all);
  void prepare_own_options();

};

class gmmreg_tps_L2: public gmmreg_tps {
 public:
  gmmreg_tps_L2(): gmmreg_tps() {
    func = new gmmreg_tps_L2_func;
  }
};

class gmmreg_tps_KC: public gmmreg_tps {
 public:
  gmmreg_tps_KC(): gmmreg_tps() {
    func = new gmmreg_tps_KC_func;
  }
};

#endif //#ifndef gmmreg_tps_h