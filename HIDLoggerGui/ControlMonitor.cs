using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HIDLoggerGui
{
  /// <summary>
  /// Monitors individual controls on a device. A list of these will be contained by a device.
  /// </summary>
  public class ControlMonitor : INotifyPropertyChanged
  {

    public ControlMonitor(string controlName)
    {
      ControlName = controlName;
    }

    public string ControlName
    {
      get;
      private set;
    }

    private int _state = -1;
    public int State
    {
      get
      {
        return _state;
      }
      set
      {
        if (value != _state)
        {
          _state = value;
          OnPropertyChanged("State");
          UpdateCount += 1;
          LastUpdate = DateTime.Now;
        }
      }
    }

    private int _updateCount = 0;
    public int UpdateCount
    {
      get
      {
        return _updateCount;
      }
      private set
      {
        if (value != _updateCount)
        {
          _updateCount = value;
          OnPropertyChanged("UpdateCount");
        }
      }
    }

    private DateTime _lastUpdate = DateTime.Now;
    public DateTime LastUpdate
    {
      get
      {
        return _lastUpdate;
      }
      private set
      {
        if (value != _lastUpdate)
        {
          _lastUpdate = value;
          OnPropertyChanged("LastUpdate");
        }
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