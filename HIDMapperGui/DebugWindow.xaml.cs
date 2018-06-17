using HIDMapperGui.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace HIDMapperGui
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class DebugWindow : Window
  {
    private Workspace _workspace;
    public DebugWindow()
    {
      this.DataContextChanged += DebugWindow_DataContextChanged;
      this.Loaded += MainWindow_Loaded;
      this.Closing += MainWindow_Closing;
      InitializeComponent();
    }

    private void DebugWindow_DataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
    {
      _workspace = (Workspace)DataContext;
    }

    void MainWindow_Loaded(object sender, RoutedEventArgs e)
    {
      if (!_workspace.DevicesManager.Running)
      {
        _workspace.DevicesManager.Start();
      }
    }

    void MainWindow_Closing(object sender, System.ComponentModel.CancelEventArgs e)
    {
      _workspace.DevicesManager.Stop();
    }

    private void Start_Click(object sender, RoutedEventArgs e)
    {
      _workspace.DevicesManager.Start();
    }
    private void Stop_Click(object sender, RoutedEventArgs e)
    {
      _workspace.DevicesManager.Stop();
    }
  }
}
