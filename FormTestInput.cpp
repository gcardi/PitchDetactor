//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <System.SysUtils.hpp>

#include <memory>
#include <algorithm>
#include <limits>

#include "FormTestInput.h"
#include "Globals.h"
#include "SysUt.Fmt.h"
#include "Utils.h"

using std::make_unique;
using std::unique_ptr;
using std::min;
using std::max;
using std::numeric_limits;

using PitchDet::LogMessage;
using PitchDet::AppendWaveInDevices;
using PitchDet::ValueTodB;
using PitchDet::dBToValue;
using PitchDet::DiscretizeValueOnSortedRange;

using SysUt::Fmt;

using cycfi::q::leaky_integrator;
using cycfi::q::literals::operator""_Hz;
using cycfi::q::frequency;
using cycfi::q::decibel;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "FrameLevelMeter"
#pragma resource "*.dfm"
//TfrmTestInput *frmTestInput;
//---------------------------------------------------------------------------

__fastcall TfrmTestInput::TfrmTestInput(TComponent* Owner)
    : TConfigRegistryForm( Owner, StoreOpts::None, nullptr )
{
    channels_ = comboboxChannels->ItemIndex + 1;
    comboboxInputDev->Items->Append( _D( "Wave Mapper" ) );
    AppendWaveInDevices( *comboboxInputDev->Items );
    FillSampleRateCombo();
    FillBufferLenCombo();
    RestoreProperties();
}
//---------------------------------------------------------------------------

__fastcall TfrmTestInput::~TfrmTestInput()
{
    try {
        wi_.reset();
        SaveProperties();
    }
    catch ( ... ) {
    }
}
//---------------------------------------------------------------------------

void TfrmTestInput::FillSampleRateCombo()
{
    for ( auto SampleRate : SampleRates ) {
        comboboxSampleRate->Items->Append( SampleRate );
    }
}
//---------------------------------------------------------------------------

void TfrmTestInput::FillBufferLenCombo()
{
int n {};
    for ( auto BuffLen : BuffLenghts ) {
        comboboxBufferLen->Items->Append( BuffLen );
SysUt::OutputDebugString( _D( "%d-%d" ), n++, BuffLen );
    }
}
//---------------------------------------------------------------------------

void TfrmTestInput::RestoreProperties()
{
    RESTORE_LOCAL_PROPERTY( InputDeviceName );
    RESTORE_LOCAL_PROPERTY( Channels );
    RESTORE_LOCAL_PROPERTY( Sps );
    RESTORE_LOCAL_PROPERTY( OutputFileName );
    RESTORE_LOCAL_PROPERTY( SaveToFile );
    RESTORE_LOCAL_PROPERTY( LoFreq );
    RESTORE_LOCAL_PROPERTY( HiFreq );
    RESTORE_LOCAL_PROPERTY( Hysteresis );
    RESTORE_LOCAL_PROPERTY( OverallGain_dB );
    RESTORE_LOCAL_PROPERTY( BufferLen );
}
//---------------------------------------------------------------------------

void TfrmTestInput::SaveProperties() const
{
    SAVE_LOCAL_PROPERTY( InputDeviceName );
    SAVE_LOCAL_PROPERTY( Channels );
    SAVE_LOCAL_PROPERTY( Sps );
    SAVE_LOCAL_PROPERTY( OutputFileName );
    SAVE_LOCAL_PROPERTY( SaveToFile );
    SAVE_LOCAL_PROPERTY( LoFreq );
    SAVE_LOCAL_PROPERTY( HiFreq );
    SAVE_LOCAL_PROPERTY( Hysteresis );
    SAVE_LOCAL_PROPERTY( OverallGain_dB );
    SAVE_LOCAL_PROPERTY( BufferLen );
}
//---------------------------------------------------------------------------

String TfrmTestInput::GetInputDeviceName() const
{
    return comboboxInputDev->Text;
}
//---------------------------------------------------------------------------

void TfrmTestInput::SetInputDeviceName( String Val )
{
    if ( auto Idx = comboboxInputDev->Items->IndexOf( Val ) ; Idx >= 0 ) {
        comboboxInputDev->ItemIndex = Idx;
    }
    else {
        comboboxInputDev->ItemIndex = 0;
    }
}
//---------------------------------------------------------------------------

