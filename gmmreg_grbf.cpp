#include <assert.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

#include <vcl_iostream.h>
#include <vnl/vnl_matrix.h>
#include <vnl/algo/vnl_qr.h>
#include <vnl/algo/vnl_lbfgs.h>
#include <vnl/vnl_trace.h>

#include "gmmreg_utils.h"
#include "gmmreg_grbf.h"

void gmmreg_grbf::prepare_input() {
   gmmreg_base::prepare_input();
   n = ctrl_pts.rows();
   //beta = 1;
}

void gmmreg_grbf::start_registration(vnl_vector<double>& params, int n_max_func_evals) {
  vnl_lbfgs minimizer(*func);
  func->set_gmmreg(this);
  iterid = 0;
  //cout << n_max_func_evals << endl;
  for (unsigned int k = 0; k < level; ++k) {
    func->set_scale(estimate_scale(model));
    func->set_lambda(v_lambda[k]);
    func->prepare_param_gradient();
    set_param(params);
    //int n_max_func_evals = v_func_evals[k];
    minimizer.set_max_function_evals(n_max_func_evals);
    // For more options, see
    // http://public.kitware.com/vxl/doc/release/core/vnl/html/vnl__nonlinear__minimizer_8h-source.html
    minimizer.minimize(params);
    if (minimizer.get_failure_code() < 0) {
      break;
    }
  }
}

void gmmreg_grbf::set_init_params() {
  //char f_init_grbf[80]={0};
  //GetPrivateProfileString(common_section, "init_grbf", NULL,
  //    f_init_grbf, 80, f_config);
  set_init_grbf();
  param_all.set_size(n, d);
}

void gmmreg_grbf::set_init_grbf() {
    assert(n > 0);
    assert(d > 0);
    param_grbf.set_size(n, d);
    param_grbf.fill(0);
}

void gmmreg_grbf::prepare_basis_kernel() {
  ComputeGaussianKernel(model, ctrl_pts, basis, kernel, beta);
}

void gmmreg_grbf::perform_transform(const vnl_vector<double> &x) {
  set_grbf(x);
  transformed_model = model + basis* param_grbf;
  cout<<"The "<< ++iterid <<"th iter GRBF" <<endl;
}

double gmmreg_grbf::bending_energy() {
  return vnl_trace(param_grbf.transpose() * kernel * param_grbf);
}

void gmmreg_grbf::compute_gradient(double lambda,
    const vnl_matrix<double>& gradient, vnl_matrix<double>& grad_all) {
  grad_all = basis.transpose() * gradient;
  if (lambda > 0) {
    grad_all += 2 * lambda * kernel * param_grbf;
  }
}

//void gmmreg_grbf::save_results(const char* f_config,
//    const vnl_vector<double>& params) {
//  char f_transformed[256] = {0};
//  char f_final_grbf[256] = {0};
//  GetPrivateProfileString(common_section, "final_grbf", NULL,
//      f_final_grbf, 255, f_config);
//  GetPrivateProfileString(common_section, "transformed_model", NULL,
//      f_transformed, 255, f_config);
//  save_transformed( f_transformed, params, f_config );
//  save_matrix(f_final_grbf, param_grbf);
//}

void gmmreg_grbf::prepare_own_options() {
  //multi_scale_options(f_config);
  //char delims[] = " -,;";
  //char s_lambda[256] = {0};
  //GetPrivateProfileString(section, "lambda", NULL, s_lambda, 255, f_config);
  //parse_tokens(s_lambda, delims, v_lambda);
  //if (v_lambda.size() < level) {
  //  std::cerr<< " too many levels " << std::endl;
  //  exit(1);
  //}
}

void gmmreg_grbf::set_param(vnl_vector<double>& x0) {
  int k = 0;
  x0.set_size(func->get_number_of_unknowns());
  x0.fill(0);
  for (unsigned int i = 0; i < param_grbf.rows(); i++) {
    for (int j = 0; j < d; ++j, ++k) {
      x0[k] = param_grbf(i, j);
    }
  }
}

void gmmreg_grbf::set_grbf(const vnl_vector<double>& x) {
  /* reshape x, assuming x is row major; */
  int rows_x = x.size() / d;
  for (int i = 0, k = 0; i < rows_x; ++i) {
    for (int j = 0; j < d; ++j, ++k) {
      param_grbf(i, j) = x[k];
    }
  }
}