object frmSettings: TfrmSettings
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Application settings'
  ClientHeight = 209
  ClientWidth = 390
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poScreenCenter
  TextHeight = 15
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 390
    Height = 168
    ActivePage = tbshtLog
    Align = alClient
    TabOrder = 0
    object tbshtLog: TTabSheet
      Caption = 'General'
      ImageIndex = 1
      object checkboxStartMinimized: TCheckBox
        Left = 16
        Top = 16
        Width = 113
        Height = 17
        Caption = 'Start minimized'
        TabOrder = 0
      end
    end
    object tabshtServer: TTabSheet
      Caption = 'Server'
      object Label1: TLabel
        Left = 24
        Top = 16
        Width = 22
        Height = 15
        Caption = 'Port'
      end
      object edtServerPort: TEdit
        Left = 24
        Top = 32
        Width = 73
        Height = 23
        NumbersOnly = True
        TabOrder = 0
        Text = '6060'
        TextHint = 'Listen port'
      end
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 168
    Width = 390
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    Caption = 'Panel1'
    ShowCaption = False
    TabOrder = 1
    ExplicitTop = 281
    object Button1: TButton
      Left = 87
      Top = 8
      Width = 75
      Height = 25
      Action = actCancel
      Cancel = True
      TabOrder = 0
    end
    object Button2: TButton
      Left = 232
      Top = 8
      Width = 75
      Height = 25
      Action = actOK
      Default = True
      TabOrder = 1
    end
  end
  object ActionList1: TActionList
    Left = 284
    Top = 98
    object actOK: TAction
      Caption = 'OK'
      OnExecute = actOKExecute
      OnUpdate = actOKUpdate
    end
    object actCancel: TAction
      Caption = 'Cancel'
      OnExecute = actCancelExecute
    end
  end
end
