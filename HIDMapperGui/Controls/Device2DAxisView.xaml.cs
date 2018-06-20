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

namespace HIDMapperGui.Controls
{
  /// <summary>
  /// Interaction logic for Device2DAxisView.xaml
  /// </summary>
  public partial class Device2DAxisView : UserControl
  {
    public Device2DAxisView()
    {
      InitializeComponent();
    }

    public DeviceMonitor DeviceMonitor
    {
      get { return (DeviceMonitor)GetValue(DeviceMonitorProperty); }
      set { SetValue(DeviceMonitorProperty, value); }
    }

    // Using a DependencyProperty as the backing store for DeviceMonitor.  This enables animation, styling, binding, etc...
    public static readonly DependencyProperty DeviceMonitorProperty =
        DependencyProperty.Register("DeviceMonitor", typeof(DeviceMonitor), typeof(Device2DAxisView), new PropertyMetadata(null,(s,e)=>
        {
          if (s is Device2DAxisView control && e.NewValue is DeviceMonitor deviceMonitor)
          {
            control.ViewModel.DeviceMonitor = deviceMonitor;
          }
        }));



    public Device2DAxisViewModel ViewModel
    {
      get { return (Device2DAxisViewModel)GetValue(ViewModelProperty); }
      set { SetValue(ViewModelProperty, value); }
    }

    // Using a DependencyProperty as the backing store for ViewModel.  This enables animation, styling, binding, etc...
    public static readonly DependencyProperty ViewModelProperty =
        DependencyProperty.Register("ViewModel", typeof(Device2DAxisViewModel), typeof(Device2DAxisView), new PropertyMetadata(new Device2DAxisViewModel(), (s,e)=>
        {
          if (s is Device2DAxisView control && e.NewValue is Device2DAxisViewModel viewModel)
          {
            control.DeviceMonitor = viewModel.DeviceMonitor;
          }
        }));
  }
}
