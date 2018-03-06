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

namespace HIDLoggerGui {
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window {
    public MainWindow() {
      this.Loaded += MainWindow_Loaded;
      this.Closing += MainWindow_Closing;
      InitializeComponent();
    }

    void MainWindow_Loaded(object sender, RoutedEventArgs e) {
      if (!_devicesControl.DevicesManager.Running) {
        _devicesControl.DevicesManager.Start();
      }
    }

    void MainWindow_Closing(object sender, System.ComponentModel.CancelEventArgs e) {
      _devicesControl.DevicesManager.Stop();
    }

    private void Start_Click(object sender, RoutedEventArgs e) {
      _devicesControl.DevicesManager.Start();
    }
    private void Stop_Click(object sender, RoutedEventArgs e) {
      _devicesControl.DevicesManager.Stop();
    }
    private void ChangeLogDirectory_Click(object sender, RoutedEventArgs e) {
      _devicesControl.ChangeLogDirectory();
    }
  }
}