int TfrmTestInput::GetChannels() const
{
    return channels_;
}
//---------------------------------------------------------------------------

void TfrmTestInput::SetChannels( int Val )
{
    channels_ = max( min( comboboxChannels->Items->Count,  Val ), 1 );
    comboboxChannels->ItemIndex = channels_ - 1;
    UpdateLevelMetersConfig();
}
//---------------------------------------------------------------------------

int TfrmTestInput::GetSps() const
{
//    return comboboxSampleRate->Text.ToIntDef( 48000 );
    return sps_.load();
}
//---------------------------------------------------------------------------

void TfrmTestInput::SetSps( int Val )
{
    auto Result =
        DiscretizeValueOnSortedRange(
            begin( SampleRates ),
            end( SampleRates ),
            Val
        );
    comboboxSampleRate->ItemIndex = Result.first;
    sps_ = Result.second;;
}
//---------------------------------------------------------------------------

String TfrmTestInput::GetOutputFileName() const
{
    return Trim( edtOutFileName->Text );
}
//---------------------------------------------------------------------------

void TfrmTestInput::SetOutputFileName( String Val )
{
    edtOutFileName->Text = Trim( Val );
}
//---------------------------------------------------------------------------

bool TfrmTestInput::GetSaveToFile() const
{
    return checkboxSaveToFile->Checked;
}
//---------------------------------------------------------------------------

void TfrmTestInput::SetSaveToFile( bool Val )
{
    checkboxSaveToFile->Checked = Val;
}
//---------------------------------------------------------------------------

double TfrmTestInput::GetLoFreq() const
{
    return StrToFloatDef( edtLoFreq->Text, 100.0 );
}
//---------------------------------------------------------------------------

void TfrmTestInput::SetLoFreq( double Val )
{
    edtLoFreq->Text = Fmt( _D( "%.0f" ), Val );
}
//---------------------------------------------------------------------------

double TfrmTestInput::GetHiFreq() const
{
    return StrToFloatDef( edtHiFreq->Text, 2000.0 );
}
//---------------------------------------------------------------------------

void TfrmTestInput::SetHiFreq( double Val )
{
    edtHiFreq->Text = Fmt( _D( "%.0f" ), Val );
}
//---------------------------------------------------------------------------

double TfrmTestInput::GetHysteresis() const
{
    return StrToFloatDef( edtHyst->Text, 3.0 );
}
//---------------------------------------------------------------------------

void TfrmTestInput::SetHysteresis( double Val )
{
    edtHyst->Text = Fmt( _D( "%.1f" ), Val );
}
//---------------------------------------------------------------------------

double TfrmTestInput::GetOverallGain_dB() const
{
    return StrToFloatDef( edtOverallGain_dB->Text, 0.0 );
}
//---------------------------------------------------------------------------

void TfrmTestInput::SetOverallGain_dB( double Val )
{
    edtOverallGain_dB->Text = Fmt( _D( "%.1f" ), Val );
    overallGain_ = dBToValue( Val );
}
//---------------------------------------------------------------------------

double TfrmTestInput::GetDetectionThreshold_dB() const
{
    return StrToFloatDef( edtThreshold_dB->Text, -50.0 );
}
//---------------------------------------------------------------------------

void TfrmTestInput::SetDetectionThreshold_dB( double Val )
{
    edtThreshold_dB->Text = Fmt( _D( "%.1f" ), Val );
    detectionThreshold_ = dBToValue( Val );
}
//---------------------------------------------------------------------------

int TfrmTestInput::GetBufferLen() const
{
    return bufferLen_;
}
//---------------------------------------------------------------------------

