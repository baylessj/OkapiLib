/**
 * @author Ryan Benasutti, WPI
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _OKAPI_SKIDSTEERMODEL_HPP_
#define _OKAPI_SKIDSTEERMODEL_HPP_

#include "okapi/api/chassis/model/chassisModel.hpp"
#include "okapi/api/device/motor/abstractMotor.hpp"
#include "okapi/api/device/rotarysensor/continuousRotarySensor.hpp"

namespace okapi {
class SkidSteerModelArgs : public ChassisModelArgs {
  public:
  // Create the sensors using the integrated encoder
  SkidSteerModelArgs(std::shared_ptr<AbstractMotor> ileftSideMotor,
                     std::shared_ptr<AbstractMotor> irightSideMotor, double imaxOutput = 127);

  SkidSteerModelArgs(std::shared_ptr<AbstractMotor> ileftSideMotor,
                     std::shared_ptr<AbstractMotor> irightSideMotor,
                     std::shared_ptr<ContinuousRotarySensor> ileftEnc,
                     std::shared_ptr<ContinuousRotarySensor> irightEnc, double imaxOutput = 127);

  std::shared_ptr<AbstractMotor> leftSideMotor;
  std::shared_ptr<AbstractMotor> rightSideMotor;
  std::shared_ptr<ContinuousRotarySensor> leftSensor;
  std::shared_ptr<ContinuousRotarySensor> rightSensor;
  const double maxOutput;
};

class SkidSteerModel : public ChassisModel {
  public:
  /**
   * Model for a skid steer drive (wheels parallel with robot's direction of motion). When all
   * motors are powered +127, the robot should move forward in a straight line.
   *
   * This constructor infers the two sensors from the left and right motors (using the integrated
   * encoders).
   *
   * @param ileftSideMotor left side motor
   * @param irightSideMotor right side motor
   */
  SkidSteerModel(std::shared_ptr<AbstractMotor> ileftSideMotor,
                 std::shared_ptr<AbstractMotor> irightSideMotor, double imaxOutput = 127);

  /**
   * Model for a skid steer drive (wheels parallel with robot's direction of motion). When all
   * motors are powered +127, the robot should move forward in a straight line.
   *
   * @param ileftSideMotor left side motor
   * @param irightSideMotor right side motor
   * @param ileftEnc  left side encoder
   * @param irightEnc right side encoder
   */
  SkidSteerModel(std::shared_ptr<AbstractMotor> ileftSideMotor,
                 std::shared_ptr<AbstractMotor> irightSideMotor,
                 std::shared_ptr<ContinuousRotarySensor> ileftEnc,
                 std::shared_ptr<ContinuousRotarySensor> irightEnc, double imaxOutput = 127);

  explicit SkidSteerModel(const SkidSteerModelArgs &iparams);

  /**
   * Drive the robot forwards (using open-loop control). Uses velocity mode.
   *
   * @param ispeed motor power
   */
  void forward(double ispeed) const override;

  /**
   * Drive the robot in an arc (using open-loop control). Uses velocity mode.
   * The algorithm is (approximately):
   *   leftPower = ySpeed + zRotation
   *   rightPower = ySpeed - zRotation
   *
   * @param iySpeed speed on y axis (forward)
   * @param izRotation speed around z axis (up)
   */
  void driveVector(double iySpeed, double izRotation) const override;

  /**
   * Turn the robot clockwise (using open-loop control). Uses velocity mode.
   *
   * @param ispeed motor power
   */
  void rotate(double ispeed) const override;

  /**
   * Stop the robot (set all the motors to 0). Uses velocity mode.
   */
  void stop() override;

  /**
   * Drive the robot with a tank drive layout. Uses voltage mode.
   *
   * @param ileftSpeed left side speed
   * @param irightSpeed right side speed
   * @param ithreshold deadband on joystick values
   */
  void tank(double ileftSpeed, double irightSpeed, double ithreshold = 0) const override;

  /**
   * Drive the robot with an arcade drive layout. Uses voltage mode.
   *
   * @param iySpeed speed on y axis (forward)
   * @param izRotation speed around z axis (up)
   * @param ithreshold deadband on joystick values
   */
  void arcade(double iySpeed, double izRotation, double ithreshold = 0) const override;

  /**
   * Power the left side motors. Uses velocity mode.
   *
   * @param ispeed motor power
   */
  void left(double ispeed) const override;

  /**
   * Power the right side motors. Uses velocity mode.
   *
   * @param ispeed motor power
   */
  void right(double ispeed) const override;

  /**
   * Read the sensors.
   *
   * @return sensor readings in the format {left, right}
   */
  std::valarray<std::int32_t> getSensorVals() const override;

  /**
   * Reset the sensors to their zero point.
   */
  void resetSensors() const override;

  /**
   * Set the brake mode for each motor.
   *
   * @param mode new brake mode
   */
  void setBrakeMode(AbstractMotor::brakeMode mode) const override;

  /**
   * Set the encoder units for each motor.
   *
   * @param units new motor encoder units
   */
  void setEncoderUnits(AbstractMotor::encoderUnits units) const override;

  /**
   * Set the gearset for each motor.
   *
   * @param gearset new motor gearset
   */
  void setGearing(AbstractMotor::gearset gearset) const override;

  /**
   * Returns the left side motor.
   *
   * @return the left side motor
   */
  std::shared_ptr<AbstractMotor> getLeftSideMotor() const;

  /**
   * Returns the left side motor.
   *
   * @return the left side motor
   */
  std::shared_ptr<AbstractMotor> getRightSideMotor() const;

  protected:
  std::shared_ptr<AbstractMotor> leftSideMotor;
  std::shared_ptr<AbstractMotor> rightSideMotor;
  std::shared_ptr<ContinuousRotarySensor> leftSensor;
  std::shared_ptr<ContinuousRotarySensor> rightSensor;
  const double maxOutput;
};
} // namespace okapi

#endif