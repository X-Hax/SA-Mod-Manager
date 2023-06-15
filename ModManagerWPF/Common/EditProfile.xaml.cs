using IniFile;
using ModManagerWPF.Game;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using static ModManagerWPF.MainWindow;

namespace ModManagerWPF.Common
{
	/// <summary>
	/// Interaction logic for EditProfile.xaml
	/// </summary>
	/// 
	public class EditProfileResult
	{
		public char Index { get; set; }
		public string Name { get; set; }
	}

	public partial class EditProfile : Window
	{
		private char index { get; set; }
		private bool isEditing { get; set; } = false;

		public EditProfileResult Result { get; private set; }

		private void CloseEditProfile()
		{
			Result = new EditProfileResult
			{
				Index = index, 
				Name = ProfileNameTextbox.Text
			};

			DialogResult = true; // Set the dialog result to true to indicate success
		}

	
		public Profile NewProResult { get; private set; }

		public EditProfile(string item, char index)
		{
			InitializeComponent();
			Title = Lang.GetString("TitleRenameModProfile");
			Header.Text = Title;
			ProfileNameTextbox.Text = item;
			ProfileNameTextbox.Focusable = true;
			this.index = index;
			Keyboard.Focus(ProfileNameTextbox);
			isEditing = true;
		}

		private void CloseNewProfile()
		{
		    DialogResult = true; // Set the dialog result to true to indicate success
		}

		public EditProfile()
		{
			InitializeComponent();
			Title = Lang.GetString("TitleCreateModProfile");
			Header.Text = Title;
			ProfileNameTextbox.Focusable = true;
			Keyboard.Focus(ProfileNameTextbox);
		}

		private async void UI_OK_Click(object sender, RoutedEventArgs e)
		{
			if (isEditing)
			{
				CloseEditProfile();
			}
			else
			{
				string name = ProfileNameTextbox.Text;
				string fullPath = Path.Combine(modDirectory, name + ".ini");

				if (File.Exists(fullPath)) 
				{
					MessageBox.Show(Lang.GetString("ProfileDuplicateError"), Lang.GetString("ProfileDuplicateErrorTitle"), MessageBoxButton.OK, MessageBoxImage.Error);			
					return;
				}

				((MainWindow)Application.Current.MainWindow).Save();
				await Task.Delay(30);
				File.Copy(loaderinipath, fullPath);
				await Task.Delay(100);

				NewProResult = new()
				{
					iniPath = fullPath,
					name = name
				};
				CloseNewProfile();
			}
		}

		public EditProfileResult GetEditProfileResult()
		{
			return Result;
		}

		public Profile GetNewProfileResult()
		{
			return NewProResult;
		}

		private void UI_Cancel_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
		}
	}
}
