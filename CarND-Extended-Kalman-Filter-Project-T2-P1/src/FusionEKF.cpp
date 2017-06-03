#include "FusionEKF.h"
#include "tools.h"
#include "Eigen/Dense"
#include <iostream>
// Intilalize EPS Macro with small number
#define EPS 0.0001 

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::vector;

/*
 * Constructor.
 */
FusionEKF::FusionEKF() {
  is_initialized_ = false;

  previous_timestamp_ = 0;

  // initializing matrices
  R_laser_ = MatrixXd(2, 2);
  R_radar_ = MatrixXd(3, 3);
  H_laser_ = MatrixXd(2, 4);
  H_laser_ << 1, 0, 0, 0,
        0, 1, 0, 0;
  Hj_ = MatrixXd(3, 4);

  //measurement covariance matrix - laser
  R_laser_ << 0.0225, 0,
        0, 0.0225;

  //measurement covariance matrix - radar
  R_radar_ << 0.09, 0, 0,
        0, 0.0009, 0,
        0, 0, 0.09;

  /**
  TODO:
    * Finish initializing the FusionEKF.
    * Set the process and measurement noises
  */


}

/**
* Destructor.
*/
FusionEKF::~FusionEKF() {}

void FusionEKF::ProcessMeasurement(const MeasurementPackage &measurement_pack) {


  /*****************************************************************************
   *  Initialization
   ****************************************************************************/
  if (!is_initialized_) {
    /**
    TODO:
      * Initialize the state ekf_.x_ with the first measurement.
      * Create the covariance matrix.
      * Remember: you'll need to convert radar from polar to cartesian coordinates.
    */
    // first measurement
    cout << "EKF: " << endl;
    ekf_.x_ = VectorXd(4);

    if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
      /**
      Convert radar from polar to cartesian coordinates and initialize state.
      */
      float rho = measurement_pack.raw_measurements_[0]; //range
      float phi = measurement_pack.raw_measurements_[1]; // bearing
      float rho_dot = measurement_pack.raw_measurements_[2]; // velocity of rho

      //coordinates convertion from polar to cartesian
      float x = rho * cos(phi);
      float y = rho * sin(phi);
      float vx = rho_dot * cos(phi);
      float vy = rho_dot * sin(phi);
      ekf_.x_ << x, y, vx, vy;
    }
    else if (measurement_pack.sensor_type_ == MeasurementPackage::LASER) {
      /**
      Initialize state.
      */
      // Laser Velocity is set to zero
      ekf_.x_ << measurement_pack.raw_measurements_[0], measurement_pack.raw_measurements_[1], 0 , 0;
    }
    // Special case intialisation
    if(fabs(ekf_.x_(0)) < EPS and fabs(ekf_.x_(1)) < EPS){
      ekf_.x_(0) = EPS;
      ekf_.x_(1) = EPS;
    }

    // Initial covariance matrix
    ekf_.P_ = MatrixXd(4, 4);
    ekf_.P_ << 1, 0, 0, 0,
           0, 1, 0, 0,
           0, 0, 1000, 0,
           0, 0, 0, 1000;  
    cout << "EKF: After Init" << ekf_.x_ << endl;

    //previous timestamp save
    previous_timestamp_ = measurement_pack.timestamp_;
    // done initializing, no need to predict or update
    is_initialized_ = true;
    return;
  }

  /*****************************************************************************
   *  Prediction
   ****************************************************************************/

  /**
   TODO:
     * Update the state transition matrix F according to the new elapsed time.
      - Time is measured in seconds.
     * Update the process noise covariance matrix.
     * Use noise_ax = 9 and noise_ay = 9 for your Q matrix.
   */
  // calcluate timestamp measurement in seconds
  float diftime = (measurement_pack.timestamp_ - previous_timestamp_);
  diftime = diftime/ 1000000.0; // conver ms to Seconds
  previous_timestamp_ = measurement_pack.timestamp_;

  // state transition matrix
  ekf_.F_ = MatrixXd(4, 4);
  ekf_.F_ << 1, 0, diftime, 0,
             0, 1, 0, diftime,
             0, 0, 1, 0,
             0, 0, 0, 1;
  // Noise covaraince matrix computation 

  float noise_ax = 9.0;
  float noise_ay = 9.0;
  float diftime_1 =  diftime * diftime; // diftime^2
  float diftime_2 = diftime_1 * diftime; // diftime^3
  float diftime_3 = diftime_2 * diftime; // diftime^4
  float diftime_4 = diftime_3/4; //diftime^4/4
  float diftime_5 = diftime_2 / 2; // diftime^3/2
  ekf_.Q_ = MatrixXd(4, 4);
  ekf_.Q_ << diftime_4 * noise_ax, 0, diftime_5 * noise_ax, 0,
          0, diftime_4 * noise_ay, 0, diftime_5 * noise_ay,
          diftime_5 * noise_ax, 0, diftime_1 * noise_ax, 0,
          0, diftime_5 * noise_ay, 0, diftime_1 * noise_ay;

  ekf_.Predict();


  /*****************************************************************************
   *  Update
   ****************************************************************************/

  /**
   TODO:
     * Use the sensor type to perform the update step.
     * Update the state and covariance matrices.
   */

  if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
    // Radar updates
    // Jacobian usage
    ekf_.H_ =  tools.CalculateJacobian(ekf_.x_);
    ekf_.R_ = R_radar_;
    ekf_.UpdateEKF(measurement_pack.raw_measurements_);
  } else {
    // Laser updates
    ekf_.H_ = H_laser_;
    ekf_.R_ = R_laser_;
    ekf_.Update(measurement_pack.raw_measurements_);
  }

  // print the output
  cout << "x_ = " << ekf_.x_ << endl;
  cout << "P_ = " << ekf_.P_ << endl;
}
