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
    private const double _xAxisMax = 1000;
    private const double _xAxisMin = -1000;
    private const double _yAxisMax = 1000;
    private const double _yAxisMin = -1000;

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

    public Device2DAxisViewModel()
    {
    }

    public DeviceMonitor DeviceMonitor
    {
      get => _deviceMonitor;
      set
      {
        // Potential here for references from the source to not be released, need to use this class carefully
        _deviceMonitor = value;
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
        _normalizedPositionX = (control.State - _xAxisMin) / (_xAxisMax - _xAxisMin) * 2.0 - 1.0;
        _normalizedPositionX = _normalizedPositionX % 1.0;
        Update();
      }
    }
    private void Y_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
    {
      if (sender is ControlMonitor control)
      {
        _normalizedPositionY = (control.State - _yAxisMin) / (_yAxisMax - _yAxisMin) * 2.0 - 1.0;
        _normalizedPositionY = _normalizedPositionY % 1.0;
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


    private void CalculateNormalizedPositions()
    {
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
    }

    private void CalculateInnerThreshold()
    {

    }

    private void CalculateOuterThreshold()
    {

    }

    private void Update()
    {
      CalculateNormalizedPositions();
      CalculateDevicePositionIndicator();
      CalculateInnerThreshold();
      CalculateOuterThreshold();
      OnPropertyChanged();
    }
  }
}
