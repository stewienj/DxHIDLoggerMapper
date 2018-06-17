using System;
using System.Collections.Generic;
using System.ComponentModel;
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

namespace HIDMapperGui {
  /// <summary>
  /// Interaction logic for DevicesControl.xaml
  /// </summary>
  public partial class DebugDevicesControl : UserControl, INotifyPropertyChanged {
    private DevicesManager _devicesManager;

    public DebugDevicesControl() {
      _devicesManager = new DevicesManager(this.Dispatcher);
      OnPropertyChanged("DevicesManager");
      this.DataContext = _devicesManager;
      _devicesManager.Devices.CollectionChanged += Devices_CollectionChanged;
      InitializeComponent();
    }

    void Devices_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e) {
      if (_deviceTabs.SelectedItem == null && _deviceTabs.Items.Count > 0) {
        _deviceTabs.SelectedIndex = 0;
      }
    }

    private void DevicesControl_Unloaded(object sender, RoutedEventArgs e) {
      _devicesManager.Stop();
    }


    private void Start_Click(object sender, RoutedEventArgs e) {
      _devicesManager.Start();
    }

    private void Stop_Click(object sender, RoutedEventArgs e) {
      _devicesManager.Stop();
    }

    public DevicesManager DevicesManager {
      get {
        return _devicesManager;
      }
    }

    private void OnPropertyChanged(string propertyName) {
      if (PropertyChanged != null) {
        PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
      }
    }

    public event PropertyChangedEventHandler PropertyChanged;

  }
}
