#include "tools.h"
#include <iostream>
#include "Eigen/Dense"

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;
using std::cout;
using std::endl;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
  /**
   * TODO: Calculate the RMSE here.
   */

  VectorXd rmse(4);
  rmse << 0,0,0,0;

  // Check the validity of the following inputs:
  //  * the estimation vector size should not be zero
  //  * the estimation vector size should equal ground truth vector size
  if ( estimations.size() == 0 || estimations.size() != ground_truth.size() ){
      cout << "Invalid estimation or ground_truth data " << endl;
      return rmse;
  }
 
  // Accumulate squared residuals
  for (int i=0; i < estimations.size(); ++i) {
    VectorXd residual = estimations[i] - ground_truth[i];  
    // coefficient-wise multiplication
    residual = residual.array() * residual.array();  
    rmse += residual;
  }

  // Calculate the mean
  rmse = rmse / estimations.size();
  // Calculate the squared root
  rmse = rmse.array().sqrt();   
  // return the result
  return rmse;
}

MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {
  /**
   * TODO:
   * Calculate a Jacobian here.
   */
  
  MatrixXd Hj(3,4);
  // Recover state parameters
  float px = x_state(0);
  float py = x_state(1);
  float vx = x_state(2);
  float vy = x_state(3);

  // Pre-calcuate useful values
  float c1 = px*px + py*py;
  float c2 = sqrt(c1);
  float c3 = c1 * c2;

  // Check division by zero
  if (fabs(c1) < 0.0001) {
      cout << "Error division by zero" << endl;   
      //return Hj;  
      c1 = 0.01;
  }
  
  // Compute the Jacobian matrix
  Hj << px/c2,  py/c2,   0, 0,
        -py/c1, px/c1 ,  0, 0,
        py*(vx*py-vy*px)/c3, px*(vy*px-vx*py)/c3 , px/c2 , py/c2 ;
  
  return Hj;
}
