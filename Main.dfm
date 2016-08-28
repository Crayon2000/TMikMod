object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 160
  ClientWidth = 426
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object TrackBar1: TTrackBar
    Left = 40
    Top = 24
    Width = 321
    Height = 45
    Max = 128
    Frequency = 10
    Position = 128
    ShowSelRange = False
    TabOrder = 0
    OnChange = TrackBar1Change
  end
  object Button1: TButton
    Left = 8
    Top = 75
    Width = 75
    Height = 25
    Caption = 'Stop'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 8
    Top = 106
    Width = 75
    Height = 25
    Caption = 'Play'
    TabOrder = 2
    OnClick = Button2Click
  end
  object Edit1: TEdit
    Left = 128
    Top = 80
    Width = 273
    Height = 21
    TabOrder = 3
  end
end
