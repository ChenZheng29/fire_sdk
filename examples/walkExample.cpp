//
// Created by chenzheng on 24-11-15.
//

#include "FireClientSDK.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <csignal>

// 控制线程的原子标志
std::atomic<bool> running{true};

// 信号处理函数
void signalHandler(const int signal) {
    running = false;
    exit(signal); // 退出程序
}

int main() {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    std::signal(SIGQUIT, signalHandler);

    std::cout << "================" << std::endl;
    std::cout << "  Version: " << Client::getVersion() << std::endl;
    std::cout << "================" << std::endl;

    const std::string serverIP = "192.168.29.100"; // 替换为服务器的实际IP
    Client client(serverIP, false);  // debug = true 打印调试信息

    // 命令变量
    HighCmd cmd{};
    cmd.bodyHeight = 0.5;

    // 连接线程函数
    auto connectThreadFunc = [&]() {
        while (running) {
            if (!client.isConnect()) {
                std::cout << "> 尝试连接服务器 " << serverIP << " ..." << std::endl;

                if (client.connectToServer())
                    std::cout << "√ 成功连接到服务器 " << serverIP << std::endl;
                else
                    std::cout << "× 无法连接到服务器 " << serverIP << std::endl;
            }
            sleep(1); // 每1秒尝试连接一次
        }
    };

    // 发送指令线程函数
    auto sendThreadFunc = [&]() {
        while (running) {
            if (client.isConnect()) {
                if (!client.sendCmd(cmd))
                    std::cout << "发送数据失败，连接可能已断开" << std::endl;
            }
            usleep(10000); // 100hz发布命令
        }
    };

    // 启动连接线程和发送线程
    std::thread connectThread(connectThreadFunc);
    std::thread sendThread(sendThreadFunc);

    // 等待一段时间让连接建立
    usleep(500000);

    std::cout << "输入要发送的命令：" << std::endl;
    while (running) {
        std::cout << "> ";
        std::string input{};
        std::cin >> input;

        if (input == "0") {               // 空闲状态
            cmd.state = 0;
        } else if (input == "1") {              // 趴下状态
            cmd.state = 1;
        } else if (input == "2") {              // 站立状态
            cmd.state = 2;
        } else if (input == "3") {              // 运动状态
            cmd.state = 3;
        } else if (input == "4") {              // 阻尼状态
            cmd.state = 4;
        } else if (input == "5") {              // balance模式
            cmd.mode = 0;
        } else if (input == "6") {              // walk模式
            cmd.mode = 1;
        } else if (input == "7") {              // run模式
            cmd.mode = 2;
        } else if (input == "8") {              // climb模式
            cmd.mode = 3;
        } else if (input == "9") {              // perceptive模式
            cmd.mode = 4;
        } else if (input == "w") {              // 前进
            cmd.bodyVel[0] += 0.4;
        } else if (input == "s") {              // 后退
            cmd.bodyVel[0] -= 0.4;
        } else if (input == "a") {              // 左移
            cmd.bodyVel[1] += 0.2;
        } else if (input == "d") {              // 右移
            cmd.bodyVel[1] -= 0.2;
        } else if (input == "q") {              // 左旋
            cmd.bodyVel[2] += 0.7;
        } else if (input == "e") {              // 右旋
            cmd.bodyVel[2] -= 0.7;
        } else if (input == "r") {              // 获取数据
            HighState highState{};
            if (!client.getState(highState)) {
                std::cout << "获取数据失败（未连接）" << std::endl;
            } else {
                std::cout << "remote mode: " << highState.remoteMode << std::endl;
                std::cout << "state: " << static_cast<int>(highState.state) << std::endl;
                std::cout << "mode: " << static_cast<int>(highState.mode) << std::endl;
                std::cout << "body linear velocity x: " << highState.bodyLinVel[0] << std::endl;
                std::cout << "LF joint pos: " << highState.jointPos[0] << " " << highState.jointPos[1] << " " << highState.jointPos[2] << std::endl;
                std::cout << "batteryV: " << highState.batteryV << std::endl;
                std::cout << "LF motor temperature: " << highState.motorTemp[0] << " " << highState.motorTemp[1] << " " << highState.motorTemp[2] <<
                        std::endl;
            }
            continue;
        } else {
            std::cout << "无效输入" << std::endl;
            continue;
        }

        // 限制速度范围
        cmd.bodyVel[0] = std::min(std::max(cmd.bodyVel[0], -0.8), 0.8);
        cmd.bodyVel[1] = std::min(std::max(cmd.bodyVel[1], -0.4), 0.4);
        cmd.bodyVel[2] = std::min(std::max(cmd.bodyVel[2], -1.4), 1.4);

        std::cout << "命令已更新: state=" << static_cast<int>(cmd.state)
                << ", mode=" << static_cast<int>(cmd.mode)
                << ", vel=(" << cmd.bodyVel[0] << ", " << cmd.bodyVel[1] << ", " << cmd.bodyVel[2] << ")" << std::endl;
    }
}
