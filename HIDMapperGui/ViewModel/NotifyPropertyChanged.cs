﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace HIDMapperGui.ViewModel
{
  /// <summary>
  /// Provides a INotifyPropertyChanged implementation that uses a dispatcher.
  /// </summary>
  public class NotifyPropertyChanged : INotifyPropertyChanged
  {

    /// <summary>
    /// Checks if the value has changed and uses the call stack to determine
    /// which property this was called from, then calls OnPropertyChanged
    /// with the property name.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="current"></param>
    /// <param name="newValue"></param>
    protected bool SetValue<T>(ref T current, T newValue, [CallerMemberName] string propertyName = "")
    {
      if (!EqualityComparer<T>.Default.Equals(current, newValue))
      {
        T oldValue = current;
        current = newValue;
        OnPropertyChanged(propertyName);
        return true;
      }
      else
      {
        return false;
      }
    }

    //-------------------------------------------------------------------------
    protected bool SetValue<T>(ref T current, T newValue, params string[] propertyNames)
    {
      if (!EqualityComparer<T>.Default.Equals(current, newValue))
      {
        current = newValue;
        OnPropertyChanged(propertyNames);
        return true;
      }
      else
      {
        return false;
      }
    }

    /// <summary>
    /// Triggers PropertyChanged event in a way that it can be handled
    /// by controls on a different thread.
    /// </summary>
    /// <param name="propertyName"></param>
    protected virtual void OnPropertyChanged(params string[] propertyNames)
    {
      if (PropertyChanged != null)
      {
        foreach (string propertyName in propertyNames)
        {
          PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
        if (propertyNames.Length == 0)
        {
          PropertyChanged(this, new PropertyChangedEventArgs(null));
        }
      }
    }

    #region INotifyPropertyChanged Members

    public event PropertyChangedEventHandler PropertyChanged;

    #endregion
  }
}
