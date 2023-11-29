namespace WinFormsApp1
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            checkBox1 = new CheckBox();
            txtPortName = new TextBox();
            label1 = new Label();
            panel1 = new Panel();
            label2 = new Label();
            lblPos = new Label();
            SuspendLayout();
            // 
            // checkBox1
            // 
            checkBox1.AutoSize = true;
            checkBox1.Location = new Point(362, 75);
            checkBox1.Name = "checkBox1";
            checkBox1.Size = new Size(89, 21);
            checkBox1.TabIndex = 0;
            checkBox1.Text = "checkBox1";
            checkBox1.UseVisualStyleBackColor = true;
            checkBox1.CheckedChanged += checkBox1_CheckedChanged;
            // 
            // txtPortName
            // 
            txtPortName.Location = new Point(164, 66);
            txtPortName.Name = "txtPortName";
            txtPortName.Size = new Size(100, 23);
            txtPortName.TabIndex = 1;
            txtPortName.Text = "COM3";
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(91, 69);
            label1.Name = "label1";
            label1.Size = new Size(67, 17);
            label1.TabIndex = 2;
            label1.Text = "PortName";
            // 
            // panel1
            // 
            panel1.BackColor = Color.White;
            panel1.Location = new Point(91, 131);
            panel1.Name = "panel1";
            panel1.Size = new Size(500, 500);
            panel1.TabIndex = 3;
            panel1.MouseMove += panel1_MouseMove;
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(699, 95);
            label2.Name = "label2";
            label2.Size = new Size(35, 17);
            label2.TabIndex = 4;
            label2.Text = "POS:";
            // 
            // lblPos
            // 
            lblPos.AutoSize = true;
            lblPos.Location = new Point(740, 95);
            lblPos.Name = "lblPos";
            lblPos.Size = new Size(43, 17);
            lblPos.TabIndex = 5;
            lblPos.Text = "label3";
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 17F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1435, 917);
            Controls.Add(lblPos);
            Controls.Add(label2);
            Controls.Add(panel1);
            Controls.Add(label1);
            Controls.Add(txtPortName);
            Controls.Add(checkBox1);
            Name = "Form1";
            Text = "Form1";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private CheckBox checkBox1;
        private TextBox txtPortName;
        private Label label1;
        private Panel panel1;
        private Label label2;
        private Label lblPos;
    }
}
