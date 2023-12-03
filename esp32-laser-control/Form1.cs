using System.IO.Ports;

namespace esp32_laser_control
{
    public partial class Form1 : Form
    {
        SerialPort serialPort;
        public Form1()
        {
            InitializeComponent();

            serialPort = new SerialPort();
            serialPort.BaudRate = 115200;
            serialPort.DataReceived += SerialPort_DataReceived;
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {

            //var data = serialPort.ReadLine();
            //this.Invoke(new Action(() =>
            //{
            //    this.label1.Text = data;
            //}));

        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
            {
                this.serialPort.PortName = txtPortName.Text;
                serialPort.Open();
            }
            else
            {
                serialPort.Close();
            }
        }

        private void panel1_MouseMove(object sender, MouseEventArgs e)
        {
            var pos = $"{e.X*10 },{e.Y * 10}";
            this.lblPos.Text = pos;
            if (this.checkBox1.Checked)
            {

                serialPort.WriteLine(pos);
            }
        }
    }
}
