using HIDMonitorGui.ViewModel;
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

namespace HIDMonitorGui.Controls
{
  /// <summary>
  /// Interaction logic for DevicesControl.xaml
  /// </summary>
  public partial class DebugDevicesControl : UserControl, INotifyPropertyChanged
  {
    private DevicesManager? _devicesManager = null;

    public DebugDevicesControl()
    {
      InitializeComponent();
    }

    public DevicesManager DevicesManager
    {
      get { return (DevicesManager)GetValue(DevicesManagerProperty); }
      set { SetValue(DevicesManagerProperty, value); }
    }

    // Using a DependencyProperty as the backing store for DevicesManager.  This enables animation, styling, binding, etc...
    public static readonly DependencyProperty DevicesManagerProperty =
        DependencyProperty.Register("DevicesManager", typeof(DevicesManager), typeof(DebugDevicesControl), new PropertyMetadata(null, (d, e) =>
        {
          if (d is DebugDevicesControl control)
          {
            if (e.OldValue is DevicesManager oldManager)
            {
              oldManager.Devices.CollectionChanged -= control.Devices_CollectionChanged;
            }
            if (e.NewValue is DevicesManager newManager)
            {
              newManager.Devices.CollectionChanged += control.Devices_CollectionChanged;
            }

            control._devicesManager = e.NewValue as DevicesManager;
          }
        }));



    void Devices_CollectionChanged(object? sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
    {
      if (_deviceTabs.SelectedItem == null && _deviceTabs.Items.Count > 0)
      {
        _deviceTabs.SelectedIndex = 0;
      }
    }

    private void Start_Click(object sender, RoutedEventArgs e)
    {
      _devicesManager?.Start();
    }

    private void Stop_Click(object sender, RoutedEventArgs e)
    {
      _devicesManager?.Stop();
    }

    private void OnPropertyChanged(string propertyName)
    {
      PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }

    public event PropertyChangedEventHandler? PropertyChanged;

  }
}
