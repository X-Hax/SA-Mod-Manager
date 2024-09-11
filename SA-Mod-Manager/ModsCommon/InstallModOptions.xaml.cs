using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using SAModManager.UI;

namespace SAModManager
{
    /// <summary>
    /// Interaction logic for NewModOptions.xaml
    /// </summary>
    public partial class InstallModOptions : Window
    {
        public enum Type : int
        {
            NewMod,
            ModArchive,
            ModFolder,
            ModUrl,
        }

        public InstallModOptions()
        {
            InitializeComponent();
            Header.Text = Lang.GetString("InstallModOptions.Title");
            Title = Lang.GetString("EditMod.Header.NewMod");

            CheckStack.Children.Add(new RadioButton() { IsChecked = true, Content = Lang.GetString("InstallModOptions.Radio.New"), Margin = new Thickness(2) }); ;
            CheckStack.Children.Add(new RadioButton() { Content = Lang.GetString("InstallModOptions.Radio.Archive"), Margin = new Thickness(2) });
            CheckStack.Children.Add(new RadioButton() { Content = Lang.GetString("InstallModOptions.Radio.Folder"), Margin = new Thickness(2) });
            CheckStack.Children.Add(new RadioButton() { Content = Lang.GetString("InstallModOptions.Radio.Url"), Margin = new Thickness(2) });
        }


        private void Window_ContentRendered(object sender, EventArgs e)
        {
            InvalidateVisual(); //fix render
        }

        private void ButtonCancel_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        public int Ask()
        {
            ShowDialog();
            if (DialogResult == false)
                return -1;

            return GetSelected();
        }

        private int GetSelected()
        {
            for (int i = 0; i < CheckStack.Children.Count; i++)
            {
                if (((RadioButton)CheckStack.Children[i]).IsChecked.Value)
                {
                    return i;
                }
            }
            return -1;
        }

        private void OK_Click(object sender, RoutedEventArgs e)
        {
            if (GetSelected() < 0)
            {
                return;
            }
            DialogResult = true;
        }

        public async Task InstallModOptionsArchive(string[] path, string root)
        {
            int index = 0;
            try
            {
                var tempFolder = Path.Combine(root, App.tempFolder);

                Util.CreateSafeDirectory(tempFolder);
                

                foreach (string file in path)
                {

                    await Util.Extract(file, tempFolder, true);

                    //if the mod archive doesn't have a folder as a container, create one and move the files inside.
                    if (File.Exists(Path.Combine(tempFolder, "mod.ini")))
                    {
                        var newModPath = Path.Combine(tempFolder, Path.GetFileNameWithoutExtension(file));

                        Util.CreateSafeDirectory(newModPath);

                        if (Util.MoveAllFilesAndSubfolders(tempFolder, newModPath, newModPath))
                            Util.MoveDirectory(newModPath, Path.Combine(root, Path.GetFileNameWithoutExtension(file)));
                    }
                    else
                    {
                        string[] subfolders = Directory.GetDirectories(tempFolder);
     
                        //move all folders in mods folder (sometimes a zip can have multiple mods)
                        foreach (var subfolder in subfolders)
                        {
                            string dest = Path.Combine(App.CurrentGame.modDirectory, Path.GetFileNameWithoutExtension(subfolder));
                            if (!Directory.Exists(dest))
                            {
                                Util.MoveDirectory(subfolder, dest);
                            }
                            else
                            {
                                Util.MoveAllFilesAndSubfolders(subfolder, dest, dest);
                            }
                        }
                    }
                    index++;
                }

                Util.ClearTempFolder();
                ((MainWindow)App.Current.MainWindow).Save();
                ((MainWindow)Application.Current.MainWindow).Refresh();

            }
            catch
            {
                if (index > 0 && path.Length <= 1)
                {
                    index = 0;
                }

                throw new Exception("Failed to install one mod. " + path[index]);
            }
        }

        public async Task InstallMod(string[] path, string root)
        {
            if (File.Exists(path[0]))
                await InstallModOptionsArchive(path, root);
            else if (Directory.Exists(path[0]))
                InstallModDirectory(path[0], root);
        }

        public static void InstallModDirectory(string path, string root)
        {
            if (path == root || path.Contains(root)) //prevent user from trying to install a mod that is already installed.
            {
                MessageWindow msg = new(Lang.GetString("MessageWindow.DefaultTitle.Error"), Lang.GetString("MessageWindow.Errors.DontDoThat"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning);
                msg.ShowDialog();
                return;
            }

            // A list of folders that have mod.ini in them
            var directories = new List<string>();

            // Looks though all the folders for mods
            directories.AddRange(Directory.GetDirectories(path, "*", SearchOption.AllDirectories)
                .Where(t => File.Exists(Path.Combine(t, "mod.ini"))));

            // Checks if there is a file called "mod.ini" inside the selected folder
            if (File.Exists(Path.Combine(path, "mod.ini")))
                directories.Add(path);

            // Check if there is any mods
            if (directories.Count > 0)
            {
                foreach (string folder in directories)
                {
                    string directoryName = Path.GetFileName(folder);

                    // If it doesn't know the name of the mod its installing

                    // Creates all of the directories.
                    Util.CreateSafeDirectory(Path.Combine(root, Path.GetFileName(folder)));
                    foreach (string dirPath in Directory.GetDirectories(folder, "*", SearchOption.AllDirectories))
                        Util.CreateSafeDirectory(dirPath.Replace(folder, Path.Combine(root, directoryName)));

                    // Copies all the files from the Directories.
                    foreach (string filePath in Directory.GetFiles(folder, "*.*", SearchOption.AllDirectories))
                        File.Copy(filePath, filePath.Replace(folder, Path.Combine(root, directoryName)), true);
                }
            }

            Util.ClearTempFolder();
            ((MainWindow)App.Current.MainWindow).Save();
            ((MainWindow)Application.Current.MainWindow).Refresh();
        }
    }
}
