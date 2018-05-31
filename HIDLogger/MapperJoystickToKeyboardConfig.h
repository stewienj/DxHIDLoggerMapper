using namespace System;
using namespace System::Collections::Generic;

namespace HIDLogger 
{
  public value class ThresholdAndKey
  {
  public:
    ThresholdAndKey(int threshold, int key) 
    {
      Threshold = threshold;
      Key = key;
    }
    int Threshold;
    int Key;
  };

  public ref class MapperJoystickToKeyboardConfig
  {
  private:
    array<array<int>^>^ _buttonToKeySequence;
    List<ThresholdAndKey>^ _x;
    List<ThresholdAndKey>^ _y;
    List<ThresholdAndKey>^ _z;
    List<ThresholdAndKey>^ _rx;
    List<ThresholdAndKey>^ _ry;
    List<ThresholdAndKey>^ _rz;
  public:
    MapperJoystickToKeyboardConfig() 
    {
      _buttonToKeySequence = gcnew array<array<int>^>(sizeof(DIJOYSTATE2::rgbButtons));
      _x = gcnew List<ThresholdAndKey>();
      _y = gcnew List<ThresholdAndKey>();
      _z = gcnew List<ThresholdAndKey>();
      _rx = gcnew List<ThresholdAndKey>();
      _ry = gcnew List<ThresholdAndKey>();
      _rz = gcnew List<ThresholdAndKey>();
    }

    void ConfigureForFortnite()
    {
      SetButtonKey(0, 0x39); // Jump / Space
      SetButtonKey(1, 0x1D); // Crouch / Left Control
      SetButtonKey(2, 0x12); // Pick up Item / E
      SetButtonKey(3, 0x3B); // Select first build item / F1
      SetButtonKey(4, 0x02); // Select first weapon / 1

      _x->Add(ThresholdAndKey( 400, 0x20)); // D
      _x->Add(ThresholdAndKey( 900, 0x2A)); // Left Shift
      _x->Add(ThresholdAndKey(-400, 0x1E)); // A
      _x->Add(ThresholdAndKey(-900, 0x2A)); // Left Shift

      _y->Add(ThresholdAndKey(-400, 0x11)); // W
      _y->Add(ThresholdAndKey(-900, 0x2A)); // Left Shift
      _y->Add(ThresholdAndKey( 400, 0x1F)); // S
      _y->Add(ThresholdAndKey( 900, 0x2A)); // Left Shift
    }

    void SetButtonKey(int buttonNo, ... array<int>^ args)
    {
      _buttonToKeySequence[buttonNo] = args;
    }

    property array<array<int>^>^ ButtonToKeySequence
    {
      array<array<int>^>^ get() {
        return _buttonToKeySequence;
      }
      void set(array<array<int>^>^ buttonToKeySequence) {
        _buttonToKeySequence = buttonToKeySequence;
      }
    }

    property List<ThresholdAndKey>^ X
    {
      List<ThresholdAndKey>^ get() {
        return _x;
      }
      void set(List<ThresholdAndKey>^ x) {
        _x = x;
      }
    }

    property List<ThresholdAndKey>^ Y
    {
      List<ThresholdAndKey>^ get() {
        return _y;
      }
      void set(List<ThresholdAndKey>^ y) {
        _y = y;
      }
    }

    property List<ThresholdAndKey>^ Z
    {
      List<ThresholdAndKey>^ get() {
        return _z;
      }
      void set(List<ThresholdAndKey>^ z) {
        _z = z;
      }
    }

    property List<ThresholdAndKey>^ RX
    {
      List<ThresholdAndKey>^ get() {
        return _rx;
      }
      void set(List<ThresholdAndKey>^ rx) {
        _rx = rx;
      }
    }

    property List<ThresholdAndKey>^ RY
    {
      List<ThresholdAndKey>^ get() {
        return _ry;
      }
      void set(List<ThresholdAndKey>^ ry) {
        _ry = ry;
      }
    }

    property List<ThresholdAndKey>^ RZ
    {
      List<ThresholdAndKey>^ get() {
        return _rz;
      }
      void set(List<ThresholdAndKey>^ rz) {
        _rz = rz;
      }
    }

  };
}