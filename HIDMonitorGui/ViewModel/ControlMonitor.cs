using System;

namespace HIDMonitorGui.ViewModel
{
  /// <summary>
  /// Monitors individual controls on a device. A list of these will be contained by a device.
  /// </summary>
  public class ControlMonitor : NotifyPropertyChanged
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
      get => _state;
      private set => SetValue(ref _state, value);
    }

    private int _previousState = -1;
    public int PreviousState
    {
      get => _previousState;
      private set => SetValue(ref _previousState, value);
    }

    private int _updateCount = 0;
    public int UpdateCount
    {
      get => _updateCount;
      private set => SetValue(ref _updateCount, value);
    }

    private DateTime _lastUpdate = DateTime.Now;
    public DateTime LastUpdate
    {
      get => _lastUpdate;
      private set => SetValue(ref _lastUpdate, value);
    }

  }
}
