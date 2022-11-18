using System;
using System.ComponentModel;
using System.Linq.Expressions;
using System.Reflection;
using System.Windows.Controls;

namespace HIDMonitorGui.ViewModel
{
  public static class Extensions
  {

    /// <summary>
    /// Used for using the DisplayName attribute as the column header when generating columns in DataGrid controls
    /// </summary>
    /// <example>
    /// 
    /// include this at the top of the xaml file
    /// xmlns:SwordfishControls="clr-namespace:Swordfish.NET.WPF.Controls;assembly=Swordfish.NET.WPF"
    /// 
    /// have the event handler in the control and AutoGenerateColumns = True
    /// <DataGrid x:Name="theGrid" AutoGenerateColumns="True" AutoGeneratingColumn="dg_AutoGeneratingColumn"/>
    /// 
    /// </example>
    /// <param name="control"></param>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    public static void dg_AutoGeneratingColumn(this UserControl control, object sender, DataGridAutoGeneratingColumnEventArgs e)
    {

      string? displayName = PropertyHelper.GetPropertyDisplayName(e.PropertyDescriptor);
      if (!string.IsNullOrEmpty(displayName))
      {
        e.Column.Header = displayName;
      }
      else if (displayName == "")
      {
        e.Cancel = true;
      }
    }
  }

  /// <summary>
  /// Gets the property info from a property
  /// </summary>
  /// <example>
  /// // Literally use "x" but use the proper property name
  /// PropertyInfo prop = PropertyHelper<Foo>.GetProperty(x => x.Bar);
  /// 
  /// How to use in a DataGrid column generator
  /// 
  /// private void dg_AutoGeneratingColumn(object sender, DataGridAutoGeneratingColumnEventArgs e) {
  ///
  ///   string displayName = PropertyHelper.GetPropertyDisplayName(e.PropertyDescriptor);
  ///   if (!string.IsNullOrEmpty(displayName)){
  ///     e.Column.Header = displayName;
  ///   }
  /// }
  /// 
  /// Where the properties have meta data like this:
  /// 
  /// [DisplayName("Action Performed")]
  /// public string Action {
  ///   ...
  /// }
  /// </example>
  /// <typeparam name="T"></typeparam>
  public static class PropertyHelper<T>
  {
    public static PropertyInfo GetProperty<TValue>(Expression<Func<T, TValue>> selector)
    {
      Expression body = selector;
      if (body is LambdaExpression)
      {
        body = ((LambdaExpression)body).Body;
      }
      switch (body.NodeType)
      {
        case ExpressionType.MemberAccess:
          return (PropertyInfo)((MemberExpression)body).Member;
        default:
          throw new InvalidOperationException();
      }
    }

    public static string GetPropertyName<TValue>(Expression<Func<T, TValue>> selector)
    {
      PropertyInfo info = GetProperty(selector);
      return info.Name;
    }
  }

  public static class PropertyHelper
  {
    public static string? GetPropertyDisplayName(object descriptor)
    {
      if (descriptor is PropertyDescriptor pd)
      {
        // Check for DisplayName attribute and set the column header accordingly
        DisplayNameAttribute? displayName = pd.Attributes[typeof(DisplayNameAttribute)] as DisplayNameAttribute;
        if (displayName != null && displayName != DisplayNameAttribute.Default)
        {
          return displayName.DisplayName;
        }

      }
      else
      {
        if (descriptor is PropertyInfo pi)
        {
          // Check for DisplayName attribute and set the column header accordingly
          Object[] attributes = pi.GetCustomAttributes(typeof(DisplayNameAttribute), true);
          for (int i = 0; i < attributes.Length; ++i)
          {
            DisplayNameAttribute? displayName = attributes[i] as DisplayNameAttribute;
            if (displayName != null && displayName != DisplayNameAttribute.Default)
            {
              return displayName.DisplayName;
            }
          }
        }
      }
      return null;
    }
  }
}
