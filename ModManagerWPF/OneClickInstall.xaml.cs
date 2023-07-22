using ModManagerCommon.Forms;
using ModManagerCommon;
using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using ModManagerWPF.Themes;
using System.Drawing;
using System.Windows.Media;
using System.Linq;
using System.Windows.Documents;
using System.Security.Policy;
using Octokit;
using System.Xml.Linq;
using System.IO;
using ModManagerWPF.Common;
using System.Windows.Interop;
using System.Diagnostics;
using static System.Runtime.InteropServices.JavaScript.JSType;
using System.Threading;
using System.Threading.Tasks;

namespace ModManagerWPF
{
	/// <summary>
	/// Interaction logic for OneClickInstall.xaml
	/// </summary>
	public partial class OneClickInstall : Window
	{

		private Uri url;
		private GameBananaItem gbi;
		private string author;
		private string updatePath;
		private string modPath;
		private Dictionary<string, string> fields;

		public OneClickInstall(string updatePath, string modPath)
		{
			this.updatePath = updatePath;
			this.modPath = modPath;
			InitializeComponent();

			List<string> uris = App.UriQueue.GetUris();

			foreach (string str in uris)
			{
				HandleUri(str);
			}

			App.UriQueue.UriEnqueued += UriQueueOnUriEnqueued;

		}


