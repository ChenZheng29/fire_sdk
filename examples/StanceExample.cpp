//
// Created by chenzheng on 24-11-15.
//

#include "FireClientSDK.h"
#include <iostream>
#include <string>

int main() {
  const std::string serverIP = "192.168.0.107"; // 替换为服务器的实际IP
  Client client(serverIP);

  if (!client.connectToServer()) {
    std::cerr << "无法连接到服务器\n";
    return 1;
  }

  std::cout << "已连接到服务器 " << serverIP << std::endl;
  std::cout << "输入要发送的数据，输入 'exit' 退出" << std::endl;

  HighCmd cmd{};
  while (true) {
    std::cout << "> ";
    std::string input{};
    std::cin >> input;

    if (input == "enable")           // 启用电机
      cmd.motorEnable = true;
    else if (input == "disable")     // 关闭电机
      cmd.motorEnable = false;
    else if (input == "1")           // 站立
      cmd.stage = 1;
    else if (input == "0")           // 趴下
      cmd.stage = 0;
    else if (input == "s") {         // 获取数据
      HighState state{};
      if (!client.getState(state))
        std::cerr << "获取数据失败" << std::endl;
      else {
        std::cout << "motorEnable: " << state.motorEnable << std::endl;
        std::cout << "stage: " << static_cast<int>(state.stage) << std::endl;
      }
      continue;
    } else {
      std::cout << "无效输入" << std::endl;
      continue;
    }

    if (!client.sendCmd(cmd))
      std::cerr << "发送数据失败" << std::endl;
  }

  client.disconnect();
  return 0;
}

