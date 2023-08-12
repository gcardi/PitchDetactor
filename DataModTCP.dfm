object dmTCP: TdmTCP
  Height = 201
  Width = 389
  object IdCmdTCPServer1: TIdCmdTCPServer
    Bindings = <>
    DefaultPort = 6060
    MaxConnections = 1
    OnConnect = IdCmdTCPServer1Connect
    OnDisconnect = IdCmdTCPServer1Disconnect
    OnException = IdCmdTCPServer1Exception
    CommandHandlers = <
      item
        CmdDelimiter = ' '
        Command = 'LAI'
        Description.Strings = (
          'Show all available wave input devices on system')
        Disconnect = False
        ExceptionReply.Code = ''
        Name = 'srvCmdListAudioInputs'
        NormalReply.Code = '200'
        ParamDelimiter = ' '
        ParseParams = False
        Tag = 0
        HelpSuperScript = ': list available input devices'
        OnCommand = IdCmdTCPServer1CommandHandlers0Command
      end
      item
        CmdDelimiter = ' '
        Command = 'DETF'
        Description.Strings = (
          
            'Parameters: <chn> <sps> <len> <hys> <flo> <fhi> <listen_for_ms> ' +
            '<amplitude_dB>'
          ''
          '<chn> = Input device number'
          '<sps> = Samples per second'
          '<len> = Sampling buffer length'
          '<hys> = Detection hysterisis'
          '<flo> = minimum detectable frequency'
          '<fhi> = maximum detectable frequency'
          '<listen_for_ms> = listen duration in milliseconds'
          '<amplitude_dB> = gain or attenuation to apply to the signal')
        Disconnect = False
        ExceptionReply.Code = ''
        Name = 'srvCmdSetParams'
        NormalReply.Code = '200'
        ParamDelimiter = ' '
        ParseParams = True
        Tag = 0
        HelpSuperScript = ' - Detect frequency'
        OnCommand = IdCmdTCPServer1CommandHandlers1Command
      end
      item
        CmdDelimiter = ' '
        Command = 'QUIT'
        Description.Strings = (
          'Close the current connection')
        Disconnect = False
        ExceptionReply.Code = ''
        Name = 'srvCmdQuit'
        NormalReply.Code = '200'
        ParamDelimiter = ' '
        ParseParams = False
        Tag = 0
        HelpSuperScript = ' - close connection'
        OnCommand = IdCmdTCPServer1CommandHandlers2Command
      end>
    ExceptionReply.Code = '500'
    ExceptionReply.Text.Strings = (
      'Unknown Internal Error')
    Greeting.Code = '200'
    Greeting.Text.Strings = (
      'Welcome, ')
    HelpReply.Code = '100'
    HelpReply.Text.Strings = (
      'Available commands:')
    MaxConnectionReply.Code = '300'
    MaxConnectionReply.Text.Strings = (
      'Too many connections. Try again later.')
    ReplyTexts = <>
    ReplyUnknownCommand.Code = '400'
    ReplyUnknownCommand.Text.Strings = (
      'Unknown Command')
    Left = 96
    Top = 120
  end
end
