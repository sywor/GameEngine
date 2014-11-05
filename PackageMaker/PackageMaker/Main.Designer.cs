namespace PackageMaker
{
    partial class Main
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Main));
            this.btn_browse = new System.Windows.Forms.Button();
            this.btn_createPackage = new System.Windows.Forms.Button();
            this.txb_in = new System.Windows.Forms.TextBox();
            this.txb_out = new System.Windows.Forms.TextBox();
            this.fbd = new System.Windows.Forms.FolderBrowserDialog();
            this.bgWorker = new System.ComponentModel.BackgroundWorker();
            this.cb_compress = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // btn_browse
            // 
            this.btn_browse.Location = new System.Drawing.Point(413, 38);
            this.btn_browse.Name = "btn_browse";
            this.btn_browse.Size = new System.Drawing.Size(75, 23);
            this.btn_browse.TabIndex = 0;
            this.btn_browse.Text = "Browse";
            this.btn_browse.UseVisualStyleBackColor = true;
            this.btn_browse.Click += new System.EventHandler(this.btn_browse_Click);
            // 
            // btn_createPackage
            // 
            this.btn_createPackage.Location = new System.Drawing.Point(494, 38);
            this.btn_createPackage.Name = "btn_createPackage";
            this.btn_createPackage.Size = new System.Drawing.Size(100, 23);
            this.btn_createPackage.TabIndex = 1;
            this.btn_createPackage.Text = "Create package";
            this.btn_createPackage.UseVisualStyleBackColor = true;
            this.btn_createPackage.Click += new System.EventHandler(this.btn_createPackage_Click);
            // 
            // txb_in
            // 
            this.txb_in.Location = new System.Drawing.Point(12, 12);
            this.txb_in.Name = "txb_in";
            this.txb_in.Size = new System.Drawing.Size(660, 20);
            this.txb_in.TabIndex = 2;
            // 
            // txb_out
            // 
            this.txb_out.Location = new System.Drawing.Point(12, 67);
            this.txb_out.Multiline = true;
            this.txb_out.Name = "txb_out";
            this.txb_out.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txb_out.Size = new System.Drawing.Size(660, 112);
            this.txb_out.TabIndex = 3;
            // 
            // bgWorker
            // 
            this.bgWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.bgWorker_DoWork);
            // 
            // cb_compress
            // 
            this.cb_compress.AutoSize = true;
            this.cb_compress.Location = new System.Drawing.Point(600, 42);
            this.cb_compress.Name = "cb_compress";
            this.cb_compress.Size = new System.Drawing.Size(72, 17);
            this.cb_compress.TabIndex = 4;
            this.cb_compress.Text = "Compress";
            this.cb_compress.UseVisualStyleBackColor = true;
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(684, 192);
            this.Controls.Add(this.cb_compress);
            this.Controls.Add(this.txb_out);
            this.Controls.Add(this.txb_in);
            this.Controls.Add(this.btn_createPackage);
            this.Controls.Add(this.btn_browse);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Main";
            this.Text = "Package Maker";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btn_browse;
        private System.Windows.Forms.Button btn_createPackage;
        private System.Windows.Forms.TextBox txb_in;
        private System.Windows.Forms.TextBox txb_out;
        private System.Windows.Forms.FolderBrowserDialog fbd;
        private System.ComponentModel.BackgroundWorker bgWorker;
        private System.Windows.Forms.CheckBox cb_compress;
    }
}

