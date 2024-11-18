//
// Created by chenzheng on 24-11-15.
//

#include "FireClientSDK.h"
#include <iostream>
#include <string>
#include <unistd.h>

int main() {
  const std::string serverIP = "192.168.0.125"; // 替换为服务器的实际IP
  Client client(serverIP);

  if (!client.connectToServer()) {
    std::cerr << "无法连接到服务器\n";
    return 1;
  }

  std::cout << "已连接到服务器 " << serverIP << std::endl;
  std::cout << "输入要发送的命令：" << std::endl;

  HighCmd cmd{};

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
        const std::string gait[3] = {"stance", "trot", "flying_trot"};
        std::cout << "gait: " << gait[highState.gait] << std::endl;
      }
      continue;
    } else {
      std::cout << "无效输入" << std::endl;
      continue;
    }

    cmd.gait = 1;
    cmd.bodyHeight = 0.5;
    cmd.footSwingHeight = 0.1;
    cmd.bodyVel[0] = std::min(std::max(cmd.bodyVel[0], -0.8f), 0.8f);
    cmd.bodyVel[1] = std::min(std::max(cmd.bodyVel[1], -0.4f), 0.4f);
    cmd.bodyVel[2] = std::min(std::max(cmd.bodyVel[2], -1.4f), 1.4f);
  }

  return 0;
}

