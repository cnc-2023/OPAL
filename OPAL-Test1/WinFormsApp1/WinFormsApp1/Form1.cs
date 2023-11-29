using System.IO.Ports;

namespace WinFormsApp1
{
    public partial class Form1 : Form
    {
        SerialPort serialPort;
        public Form1()
        {
            InitializeComponent();

            serialPort = new SerialPort();
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

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void panel1_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.checkBox1.Checked)
            {
                var pos = $"{(float)e.X / 2},{(float)e.Y / 2}";
                this.lblPos.Text = pos;
                serialPort.WriteLine(pos);
            }
        }
    }
}
