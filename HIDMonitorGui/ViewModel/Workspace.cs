namespace HIDMonitorGui.ViewModel
{
  public class Workspace : NotifyPropertyChanged
  {
    public Workspace()
    {
      _devicesManager = new DevicesManager(App.Current.Dispatcher);
    }

    private DevicesManager _devicesManager;
    public DevicesManager DevicesManager
    {
      get => _devicesManager;
      set => SetValue(ref _devicesManager, value);
    }

  }
}
