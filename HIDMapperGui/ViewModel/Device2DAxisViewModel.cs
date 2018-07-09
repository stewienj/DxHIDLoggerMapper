using HIDMapperDLL;
using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HIDMapperGui.ViewModel
{
  public class Device2DAxisViewModel : NotifyPropertyChanged
  {
    private const double _xAxisMax = 1001;
    private const double _xAxisMin = -1001;
    private const double _yAxisMax = 1001;
    private const double _yAxisMin = -1001;

    /// <summary>
    /// Device X position normalized to between -1.0 and 1.0
    /// </summary>
    private double _normalizedPositionX = 0;
    /// <summary>
    /// Device Y position normalized to between -1.0 and 1.0
    /// </summary>
    private double _normalizedPositionY = 0;
    /// <summary>
    /// The device from which the X-Y axis comes from
    /// </summary>
    private DeviceMonitor _deviceMonitor;

    /// <summary>
    /// Keep a local version here, in the future this will be pushed down to the device using the Guid
    /// </summary>
    private MapperJoystickToKeyboardConfig _config = null;

    public Device2DAxisViewModel()
    {
    }

    public DeviceMonitor DeviceMonitor
    {
      get => _deviceMonitor;
      set
      {
        _deviceMonitor = value;
        _config = _deviceMonitor.MapperConfig;
        // Potential here for references from the source to not be released, need to use this class carefully
        _deviceMonitor.Controls.CollectionChanged += Controls_CollectionChanged;
        Controls_CollectionChanged(
          _deviceMonitor.Controls,
          new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Add, _deviceMonitor.Controls));
      }
    }


    private void Controls_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
    {
      if (e.Action == System.Collections.Specialized.NotifyCollectionChangedAction.Add)
      {
        foreach(ControlMonitor newItem in e.NewItems)
        {
          switch(newItem.ControlName)
          {
            case "X":
              newItem.PropertyChanged += X_PropertyChanged;
              X_PropertyChanged(newItem, new System.ComponentModel.PropertyChangedEventArgs(nameof(ControlMonitor.State)));
              break;
            case "Y":
              newItem.PropertyChanged += Y_PropertyChanged;
              Y_PropertyChanged(newItem, new System.ComponentModel.PropertyChangedEventArgs(nameof(ControlMonitor.State)));
              break;
          }
        }
      }
     
    }

    private void X_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
    {
      if (sender is ControlMonitor control)
      {
        _normalizedPositionX = NormalizeX(control.State);
        Update();
      }
    }
    private void Y_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
    {
      if (sender is ControlMonitor control)
      {
        _normalizedPositionY = NormalizeY(control.State);
        Update();
      }
    }


    private double _canvasHeight = 0;
    public double HeightFromCanvas
    {
      get => _canvasHeight;
      set
      {
        _canvasHeight = value;
        Update();
      }
    }

    private double _canvasWidth = 0;
    public double WidthFromCanvas
    {
      get => _canvasWidth;
      set
      {
        _canvasWidth = value;
        Update();
      }
    }

    public double InnerRangeTop
    {
      get; private set;
    }

    public double InnerRangeLeft
    {
      get; private set;
    }

    public double InnerRangeHeight
    {
      get; private set;
    }

    public double InnerRangeWidth
    {
      get; private set;
    }


    public double OuterRangeTop
    {
      get; private set;
    }

    public double OuterRangeLeft
    {
      get; private set;
    }

    public double OuterRangeHeight
    {
      get; private set;
    }

    public double OuterRangeWidth
    {
      get; private set;
    }

    public double DevicePositionTop
    {
      get; private set;
    }

    public double DevicePositionLeft
    {
      get; private set;
    }

    public double DevicePositionWidth
    {
      get; private set;
    }

    public double DevicePositionHeight
    {
      get; private set;
    }

    public double CentreX
    {
      get; private set;
    }

    public double CentreY
    {
      get; private set;
    }

    private void CalculateDevicePositionIndicator()
    {
      // -ve Y is up for joysticks
      if (_normalizedPositionY > 0)
      {
        DevicePositionTop = HeightFromCanvas * 0.5;
        DevicePositionHeight = HeightFromCanvas * 0.5 * _normalizedPositionY;
      }
      else
      {
        DevicePositionTop = HeightFromCanvas * 0.5 * (1 + _normalizedPositionY);
        DevicePositionHeight = HeightFromCanvas * 0.5 * (-_normalizedPositionY);
      }

      if (_normalizedPositionX > 0)
      {
        DevicePositionLeft = WidthFromCanvas * 0.5;
        DevicePositionWidth = WidthFromCanvas * 0.5 * _normalizedPositionX;
      }
      else
      {
        DevicePositionLeft = WidthFromCanvas * 0.5 * (1 + _normalizedPositionX);
        DevicePositionWidth = WidthFromCanvas * 0.5 * (-_normalizedPositionX);
      }

      CentreX = WidthFromCanvas * 0.5;
      CentreY = HeightFromCanvas * 0.5;
    }

    private void CalculateInnerOuterThresholds()
    {
      if (_config != null)
      {
        // Get the min max ranges from the mapping config

        var leftThresholds = _config.X.Select(kx => kx.Threshold).Where(x => x < 0);
        var rightThresholds = _config.X.Select(kx => kx.Threshold).Where(x => x > 0);
        var topThresholds = _config.Y.Select(ky => ky.Threshold).Where(y => y < 0);
        var bottomThresholds = _config.Y.Select(ky => ky.Threshold).Where(y => y > 0);

        var maxLeft = leftThresholds.Count() > 0 ? leftThresholds.Min() : 0;
        var maxRight = rightThresholds.Count() > 0 ? rightThresholds.Max() : 0;
        var maxTop = topThresholds.Count() > 0 ? topThresholds.Min() : 0;
        var maxBottom = bottomThresholds.Count() > 0 ? bottomThresholds.Max() : 0;

        var minLeft = leftThresholds.Count() > 0 ? leftThresholds.Max() : 0;
        var minRight = rightThresholds.Count() > 0 ? rightThresholds.Min() : 0;
        var minTop = topThresholds.Count() > 0 ? topThresholds.Max() : 0;
        var minBottom = bottomThresholds.Count() > 0 ? bottomThresholds.Min() : 0;

        // Show the min max ranges as rectangles

        OuterRangeLeft = (1.0+NormalizeX(maxLeft)) * WidthFromCanvas * 0.5;
        OuterRangeWidth = (NormalizeX(maxRight) - NormalizeX(maxLeft)) * 0.5 * WidthFromCanvas;
        OuterRangeTop = (1.0 + NormalizeY(maxTop)) * HeightFromCanvas * 0.5;
        OuterRangeHeight = (NormalizeY(maxBottom) - NormalizeY(maxTop)) * 0.5 * HeightFromCanvas;

        InnerRangeLeft = (1.0 + NormalizeX(minLeft)) * WidthFromCanvas * 0.5;
        InnerRangeWidth = (NormalizeX(minRight) - NormalizeX(minLeft)) * 0.5 * WidthFromCanvas;
        InnerRangeTop = (1.0 + NormalizeY(minTop)) * HeightFromCanvas * 0.5;
        InnerRangeHeight = (NormalizeY(minBottom) - NormalizeY(minTop)) * 0.5 * HeightFromCanvas;
      }
    }

    private double NormalizeX(int x) => ((x - _xAxisMin) / (_xAxisMax - _xAxisMin) * 2.0 - 1.0) % 1;
    private double NormalizeY(int y) => ((y - _yAxisMin) / (_yAxisMax - _yAxisMin) * 2.0 - 1.0) % 1;

    private void Update()
    {
      CalculateDevicePositionIndicator();
      CalculateInnerOuterThresholds();
      OnPropertyChanged();
    }
  }
}
