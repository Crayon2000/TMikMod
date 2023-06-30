object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Module Player'
  ClientHeight = 406
  ClientWidth = 422
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 50
    Width = 65
    Height = 13
    Caption = 'Module Type:'
  end
  object Label2: TLabel
    Left = 8
    Top = 74
    Width = 51
    Height = 13
    Caption = 'Song Title:'
  end
  object Label3: TLabel
    Left = 8
    Top = 101
    Width = 49
    Height = 13
    Caption = 'Comment:'
  end
  object txtModuleType: TEdit
    Left = 89
    Top = 44
    Width = 273
    Height = 21
    ReadOnly = True
    TabOrder = 0
  end
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 422
    Height = 33
    ButtonHeight = 30
    ButtonWidth = 31
    Caption = 'ToolBar1'
    Images = ImageList1
    TabOrder = 3
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
    object tbRepeat: TToolButton
      Left = 132
      Top = 0
      Caption = 'tbRepeat'
      ImageIndex = 9
      Style = tbsCheck
      OnClick = tbRepeatClick
    end
    object tbSeparator2: TToolButton
      Left = 163
      Top = 0
      Width = 8
      ImageIndex = 3
      Style = tbsSeparator
    end
    object tbMute: TToolButton
      Left = 171
      Top = 0
      ImageIndex = 7
      Style = tbsCheck
      OnClick = tbMuteClick
    end
    object TrackBar1: TTrackBar
      Left = 202
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
    Top = 71
    Width = 273
    Height = 21
    ReadOnly = True
    TabOrder = 1
  end
  object memoComment: TMemo
    Left = 89
    Top = 98
    Width = 273
    Height = 56
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 2
  end
  object Panel1: TPanel
    AlignWithMargins = True
    Left = 8
    Top = 162
    Width = 406
    Height = 236
    Margins.Left = 8
    Margins.Top = 8
    Margins.Right = 8
    Margins.Bottom = 8
    Align = alBottom
    BevelOuter = bvLowered
    ShowCaption = False
    TabOrder = 4
  end
  object Timer1: TTimer
    Interval = 30
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
