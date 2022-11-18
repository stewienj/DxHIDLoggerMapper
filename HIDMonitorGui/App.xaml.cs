using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace HIDMonitorGui
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {

    private void Application_Startup(object sender, StartupEventArgs e)
    {
      DispatcherUnhandledException += App_DispatcherUnhandledException;
      MainWindow = new DebugWindow();
      MainWindow.Show();
    }

    private void App_DispatcherUnhandledException(object sender, System.Windows.Threading.DispatcherUnhandledExceptionEventArgs e)
    {
      e.Handled = true;
    }
  }
}
