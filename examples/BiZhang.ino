#include <string.h> //字符串处理头文件
#include "SR04.h" //超声波测距头文件
#include <JY901.h> //姿态传感器头文件
//超声波变量定义
#define TRIG_PIN_1 23 //1 号超声波 23 引脚为输入
#define ECHO_PIN_1 22 //1 号超声波 22 引脚为控制
#define TRIG_PIN_2 21 //2 号超声波 21 引脚为输入
#define ECHO_PIN_3 20 //2 号超声波 20 引脚为控制
#define LED_L 9
#define LED_R 8
/*************************************************************************/
SR04 sr04_1=SR04(ECHO_PIN_1,TRIG_PIN_1); //声明一个类名为 sr04_1 的类
SR04 sr04_2=SR04(ECHO_PIN_3,TRIG_PIN_2); //声明一个类名为 sr04_2 的类
int distance1=60; //超声波 1 距离
int distance2=60; //超声波 2 距离
char str[4];
int count = 0;
unsigned long rx_time = 0;
unsigned char cmd [8] = {0x19,0x88,0x00,0x00,0x00,0x00,0x00,0x11};
int t=0;
int flag;
int d;
/*************************************************************************/
typedef struct
{
  double angle_yaw;           //航向角
  double angle_pitch;         //俯仰角
  double angle_roll;          //横滚角
  int16_t raw_ax;             //X轴加速度原始数据
  int16_t raw_ay;             //.......
  int16_t raw_az;             //..........
  int16_t raw_gx;             //X轴角速度原始数据
  int16_t raw_gy;             //.......
  int16_t raw_gz;             //................   
  int16_t raw_mx;             //X轴磁力计 
  int16_t raw_my;             //..........
  int16_t raw_mz;             //............
  double ax;                  //X轴加速度（单位g）
  double ay;                  //................
  double az;                  //....................
  double gx;                  //X轴角速度（单位°/s）
  double gy;                  //..............
  double gz;                  //...............
  unsigned long last_rx_time;  
}IMU_struct;
IMU_struct IMU;
/*************************************************************************/
void setup()
{
  Serial.begin(115200);                 //与电脑通信波特率
  Serial1.begin(9600);                  //姿态传感器波特率
  JY901.attach(Serial1);
  Serial2.begin(115200); //驱动板初始化
  Serial3.begin(115200);
  Serial2.write(0x55);
  delay(10);
  Serial3.write(0x55);
  delay(10);
  pinMode(9,OUTPUT);
  pinMode(8,OUTPUT);
  flag=0;
}

