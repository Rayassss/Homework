using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.IO.Ports;
using System.Threading;
using System.Windows.Forms;

namespace 上位机
{
    public partial class Main : Form
    {
        public Main()
        {
            System.Windows.Forms.Control.CheckForIllegalCrossThreadCalls = false;
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen)
            {
                try
                {
                    //设置串口号
                    string serialName = comboBox1.SelectedItem.ToString();
                    serialPort1.PortName = serialName;
                    serialPort1.BaudRate = 115200;
                    serialPort1.DataBits = 8;
                    serialPort1.StopBits = StopBits.One;
                    serialPort1.Parity = Parity.None;
                    if (serialPort1.IsOpen == true)
                    {
                        serialPort1.Close();
                    }
                    serialPort1.Open();
                    button1.Text = "关闭";
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show("Error:" + ex.Message, "Error");
                    return;
                }
            }
            else
            {
                serialPort1.Close();
                button1.Text = "打开";
                label1.Text = "Motor1：NAN";
            }
        }
        void Received_CallBack(byte[] Receive_Buffer)
        {
            byte Buffer_Data_Counter = 0;
            byte Buffer_Flag = 0;
            while (true)
            {
                if (Buffer_Flag == 0 && Receive_Buffer[Buffer_Data_Counter] == 0xff)
                    Buffer_Flag = 1;
                else if (Buffer_Flag == 1 && Receive_Buffer[Buffer_Data_Counter] == 0xff)
                    break;
                Buffer_Data_Counter++;
            }
            Buffer_Data_Counter++;
            if (Buffer_Data_Counter == 12)   //if the package had been broken
            {
                byte[] Data_Buffer = new byte[8];
                byte PID_Bit = Convert.ToByte(Receive_Buffer[1] >> 4);  //Get The PID Bit
                if (PID_Bit == ((~(Receive_Buffer[1] & 0x0f)) & 0x0f))  //PID Verify Success
                {
                    byte Temp_Var = 0x00;
                    while (true)     //Memory Copy
                    {
                        Data_Buffer[Temp_Var] = Receive_Buffer[2 + Temp_Var];
                        Temp_Var++;
                        if (Temp_Var == 8)
                            break;
                    }
                    if (Receive_Buffer[10] != 0x00)   //Some Byte had been replace
                    {
                        byte Temp_Filter = 0x00;
                        Temp_Var = 0;
                        while (true)
                        {
                            if (((Receive_Buffer[10] & (Temp_Filter | (0x01 << Temp_Var))) >> Temp_Var) == 1)  //This Byte Need To Adjust
                                Data_Buffer[Temp_Var] = 0xff;    //Adjust to 0xff
                            Temp_Var++;
                            if (Temp_Var == 8)
                                break;
                        }
                    }
                    switch (PID_Bit)
                    {
                        case 0:
                            float humi = (Convert.ToSingle(Convert.ToInt16(Data_Buffer[0].ToString("X2") + Data_Buffer[1].ToString("X2"), 16)) / 10.0f);
                            float cpu_temp = (Convert.ToSingle(Convert.ToInt16(Data_Buffer[2].ToString("X2") + Data_Buffer[3].ToString("X2"), 16)) / 10.0f);
                            float humi_temp = (Convert.ToSingle(Convert.ToInt16(Data_Buffer[4].ToString("X2") + Data_Buffer[5].ToString("X2"), 16)) / 10.0f);
                            label1.Text = "湿度:" + humi.ToString("0.#") + "%";
                            label1.Text = "温度1:" + cpu_temp.ToString("0.#") + "%";
                            label1.Text = "温度2:" + humi_temp.ToString("0.#") + "%";
                            AddData(humi, cpu_temp, humi_temp);
                            break;
                    }
                }
            }
        }
        Queue<float> data1 = new Queue<float>();
        Queue<float> data2 = new Queue<float>();
        Queue<float> data3 = new Queue<float>();
        private void AddData(float number1, float number2, float number3)
        {
            data1.Enqueue(number1);
            if (data1.Count > 100)
                data1.Dequeue();
            data2.Enqueue(number2);
            if (data2.Count > 100)
                data2.Dequeue();
            data3.Enqueue(number3);
            if (data3.Count > 100)
                data3.Dequeue();
        }
        private void DrawPIcture()
        {
            Bitmap bmp1 = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            PointF[] points1 = new PointF[data1.Count];
            PointF[] points2 = new PointF[data1.Count];
            PointF[] points3 = new PointF[data1.Count];
            float[] dataInt1 = data1.ToArray();
            float[] dataInt2 = data1.ToArray();
            float[] dataInt3 = data1.ToArray();
            for (int i = 0; i < data1.Count; i++)
                points1[i] = new PointF((bmp1.Width / 100) * i * 1.1f, ((pictureBox1.Height / 2) - (dataInt1[i] / 10)));
            for (int i = 0; i < data2.Count; i++)
                points2[i] = new PointF((bmp1.Width / 100) * i * 1.1f, ((pictureBox1.Height / 2) - (dataInt2[i] / 10)));
            for (int i = 0; i < data3.Count; i++)
                points3[i] = new PointF((bmp1.Width / 100) * i * 1.1f, ((pictureBox1.Height / 2) - (dataInt3[i] / 10)));
            using (var g1 = Graphics.FromImage(bmp1))
            {
                Pen pe1 = new Pen(Color.Blue, 2);
                Pen pe2 = new Pen(Color.Red, 2);
                Pen pe3 = new Pen(Color.Yellow, 2);
                g1.Clear(BackColor);
                g1.SmoothingMode = SmoothingMode.HighQuality;
                g1.DrawCurve(pe1, points1);
                g1.DrawCurve(pe2, points2);
                g1.DrawCurve(pe3, points3);
            }
            pictureBox1.Image = bmp1;
        }
        Thread Display_Thread;
        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                Byte[] receivedData = new Byte[serialPort1.BytesToRead];        //创建接收字节数组
                serialPort1.Read(receivedData, 0, receivedData.Length);         //读取数据
                serialPort1.DiscardInBuffer();                                  //清空SerialPort控件的Buffer
                Received_CallBack(receivedData);
                Display_Thread = new Thread(DrawPIcture);
                Display_Thread.IsBackground = true;
                Display_Thread.Start();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                Display_Thread.Abort();
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            comboBox1.Items.Clear();
            string[] str = SerialPort.GetPortNames();
            if (str == null)
            {
                comboBox1.Text = "本机没有串口";
                return;
            }
            foreach (string s in System.IO.Ports.SerialPort.GetPortNames())
            {
                comboBox1.Items.Add(s);
            }
            try
            {
                comboBox1.SelectedIndex = 0;
            }
            catch { }
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}