void TfrmTestInput::SetBufferLen( int Val )
{
    auto Result =
        DiscretizeValueOnSortedRange(
            begin( BuffLenghts ),
            end( BuffLenghts ),
            Val
        );
    comboboxBufferLen->ItemIndex = Result.first;
    bufferLen_ = Result.second;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTestInput::speedbtnBrowseOutFileClick(TObject *Sender)
{
    FileSaveDialog1->FileName = OutputFileName;
    if ( FileSaveDialog1->Execute() ) {
        OutputFileName = FileSaveDialog1->FileName;
    }
}
//---------------------------------------------------------------------------

int TfrmTestInput::GetInputDeviceIndex() const
{
    return comboboxInputDev->ItemIndex - 1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTestInput::actStartExecute(TObject *Sender)
{
    try {
        if ( SaveToFile ) {
            outStream_ = make_unique<TFileStream>( OutputFileName, fmCreate );
        }

        CreatePitchDetector();
        CreateIntegrators();
        wi_ =
            make_unique<WaveInType>(
                GetInputDeviceIndex(),
                Channels,
                bufferLen_,
                Sps,
                [this]( auto& Buffer ) {
                    FeedIntegrators( Buffer );
                    FeedPitchDetector( Buffer );
                    if ( outStream_ ) {
                        outStream_->WriteBuffer(
                            Buffer.data(),
                            Buffer.size() * sizeof( WaveInType::SampleType )
                        );
                    }
                }
            );
        wi_->Start();
        tmrLevelMeter->Enabled = true;
    }
    catch ( Exception const & E ) {
        outStream_.reset();
        throw;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTestInput::actStartUpdate(TObject *Sender)
{
    TAction& Act = static_cast<TAction&>( *Sender );
    Act.Enabled = !wi_ || !wi_->IsRunning();
}
//---------------------------------------------------------------------------

void __fastcall TfrmTestInput::actStopExecute(TObject *Sender)
{
    tmrLevelMeter->Enabled = false;
    wi_.reset();
    outStream_.reset();
    outLevel_[0] = {};
    outLevel_[1] = {};
    frmeLevelMeterLeft->Reset();
    frmeLevelMeterRight->Reset();
}
//---------------------------------------------------------------------------

void __fastcall TfrmTestInput::actStopUpdate(TObject *Sender)
{
    TAction& Act = static_cast<TAction&>( *Sender );
    Act.Enabled = wi_ && wi_->IsRunning();
}
//---------------------------------------------------------------------------

void __fastcall TfrmTestInput::tmrLevelMeterTimer(TObject *Sender)
{
    auto LVal = outLevel_[0].load();
    auto RVal = outLevel_[1].load();
    auto Freq = freq_.load();

    frmeLevelMeterLeft->Value = LVal;
    frmeLevelMeterRight->Value = RVal;

    // https://www.extron.com/calculators/db-to-volts/?tab=tools
    //
    // dBV = dBu - 2,21
    // V = 10( dBu / 20 ) x 0,775
    // dBu = dBV + 2,21
    // V = 10^( dBV / 20 )
    // dBu = 20 log10( V / 0,775 )
    // dBV = 20 log10( V )

    float OutLevel;

    switch ( Channels ) {
        case 1:
            OutLevel = LVal;
            break;
        case 2:
            OutLevel = ( LVal + RVal ) / 2.0F;
            break;
        default:
            break;
    }

    if ( OutLevel > detectionThreshold_ && Freq > 1.0F ) {
        lblFreq->Caption = Fmt( _D( "%.2f Hz" ), Freq );
    }
    else {
        lblFreq->Caption =
            Fmt( _D( "-%s-- Hz" ), FormatSettings.DecimalSeparator );
    }

    lblLevel->Caption = Fmt( _D( "%.1f dB" ), ValueTodB( OutLevel ) );
}
//---------------------------------------------------------------------------

void __fastcall TfrmTestInput::ApplicationEvents1Idle(TObject *Sender, bool &Done)
{
    auto Enb = !wi_ || !wi_->IsRunning();
    comboboxChannels->Enabled = Enb;
    comboboxInputDev->Enabled = Enb;
    comboboxSampleRate->Enabled = Enb;
    edtOutFileName->Enabled = Enb;
    speedbtnBrowseOutFile->Enabled = Enb;
    checkboxSaveToFile->Enabled = Enb;
    edtLoFreq->Enabled = Enb;
    edtHiFreq->Enabled = Enb;
    edtHyst->Enabled = Enb;
    edtOverallGain_dB->Enabled = Enb;
    comboboxBufferLen->Enabled = Enb;
    edtThreshold_dB->Enabled = Enb;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTestInput::comboboxChannelsChange(TObject *Sender)
{
    channels_ = comboboxChannels->ItemIndex + 1;
    UpdateLevelMetersConfig();
}
//---------------------------------------------------------------------------

void __fastcall TfrmTestInput::comboboxSampleRateChange(TObject *Sender)
{
    sps_ = SampleRates[comboboxSampleRate->ItemIndex];
}
//---------------------------------------------------------------------------

void __fastcall TfrmTestInput::comboboxBufferLenChange(TObject *Sender)
{
    bufferLen_ = BuffLenghts[comboboxBufferLen->ItemIndex];
}
//---------------------------------------------------------------------------

void TfrmTestInput::UpdateLevelMetersConfig()
{
    switch ( Channels ) {
        case 1:
            lblMeterLeft->Caption = _T( "M" );
            pnlMeterLeft->Visible = true;
            pnlMeterRight->Visible = false;
            break;
        case 2:
            lblMeterLeft->Caption = _T( "L" );
            pnlMeterLeft->Visible = true;
            pnlMeterRight->Visible = true;
            break;
        default:
            pnlMeterLeft->Visible = false;
            pnlMeterRight->Visible = false;
            break;
    }
    frmeLevelMeterLeft->LoLim_dB = -64.0;
    frmeLevelMeterRight->LoLim_dB = -64.0;
}
//---------------------------------------------------------------------------

void TfrmTestInput::CreatePitchDetector()
{
    pd_ = make_unique<PitchDetType>(
        frequency( LoFreq ),
        frequency( HiFreq ),
        Sps,
        decibel( Hysteresis )
    );
}
//---------------------------------------------------------------------------

void TfrmTestInput::CreateIntegrators()
{
    li_[0] = make_unique<IntegratorType>( 20_Hz, Sps );
    li_[1] = make_unique<IntegratorType>( 20_Hz, Sps );
}
//---------------------------------------------------------------------------

float TfrmTestInput::FeedIntegrators( WaveInType::BufferCont const & Buffer )
{
    auto Gain = overallGain_.load();
    auto Len = Buffer.size();
    switch ( Channels ) {
        case 1:
            {
                float Out {};
                for ( size_t Idx {} ; Idx < Len ; ++Idx ) {
                    auto Out = (*li_[0])(
                        fabs( Gain * static_cast<float>( Buffer[Idx] ) ) /
                        std::numeric_limits<WaveInType::SampleType>::max()
                    );
                    outLevel_[0] = Out;
                }
                return Out;
            }
        case 2:
            {
                float OutL {};
                float OutR {};
                for ( size_t Idx {} ; Idx < Len ; Idx += 2 ) {
                    OutL = (*li_[0])(
                        fabs( Gain * static_cast<float>( Buffer[Idx] ) ) /
                        std::numeric_limits<WaveInType::SampleType>::max()
                    );
                    OutR = (*li_[1])(
                        fabs( Gain * static_cast<float>( Buffer[Idx + 1] ) ) /
                        std::numeric_limits<WaveInType::SampleType>::max()
                    );
                    outLevel_[0] = OutL;
                    outLevel_[1] = OutR;
                }
                return ( OutL + OutR ) / 2.0F;
            }
        default:
            break;
    }
    return {};
}
//---------------------------------------------------------------------------

void TfrmTestInput::FeedPitchDetector( WaveInType::BufferCont const & Buffer )
{
    auto Gain = overallGain_.load();
    auto Len = Buffer.size();
    pd_->reset();
    for ( WaveInType::BufferCont::size_type Idx {} ; Idx < Len ; Idx += Channels ) {
        (*pd_)( Gain * static_cast<float>( Buffer[Idx] ) );
    }
    freq_ = pd_->get_frequency();
    //pd_->reset();
}
//---------------------------------------------------------------------------

void __fastcall TfrmTestInput::edtOverallGain_dBKeyPress(TObject *Sender, System::WideChar &Key)
{
    if ( Key == VK_RETURN ) {
        overallGain_ = dBToValue( GetOverallGain_dB() );
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTestInput::edtOverallGain_dBExit(TObject *Sender)
{
    overallGain_ = dBToValue( GetOverallGain_dB() );
}
//---------------------------------------------------------------------------



