using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace PackageMaker
{
    public partial class Main : Form
    {
        List<byte> byteBuffer = new List<byte>();
        Encoding utf8 = Encoding.UTF8;
        Queue<string> logBuffer = new Queue<string>(200);
        string folderPath = "";
        const int txbWidth = 100;
        const string fileFormatName = ".sorryForPotato";

        public Main()
        {
            InitializeComponent();
        }

        private void btn_browse_Click(object sender, EventArgs e)
        {
            DialogResult res = fbd.ShowDialog();

            if(res == DialogResult.OK)
            {
                folderPath = fbd.SelectedPath;
                txb_in.Text = folderPath;
            }
        }

        private void btn_createPackage_Click(object sender, EventArgs e)
        {
            if (folderPath != "")
            {
                bgWorker.RunWorkerAsync();                
            }                
            else
                MessageBox.Show("Please select a folder to create a package from", "Select a folder", MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }

        private void crawlDirectoryTree(string _rootDirectory)
        {
            //print("serializing directory", _rootDirectory);
            serializeString(_rootDirectory);
            serializeAllFiles(Directory.GetFiles(_rootDirectory));
    
            string[] directories = Directory.GetDirectories(_rootDirectory);

            byteBuffer.AddRange(BitConverter.GetBytes((UInt32)directories.Length));
            
            foreach(string s in directories)
            {
                crawlDirectoryTree(s);
            }
        }

        private void serializeAllFiles(string[] _files)
        {
            UInt32 lenght = (UInt32)_files.Length;
            byteBuffer.AddRange(BitConverter.GetBytes(lenght));


            for(UInt32 i = 0; i < lenght; i++)
            {
                //print("serializing file", _files[i]);
                serializeString(_files[i]);
                byte[] tmp = File.ReadAllBytes(_files[i]);
                byteBuffer.AddRange(BitConverter.GetBytes((UInt32)tmp.Length));
                byteBuffer.AddRange(tmp);
            }
        }

        private void serializeString(string _str)
        {
            UInt32 length = (UInt32)utf8.GetByteCount(_str);
            byteBuffer.AddRange(BitConverter.GetBytes(length));
            byte[] tmp = new byte[length];
            utf8.GetBytes(_str, 0, _str.Length, tmp, tmp.GetLowerBound(0));
            byteBuffer.AddRange(tmp);
        }

        private void print(string _title, string _body)
        {            
            int availableChars = txbWidth - (_title.Length + 2);
            int bodyToRemove = _body.Length - availableChars;
            List<char> bodyChArr = new List<char>();
            string finalBody;

            if(bodyToRemove > 0)
            {
                bodyChArr.AddRange(_body.Reverse().Take(availableChars - 3));
                bodyChArr.AddRange(new char[] {'.','.','.'});
                bodyChArr.Reverse();
                finalBody = new string(bodyChArr.ToArray());
            }
            else
            {
                finalBody = _body;
            }

            string fullLogMessage = _title + ": " + finalBody;

            if(logBuffer.Count < 200)
            {
                logBuffer.Enqueue(fullLogMessage);
            }
            else
            {
                logBuffer.Dequeue();
                logBuffer.Enqueue(fullLogMessage);
            }
            
            StringBuilder sb = new StringBuilder();

            foreach(string s in logBuffer)
            {
                sb.Append(s + "\n");
            }

            if(txb_out.InvokeRequired)
            {
                txb_out.Invoke((MethodInvoker)delegate
                {
                    txb_out.Clear();
                    txb_out.Text = sb.ToString();
                    txb_out.ScrollToCaret();
                });
            }
            else
            {
                txb_out.Clear();
                txb_out.Text = sb.ToString();
                txb_out.ScrollToCaret();
            }
        }

        private void bgWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            crawlDirectoryTree(folderPath);

            string[] tmpStrArr = folderPath.Split('\\');
            string fileName = tmpStrArr.Last() + fileFormatName;
            int lastIndexOfSlash = folderPath.LastIndexOf('\\');
            string filePath = folderPath.Substring(0, lastIndexOfSlash) + "\\";

            using (FileStream fs = File.Create(filePath + fileName))
            {
                fs.Write(byteBuffer.ToArray(), 0, byteBuffer.Count);
            }
        }
    }
}
