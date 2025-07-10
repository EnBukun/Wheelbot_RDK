#include "pub_serialport.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

int fp;
unsigned char Msg[128] = { 0 };
const char* constc = nullptr;
unsigned char byteArray[10] = { 0xFF,0x32,0x01,0x01,0x00,0x32,0x00,0x01,0x00,0xFC }; // "Hello World!" ���ֽ������ʾ


Serial sp;
class TopicSuscribe01 : public rclcpp::Node
{
public:
    TopicSuscribe01(std::string name) : Node(name)
    {
        RCLCPP_INFO(this->get_logger(),"����%s�����Ļ���Ϊ��/subscribe_and_publish.",name.c_str());
        command_subscribe_ = this->create_subscription<std_msgs::msg::String>("subscribe_and_publish",10,std::bind(&TopicSuscribe01::command_callback,this,std::placeholders::_1));
    }
private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr command_subscribe_;
    void command_callback(const std_msgs::msg::String::SharedPtr msg)
    {
        double speed = 0.0f;
        // if(msg->data == "9999")
        // {
        //     speed = 0.2f;

        // }
        RCLCPP_INFO(this->get_logger(),"�յ���[%s]ָ������ٶ�%f",msg->data.c_str(),speed);
        
         std::string data = msg->data;
         
            
         
       //     std::string hex_number = "FF3101000031000000FC";

    if (data == "1000") {
       
        
        byteArray[0] = 0xFF;
        byteArray[1] = 0x31;
        byteArray[2] = 0x01;
        byteArray[3] = 0x00;
        byteArray[4] = 0x00;
        byteArray[5] = 0x31;
        byteArray[6] = 0x00;
        byteArray[7] = 0x00;
        byteArray[8] = 0x00;
        byteArray[9] = 0xFC;
        
        
    } else if (data == "0100") {
        // byteArray[10] = { 0xFF,31,00,00,00,31,01,00,00,0xFC };
        byteArray[0] = 0xFF;
        byteArray[1] = 0x31;
        byteArray[2] = 0x00;
        byteArray[3] = 0x00;
        byteArray[4] = 0x00;
        byteArray[5] = 0x31;
        byteArray[6] = 0x01;
        byteArray[7] = 0x00;
        byteArray[8] = 0x00;
        byteArray[9] = 0xFC;
    } else if (data == "0010") {
        // byteArray[10] = { 0xFF,32,01,00,00,31,00,00,00,0xFC };
        byteArray[0] = 0xFF;
        byteArray[1] = 0x32;
        byteArray[2] = 0x01;
        byteArray[3] = 0x00;
        byteArray[4] = 0x00;
        byteArray[5] = 0x31;
        byteArray[6] = 0x00;
        byteArray[7] = 0x00;
        byteArray[8] = 0x00;
        byteArray[9] = 0xFC;
    } else if (data == "0001") {
        // byteArray[10] = { 0xFF,31,01,00,00,32,00,00,00,0xFC };
                byteArray[0] = 0xFF;
        byteArray[1] = 0x31;
        byteArray[2] = 0x01;
        byteArray[3] = 0x00;
        byteArray[4] = 0x00;
        byteArray[5] = 0x32;
        byteArray[6] = 0x00;
        byteArray[7] = 0x00;
        byteArray[8] = 0x00;
        byteArray[9] = 0xFC;
    } else {
      
               byteArray[0] = 0xFF;
        byteArray[1] = 0x32;
        byteArray[2] = 0x01;
        byteArray[3] = 0x01;
        byteArray[4] = 0x00;
        byteArray[5] = 0x32;
        byteArray[6] = 0x00;
        byteArray[7] = 0x01;
        byteArray[8] = 0x00;
        byteArray[9] = 0xFC;
    }
        
        
       //  std::string str(byteArray, byteArray + sizeof(byteArray) / sizeof(unsigned char)); // ʹ�� string ���캯�����ֽ�����ת��Ϊ�ַ���
      //  str += '\0'; // ��� NULL �ַ�
      //  constc = str.c_str();
        
      //  int len = strlen(constc);
      //  sp.WriteData(fp,constc,len);
            // ��������ʮ�����Ʒ�ʽ���͵�����
    for (int i = 0; i < 10; i++) {
        char hex[5];
        sprintf(hex, "%02X ", byteArray[i]);
        write(fp, hex, strlen(hex));
    }
    write(fp, "\n", 1); // ���ͻ��з���Ϊ�������

      
    }

};


int main(int argc, char **argv)
{
    /* code */
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TopicSuscribe01>("subscribe1");
    // ��ӡһ�����ҽ���
    // int fp;
    // Serial sp;
    sp.BaudRate(921600);
    fp = sp.OpenPort(COM1);
   // fp = sp.OpenPort(/dev/pts/3);
    if(fp>0){
      RCLCPP_INFO(node->get_logger(), "serial success!.");
    }
  //  sp.WriteData(fp,"1024",20);

    
    // cout<<fp<<endl;

    RCLCPP_INFO(node->get_logger(), "��Һã�����subscribe1.");
    /* ���нڵ㣬������˳��ź�*/
    rclcpp::spin(node);
    rclcpp::shutdown();
    sp.ClosePort(fp);
    return 0;
    
    return 0;
}