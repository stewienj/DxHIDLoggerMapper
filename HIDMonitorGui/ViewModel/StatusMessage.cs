using System;
using System.ComponentModel;

namespace HIDMonitorGui.ViewModel
{
  /// <summary>
  /// Status messages from the HID Logger. This should be viewed in a GridView
  /// </summary>
  public class StatusMessage : NotifyPropertyChanged
  {
    private string _signature;

    public StatusMessage(string deviceName, string message)
    {
      _signature = deviceName + message;
      DeviceName = deviceName;
      Message = message;
    }

    public void Update()
    {
      LastUpdate = DateTime.Now;
      UpdateCount++;
      RaisePropertyChanged("LastUpdate");
      RaisePropertyChanged("UpdateCount");
    }

    public string DeviceName
    {
      get;
      private set;
    }
    public string Message
    {
      get;
      private set;
    }

    public int UpdateCount
    {
      get;
      set;
    }

    public DateTime LastUpdate
    {
      get;
      set;
    }

    public override bool Equals(object? obj)
    {
      if (obj is StatusMessage)
      {
        return ((StatusMessage)obj)._signature == _signature;
      }
      else
      {
        return false;
      }
    }

    public override int GetHashCode()
    {
      return _signature.GetHashCode();
    }
  }
}
