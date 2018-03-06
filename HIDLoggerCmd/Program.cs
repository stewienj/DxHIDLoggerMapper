using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using HIDLogger;
namespace HIDLoggerCmd
{
  class Program
  {
    static void Main(string[] args)
    {
      var logger = new HIDLoggerInterface();
      logger.InfoLogged += logger_InfoLogged;
      logger.DeviceChanged += logger_DeviceChanged;
      logger.StartLogging(@"C:\HIDLogs");

      Thread.Sleep(-1);
    }

    static void logger_DeviceChanged(object sender, DeviceInfo e)
    {
      if (e.InfoAction != DeviceInfo.InfoType.Checked)
      {
        Console.WriteLine("{0} {1}", e.Device, e.InfoAction);
      }
    }

    static void logger_InfoLogged(object sender, LoggedDataArgs e)
    {
      Console.WriteLine("{0} {1} {2}", e.Device, e.Control, e.State);
    }
  }
}