using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using static SAModManager.Updater.DownloadDialog;

namespace SAModManager.Updater
{
    public class DownloadInfo
    {
        public string Name;
        public string FileName;
        public string Folder;
        public DLType Type = DLType.Download;
        public readonly Uri uri;
        public string errorMessage = null;

        public DownloadInfo(string name, string fileName, string folder, Uri uri_, DLType type)
        {
            Name = name;
            FileName = fileName;
            uri = uri_;
            Folder = string.IsNullOrEmpty(folder) ? App.tempFolder : folder;
            Type = type;
        }
    }


}
