//
// Created by chenzheng on 24-11-11.
//

#ifndef FIRE_SDK_LIB_INCLUDE_DATA_DEFINE_H_
#define FIRE_SDK_LIB_INCLUDE_DATA_DEFINE_H_

#include <cstdint>
#include <array>

typedef struct {
  std::array<double, 4> quaternion; // quaternion, normalized, (w,x,y,z)
  std::array<double, 3> gyroscope; // angular velocity （unit: rad/s)    (raw data)
  std::array<double, 3> accelerometer; // acceleration （unit: m/(s2))       (raw data)
  std::array<double, 3> rpy; // euler angle（unit: rad)
  int8_t temperature; // the temperature of imu (unit: °C)
} IMU; // when under accelerated motion, the attitude of the robot calculated by IMU will drift.

typedef struct {
  double pos; // current angle (unit: radian)
  double vel; // current velocity (unit: radian/second)
  double tau; // current estimated output torque (unit: N.m)
} MotorState; // motor feedback

typedef struct {
  uint8_t mode; // desired working mode. Servo : 0x0A, Damping : 0x00.
  double q; // desired angle (unit: radian)
  double dq; // desired velocity (unit: radian/second)
  double tau; // desired output torque (unit: N.m)
  double Kp; // desired position stiffness (unit: N.m/rad )
  double Kd; // desired velocity stiffness (unit: N.m/(rad/s) )
  std::array<uint32_t, 3> reserve;
} MotorCmd; // motor control

typedef struct {
  IMU imu;
  std::array<MotorState, 20> motorState;
  std::array<int16_t, 4> footForce; // Data from foot airbag sensor
  std::array<int16_t, 4> footForceEst; // reserve，typically zero
  uint32_t tick; // reference real-time from motion controller (unit: ms)
  std::array<uint8_t, 40> wirelessRemote; // Data from Unitree Joystick.
  uint32_t reserve;

  uint32_t crc;
} LowState; // low level feedback

typedef struct {
  std::array<MotorCmd, 20> motorCmd;
  std::array<uint8_t, 40> wirelessRemote; // reserve
  uint32_t reserve;

  uint32_t crc;
} LowCmd; // low level control

typedef struct {
  uint8_t state;              // [0].idle               [1].lie             [2].stance          [3].locomotion          [4].damping
  uint8_t mode;               // [0].balance            [1].walk            [2].run             [3].climb               [4].perceptive
  bool continueEnable;        // continue enable

  // Body data
  std::array<double, 3> bodyPos;       // body position in world frame (unit: m) (Ps:from own odometry in world frame, usually drift)
  std::array<double, 4> bodyOri;       // body orientation in world frame (unit: quaternion)
  std::array<double, 3> bodyLinVel;    // body linear velocity in base frame (unit: m/s)
  std::array<double, 3> bodyAngVel;    // body angular velocity in base frame (unit: rad/s)

  // Joint data (LF, RF, LH, RH) (HAA, HFE, KFE)
  std::array<double, 12> jointPos;     // joint position (unit: rad)
  std::array<double, 12> jointVel;     // joint velocity (unit: rad/s)
  std::array<double, 12> jointTau;     // joint torque (unit: Nm)
  std::array<double, 4> footForce;     // data from joint torque compute

  uint32_t crc;
} HighState; // high level feedback

typedef struct {
  uint8_t state;              // [0].idle               [1].lie             [2].stance          [3].locomotion          [4].damping
  uint8_t mode;               // [0].balance            [1].walk            [2].run             [3].climb               [4].perceptive
  bool continueEnable;        // continue enable

  double bodyHeight; // body stance height (unit: m, range: 0.20~0.40m, default: 0.31m)
  double pitch; // body pitch angle (unit: rad, range: -0.3~0.3rad)

  // velocity command
  std::array<double, 3> bodyVel; // xy linear velocity (unit: m/s) in body frame and yaw angular velocity (unit: rad/s) in world frame
  // (range: trot         : vx:-0.8~0.8m/s, vy:-0.4~0.4m/s, vyaw:-1.5~1.5rad/s)
  // (range: trot_flying  : vx:-1.6~1.6m/s, vy:-0.4~0.4m/s, vyaw:-1.5~1.5rad/s)

  uint32_t crc;
} HighCmd; // high level control

#endif //FIRE_SDK_LIB_INCLUDE_DATA_DEFINE_H_

