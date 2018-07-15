using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace HIDMapperGui
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {

    private void Application_Startup(object sender, StartupEventArgs e)
    {
      this.DispatcherUnhandledException += App_DispatcherUnhandledException;
#if DEBUG
      MainWindow = new DebugWindow();
#else
      MainWindow = new MainWindow();
#endif
      MainWindow.Show();
    }

    private void App_DispatcherUnhandledException(object sender, System.Windows.Threading.DispatcherUnhandledExceptionEventArgs e)
    {
      e.Handled = true;
    }
  }
}
