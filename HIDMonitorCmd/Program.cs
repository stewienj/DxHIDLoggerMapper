using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using HIDMonitorDLL;
namespace HIDMonitorCmd
{
  class Program
  {
    static void Main(string[] args)
    {
      var mapper = new HIDMonitorInterface();
      mapper.HIDStateChanged += Monitor_HIDStateChanged;
      mapper.DeviceChanged += Monitor_DeviceChanged;
      mapper.StartMonitoring();

      Thread.Sleep(-1);
    }

    private static void Monitor_HIDStateChanged(object? sender, HIDStateChangeArgs e)
    {
      Console.WriteLine($"{e.Device} {e.Control} {e.State} {e.PreviousState}");
    }

    static void Monitor_DeviceChanged(object? sender, DeviceInfo e)
    {
      if (e.InfoAction != DeviceInfo.InfoType.Checked)
      {
        Console.WriteLine("{0} {1}", e.Device, e.InfoAction);
      }
    }
  }
}
