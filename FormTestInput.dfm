object frmTestInput: TfrmTestInput
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Test input source'
  ClientHeight = 386
  ClientWidth = 427
  Color = clBtnFace
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poScreenCenter
  DesignSize = (
    427
    386)
  TextHeight = 15
  object Label1: TLabel
    Left = 16
    Top = 62
    Width = 100
    Height = 15
    Caption = 'Audio input device'
  end
  object Label2: TLabel
    Left = 16
    Top = 325
    Width = 86
    Height = 15
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Output file name'
    ExplicitWidth = 90
  end
  object Label3: TLabel
    Left = 16
    Top = 118
    Width = 49
    Height = 15
    Caption = 'Channels'
  end
  object speedbtnBrowseOutFile: TSpeedButton
    Left = 372
    Top = 346
    Width = 24
    Height = 23
    Anchors = [akTop, akRight]
    Caption = '...'
    OnClick = speedbtnBrowseOutFileClick
    ExplicitLeft = 380
  end
  object Label4: TLabel
    Left = 96
    Top = 118
    Width = 62
    Height = 15
    Caption = 'Sample rate'
  end
  object Label5: TLabel
    Left = 16
    Top = 175
    Width = 39
    Height = 15
    Caption = 'Lo Freq'
  end
  object Label6: TLabel
    Left = 96
    Top = 175
    Width = 38
    Height = 15
    Caption = 'Hi Freq'
  end
  object Label7: TLabel
    Left = 176
    Top = 175
    Width = 53
    Height = 15
    Caption = 'Hysteresis'
  end
  object lblFreq: TLabel
    Left = 204
    Top = 233
    Width = 180
    Height = 37
    Alignment = taRightJustify
    Anchors = [akTop, akRight]
    Caption = '1234.56 Hz'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBtnText
    Font.Height = -32
    Font.Name = 'Consolas'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
    Transparent = True
    ExplicitLeft = 212
  end
  object lblLevel: TLabel
    Left = 276
    Top = 275
    Width = 108
    Height = 37
    Alignment = taRightJustify
    Anchors = [akTop, akRight]
    Caption = '-30 dB'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = 'Consolas'
    Font.Style = [fsBold]
    ParentFont = False
    Transparent = True
    ExplicitLeft = 284
  end
  object Label8: TLabel
    Left = 256
    Top = 175
    Width = 49
    Height = 15
    Caption = 'Gain (dB)'
  end
  object Label9: TLabel
    Left = 199
    Top = 118
    Width = 69
    Height = 15
    Caption = 'Buffer lenght'
  end
  object Label10: TLabel
    Left = 16
    Top = 232
    Width = 69
    Height = 15
    Hint = 'Minimum detection threshold in dB'
    Caption = 'Threshold dB'
  end
  object edtOutFileName: TEdit
    Left = 16
    Top = 346
    Width = 350
    Height = 23
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 3
    Text = 'out.raw'
    ExplicitWidth = 346
  end
  object comboboxChannels: TComboBox
    Left = 16
    Top = 135
    Width = 57
    Height = 23
    Style = csDropDownList
    ItemIndex = 0
    TabOrder = 0
    Text = '1'
    OnChange = comboboxChannelsChange
    Items.Strings = (
      '1'
      '2')
  end
  object Button1: TButton
    Left = 16
    Top = 16
    Width = 75
    Height = 25
    Action = actStart
    TabOrder = 4
  end
  object Button2: TButton
    Left = 110
    Top = 16
    Width = 75
    Height = 25
    Action = actStop
    TabOrder = 5
  end
  object checkboxSaveToFile: TCheckBox
    Left = 16
    Top = 295
    Width = 97
    Height = 17
    Caption = 'SaveToFile'
    TabOrder = 2
  end
  object comboboxSampleRate: TComboBox
    Left = 96
    Top = 135
    Width = 81
    Height = 23
    Style = csDropDownList
    TabOrder = 1
    TextHint = 'sps'
    OnChange = comboboxSampleRateChange
  end
  object pnlMeterLeft: TPanel
    Left = 336
    Top = 8
    Width = 32
    Height = 200
    Anchors = [akTop, akRight]
    BevelOuter = bvLowered
    Caption = 'pnlMeterLeft'
    ShowCaption = False
    TabOrder = 6
    ExplicitLeft = 332
    object lblMeterLeft: TLabel
      Left = 1
      Top = 1
      Width = 30
      Height = 15
      Align = alTop
      Alignment = taCenter
      AutoSize = False
      Caption = 'L'
      ExplicitLeft = -3
      ExplicitTop = 24
      ExplicitWidth = 25
    end
    inline frmeLevelMeterLeft: TfrmeLevelMeter
      Left = 1
      Top = 16
      Width = 30
      Height = 183
      Align = alClient
      DoubleBuffered = True
      Color = clBtnFace
      ParentBackground = False
      ParentColor = False
      ParentDoubleBuffered = False
      TabOrder = 0
      ExplicitLeft = 1
      ExplicitTop = 16
      ExplicitWidth = 30
      ExplicitHeight = 183
      inherited PaintBox1: TPaintBox
        Width = 24
        Height = 177
        ExplicitWidth = 14
        ExplicitHeight = 89
      end
    end
  end
  object pnlMeterRight: TPanel
    Left = 376
    Top = 8
    Width = 32
    Height = 200
    Anchors = [akTop, akRight]
    BevelOuter = bvLowered
    Caption = 'pnlMeterLeft'
    ShowCaption = False
    TabOrder = 7
    Visible = False
    ExplicitLeft = 372
    object lblMeterRight: TLabel
      Left = 1
      Top = 1
      Width = 30
      Height = 15
      Align = alTop
      Alignment = taCenter
      AutoSize = False
      Caption = 'R'
      ExplicitLeft = 9
      ExplicitTop = 9
      ExplicitWidth = 20
    end
    inline frmeLevelMeterRight: TfrmeLevelMeter
      Left = 1
      Top = 16
      Width = 30
      Height = 183
      Align = alClient
      DoubleBuffered = True
      Color = clBtnFace
      ParentBackground = False
      ParentColor = False
      ParentDoubleBuffered = False
      TabOrder = 0
      ExplicitLeft = 1
      ExplicitTop = 16
      ExplicitWidth = 30
      ExplicitHeight = 183
      inherited PaintBox1: TPaintBox
        Width = 24
        Height = 177
        ExplicitWidth = 14
        ExplicitHeight = 89
      end
    end
  end
  object comboboxInputDev: TComboBox
    Left = 16
    Top = 80
    Width = 281
    Height = 23
    Style = csDropDownList
    TabOrder = 8
  end
  object edtLoFreq: TEdit
    Left = 16
    Top = 193
    Width = 57
    Height = 23
    NumbersOnly = True
    TabOrder = 9
    Text = '100'
  end
  object edtHiFreq: TEdit
    Left = 96
    Top = 193
    Width = 57
    Height = 23
    NumbersOnly = True
    TabOrder = 10
    Text = '2000'
  end
  object edtHyst: TEdit
    Left = 176
    Top = 193
    Width = 57
    Height = 23
    TabOrder = 11
    Text = '3'
  end
  object edtOverallGain_dB: TEdit
    Left = 256
    Top = 193
    Width = 57
    Height = 23
    TabOrder = 12
    Text = '0'
    OnExit = edtOverallGain_dBExit
    OnKeyPress = edtOverallGain_dBKeyPress
  end
  object comboboxBufferLen: TComboBox
    Left = 199
    Top = 135
    Width = 81
    Height = 23
    Style = csDropDownList
    TabOrder = 13
    TextHint = 'sps'
    OnChange = comboboxBufferLenChange
  end
  object edtThreshold_dB: TEdit
    Left = 16
    Top = 250
    Width = 69
    Height = 23
    TabOrder = 14
    Text = '-50'
    OnExit = edtOverallGain_dBExit
    OnKeyPress = edtOverallGain_dBKeyPress
  end
  object FileSaveDialog1: TFileSaveDialog
    DefaultExtension = '.raw'
    FavoriteLinks = <>
    FileTypes = <
      item
        DisplayName = 'Raw PCM file'
        FileMask = '*.raw'
      end
      item
        DisplayName = 'All files'
        FileMask = '*.*'
      end>
    Options = []
    Title = 'Enter destination file name'
    Left = 136
    Top = 16
  end
  object ActionList1: TActionList
    Left = 240
    Top = 16
    object actStart: TAction
      Caption = 'Start'
      OnExecute = actStartExecute
      OnUpdate = actStartUpdate
    end
    object actStop: TAction
      Caption = 'Stop'
      OnExecute = actStopExecute
      OnUpdate = actStopUpdate
    end
  end
  object tmrLevelMeter: TTimer
    Enabled = False
    Interval = 50
    OnTimer = tmrLevelMeterTimer
    Left = 40
    Top = 16
  end
  object ApplicationEvents1: TApplicationEvents
    OnIdle = ApplicationEvents1Idle
    Left = 312
    Top = 16
  end
end
