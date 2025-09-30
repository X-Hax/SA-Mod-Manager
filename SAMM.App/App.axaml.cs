using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Markup.Xaml;
using SAMM.App.Models;


//using SAMM.App.ViewModels;
using SAMM.App.Views;

namespace SAMM.App;

public partial class App : Application
{
    public override void Initialize()
    {
        AvaloniaXamlLoader.Load(this);
    }

    public override void OnFrameworkInitializationCompleted()
    {
        if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
        {
            desktop.MainWindow = new MainWindow();
        }
        else if (ApplicationLifetime is ISingleViewApplicationLifetime singleViewPlatform)
        {
            singleViewPlatform.MainView = new MainWindow
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
