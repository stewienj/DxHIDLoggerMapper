using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using HIDMapperDLL;

namespace HIDMapperGui
{
  public class DeviceMonitor : INotifyPropertyChanged
  {
    private ObservableCollection<ControlMonitor> _controls = new ObservableCollection<ControlMonitor>();
    private Dictionary<string, ControlMonitor> _nameToControlMonitor = new Dictionary<string, ControlMonitor>();

    public DeviceMonitor(string deviceName)
    {
      DeviceName = deviceName;
    }

    public void UpdateControl(string controlName, int state, int previousState)
    {
      if (controlName.IndexOf("Button") == 0)
      {
        string number = controlName.Substring("Button".Length);
        while (number.Length < 4)
        {
          number = " " + number;
        }
        controlName = "Button" + number;
      }

      ControlMonitor controlMonitor = null;
      if (_nameToControlMonitor.TryGetValue(controlName, out controlMonitor))
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


    public ObservableCollection<ControlMonitor> Controls
    {
      get
      {
        return _controls;
      }
    }

    private void OnPropertyChanged(string propertyName)
    {
      if (PropertyChanged != null)
      {
        PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
      }
    }

    public event PropertyChangedEventHandler PropertyChanged;
  }
}