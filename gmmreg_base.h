#ifndef gmmreg_base_h
#define gmmreg_base_h

#ifdef WIN32
#include <windows.h>
//#else
//#include "port_ini.h"
#endif

#include <vector>
#include <vnl/vnl_vector.h>
#include <vnl/vnl_matrix.h>

class gmmreg_base {
 public:
  gmmreg_base() {
  }
  int initialize();
  int initializeTPS();
  void denormalize_all();
  void denormalize_transformed_model();
  int m, n, s, d;
  //int m_all, d_all;
  double sigma, lambda;
  /* m: # of points in model */
  /* s: # of points in scene */
  /* n: # of points in ctrl_pts */
  /* d: dimensionality, e.g. 2 for 2D points, 3 for 3D points */

  // each row is a sample point
  vnl_matrix<double> model, scene, ctrl_pts, transformed_model;
  //vnl_matrix<double> model_all, scene_all, transformed_model_all;

  unsigned int level;//multi_scale_options()
  bool b_fix_affine;
  std::vector<double> v_lambda;
  void run(int n_max_func_evals);
  virtual void perform_transform(const vnl_vector<double>&) = 0;
  virtual double bending_energy() = 0; //serving as a regularization term
  virtual void compute_gradient(double lambda,
      const vnl_matrix<double>& gradient, vnl_matrix<double>& grad_all) = 0;

  virtual ~gmmreg_base() {}

 protected:
  //double sigma, lambda;
  vnl_matrix<double> kernel;
  vnl_vector<double> model_centroid, scene_centroid, ctrl_pts_centroid;


  std::vector<double> v_scale;
  std::vector<int> v_func_evals;

  // get params from input
  virtual void prepare_input();
  //void denormalize_all();
  //int initialize();

 private:
  double model_scale, scene_scale, ctrl_pts_scale;
  void prepare_common_options();
  virtual void prepare_own_options() = 0;
  virtual void prepare_basis_kernel() = 0;
  virtual void prepare_basis() = 0;
  virtual void set_init_params() = 0;
  virtual void start_registration(vnl_vector<double>& params, int n_max_func_evals) = 0;
};

#endif // #ifndef gmmreg_base_h
