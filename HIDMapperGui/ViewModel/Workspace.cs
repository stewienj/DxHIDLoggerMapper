using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HIDMapperGui.ViewModel
{
  public class Workspace : NotifyPropertyChanged
  {
    public Workspace()
    {
      DevicesManager = new DevicesManager(App.Current.Dispatcher);
    }

    public Workspace(bool suppressMapping)
    {
      DevicesManager = new DevicesManager(App.Current.Dispatcher, suppressMapping);
    }

    public string Title => "Joystick Mapper V1.0";

    private DevicesManager _devicesManager = null;
    public DevicesManager DevicesManager
    {
      get => _devicesManager;
      set => SetValue(ref _devicesManager, value);
    }

  }
}
