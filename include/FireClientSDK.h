//
// Created by chenzheng on 24-11-11.
//

#ifndef FIRE_CLIENT_SDK_H
#define FIRE_CLIENT_SDK_H

#define SERVER_PORT 29575
#define VERSION 1.1

#include <string>
#include <thread>
#include <atomic>
#include <mutex>

#include "DataDefine.h"

class Client {
  public:
    explicit Client(std::string serverIP);
    ~Client();

    // 连接到服务器
    bool connectToServer();

    // 断开连接
    void disconnect();

    // 发送数据
    bool sendCmd(const HighCmd& cmd) const;

    // 获取数据
    bool getState(HighState& state);

    // 接收数据
    void receiveThread();

    // 返回连接状态
    bool isConnect() { return connected_; }

    // 返回当前SDK版本
    static double getVersion() { return VERSION; }

  private:
    HighState state_{};
    HighCmd cmd_{};

    std::string serverIP_{};
    int clientSocket_{};
    std::thread receiveThread_;
    std::atomic<bool> connected_;
    std::mutex stateMutex_;
};

#endif // FIRE_CLIENT_SDK_H