/*************************************************************************/
void updateIMU(void)
{
  if (JY901.receiveSerialData()) {
    IMU.raw_ax = JY901.getAccRawX();//加速度
    IMU.raw_ay = JY901.getAccRawY();
    IMU.raw_az = JY901.getAccRawZ();
    IMU.raw_gx = JY901.getGyroRawX();//角速度
    IMU.raw_gy = JY901.getGyroRawY();
    IMU.raw_gz = JY901.getGyroRawZ();
    IMU.ax = IMU.raw_ax / (32768.0/16.0);//update
    IMU.ay = IMU.raw_ay / (32768.0/16.0);
    IMU.az = IMU.raw_az / (32768.0/16.0);
    IMU.gx = IMU.raw_gx / (32768.0/2000.0);//update
    IMU.gy = IMU.raw_gy / (32768.0/2000.0);
    IMU.gz = IMU.raw_gz / (32768.0/2000.0);
    IMU.angle_yaw   = JY901.getYaw();//直接得到
    IMU.angle_pitch = JY901.getPitch();
    IMU.angle_roll  = JY901.getRoll();


  }
 
}
/*************************************************************************/
//判断此时车的状态
int judge(){
  updateIMU();
  delay(2500);
  if((IMU.angle_yaw-JY901.getYaw())>=14.4){
    Serial.println("Left");
    delay(2000);
    return 1;//1代表左转
    
  }
  else if((IMU.angle_yaw-JY901.getYaw())<=-14.4){
    Serial.println("Right");
    delay(2000);
    return 2;//2代表右转
    
  }
  else{
    Serial.println("Either");
    delay(2000);
    return 0;//表示不是左转也不是右转
    
  }
}
/*************************************************************************/
//tip1:写入正值即电机顺时针转
//tip2:M时正值输入反向电压
void youpian(int zuo,int you) //700 300
{
 setcmd1('m',-zuo);
 setcmd1('M',zuo);
 setcmd2('m',you);
setcmd2('M',-you);
}
void zuopian(int zuo,int you) //300 700
{
 setcmd1('m',-zuo);
 setcmd1('M',zuo);
 setcmd2('m',you);
 setcmd2('M',-you);
}
void forward(int qian) //600
{
 setcmd1('m',-qian);
 setcmd1('M',qian);
 setcmd2('m',qian);
 setcmd2('M',-qian);
}
void back(int hou) //600
{
 setcmd1('m',hou);
 setcmd1('M',-hou);
 setcmd2('m',-hou);
 setcmd2('M',hou);
}
void stop()
{
 setcmd1('m',0);
 setcmd1('M',0);
 setcmd2('m',0);
 setcmd2('M',0);
}
void left(int zuo ) //500
{
 setcmd1('m',zuo);
 setcmd1('M',-zuo);
 setcmd2('m',zuo);
 setcmd2('M',-zuo);
}
void right(int you) //500
{
 setcmd1('m',-you);
 setcmd1('M',you);
 setcmd2('m',-you);
 setcmd2('M',you);
}
/*************************************************************************/
void setcmd1(unsigned char Cmd_Send,long MotorSpeed)
{
 cmd[2]=Cmd_Send;
 cmd[3] = (unsigned char)(MotorSpeed >> 24);
 cmd[4] = (unsigned char)(MotorSpeed >> 16);
 cmd[5] = (unsigned char)(MotorSpeed >> 8);
 cmd[6] = (unsigned char)(MotorSpeed);
 Serial2.write(cmd,8);
 return;
}
void setcmd2(unsigned char Cmd_Send,long MotorSpeed)
{
 cmd[2]=Cmd_Send;
 cmd[3] = (unsigned char)(MotorSpeed >> 24);
 cmd[4] = (unsigned char)(MotorSpeed >> 16);
 cmd[5] = (unsigned char)(MotorSpeed >> 8);
 cmd[6] = (unsigned char)(MotorSpeed);
 Serial3.write(cmd,8);
 return;
}
void reset(){
  while((sr04_1.Distance()-sr04_2.Distance())<-2||(sr04_1.Distance()-sr04_2.Distance())>2){
    Serial.println("IN:");
    Serial.println(sr04_1.Distance());
    Serial.println(sr04_2.Distance());
    d=sr04_1.Distance()-sr04_2.Distance();
    Serial.println(d);
    if(d>0){
      t=d*70;
      if(t>0){
        left(500);
        
        delay(t);
      }
      else{
        stop();
        delay(1000);
        digitalWrite(LED_R,HIGH);
        digitalWrite(LED_L,HIGH);
        delay(2000);
        break;
      }
    }
   else if(d<0){
    t=-d*70;
      if(t>0){
        right(500);
        
        delay(t);
      }
      else{
        stop();
        delay(1000);
        digitalWrite(LED_R,HIGH);
        digitalWrite(LED_L,HIGH);
        delay(2000);
        break;
      }
    }
    else{
      stop();
      delay(1000);
      digitalWrite(LED_R,HIGH);
      digitalWrite(LED_L,HIGH);
      delay(2000);
      break;
    }
  }
}
/*******************c ******************************************************/
void loop(){
  Serial.println("1:");
  Serial.println(sr04_1.Distance());
  Serial.println("2:");
  Serial.println(sr04_2.Distance());
  reset();
  
  /*if(distance1<10&&distance2<10&&flag==0){
      flag++;
      right(500);
      e=judge();
      if(e==2){
        digitalWrite(LED_R,HIGH);
        delay(1000);
        digitalWrite(LED_R,LOW);
      }
      
  }
  else if(distance1<10&&distance2<10&&flag==1){
      flag++;
      left(500);
      e=judge();
      if(e==1){
        digitalWrite(LED_L,HIGH);
        delay(1000);
        digitalWrite(LED_L,LOW);
      }
  }
  else if(distance1<10&&distance2<10&&flag==2){
    stop();
    digitalWrite(LED_R,HIGH);
    digitalWrite(LED_L,HIGH);
    delay(2000);
    digitalWrite(LED_R,LOW);
    digitalWrite(LED_L,LOW);
    delay(10000);
    break;
  }
  */
 }
