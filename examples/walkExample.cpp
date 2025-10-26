//
// Created by chenzheng on 24-11-15.
//

#include "FireClientSDK.h"
#include <iostream>
#include <string>
#include <unistd.h>

int main() {
  std::cout << "================" << std::endl;
  std::cout << "  Version: " << Client::getVersion() << std::endl;
  std::cout << "================" << std::endl;

  const std::string serverIP = "192.168.29.100"; // 替换为服务器的实际IP
  Client client(serverIP);

  if (!client.connectToServer()) {
    std::cerr << "无法连接到服务器\n";
    return 1;
  }

  std::cout << "已连接到服务器 " << serverIP << std::endl;
  std::cout << "输入要发送的命令：" << std::endl;

  HighCmd cmd{};
  cmd.bodyHeight = 0.5;

  auto f = [&]() {
    while (client.isConnect()) {
      if (!client.sendCmd(cmd))
        std::cerr << "发送数据失败" << std::endl;
      usleep(10000);
    }
  };
  std::thread cmdSendThread(f);
  cmdSendThread.detach();

  while (client.isConnect()) {
    std::cout << "> ";
    std::string input{};
    std::cin >> input;

    if (input == "0")                // 空闲状态
      cmd.state = 0;
    else if (input == "1")           // 趴下状态
      cmd.state = 1;
    else if (input == "2")           // 站立状态
      cmd.state = 2;
    else if (input == "3")           // 运动状态
      cmd.state = 3;
    else if (input == "4")           // 阻尼状态
      cmd.state = 4;
    else if (input == "5")           // balance模式
      cmd.mode = 0;
    else if (input == "6")           // walk模式
      cmd.mode = 1;
    else if (input == "7")           // run模式
      cmd.mode = 2;
    else if (input == "8")           // climb模式
      cmd.mode = 3;
    else if (input == "9")           // perceptive模式
      cmd.mode = 4;
    else if (input == "w")           // 前进
      cmd.bodyVel[0] += 0.4;
    else if (input == "s")           // 后退
      cmd.bodyVel[0] -= 0.4;
    else if (input == "a")           // 左移
      cmd.bodyVel[1] += 0.2;
    else if (input == "d")           // 右移
      cmd.bodyVel[1] -= 0.2;
    else if (input == "q")           // 左旋
      cmd.bodyVel[2] += 0.7;
    else if (input == "e")           // 右旋
      cmd.bodyVel[2] -= 0.7;
    else if (input == "r") {         // 获取数据
      HighState highState{};
      if (!client.getState(highState))
        std::cerr << "获取数据失败" << std::endl;
      else {
        std::cout << "state: " << static_cast<int>(highState.state) << std::endl;
        std::cout << "mode: " << static_cast<int>(highState.mode) << std::endl;
        std::cout << "body linear velocity x: " << highState.bodyLinVel[0] << std::endl;
        std::cout << "LF joint pos: " << highState.jointPos[0] << " " << highState.jointPos[1] << " " << highState.jointPos[2] << std::endl;
        std::cout << "batteryV: " << highState.batteryV << std::endl;
        std::cout << "batteryLevel: " << highState.batteryLevel << std::endl;
        std::cout << "batteryIsCharging: " << highState.isCharging << std::endl;
        std::cout << "LF motor temperature: " << highState.motorTemp[0] << " " << highState.motorTemp[1] << " " << highState.motorTemp[2] <<
            std::endl;
      }
      continue;
    } else {
      std::cout << "无效输入" << std::endl;
      continue;
    }

    cmd.bodyVel[0] = std::min(std::max(cmd.bodyVel[0], -0.8), 0.8);
    cmd.bodyVel[1] = std::min(std::max(cmd.bodyVel[1], -0.4), 0.4);
    cmd.bodyVel[2] = std::min(std::max(cmd.bodyVel[2], -1.4), 1.4);
  }

  return 0;
}

