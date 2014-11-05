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
        //List<byte> assetByteBuffer = new List<byte>();        
        Queue<string> logBuffer = new Queue<string>(200);
        string folderPath = @"C:\temp\zlib128-dll";
        const int txbWidth = 100;
        const string fileFormatName = ".Spud";

        public Main()
        {
            InitializeComponent();
            txb_in.Text = folderPath;
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
            Folder root = new Folder(_rootDirectory);
            serializeAllFiles(ref root, Directory.GetFiles(_rootDirectory));

            string[] directories = Directory.GetDirectories(_rootDirectory);
            
            foreach(string s in directories)
            {
                Folder f = new Folder(s);
                root.addSubFolder(f);
                crawlDirectoryTree(ref f);
            }

            List<byte> byteBuffer = new List<byte>();
            UInt32 offset = sizeof(UInt32);
            getAssetBinaryBlob(root, ref offset, ref byteBuffer);
            getFolderHeaderData(root, ref byteBuffer);

            List<byte> finalByteBuffer = new List<byte>(byteBuffer.Count + sizeof(UInt32));
            finalByteBuffer.AddRange(BitConverter.GetBytes(offset));
            finalByteBuffer.AddRange(byteBuffer);

            using (FileStream fs = File.Create(folderPath + fileFormatName))
            {
                fs.Write(finalByteBuffer.ToArray(), 0, finalByteBuffer.Count);
            }
        }

        private void crawlDirectoryTree(ref Folder _rootDirectory)
        {
            serializeAllFiles(ref _rootDirectory, Directory.GetFiles(_rootDirectory.getPath()));

            string[] directories = Directory.GetDirectories(_rootDirectory.getPath());

            foreach (string s in directories)
            {
                Folder f = new Folder(s);
                _rootDirectory.addSubFolder(f);
                crawlDirectoryTree(ref f);
            }
        }

        private void serializeAllFiles(ref Folder _folder, string[] _files)
        {
            for (int i = 0; i < _files.Length; i++)
            {
                byte[] tmp = File.ReadAllBytes(_files[i]);
                _folder.addAsset(new Asset(_files[i], tmp));
            }
        }

        private void getAssetBinaryBlob(Folder _f, ref UInt32 _offset, ref List<byte> _list)
        {
            foreach (Asset a in _f.getAssets())
            {
                a.setStart(_offset);
                _offset += a.getSize();
                _list.AddRange(a.getBytes());
            }

            foreach (Folder f in _f.getSubFolders())
            {
                getAssetBinaryBlob(f, ref _offset, ref _list);
            }
        }

        private void getFolderHeaderData(Folder _f, ref List<byte> _list)
        {
            _list.AddRange(Utils.serializeString(_f.getName()));
            _list.AddRange(BitConverter.GetBytes((UInt32)_f.getAssets().Count));

            foreach(Asset a in _f.getAssets())
            {
                _list.AddRange(Utils.serializeString(a.getName()));
                _list.AddRange(BitConverter.GetBytes((UInt32)a.getStart()));
                _list.AddRange(BitConverter.GetBytes((UInt32)a.getSize()));
            }

            _list.AddRange(BitConverter.GetBytes((UInt32)_f.getSubFolders().Count()));

            foreach(Folder f in _f.getSubFolders())
            {
                getFolderHeaderData(f, ref _list);
            }
        }

        //private void print(string _title, string _body)
        //{            
        //    int availableChars = txbWidth - (_title.Length + 2);
        //    int bodyToRemove = _body.Length - availableChars;
        //    List<char> bodyChArr = new List<char>();
        //    string finalBody;

        //    if(bodyToRemove > 0)
        //    {
        //        bodyChArr.AddRange(_body.Reverse().Take(availableChars - 3));
        //        bodyChArr.AddRange(new char[] {'.','.','.'});
        //        bodyChArr.Reverse();
        //        finalBody = new string(bodyChArr.ToArray());
        //    }
        //    else
        //    {
        //        finalBody = _body;
        //    }

        //    string fullLogMessage = _title + ": " + finalBody;

        //    if(logBuffer.Count < 200)
        //    {
        //        logBuffer.Enqueue(fullLogMessage);
        //    }
        //    else
        //    {
        //        logBuffer.Dequeue();
        //        logBuffer.Enqueue(fullLogMessage);
        //    }
            
        //    StringBuilder sb = new StringBuilder();

        //    foreach(string s in logBuffer)
        //    {
        //        sb.Append(s + "\n");
        //    }

        //    if(txb_out.InvokeRequired)
        //    {
        //        txb_out.Invoke((MethodInvoker)delegate
        //        {
        //            txb_out.Clear();
        //            txb_out.Text = sb.ToString();
        //            txb_out.ScrollToCaret();
        //        });
        //    }
        //    else
        //    {
        //        txb_out.Clear();
        //        txb_out.Text = sb.ToString();
        //        txb_out.ScrollToCaret();
        //    }
        //}

        private void bgWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            crawlDirectoryTree(folderPath);
        }
    }
}
