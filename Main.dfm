object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Module Player'
  ClientHeight = 420
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
  object Label1: TLabel
    Left = 8
    Top = 42
    Width = 65
    Height = 13
    Caption = 'Module Type:'
  end
  object Label2: TLabel
    Left = 8
    Top = 66
    Width = 51
    Height = 13
    Caption = 'Song Title:'
  end
  object Label3: TLabel
    Left = 8
    Top = 93
    Width = 49
    Height = 13
    Caption = 'Comment:'
  end
  object txtModuleType: TEdit
    Left = 89
    Top = 36
    Width = 273
    Height = 21
    ReadOnly = True
    TabOrder = 0
  end
  object ProgressBar1: TProgressBar
    Left = 40
    Top = 176
    Width = 33
    Height = 224
    Orientation = pbVertical
    TabOrder = 1
  end
  object ProgressBar2: TProgressBar
    Left = 79
    Top = 176
    Width = 33
    Height = 224
    Orientation = pbVertical
    TabOrder = 2
  end
  object ProgressBar3: TProgressBar
    Left = 118
    Top = 176
    Width = 33
    Height = 224
    Orientation = pbVertical
    TabOrder = 3
  end
  object ProgressBar4: TProgressBar
    Left = 157
    Top = 176
    Width = 33
    Height = 224
    Orientation = pbVertical
    TabOrder = 4
  end
  object ProgressBar5: TProgressBar
    Left = 196
    Top = 176
    Width = 33
    Height = 224
    Orientation = pbVertical
    TabOrder = 5
  end
  object ProgressBar6: TProgressBar
    Left = 235
    Top = 176
    Width = 33
    Height = 224
    Orientation = pbVertical
    TabOrder = 6
  end
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 426
    Height = 33
    ButtonHeight = 30
    ButtonWidth = 31
    Caption = 'ToolBar1'
    Images = ImageList1
    TabOrder = 7
    object tbOpenFile: TToolButton
      Left = 0
      Top = 0
      ImageIndex = 3
      OnClick = tbOpenFileClick
    end
    object tbSeparator1: TToolButton
      Left = 31
      Top = 0
      Width = 8
      ImageIndex = 3
      Style = tbsSeparator
    end
    object tbPlay: TToolButton
      Left = 39
      Top = 0
      ImageIndex = 0
      OnClick = tbPlayClick
    end
    object tbPause: TToolButton
      Left = 70
      Top = 0
      ImageIndex = 1
      OnClick = tbPauseClick
    end
    object tbStop: TToolButton
      Left = 101
      Top = 0
      ImageIndex = 2
      OnClick = tbStopClick
    end
    object tbSeparator2: TToolButton
      Left = 132
      Top = 0
      Width = 8
      ImageIndex = 3
      Style = tbsSeparator
    end
    object tbMute: TToolButton
      Left = 140
      Top = 0
      ImageIndex = 7
      Style = tbsCheck
      OnClick = tbMuteClick
    end
    object TrackBar1: TTrackBar
      Left = 171
      Top = 0
      Width = 165
      Height = 30
      Max = 128
      Frequency = 10
      Position = 128
      ShowSelRange = False
      TabOrder = 0
      OnChange = TrackBar1Change
    end
  end
  object txtSongTitle: TEdit
    Left = 89
    Top = 63
    Width = 273
    Height = 21
    ReadOnly = True
    TabOrder = 8
  end
  object memoComment: TMemo
    Left = 89
    Top = 90
    Width = 273
    Height = 56
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 9
  end
  object Timer1: TTimer
    Interval = 33
    OnTimer = Timer1Timer
    Left = 368
    Top = 240
  end
  object ImageList1: TImageList
    ColorDepth = cd32Bit
    Height = 24
    Width = 24
    Left = 368
    Top = 184
  end
  object OpenDialog1: TOpenDialog
    FilterIndex = 0
    Left = 304
    Top = 184
  end
end
