#include <assert.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

#include <vcl_string.h>
#include <vcl_iostream.h>

#include "gmmreg_base.h"
#include "gmmreg_utils.h"

void gmmreg_base::run(int n_max_func_evals) {
  initialize();
  vnl_vector<double> params;
  start_registration(params, n_max_func_evals);
  denormalize_all();
}

int gmmreg_base::initialize() {
  prepare_input();
  set_init_params();
  prepare_common_options();
  prepare_own_options();
  prepare_basis_kernel();
  return 0;
}

int gmmreg_base::initializeTPS() {
  m = model.rows();
  n = ctrl_pts.rows();
  d = model.cols();
  transformed_model.set_size(m, d);
  normalize(model, model_centroid, model_scale);
  normalize_same(ctrl_pts, model_centroid, model_scale);
  //prepare_input();
  set_init_params();
  //prepare_common_options();
  //prepare_own_options();
  prepare_basis();
  return 0;
}

void gmmreg_base::prepare_input() {

  m = model.rows();
  d = model.cols();
  transformed_model.set_size(m, d);
  s = scene.rows();
  assert(scene.cols() == d);

  //m_all = model_all.rows();
  //d_all = model_all.cols();
  //transformed_model_all.set_size(m_all, d_all);
}

void gmmreg_base::denormalize_all() {
    denormalize(transformed_model, scene_centroid, scene_scale);
    denormalize(model, scene_centroid, scene_scale);
    denormalize(scene, scene_centroid, scene_scale);
}

void gmmreg_base::denormalize_transformed_model() {
    denormalize(transformed_model, model_centroid, model_scale);
}


void gmmreg_base::prepare_common_options() {
    normalize(model, model_centroid, model_scale);
    normalize(scene, scene_centroid, scene_scale);
    normalize(ctrl_pts, ctrl_pts_centroid, ctrl_pts_scale);
}

