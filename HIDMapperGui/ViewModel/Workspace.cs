using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HIDMapperGui.ViewModel
{
  public class Workspace
  {
    private static Lazy<Workspace> _instance = new Lazy<Workspace>(() => new Workspace(), true);

    private Workspace()
    {
    }

    public static Workspace Instance => _instance.Value;

    public string Title => "Joystick Mapper V1.0";

  }
}
