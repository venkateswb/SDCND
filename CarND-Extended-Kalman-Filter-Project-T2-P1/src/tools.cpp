#include <iostream>
#include "tools.h"
// Define Macros
#define EPS 0.0001 
#define EPS1 0.0000001

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
  /**
  TODO:
    * Calculate the RMSE here.
  */
  VectorXd rmse(4);
  rmse << 0, 0, 0, 0;
  //check the estimate size is zero
  if(estimations.size() == 0){
  	cout << " Empty Input" << endl;
  	return rmse;
  }
  // check the size of estimations and ground is equal
  if(estimations.size() != ground_truth.size()){
  	cout << " Invalid estimations or Ground truth, both should contain same size" << endl;
  	return rmse;
  }
  // Squared residual accumalation
  for(unsigned int i =0; i < estimations.size(); ++i){
  	VectorXd residual = estimations[i] - ground_truth[i];
  	residual = residual.array() * residual.array();
  	rmse += residual;
  }
  //calculate the mean
  rmse /= estimations.size();
  rmse = rmse.array().sqrt();
  return rmse;

}

MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {
  /**
  TODO:
    * Calculate a Jacobian here.
  */
  float px = x_state(0);
  float py = x_state(1);
  float vx = x_state(2);
  float vy = x_state(3);
  MatrixXd Hj(3,4);
  // Special case
  if(fabs(px) < EPS and fabs(py) < EPS){
  	px = EPS;
  	py = EPS;
  }
  //pre-compute to avoid repeated calculation
  float cal1 = px*px + py*py;
  //check divison by zero
  if(fabs(cal1)< EPS1){
  	cal1 = EPS1;
  }
  float cal2 = sqrt(cal1);
  float cal3 = cal1*cal2;

  //compute Jacobian matrix
  Hj << (px/cal2), (py/cal2), 0, 0,
      -(py/cal1), (px/cal1), 0, 0,
      py*(vx*py - vy*px)/cal3, px*(px*vy - py*vx)/cal3, px/cal2, py/cal2;
  return Hj;  
}
