//
// Created by chenzheng on 24-11-11.
//

#ifndef FIRE_CLIENT_SDK_H
#define FIRE_CLIENT_SDK_H

#include <string>
#include <atomic>
#include <mutex>

#include "DataDefine.h"

class Client {
  public:
    explicit Client(std::string serverIP, bool debug);
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
    bool isConnect();

    // 返回当前SDK版本
    static double getVersion();

  private:
    bool checkConnection() const;

    HighState state_{};
    HighCmd cmd_{};

    std::string serverIP_{};
    int clientSocket_{};
    std::atomic<bool> connected_;
    std::mutex stateMutex_;

    bool debug_{};
};

#endif // FIRE_CLIENT_SDK_H

