#include "kalman_filter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in; // object state
  P_ = P_in; // Object covariance matrix
  F_ = F_in; // state transition matrix
  H_ = H_in; // meaurement matrix
  R_ = R_in; // meaurement covariance matrix
  Q_ = Q_in; // process covariance matrix
}

// The Kalman filter predict function, same for linear and extended Kalman filter
void KalmanFilter::Predict() {
  /**
  TODO:
    * predict the state
  */
  x_ = F_ * x_;
  MatrixXd Ftranspose = F_.transpose();
  P_ = F_ * P_ * Ftranspose + Q_;
}

// Kalman filter update step. Equations from the lectures
void KalmanFilter::Update(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Kalman Filter equations
  */
  VectorXd y = z- H_ * x_;
  KalmanFilterUpdateforboth(y);
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Extended Kalman Filter equations
  */
  //Calculate x object state to rho, theta, rho_dot coordinates
  double rho = sqrt(x_(0) * x_(0) + x_(1) * x_(1));
  double theta = atan2(x_(1), x_(0));
  double rho_dot = (x_(0) * x_(2) + x_(1) * x_(3)) / rho;
  VectorXd z_predict = VectorXd(3);
  z_predict << rho, theta, rho_dot;
  VectorXd y = z - z_predict;
  KalmanFilterUpdateforboth(y);

}
//Common update function for KF and EKF
void KalmanFilter::KalmanFilterUpdateforboth(const VectorXd &y){
  MatrixXd Ht = H_.transpose();
  MatrixXd S = H_ * P_ * Ht + R_;
  MatrixXd Si = S.inverse();
  MatrixXd K =  P_ * Ht * Si;

  // New state
  x_ = x_ + (K * y);
  long x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K * H_) * P_;
}
