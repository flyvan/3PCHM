#ifndef gmmreg_cpd_h
#define gmmreg_cpd_h

#include "gmmreg_base.h"

class gmmreg_cpd : public gmmreg_base {
 public:
  gmmreg_cpd(): eps(0.0000000001) {
  }
  virtual ~gmmreg_cpd() {}
  double EMtol, tol, beta, anneal;
  int max_iter, max_em_iter, outliers;

 private:
  void start_registration(vnl_vector<double>&, int n_max_func_evals);
  void set_param(vnl_vector<double>&);
  void set_init_params();
  //void save_results(const char*, const vnl_vector<double>&);
  void prepare_input();
  void prepare_param_gradient(bool);
  void perform_transform(const vnl_vector<double>&);
  double bending_energy();
  void compute_gradient(double lambda,
      const vnl_matrix<double>& gradient,
      vnl_matrix<double>& grad_all) {
  };
  void prepare_own_options();
  virtual void prepare_basis_kernel() = 0;
  void prepare_basis() {};
  virtual double update_param() = 0;

 protected:
  vnl_matrix<double> basis, param_all;
  //double EMtol, tol, beta, anneal;
  //int max_iter, max_em_iter, outliers;
  //vnl_vector<double> column_sum;
  //double outlier_term;
  vnl_matrix<double> P;
  double eps;
};

class gmmreg_cpd_tps: public gmmreg_cpd {
 private:
  vnl_matrix<double> tps;
  vnl_matrix<double> affine;
  vnl_matrix<double> nP;
  vnl_matrix<double> G, Q1, Q2, R, invR, invG;

  void prepare_basis_kernel();
  double update_param();
};

class gmmreg_cpd_grbf: public gmmreg_cpd {
 private:
  vnl_matrix<double> dPG;
  vnl_matrix<double> dPY0;
  vnl_matrix<double> Gtranspose, invG;

  void prepare_basis_kernel();
  double update_param();
};

#endif // #ifndef gmmreg_cpd_h
