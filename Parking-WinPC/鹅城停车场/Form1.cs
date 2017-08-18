using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace 鹅城停车场
{
    public partial class Form1 : Form
    {
        Socket socketClient;
        delegate void ReceiveCallback(string str);
        ReceiveCallback delReceive;
        delegate void ReceiveCountCallback(string str, string x);
        ReceiveCountCallback delReceiveCount;
        delegate void ReceiveLightStateCallback(string str);
        ReceiveLightStateCallback delReceiveLightState;
        PictureBox[] pA;
        PictureBox[] pB;
        int n;
        public Form1()
        {
            InitializeComponent();



            delReceive = delreceive;
            delReceiveCount = delreceiveCount;
            delReceiveLightState = delreceiveLightState;

            pA = new PictureBox[31] { pictureBox1, pictureBox2, pictureBox3, pictureBox4, pictureBox5, pictureBox6, pictureBox7, pictureBox8, pictureBox9, pictureBox10, pictureBox11, pictureBox12, pictureBox13, pictureBox14, pictureBox15, pictureBox16, pictureBox17, pictureBox18, pictureBox19, pictureBox20, pictureBox21, pictureBox22, pictureBox23, pictureBox24, pictureBox25, pictureBox26, pictureBox27, pictureBox28, pictureBox29, pictureBox30, pictureBox31 };
            pB = new PictureBox[42] { pictureBox32, pictureBox33, pictureBox34, pictureBox35, pictureBox36, pictureBox37, pictureBox38, pictureBox39, pictureBox40, pictureBox41, pictureBox42, pictureBox43, pictureBox44, pictureBox45, pictureBox46, pictureBox47, pictureBox48, pictureBox49, pictureBox50, pictureBox51, pictureBox52, pictureBox53, pictureBox54, pictureBox55, pictureBox56, pictureBox57, pictureBox58, pictureBox59, pictureBox60, pictureBox61, pictureBox62, pictureBox63, pictureBox64, pictureBox65, pictureBox66, pictureBox67, pictureBox68, pictureBox69, pictureBox70, pictureBox71, pictureBox72, pictureBox73 };
            try
            {
                IPEndPoint point = new IPEndPoint(IPAddress.Parse("119.146.68.41"), 5000);
                socketClient = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                socketClient.Connect(point);
                Send();
                n = 1;
                comboBox1.Items.Add("自动模式");
                comboBox1.Items.Add("手动模式");
                comboBox1.Text = "自动模式";
                Thread th = new Thread(Receive);
                th.IsBackground = true;
                th.Start();

            }
            catch (Exception)
            { }

        }

        //连上服务器后先发送密码APPLIC和请求指令5226
        void Send()
        {
            string sendM = "APPLIC";
            byte[] buffer = Encoding.Default.GetBytes(sendM);
            socketClient.Send(buffer);
            string sendS = "5226";
            byte[] buffer2 = Encoding.Default.GetBytes(sendS);
            socketClient.Send(buffer2);

        }

        //接收消息
        int Count_A = 0;
        int Count_B = 0;
        int Count;
        void Receive()
        {
            try
            {
                while (true)
                {

                    byte[] buffer = new byte[1024];
                    int r = socketClient.Receive(buffer);
                    if (r == 0)
                        break;
                    //首个字符是A就将数据填入A区车位数量框
                    string str = Encoding.Default.GetString(buffer, 0, r);
                    string strReceive = str.Substring(1, str.Length - 1);
                    if (str[0] == 'A')
                    {
                        Count_A = 0;
                        for (int i = 0; i < pA.Length; i++)
                        {
                            if (strReceive[i] == '0')
                            { pA[i].BackColor = Color.Green; Count_A++; }
                            if (strReceive[i] == '1')
                                pA[i].BackColor = Color.Red;
                        }

                    }
                    textCount.Invoke(delReceiveCount, Count_A.ToString(), "A");

                    //首个字符是B就将数据填入B区车位数量框
                    if (str[0] == 'B')
                    {
                        Count_B = 0;
                        for (int i = 0; i < pB.Length; i++)
                        {
                            if (strReceive[i] == '0')
                            { pB[i].BackColor = Color.Green; Count_B++; }
                            if (strReceive[i] == '1')
                                pB[i].BackColor = Color.Red;
                        }
                    }
                    textCount.Invoke(delReceiveCount, Count_B.ToString(), "B");


                    //把两个区的空车位数填入总车位数量框
                    Count = Count_A + Count_B;
                    textCount.Invoke(delReceiveCount, Count.ToString(), "C");


                    //首个字符是L，将信息分别填入灯光亮度框和灯转态框
                    if (str[0] == 'L')
                    {

                        textLight.Invoke(delReceive, str.Substring(5, 5));
                        if (str.Substring(str.Length - 1, 1) == "1")
                            textLightState.Invoke(delReceiveLightState, "开启");
                        else textLightState.Invoke(delReceiveLightState, "关闭");
                    }
                    if (str[0] == '1')
                    {
                        if (strReceive == "1")
                            textLightState.Invoke(delReceiveLightState, "开启");//1代表灯开
                        else if (strReceive == "0")
                            textLightState.Invoke(delReceiveLightState, "关闭");//0代表灯关
                    }

                }
            }
            catch (Exception)
            { }
        }
       



        //发送开灯信息
        private void btnONo_Click(object sender, EventArgs e)
        {
            try
            {
                string str = "LIGHTON";
                byte[] buffer = Encoding.Default.GetBytes(str);
                socketClient.Send(buffer);
            }
            catch (Exception)
            { }


        }
        //发送关灯信息
        private void btnOFF_Click(object sender, EventArgs e)
        {
            try
            {

                string str = "LIGHTOFF";
                byte[] buffer = Encoding.Default.GetBytes(str);
                socketClient.Send(buffer);

            }
            catch (Exception)
            { }
        }



        //接收车位数量信息
        void delreceiveCount(string str, string x)
        {
            if (x == "A")
            {
                textCount_A.Clear();
                textCount_A.AppendText(str);
            }
            if (x == "B")
            {
                textCount_B.Clear();
                textCount_B.AppendText(str);
            }
            if (x == "C")
            {
                textCount.Clear();
                textCount.AppendText(str);
            }
        }

        //接收灯亮度信息
        void delreceive(string str)
        {
            textLight.Clear();
            textLight.AppendText(str);

        }

        //接收开关灯消息
        void delreceiveLightState(string str)
        {
            textLightState.Clear();
            textLightState.AppendText(str);
        }


        //选择模式
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

            if (n == 1)
                n = 0;
            else if (n == 0)
            {
                byte[] buffer = new byte[1024];
                if (comboBox1.Text == "自动模式")
                {
                    buffer = Encoding.Default.GetBytes("SELFCONTROL");//选自动模式发送SELFCONTROL
                    socketClient.Send(buffer);
                    btnON.Enabled = false;
                    btnOFF.Enabled = false;
                }
                else if (comboBox1.Text == "手动模式")
                {
                    buffer = Encoding.Default.GetBytes("MANUAL");//选手动模式发送MANUAL
                    socketClient.Send(buffer);
                    btnON.Enabled = true;
                    btnOFF.Enabled = true;
                }
            }
        }


    }
}
