using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HIDLoggerGui
{
  /// <summary>
  /// Status messages from the HID Logger. This should be viewed in a GridView
  /// </summary>
  public class StatusMessage : INotifyPropertyChanged
  {
    private string _signature;

    public StatusMessage(string deviceName, string message) : this(Environment.MachineName, deviceName, message) { }

    public StatusMessage(string machineName, string deviceName, string message)
    {
      _signature = machineName + deviceName + message;
      MachineName = machineName;
      DeviceName = deviceName;
      Message = message;
    }

    public void Update()
    {
      LastUpdate = DateTime.Now;
      UpdateCount++;
      OnPropertyChanged("LastUpdate");
      OnPropertyChanged("UpdateCount");
    }

    public string MachineName
    {
      get;
      private set;
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

    public override bool Equals(object obj)
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

    public void OnPropertyChanged(string propertyName)
    {
      if (PropertyChanged != null)
      {
        PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
      }
    }

    public event PropertyChangedEventHandler PropertyChanged;

  }
}