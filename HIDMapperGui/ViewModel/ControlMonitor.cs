using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HIDMapperGui
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

    public void Update(int state, int previousState)
    {
      State = state;
      PreviousState = previousState;
      UpdateCount += 1;
      LastUpdate = DateTime.Now;
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
      private set
      {
        if (value != _state)
        {
          _state = value;
          OnPropertyChanged("State");
        }
      }
    }

    private int _previousState = -1;
    public int PreviousState
    {
      get
      {
        return _previousState;
      }
      private set
      {
        if (value != _previousState)
        {
          _previousState = value;
          OnPropertyChanged("PreviousState");
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