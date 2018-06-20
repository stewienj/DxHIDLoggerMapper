﻿// From StackOverflow answer by Kent Boogaart on https://stackoverflow.com/questions/1083224/pushing-read-only-gui-properties-back-into-viewmodel

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace HIDMapperGui.Controls
{
  /// <summary>
  /// <UserControl ...
  /// SizeObserver.Observe="True"
  /// SizeObserver.ObservedWidth="{Binding Width, Mode=OneWayToSource}"
  /// SizeObserver.ObservedHeight="{Binding Height, Mode=OneWayToSource}"
  /// </summary>
  public static class SizeObserver
  {
    public static readonly DependencyProperty ObserveProperty = DependencyProperty.RegisterAttached(
        "Observe",
        typeof(bool),
        typeof(SizeObserver),
        new FrameworkPropertyMetadata(OnObserveChanged));

    public static readonly DependencyProperty ObservedWidthProperty = DependencyProperty.RegisterAttached(
        "ObservedWidth",
        typeof(double),
        typeof(SizeObserver));

    public static readonly DependencyProperty ObservedHeightProperty = DependencyProperty.RegisterAttached(
        "ObservedHeight",
        typeof(double),
        typeof(SizeObserver));

    public static bool GetObserve(FrameworkElement frameworkElement) => (bool)frameworkElement.GetValue(ObserveProperty);

    public static void SetObserve(FrameworkElement frameworkElement, bool observe) => frameworkElement.SetValue(ObserveProperty, observe);

    public static double GetObservedWidth(FrameworkElement frameworkElement) => (double)frameworkElement.GetValue(ObservedWidthProperty);

    public static void SetObservedWidth(FrameworkElement frameworkElement, double observedWidth) => frameworkElement.SetValue(ObservedWidthProperty, observedWidth);

    public static double GetObservedHeight(FrameworkElement frameworkElement) => (double)frameworkElement.GetValue(ObservedHeightProperty);

    public static void SetObservedHeight(FrameworkElement frameworkElement, double observedHeight) => frameworkElement.SetValue(ObservedHeightProperty, observedHeight);

    private static void OnObserveChanged(DependencyObject dependencyObject, DependencyPropertyChangedEventArgs e)
    {
      var frameworkElement = (FrameworkElement)dependencyObject;

      if ((bool)e.NewValue)
      {
        frameworkElement.SizeChanged += OnFrameworkElementSizeChanged;
        UpdateObservedSizesForFrameworkElement(frameworkElement);
      }
      else
      {
        frameworkElement.SizeChanged -= OnFrameworkElementSizeChanged;
      }
    }

    private static void OnFrameworkElementSizeChanged(object sender, SizeChangedEventArgs e) => UpdateObservedSizesForFrameworkElement((FrameworkElement)sender);

    private static void UpdateObservedSizesForFrameworkElement(FrameworkElement frameworkElement)
    {
      // WPF 4.0 onwards
      frameworkElement.SetCurrentValue(ObservedWidthProperty, frameworkElement.ActualWidth);
      frameworkElement.SetCurrentValue(ObservedHeightProperty, frameworkElement.ActualHeight);
    }
  }
}
