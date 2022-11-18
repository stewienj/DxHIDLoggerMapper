using HIDMonitorDLL;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Threading;
using System.Windows.Threading;

namespace HIDMonitorGui.ViewModel
{
  public class DevicesManager : NotifyPropertyChanged
  {
    private Dictionary<StatusMessage, StatusMessage> _messagesDictionary = new Dictionary<StatusMessage, StatusMessage>();
    private Dictionary<string, DeviceMonitor> _nameToDeviceMonitor = new Dictionary<string, DeviceMonitor>();
    private HIDMonitorInterface _monitor = new HIDMonitorInterface();
    private BlockingCollection<Action> _actionQueue = new BlockingCollection<Action>();
    private Dispatcher _dispatcher;

    public DevicesManager(Dispatcher dispatcher)
    {
      _dispatcher = dispatcher;
      _monitor.DeviceChanged += Monitor_DeviceChanged;
      _monitor.HIDStateChanged += Monitor_ControlStateChanged;
      _monitor.StartedOrStopped += Monitor_StartedOrStopped;
      StartMappingProcessor();
    }

    private void StartMappingProcessor()
    {
      Thread actionThread = new Thread(new ThreadStart(() =>
      {
        try
        {
          foreach (Action action in _actionQueue.GetConsumingEnumerable())
          {
            // Wait to join to the dispatcher thread
            _dispatcher.Invoke((Action)(() =>
            {
              try
              {
                action();

                // Clear the more of the action queue, up to 100 items at a time.
                // Batch up processing into lots of 100 so as to give some
                // responsiveness if the collection is being bombarded.
                int countDown = 100;

                // Note that countDown must be tested first, otherwise we throw away a queue item
                while (countDown > 0 && _actionQueue.TryTake(out var nextCommand))
                {
                  --countDown;
                  try
                  {
                    nextCommand();
                  }
                  catch (Exception ex)
                  {
                    ErrorMessages.Add($"Error: Device Action {ex.Message}");
                  }
                }
              }
              catch (Exception ex)
              {
                ErrorMessages.Add($"Error: Device Action {ex.Message}");
              }
            }));
          }
        }
        catch (Exception)
        {

        }
      }));
      actionThread.IsBackground = true;
      actionThread.Start();
    }

    private DeviceMonitor AddOrGetDevice(string deviceName, DeviceType deviceType, Guid deviceGuid)
    {
      if (_nameToDeviceMonitor.TryGetValue(deviceName, out var deviceMonitor))
      {
        return deviceMonitor;
      }
      else
      {
        deviceMonitor = new DeviceMonitor(deviceName, deviceType, deviceGuid, _monitor);
        _nameToDeviceMonitor.Add(deviceName, deviceMonitor);
        Devices.Add(deviceMonitor);
        switch (deviceType)
        {
          case DeviceType.Unknown:
            UnknownDevices.Add(deviceMonitor);
            break;
          case DeviceType.Keyboard:
            KeyboardDevices.Add(deviceMonitor);
            break;
          case DeviceType.Mouse:
            MouseDevices.Add(deviceMonitor);
            break;
          case DeviceType.Joystick:
            JoystickDevices.Add(deviceMonitor);
            break;
        }
        return deviceMonitor;
      }
    }

    void Monitor_ControlStateChanged(object? sender, HIDStateChangeArgs e)
    {
      _actionQueue.Add(new Action(() =>
      {
        DeviceMonitor deviceMonitor = AddOrGetDevice(e.Device, e.DeviceType, e.DeviceGuid);
        deviceMonitor.UpdateControl(e.Control, e.State, e.PreviousState);
        OnControlStateChanged(e);
      }));
    }

