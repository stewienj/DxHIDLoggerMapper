using HIDMonitorDLL;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace HIDMonitorGui.ViewModel
{
  public class DeviceMonitor : NotifyPropertyChanged
  {
    private ObservableCollection<ControlMonitor> _controls = new ObservableCollection<ControlMonitor>();
    private Dictionary<string, ControlMonitor> _nameToControlMonitor = new Dictionary<string, ControlMonitor>();
    private HIDMonitorInterface _monitor;

    public DeviceMonitor(string deviceName, DeviceType deviceType, Guid guid, HIDMonitorInterface monitor)
    {
      DeviceName = deviceName;
      DeviceGuid = guid;
      _monitor = monitor;
    }

    public void UpdateControl(string controlName, int state, int previousState)
    {
      if (controlName.IndexOf("Button", StringComparison.Ordinal) == 0)
      {
        string number = controlName.Substring("Button".Length);
        while (number.Length < 4)
        {
          number = " " + number;
        }
        controlName = "Button" + number;
      }

      if (_nameToControlMonitor.TryGetValue(controlName, out var controlMonitor))
      {
        controlMonitor.Update(state, previousState);
      }
      else
      {
        controlMonitor = new ControlMonitor(controlName);
        controlMonitor.Update(state, previousState);
        _nameToControlMonitor.Add(controlName, controlMonitor);
        _controls.Add(controlMonitor);
      }
    }

    public override string ToString()
    {
      return DeviceName;
    }

    public string DeviceName
    {
      get;
      private set;
    }

    public Guid DeviceGuid
    {
      get;
      private set;
    }

    public ObservableCollection<ControlMonitor> Controls
    {
      get
      {
        return _controls;
      }
    }
  }
}
