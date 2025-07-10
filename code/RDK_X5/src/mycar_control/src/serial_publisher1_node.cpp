#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <termios.h>
#include <fcntl.h>

class Publisher : public rclcpp::Node
{
public:
    Publisher(std::string name) : Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "大家好，我是%s.", name.c_str());
        subscribe_and_publish_publisher_ = this->create_publisher<std_msgs::msg::String>("subscribe_and_publish", 10);

        //保存标准输入（stdin）的属性，以恢复键盘输入的阻塞行为
        tcgetattr(STDIN_FILENO, &initial_settings_);
        //将标准输入设置为非阻塞状态
        tcgetattr(STDIN_FILENO, &new_settings_);
        new_settings_.c_lflag &= ~(ICANON);
        new_settings_.c_lflag &= ~(ECHO);
        new_settings_.c_cc[VMIN] = 0;
        new_settings_.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &new_settings_);

        //将标准输入（stdin）设置为O_NONBLOCK
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        if (flags != -1) {
            fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
        }

        //发布数据到话题的定时器
        timer_ = this->create_wall_timer(std::chrono::milliseconds(500), std::bind(&Publisher::timer_callback, this));
    }

    ~Publisher()
    {
        //恢复标准输入的阻塞行为
        tcsetattr(STDIN_FILENO, TCSANOW, &initial_settings_);
        //将标准输入（stdin）恢复为阻塞状态
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        if (flags != -1) {
            fcntl(STDIN_FILENO, F_SETFL, flags & (~O_NONBLOCK));
        }
    }

private:
    void timer_callback()
    {
        std_msgs::msg::String message;
        std::string data = "0000";
        constexpr int bufferSize = 512;
        char buffer[bufferSize];
        int bytesRead = read(STDIN_FILENO, buffer, bufferSize);

        if (bytesRead > 0) {
            RCLCPP_INFO(this->get_logger(), "%d bytes read from stdin", bytesRead);

            for (int i = 0; i < bytesRead; i++) {
                int keyCode = int(buffer[i]);
                if (keyCode == 27) {  //指令以 ESC 开头
                    int next1 = i + 1, next2 = i + 2;
                    if (next1 < bytesRead && next2 < bytesRead && buffer[next1] == '[') {
                        char dir = buffer[next2];
                        if (dir == 'A') {  //箭头向上
                            arrow_up_pressed_ = true;
                        } else if (dir == 'B') {  //箭头向下
                            arrow_down_pressed_ = true;
                        } else if (dir == 'C') {  //箭头向右
                            arrow_right_pressed_ = true;
                        } else if (dir == 'D') {  //箭头向左
                            arrow_left_pressed_ = true;
                        } 
                    }
                } 
            }
        }

    //    if (arrow_up_pressed_ || arrow_down_pressed_ || arrow_right_pressed_ || arrow_left_pressed_ ) {
            if (arrow_up_pressed_) {
                data[0] = '1';
            } else {
                data[0] = '0';
            }
            if (arrow_down_pressed_) {
                data[1] = '1';
            } else {
                data[1] = '0';
            }
            if (arrow_right_pressed_) {
                data[2] = '1';
            } else {
                data[2] = '0';
            }
            if (arrow_left_pressed_) {
                data[3] = '1';
            } else {
                data[3] = '0';
            }
            message.data = data;
            RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
            subscribe_and_publish_publisher_->publish(message);
            arrow_up_pressed_ = false;
            arrow_down_pressed_ = false;
            arrow_right_pressed_ = false;
            arrow_left_pressed_ = false;
//        }
    }

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr subscribe_and_publish_publisher_;
    termios initial_settings_;
    termios new_settings_;
    bool arrow_up_pressed_ = false;
    bool arrow_down_pressed_ = false;
    bool arrow_right_pressed_ = false;
    bool arrow_left_pressed_ = false;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Publisher>("publisher");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