    void Monitor_DeviceChanged(object? sender, DeviceInfo e)
    {
      Action? action = e.InfoAction switch
      {
        DeviceInfo.InfoType.Added => () =>
          {
            AddMessage(new StatusMessage(e.Device, "Added"));
            AddOrGetDevice(e.Device, e.DeviceType, e.DeviceGuid);
          }
        ,
        DeviceInfo.InfoType.Checked => () =>
          {
            AddMessage(new StatusMessage(e.Device, "Checked"));
            AddOrGetDevice(e.Device, e.DeviceType, e.DeviceGuid);
          }
        ,
        DeviceInfo.InfoType.Error => () =>
          {
            AddMessage(new StatusMessage(e.Device, "Error"));
            if (e is DeviceInfoError)
            {
              AddMessage(new StatusMessage(e.Device, ((DeviceInfoError)e).ErrorMessage));
            }
          }
        ,
        DeviceInfo.InfoType.Removed => () =>
          {
            AddMessage(new StatusMessage(e.Device, "Removed"));
            if (_nameToDeviceMonitor.TryGetValue(e.Device, out var deviceMonitor))
            {
              _nameToDeviceMonitor.Remove(e.Device);
              Devices.Remove(deviceMonitor);
              switch (e.DeviceType)
              {
                case DeviceType.Unknown:
                  UnknownDevices.Remove(deviceMonitor);
                  break;
                case DeviceType.Keyboard:
                  KeyboardDevices.Remove(deviceMonitor);
                  break;
                case DeviceType.Mouse:
                  MouseDevices.Remove(deviceMonitor);
                  break;
                case DeviceType.Joystick:
                  JoystickDevices.Remove(deviceMonitor);
                  break;
              }
            }
          }
        ,
        DeviceInfo.InfoType.Updated => () => AddMessage(new StatusMessage(e.Device, "Updated")),
        _ => null,
      };
      if (action != null)
      {
        _actionQueue.Add(action);
      }
    }

    private void AddMessage(string message)
    {
      AddMessage(new StatusMessage("", message));
    }

    private void AddMessage(StatusMessage statusMessage)
    {
      if (_messagesDictionary.TryGetValue(statusMessage, out var storedMessage))
      {
        storedMessage.Update();
      }
      else
      {
        Messages.Add(statusMessage);
        _messagesDictionary.Add(statusMessage, statusMessage);
        statusMessage.Update();
      }
    }

    void Monitor_StartedOrStopped(object? sender, StartStopStatus e)
    {
      _actionQueue.Add(() =>
      {
        switch (e.StartedOrStopped)
        {
          case StartStopStatus.StartStopType.Started:
            Running = true;
            AddMessage("Mapping Started");
            break;
          case StartStopStatus.StartStopType.Stopped:
            Running = false;
            AddMessage("Mapping Stopped");
            break;
        }
      });
    }

    private bool _running = false;
    public bool Running
    {
      get => _running;
      set
      {
        if (value != _running)
        {
          _running = value;
          RaisePropertyChanged("Running");
          RaisePropertyChanged("NotRunning");
        }
      }
    }

    public void Start()
    {
      _monitor.StartMonitoring();
    }

    public void Stop()
    {
      _monitor.StopMonitoring();
    }

    private void OnControlStateChanged(HIDStateChangeArgs args)
    {
      ControlStateChanged?.Invoke(this, args);
    }

    public bool NotRunning => !_running;

    public ObservableCollection<DeviceMonitor> Devices { get; } = new ObservableCollection<DeviceMonitor>();

    public ObservableCollection<DeviceMonitor> UnknownDevices { get; } = new ObservableCollection<DeviceMonitor>();

    public ObservableCollection<DeviceMonitor> KeyboardDevices { get; } = new ObservableCollection<DeviceMonitor>();

    public ObservableCollection<DeviceMonitor> MouseDevices { get; } = new ObservableCollection<DeviceMonitor>();

    public ObservableCollection<DeviceMonitor> JoystickDevices { get; } = new ObservableCollection<DeviceMonitor>();

    public ObservableCollection<StatusMessage> Messages { get; } = new ObservableCollection<StatusMessage>();
    public ObservableCollection<string> ErrorMessages { get; } = new ObservableCollection<string>();

    public bool KeyboardMonitoringEnabled
    {
      get => _monitor.KeyboardMonitoringEnabled;
      set => _monitor.KeyboardMonitoringEnabled = value;
    }
    public bool MouseMonitoringEnabled
    {
      get => _monitor.MouseMonitoringEnabled;
      set => _monitor.MouseMonitoringEnabled = value;
    }
    public bool JoystickMonitoringEnabled
    {
      get => _monitor.JoystickMonitoringEnabled;
      set => _monitor.JoystickMonitoringEnabled = value;
    }

    public event EventHandler<HIDStateChangeArgs>? ControlStateChanged;
  }
}
