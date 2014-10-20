using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PackageMaker
{
    class Folder
    {
        private List<Folder> subFolders = new List<Folder>();
        private List<Asset> assets = new List<Asset>();
        private string name;

        public Folder(string _name)
        {
            name = _name;
        }        
    }
}
