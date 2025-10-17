using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Logging;
using Avalonia.Markup.Xaml;
using DialogHostAvalonia;
using SAMM.App.Models;
using SAMM.App.Views;
using System;
using System.Linq;
using SAMM.App.Controls;
using SAMM.App.Controls.Dialogs;
using SAMM.Utilities;
using SAMM.App;

namespace SAMM.App;

public partial class App : Application
{
    public ApplicationModel AppSettings { get { return (ApplicationModel)DataContext; } }

    public static IClassicDesktopStyleApplicationLifetime? GetDesktopLifetime()
    {
        if (Application.Current.ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
            return desktop;
        else
            return null;
    }

    public override void Initialize()
    {
        AvaloniaXamlLoader.Load(this);

        DataContext = new ApplicationModel();
    }

    public override void OnFrameworkInitializationCompleted()
    {
		if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
        {
            desktop.MainWindow = new MainWindow(desktop.Args);
        }
        else if (ApplicationLifetime is ISingleViewApplicationLifetime singleViewPlatform)
        {
            singleViewPlatform.MainView = new MainWindow(null)
            {
                DataContext = new MainWindowViewModel()
            };
        }

        base.OnFrameworkInitializationCompleted();
    }

    /// <summary>
    /// Gets a Resource from the Current Application.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="resourceName"></param>
    /// <returns>The requested object is returned if resource key is found, else returns the default of <see cref="T"/></returns>
    public static T GetResource<T>(string resourceName)
    {
        if (!string.IsNullOrEmpty(resourceName))
            return (T)Current.FindResource(resourceName);
        else
            return default(T);
    }

    /// <summary>
    /// Gets a string for a corresponding key.
    /// </summary>
    /// <param name="resourceKey">Returns the resource if the key is found. If it's not found, the key itself is returned.</param>
    /// <returns></returns>
    public static string GetSring(string resourceKey)
    {
        string output = GetResource<string>(resourceKey);
        if (string.IsNullOrEmpty(output))
            return output;
        else
            return resourceKey;
    }
}
