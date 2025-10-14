# Fire SDK

欢迎使用 **FireDog四足机器人 SDK**！本 SDK 提供了一套完整的接口，帮助开发者轻松控制和管理FireDog四足机器人，实现各种复杂的动作和行为。

## 简介

Fire SDK 是一个用于开发和控制四足机器人的软件开发工具包。它封装了底层的硬件接口，提供了易于使用的高层 API，基于TCP和UDP实现。

## 功能

- **低层控制**：直接控制每个电机的目标角度、速度、扭矩，支持阻尼与伺服模式，待开发。
- **高层控制**：实现站立、行走、转向、爬坡、感知等行为控制。
- **状态反馈**：实时获取 IMU、关节状态、足端力、姿态、速度、温度、电池电压等信息。
- **远程控制**：通过网络接口实现远程操作。

## 系统要求

- 操作系统：Linux（推荐 Ubuntu 20.04）、CPU (aarch64 and x86_64)
- 编程语言：C++
- 依赖：CMake ≥ 3.10、Threads 库

## 构建

```bash
cd /path/to/fire_sdk
mkdir build
cd build
cmake ..
make
```

## 快速开始

使用方法参考示例程序`./examples/walkExample.cpp`，示例中的服务器IP应更改为实际机器人的IP地址

运行示例之前，需要先启动FireDog四足机器人，并将控制模式设为外部控制。

运行示例程序：

```bash
cd build/examples
./walk_examples
```

该例子演示了如何使用 SDK 实现四足机器人站立、行走、转向等基础动作。

## 命令与状态描述

客户端需要持续发送 `HighCmd` 指令给机器人，机器人也会一直返回 `HighState` 状态给客户端。

### HighState

| 字段名称             | 类型                       | 描述                                                                                                                                                 |
|------------------|--------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------|
| `state`          | `uint8_t`                | 当前状态<br> - `0`: idle —— 空闲状态，电机失能<br> - `1`: lie —— 趴下状态，电机使能<br> - `2`: stance —— 位置站立状态<br> - `3`: locomotion —— 运动状态<br> - `4`: damping —— 阻尼状态 |
| `mode`           | `uint8_t`                | 当前模式<br> - `0`: balance —— 平衡站立模式<br> - `1`: walk —— 行走模式<br> - `2`: run —— 跑步模式<br> - `3`: climb —— 攀爬模式<br> - `4`: perceptive —— 感知模式            |
| `continueEnable` | `bool`                   | 是否启用持续控制                                                                                                                                           |
| `bodyPos`        | `std::array<double, 3>`  | 机器人在世界坐标系中的位置 (m)<br>注：来自自身里程计，存在累计误差                                                                                                              |
| `bodyOri`        | `std::array<double, 4>`  | 机器人在世界坐标系中的方向 (四元数 [x y z w])                                                                                                                      |
| `bodyLinVel`     | `std::array<double, 3>`  | 基坐标系下的线速度 (m/s)                                                                                                                                    |
| `bodyAngVel`     | `std::array<double, 3>`  | 基坐标系下的角速度 (rad/s)                                                                                                                                  |
| `jointPos`       | `std::array<double, 12>` | 各关节位置 (rad)<br>顺序：LF, RF, LH, RH 每腿 [HAA, HFE, KFE]                                                                                                |
| `jointVel`       | `std::array<double, 12>` | 各关节速度 (rad/s)                                                                                                                                      |
| `jointTau`       | `std::array<double, 12>` | 各关节扭矩 (Nm)                                                                                                                                         |
| `batteryV`       | `double`                 | 电池电压 (V)                                                                                                                                           |
| `motorTemp`      | `std::array<double, 12>` | 电机温度 (°C)                                                                                                                                          |
| `crc`            | `uint32_t`               | 循环冗余校验码                                                                                                                                            |

### HighCmd

| 字段名称             | 类型                      | 描述                                                                                                                                                                             |
|------------------|-------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `state`          | `uint8_t`               | 目标状态<br> - `0`: idle<br> - `1`: lie<br> - `2`: stance<br> - `3`: locomotion<br> - `4`: damping                                                                                 |
| `mode`           | `uint8_t`               | 目标模式<br> - `0`: balance<br> - `1`: walk<br> - `2`: run<br> - `3`: climb<br> - `4`: perceptive                                                                                  |
| `continueEnable` | `bool`                  | 是否启用持续控制                                                                                                                                                                       |
| `bodyHeight`     | `double`                | 站立高度 (m)<br>范围：0.20 ~ 0.50                                                                                                                                                     |
| `pitch`          | `double`                | 俯仰角指令 (rad)<br>范围：-0.3 ~ 0.3                                                                                                                                                   |
| `bodyVel`        | `std::array<double, 3>` | 速度指令<br> - `vx`: x方向线速度 (m/s)<br> - `vy`: y方向线速度 (m/s)<br> - `vyaw`: 偏航角速度 (rad/s)<br><br>范围：<br> - **walk**: vx±0.8, vy±0.4, vyaw±1.5<br> - **run**: vx±1.6, vy±0.4, vyaw±1.5 |
| `crc`            | `uint32_t`              | 循环冗余校验码                                                                                                                                                                        |

## 注意事项

1. **指令发布**：客户端需要按照一定频率（100Hz以上）发送 `HighCmd` 指令给机器人，即使不更新任何指令
2. **状态切换**：状态之间的切换需要按照一定的顺序进行，机器人状态机保证了这个规则，任何违反此规则的状态切换将不生效
   ![状态机流程图](./image/state_transition.png)
3. **安全保护**：当机器人处于“危险”情况下，应迅速切换到damping状态，切换方式可以通过SDK进行状态切换，也可直接按下遥控器上的 `C` 键