using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Threading;
using HIDMapperDLL;

namespace HIDMapperGui
{
  public class DevicesManager : INotifyPropertyChanged
  {
    private Dictionary<StatusMessage, StatusMessage> _messagesDictionary = new Dictionary<StatusMessage, StatusMessage>();
    private Dictionary<string, DeviceMonitor> _nameToDeviceMonitor = new Dictionary<string, DeviceMonitor>();
    private HIDMapperInterface _mapper = new HIDMapperInterface();
    private BlockingCollection<Action> _actionQueue = new BlockingCollection<Action>();
    private Dispatcher _dispatcher;

    public DevicesManager(Dispatcher dispatcher, bool suppressMapping = false)
    {
      _dispatcher = dispatcher;
      _mapper.DeviceChanged += Mapper_DeviceChanged;
      _mapper.HIDStateChanged += Mapper_ControlStateChanged;
      _mapper.StartedOrStopped += Mapper_StartedOrStopped;
      _mapper.SuppressMapping = suppressMapping;
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
                Action nextCommand = null;

                // Note that countDown must be tested first, otherwise we throw away a queue item
                while (countDown > 0 && _actionQueue.TryTake(out nextCommand))
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
      DeviceMonitor deviceMonitor = null;
      if (!_nameToDeviceMonitor.TryGetValue(deviceName, out deviceMonitor))
      {
        deviceMonitor = new DeviceMonitor(deviceName, deviceType, deviceGuid, _mapper);
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

      }
      return deviceMonitor;
    }

    void Mapper_ControlStateChanged(object sender, HIDStateChangeArgs e)
    {
      _actionQueue.Add(new Action(() =>
      {
        DeviceMonitor deviceMonitor = AddOrGetDevice(e.Device, e.DeviceType, e.DeviceGuid);
        deviceMonitor.UpdateControl(e.Control, e.State, e.PreviousState);
        OnControlStateChanged(e);
      }));
    }

    void Mapper_DeviceChanged(object sender, DeviceInfo e)
    {
      Action action = null;
      switch (e.InfoAction)
      {
        case DeviceInfo.InfoType.Added:
          action = () =>
          {
            AddMessage(new StatusMessage(e.Device, "Added"));
            AddOrGetDevice(e.Device, e.DeviceType, e.DeviceGuid);
          };
          break;
        case DeviceInfo.InfoType.Checked:
          action = () =>
          {
            AddMessage(new StatusMessage(e.Device, "Checked"));
            AddOrGetDevice(e.Device, e.DeviceType, e.DeviceGuid);
          };
          break;
        case DeviceInfo.InfoType.Error:
          action = () =>
          {
            AddMessage(new StatusMessage(e.Device, "Error"));
            if (e is DeviceInfoError)
            {
              AddMessage(new StatusMessage(e.Device, ((DeviceInfoError)e).ErrorMessage));
            }
          };
          break;
        case DeviceInfo.InfoType.Removed:
          action = () =>
          {
            AddMessage(new StatusMessage(e.Device, "Removed"));
            DeviceMonitor deviceMonitor = null;
            if (_nameToDeviceMonitor.TryGetValue(e.Device, out deviceMonitor))
            {
              _nameToDeviceMonitor.Remove(e.Device);
              Devices.Remove(deviceMonitor);
              switch(e.DeviceType)
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
          };
          break;
        case DeviceInfo.InfoType.Updated:
          action = () => AddMessage(new StatusMessage(e.Device, "Updated"));
          break;
      }
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
      StatusMessage storedMessage = null;
      if (_messagesDictionary.TryGetValue(statusMessage, out storedMessage))
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

    void Mapper_StartedOrStopped(object sender, StartStopStatus e)
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
          OnPropertyChanged("Running");
          OnPropertyChanged("NotRunning");
        }
      }
    }

    public bool NotRunning => !_running;

    public ObservableCollection<DeviceMonitor> Devices { get; } = new ObservableCollection<DeviceMonitor>();

    public ObservableCollection<DeviceMonitor> UnknownDevices { get; } = new ObservableCollection<DeviceMonitor>();

    public ObservableCollection<DeviceMonitor> KeyboardDevices { get; } = new ObservableCollection<DeviceMonitor>();

    public ObservableCollection<DeviceMonitor> MouseDevices { get; } = new ObservableCollection<DeviceMonitor>();

    public ObservableCollection<DeviceMonitor> JoystickDevices { get; } = new ObservableCollection<DeviceMonitor>();

    public ObservableCollection<StatusMessage> Messages { get; } = new ObservableCollection<StatusMessage>();
    public ObservableCollection<string> ErrorMessages { get; } = new ObservableCollection<string>();

    public void Start()
    {
      _mapper.StartMonitoring();
    }

    public void Stop()
    {
      _mapper.StopMonitoring();
    }

    public void EnableMapping()
    {
      _mapper.SuppressMapping = false;
    }

    private void OnPropertyChanged(string propertyName)
    {
      if (PropertyChanged != null)
      {
        PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
      }
    }

    private void OnControlStateChanged(HIDStateChangeArgs args)
    {
      var handler = ControlStateChanged;
      if (handler != null)
      {
        handler(this, args);
      }
    }

    public event EventHandler<HIDStateChangeArgs> ControlStateChanged;

    public event PropertyChangedEventHandler PropertyChanged;
  }
}