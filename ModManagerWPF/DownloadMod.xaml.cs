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

namespace ModManagerWPF
{
	/// <summary>
	/// Interaction logic for DownloadMod.xaml
	/// </summary>
	public partial class DownloadMod : Window
	{
		public DownloadMod()
		{
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
				/*MessageBox.Show(this,
								$"Malformed One-Click Install URI \"{uri}\" caused parse failure:\n{ex.Message}",
								"URI Parse Failure",
								MessageBoxButton.OK,
								MessageBoxIcon.Error);*/

				return;
			}

			GameBananaItem gbi;

			try
			{
				gbi = GameBananaItem.Load(itemType, itemId);

				if (gbi is null)
				{
					throw new Exception("GameBananaItem was unexpectedly null");
				}

				TextModName.Text = gbi.Name;
				string color = GetNewColor("ForegroundBrush");
				TextModDescription.Text = "<p style=\"color:" + color + ";\">" + gbi.Body + "</p>";

				Dictionary<string, List<GameBananaCredit>> credits = gbi.Credits.Credits;

				foreach (var CreditCategory in credits)
				{
					TextBlock TexCreditCategory = new()
					{
						Text = CreditCategory.Key,
						FontSize = 12,
						TextWrapping = TextWrapping.WrapWithOverflow,
						Foreground = ThemeBrush.GetThemeBrush("ForegroundBrush"),
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
							Foreground = ThemeBrush.GetThemeBrush("ForegroundBrush"),
							FontWeight = FontWeights.Bold,
							Padding = new Thickness(0, 4, 0, .5),
						};

						CreditsPanel.Children.Add(AuthorName);
					}
				}

			}
			catch (Exception ex)
			{
				/*MessageBox.Show(this,
								$"GameBanana API query failed:\n{ex.Message}",
								"GameBanana API Failure",
								MessageBoxButtons.OK,
								MessageBoxIcon.Error);*/

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

			Uri url;

			string[] split = uri.Substring("sadxmm:".Length).Split(',');


			url = new Uri(split[0]);
			Dictionary<string, string> fields = new Dictionary<string, string>(split.Length - 1);
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
				/*MessageBox.Show(this,
								$"One-Click Install URI \"{uri}\" did not contain required fields.",
								"URI Parse Failure",
								MessageBoxButtons.OK,
								MessageBoxIcon.Error);*/

				return;
			}



			this.Show();
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

		}
	}
}