		private void HandleGBMod(Dictionary<string, string> fields)
		{
			string itemType;
			long itemId;

			try
			{
				itemType = fields["gb_itemtype"];
				itemId = long.Parse(fields["gb_itemid"]);
			}
			catch (Exception ex)
			{
				string msg = Lang.GetString($"MessageWindow.Errors.URIParseFail0") + "\"{uri}\"" + Lang.GetString("MessageWindow.Errors.URIParseFail1");

				new MessageWindow(Lang.GetString("MessageWindow.Errors.URIParseFail.Title"), msg, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();


				return;
			}


			try
			{
				gbi = GameBananaItem.Load(itemType, itemId);

				if (gbi is null)
				{
					throw new Exception("GameBananaItem was unexpectedly null");
				}

				TextModName.Text = gbi.Name;
				string color = GetNewColor("Colors.Text");
				TextModDescription.Text = "<p style=\"color:" + color + ";\">" + gbi.Body + "</p>";

				Dictionary<string, List<GameBananaCredit>> credits = gbi.Credits.Credits;

				foreach (var CreditCategory in credits)
				{
					author = CreditCategory.Value[0].MemberName;

					TextBlock TexCreditCategory = new()
					{
						Text = CreditCategory.Key,
						FontSize = 12,
						TextWrapping = TextWrapping.WrapWithOverflow,
						Foreground = ThemeBrush.GetThemeBrush("TextBox.Brushes.Foreground"),
						Padding = new Thickness(0, 4, 0, 5),
					};

					CreditsPanel.Children.Add(TexCreditCategory);

					for (int i = 0; i < CreditCategory.Value.Count; i++)
					{
						TextBlock AuthorName = new()
						{
							Text = CreditCategory.Value[i].MemberName,
							FontSize = 14,
							TextWrapping = TextWrapping.WrapWithOverflow,
							Foreground = ThemeBrush.GetThemeBrush("TextBox.Brushes.Foreground"),
							FontWeight = FontWeights.Bold,
							Padding = new Thickness(0, 4, 0, .5),
						};

						CreditsPanel.Children.Add(AuthorName);
					}
				}

			}
			catch (Exception ex)
			{
				new MessageWindow(Lang.GetString("MessageWindow.Errors.GBAPIFail.Title"), Lang.GetString("MessageWindow.Errors.GBAPIFail"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();

				return;

			}
		}

		private void HandleUri(string uri)
		{
			if (WindowState == WindowState.Minimized)
			{
				WindowState = WindowState.Normal;
			}

			Activate();


			string[] split = uri.Substring("sadxmm:".Length).Split(',');


			url = new Uri(split[0]);
			fields = new Dictionary<string, string>(split.Length - 1);
			for (int i = 1; i < split.Length; i++)
			{
				int ind = split[i].IndexOf(':');
				fields.Add(split[i].Substring(0, ind).ToLowerInvariant(), split[i].Substring(ind + 1));
			}

			if (fields.ContainsKey("gb_itemtype") && fields.ContainsKey("gb_itemid"))
			{
				HandleGBMod(fields);
			}
			else if (fields.ContainsKey("name") && fields.ContainsKey("author"))
			{
				string name = Uri.UnescapeDataString(fields["name"]);
				string author = Uri.UnescapeDataString(fields["author"]);

				TextModName.Text = name;
				TextModAuthor.Text = author;
			}
			else
			{
				string error = Lang.GetString("MessageWindow.Errors.URIParseFail0 ") + "\"{uri}\" " + Lang.GetString("MessageWindow.Errors.URIParseFailFields1");
				new MessageWindow(Lang.GetString("MessageWindow.Errors.URIParseFail.Title"), error, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();

				return;
			}



			this.ShowDialog();
		}


		private void UriQueueOnUriEnqueued(object sender, OnUriEnqueuedArgs args)
		{
			args.Handled = true;

			if (!Dispatcher.CheckAccess())
			{
				Dispatcher.Invoke((Action<object, OnUriEnqueuedArgs>)UriQueueOnUriEnqueued, sender, args);
				return;
			}

			HandleUri(args.Uri);
		}


		public static string GetNewColor(string brushname)
		{
			var fgBrush = ThemeBrush.GetThemeColor(brushname);
			string hexColor = $"#{fgBrush.R:X2}{fgBrush.G:X2}{fgBrush.B:X2}";
			return hexColor;
		}

		private void ButtonCancel_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
		}

		private void OpenGB_Click(object sender, RoutedEventArgs e)
		{
			var ps = new ProcessStartInfo("https://github.com/X-Hax/sadx-mod-loader")
			{
				UseShellExecute = true,
				Verb = "open"
			};
			Process.Start(ps);
		}

		private async void ButtonDownload_Click(object sender, RoutedEventArgs e)
		{

			//To do add support for retry with custom msg box

			bool retry = false;

			do
			{
				try
				{
					if (!Directory.Exists(updatePath))
					{
						Directory.CreateDirectory(updatePath);
					}
				}
				catch (Exception ex)
				{
					var s = Lang.GetString("MessageWindow.Errors.DirectoryCreateFail0") + ":\n" + ex.Message + "\n\n" + Lang.GetString("MessageWindow.Errors.DirectoryCreateFail1");
					var msg = new MessageWindow(Lang.GetString("MessageWindow.Errors.DirectoryCreateFail.Title"), s, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.RetryCancel);
					msg.ShowDialog();
					retry = msg.isRetry;

				}
			} while (retry == true);

			var dummyInfo = new ModInfo
			{
				Name = TextModName.Text,
				Author = author
			};

			string dummyPath = dummyInfo.Name;

			if (fields is not null && fields.ContainsKey("folder"))
				dummyPath = fields["folder"];

			foreach (char c in Path.GetInvalidFileNameChars())
			{
				dummyPath = dummyPath.Replace(c, '_');
			}

			dummyPath = Path.Combine(modPath, dummyPath);


			var updates = new List<ModDownloadWPF>
			{
				new ModDownloadWPF(dummyInfo, dummyPath, url.AbsoluteUri, null, 0)
			};

			new Updater.ModDownloadDialogWPF(updates, updatePath).ShowDialog();


			await Task.Delay(2000);
			this.Hide();
			await Task.Delay(1000);
			if (Directory.Exists(updatePath))
			{
				try
				{
					Directory.Delete(updatePath, true);
				}
				catch
				{}
			}


			((MainWindow)App.Current.MainWindow).Refresh();
			this.Close();
		}
	}
}
