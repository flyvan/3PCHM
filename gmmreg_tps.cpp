
#include "gmmreg_tps.h"

void gmmreg_tps::prepare_input() {
  gmmreg_base::prepare_input();
  n = ctrl_pts.rows();
}

void gmmreg_tps::start_registration(vnl_vector<double>& params, int n_max_func_evals) {
  vnl_lbfgs minimizer(*func);
  func->set_gmmreg(this);
  iterid = 0;
  for (unsigned int k = 0; k < level; ++k) {
    func->set_scale(estimate_scale(model));
	//estimate_scale(model);
	//consider that lambda is 0
	func->set_lambda(v_lambda[k]);
    func->set_fix_affine(b_fix_affine);
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
  //vcl_cout << "registration done" << vcl_endl;
}

void gmmreg_tps::set_init_params() {
  set_init_affine();
  set_init_tps();
  param_all.set_size(n, d);
  param_all.update(param_affine);
  param_all.update(param_tps, d + 1);
}

void gmmreg_tps::set_init_affine() {
    // set default affine parameters from identity transform
    assert(d > 0);
    param_affine.set_size(d + 1, d);
    // the first row is for translation
    param_affine.fill(0);
    // the next dxd matrix is for affine matrix
    vnl_matrix<double> id;
    id.set_size(d, d);
    id.set_identity();
    param_affine.update(id, 1); 
}

void gmmreg_tps::set_init_tps() {
    assert(n - d - 1 > 0);
    assert(d > 0);
    param_tps.set_size(n - d - 1, d);
    param_tps.fill(0);
}

void gmmreg_tps::prepare_basis_kernel() {
  //todo: detect singularity of the data
  vnl_matrix<double> K, U;
  ComputeTPSKernel(model, ctrl_pts, U, K);
  m = model.rows();
  vnl_matrix<double> Pm;
  Pm.set_size(m, d + 1);
  Pm.set_column(0, 1  );
  Pm.update(model, 0, 1);

  vnl_matrix<double> Pn;
  Pn.set_size(n, d + 1);
  Pn.set_column(0, 1);
  Pn.update(ctrl_pts, 0, 1);
  /* should use SVD(Pn), but vnl's SVD is an ``economy-size'' SVD  */

  /* vnl_svd<double> SVD(Pn.transpose());
  vnl_matrix<double> VV = SVD.V();
  std::cout << VV.rows() << " " << VV.cols() << std::endl;
  save_matrix("./VV.txt", VV);
  */

  vnl_qr<double> qr(Pn);
  vnl_matrix<double> V = qr.Q();
  vnl_matrix<double> PP = V.extract(n, n - d - 1, 0, d + 1);
  basis.set_size(m, n);
  basis.update(Pm);
  basis.update(U * PP, 0, d + 1);
  kernel = PP.transpose() * K * PP;
}

void gmmreg_tps::perform_transform(const vnl_vector<double> &x) {
  set_affine_and_tps(x);
  transformed_model = basis * param_all;
  cout<<"The "<< ++iterid <<"th iter TPS" <<endl;
}

double gmmreg_tps::bending_energy() {
  return vnl_trace(param_tps.transpose() * kernel * param_tps);
}

void gmmreg_tps::compute_gradient(double lambda,
    const vnl_matrix<double>& gradient, vnl_matrix<double>& grad_all) {
  grad_all.fill(0);
  if (lambda > 0) {
    grad_all.update(2 * lambda * kernel * param_tps, d+1);
  }
  grad_all += basis.transpose() * gradient;
}

void gmmreg_tps::prepare_own_options() {
  //multi_scale_options();
/* 
  char delims[] = " -,;";
  char s_lambda[256] = {0};
  GetPrivateProfileString(section, "lambda", NULL, s_lambda, 255, f_config);
  parse_tokens(s_lambda, delims, v_lambda);
  if (v_lambda.size() < level) {
    std::cerr<< " too many levels " << std::endl;
    exit(1);
  }*/
}

void gmmreg_tps::set_param(vnl_vector<double>& x0) {
  int k = 0;
  x0.set_size(func->get_number_of_unknowns());
  x0.fill(0);
  if (!func->fix_affine) { // x0 includes affine
    for (int i = 0; i < param_affine.rows(); ++i) {
      for (int j = 0; j < d; ++j, ++k) {
        x0[k] = param_affine(i, j);
      }
    }
  }
  for (int i = 0; i < param_tps.rows(); ++i) {
    for (int j = 0; j < d; ++j, ++k) {
      x0[k] = param_tps(i, j);
    }
  }
}

void gmmreg_tps::set_affine_and_tps(const vnl_vector<double>& x) {
  /* reshape x, assuming x is row major; */
  int rows_x = x.size() / d;
  if (func->fix_affine) 
  {   // affine is given, x does not include affine
    param_all.update(param_affine);
    for (int i = 0, k = 0; i < rows_x; ++i) {
      for (int j = 0; j < d; ++j, ++k) {
        param_tps(i, j) = x[k];
      }
    }
    param_all.update(param_tps, d + 1);
  } else 
  { // affine is not given, x includes affine already
    for (int i = 0, k = 0; i < rows_x; ++i) {
      for (int j = 0; j < d; ++j, ++k) {
        param_all(i, j) = x[k];
      }
    }
    param_affine = param_all.extract(d + 1, d);
    param_tps = param_all.extract(rows_x - d - 1, d, d + 1);
  }
}

void gmmreg_tps::prepare_basis() {
  //todo: detect singularity of the data
  vnl_matrix<double> U;
  ComputeTPSKernelU(model, ctrl_pts, U);
  m = model.rows();
  vnl_matrix<double> Pm;
  Pm.set_size(m, d + 1);
  Pm.set_column(0, 1  );
  Pm.update(model, 0, 1);

  vnl_matrix<double> Pn;
  Pn.set_size(n, d + 1);
  Pn.set_column(0, 1);
  Pn.update(ctrl_pts, 0, 1);

  vnl_qr<double> qr(Pn);
  vnl_matrix<double> V = qr.Q();
  vnl_matrix<double> PP = V.extract(n, n - d - 1, 0, d + 1);

  basis.set_size(m, n);
  basis.update(Pm);
  basis.update(U * PP, 0, d + 1);

}
