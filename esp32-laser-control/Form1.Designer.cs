namespace esp32_laser_control
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
            txtPortName = new TextBox();
            checkBox1 = new CheckBox();
            panel1 = new Panel();
            lblPos = new Label();
            label1 = new Label();
            SuspendLayout();
            // 
            // txtPortName
            // 
            txtPortName.Location = new Point(72, 88);
            txtPortName.Name = "txtPortName";
            txtPortName.Size = new Size(100, 23);
            txtPortName.TabIndex = 0;
            txtPortName.Text = "COM5";
            // 
            // checkBox1
            // 
            checkBox1.AutoSize = true;
            checkBox1.Location = new Point(201, 90);
            checkBox1.Name = "checkBox1";
            checkBox1.Size = new Size(89, 21);
            checkBox1.TabIndex = 1;
            checkBox1.Text = "checkBox1";
            checkBox1.UseVisualStyleBackColor = true;
            checkBox1.CheckedChanged += checkBox1_CheckedChanged;
            // 
            // panel1
            // 
            panel1.BackColor = Color.White;
            panel1.Location = new Point(72, 133);
            panel1.Name = "panel1";
            panel1.Size = new Size(409, 409);
            panel1.TabIndex = 2;
            panel1.MouseMove += panel1_MouseMove;
            // 
            // lblPos
            // 
            lblPos.AutoSize = true;
            lblPos.Location = new Point(462, 94);
            lblPos.Name = "lblPos";
            lblPos.Size = new Size(43, 17);
            lblPos.TabIndex = 3;
            lblPos.Text = "label1";
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(618, 94);
            label1.Name = "label1";
            label1.Size = new Size(43, 17);
            label1.TabIndex = 4;
            label1.Text = "label1";
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 17F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(822, 841);
            Controls.Add(label1);
            Controls.Add(lblPos);
            Controls.Add(panel1);
            Controls.Add(checkBox1);
            Controls.Add(txtPortName);
            Name = "Form1";
            Text = "Form1";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private TextBox txtPortName;
        private CheckBox checkBox1;
        private Panel panel1;
        private Label lblPos;
        private Label label1;
    }
}
