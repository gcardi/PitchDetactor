//---------------------------------------------------------------------------

#ifndef FormTestInputH
#define FormTestInputH

//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "FrameLevelMeter.h"
#include <Vcl.AppEvnts.hpp>

#include <memory>
#include <atomic>
#include <array>

#include <anafestica/PersistFormVCL.h>
#include <anafestica/CfgRegistrySingleton.h>

#include <q/fx/lowpass.hpp>

#define __GNUC__    // q_lib non supporta nativamente bcc32c|bcc64
#include <q/pitch/pitch_detector.hpp>
#undef __GNUC__

#include "WaveIn.h"

//---------------------------------------------------------------------------

using TConfigRegistryForm =
    Anafestica::TPersistFormVCL<Anafestica::TConfigRegistrySingleton>;

class TfrmTestInput : public TConfigRegistryForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TLabel *Label2;
    TEdit *edtOutFileName;
    TFileSaveDialog *FileSaveDialog1;
    TActionList *ActionList1;
    TAction *actStart;
    TAction *actStop;
    TLabel *Label3;
    TComboBox *comboboxChannels;
    TButton *Button1;
    TButton *Button2;
    TSpeedButton *speedbtnBrowseOutFile;
    TCheckBox *checkboxSaveToFile;
    TComboBox *comboboxSampleRate;
    TLabel *Label4;
    TTimer *tmrLevelMeter;
    TfrmeLevelMeter *frmeLevelMeterLeft;
    TApplicationEvents *ApplicationEvents1;
    TPanel *pnlMeterLeft;
    TPanel *pnlMeterRight;
    TfrmeLevelMeter *frmeLevelMeterRight;
    TLabel *lblMeterLeft;
    TLabel *lblMeterRight;
    TComboBox *comboboxInputDev;
    TEdit *edtLoFreq;
    TEdit *edtHiFreq;
    TEdit *edtHyst;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *lblFreq;
    TLabel *lblLevel;
    TLabel *Label8;
    TEdit *edtOverallGain_dB;
    TComboBox *comboboxBufferLen;
    TLabel *Label9;
    TEdit *edtThreshold_dB;
    TLabel *Label10;
    void __fastcall speedbtnBrowseOutFileClick(TObject *Sender);
    void __fastcall actStartExecute(TObject *Sender);
    void __fastcall actStartUpdate(TObject *Sender);
    void __fastcall actStopExecute(TObject *Sender);
    void __fastcall actStopUpdate(TObject *Sender);
    void __fastcall tmrLevelMeterTimer(TObject *Sender);
    void __fastcall ApplicationEvents1Idle(TObject *Sender, bool &Done);
    void __fastcall comboboxChannelsChange(TObject *Sender);
    void __fastcall edtOverallGain_dBKeyPress(TObject *Sender, System::WideChar &Key);
    void __fastcall edtOverallGain_dBExit(TObject *Sender);
    void __fastcall comboboxSampleRateChange(TObject *Sender);
    void __fastcall comboboxBufferLenChange(TObject *Sender);

private:	// User declarations
    using WaveInType = PitchDet::WaveInCO<int16_t>;
    using WaveInPtr = std::unique_ptr<WaveInType>;
    using OutStreamPtr = std::unique_ptr<TStream>;
    using IntegratorType = cycfi::q::leaky_integrator;
    using IntegratorPtr = std::unique_ptr<IntegratorType>;
    using PitchDetType = cycfi::q::pitch_detector;
    using PitchDetPtr = std::unique_ptr<PitchDetType>;

    WaveInPtr wi_;
    OutStreamPtr outStream_;
    std::array<IntegratorPtr,2> li_;
    std::array<std::atomic<float>,2> outLevel_;
    PitchDetPtr pd_;
    std::atomic<float> freq_;
    std::atomic<int> channels_;
    std::atomic<float> overallGain_ { 1.0F };
    std::atomic<int> sps_ { 24000 };
    int bufferLen_ { 1024 };
    float detectionThreshold_ { PitchDet::dBToValue( -50.0F ) };

    static constexpr std::array<size_t,6> SampleRates {
        22050, 24000, 44100, 48000, 96000, 192000,
    };

    static constexpr std::array<size_t,6> BuffLenghts {
        256, 512, 1024, 2048, 4096, 8192,
    };

    void FillSampleRateCombo();
    void FillBufferLenCombo();
    void RestoreProperties();
    void SaveProperties() const;
    [[nodiscard]] String GetInputDeviceName() const;
    void SetInputDeviceName( String Val );
    [[nodiscard]] int GetChannels() const;
    void SetChannels( int Val );
    [[nodiscard]] int GetSps() const;
    void SetSps( int Val );
    [[nodiscard]] String GetOutputFileName() const;
    void SetOutputFileName( String Val );
    [[nodiscard]] bool GetSaveToFile() const;
    void SetSaveToFile( bool Val );
    void UpdateLevelMetersConfig();
    void CreateIntegrators();
    void CreatePitchDetector();
    float FeedIntegrators( WaveInType::BufferCont const & Buffer );
    void FeedPitchDetector( WaveInType::BufferCont const & Buffer );
    [[nodiscard]] int GetInputDeviceIndex() const;
    [[nodiscard]] double GetLoFreq() const;
    void SetLoFreq( double Val );
    [[nodiscard]] double GetHiFreq() const;
    void SetHiFreq( double Val );
    [[nodiscard]] double GetHysteresis() const;
    void SetHysteresis( double Val );
    [[nodiscard]] double GetOverallGain_dB() const;
    void SetOverallGain_dB( double Val );
    [[nodiscard]] int GetBufferLen() const;
    void SetBufferLen( int Val );
    [[nodiscard]] double GetDetectionThreshold_dB() const;
    void SetDetectionThreshold_dB( double Val );
protected:
public:		// User declarations
    __fastcall TfrmTestInput(TComponent* Owner) override;
    __fastcall ~TfrmTestInput();
    __property String InputDeviceName = {
        read = GetInputDeviceName, write = SetInputDeviceName
    };
    __property int Channels = { read = GetChannels, write = SetChannels };
    __property int Sps = { read = GetSps, write = SetSps };
    __property String OutputFileName = {
        read = GetOutputFileName, write = SetOutputFileName
    };
    __property bool SaveToFile = { read = GetSaveToFile, write = SetSaveToFile };
    __property double LoFreq = { read = GetLoFreq, write = SetLoFreq };
    __property double HiFreq = { read = GetHiFreq, write = SetHiFreq };
    __property double Hysteresis = { read = GetHysteresis, write = SetHysteresis };
    __property double OverallGain_dB = {
        read = GetOverallGain_dB, write = SetOverallGain_dB
    };
    __property int BufferLen = { read = GetBufferLen, write = SetBufferLen };
    __property double DetectionThreshold_dB = {
        read = GetDetectionThreshold_dB, write = SetDetectionThreshold_dB
    };
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmTestInput *frmTestInput;
//---------------------------------------------------------------------------
#endif
