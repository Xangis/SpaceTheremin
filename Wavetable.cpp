#include "Wavetable.h"
#include <math.h>

/**
* Generates wavetables for each of the waveform types defined in the header.
*/
void WaveTable::CreateWavetables()
{
	int count;
    float nineTenths = (float)TABLESIZE * 0.95f;
	for( count = 0; count < TABLESIZE; count++ )
	{
		_waveformTable[WAVEFORM_SINE][count] = (sin(2.0f*PI*((float)count/(float)TABLESIZE)));
        if( count < nineTenths)
        {
		    _waveformTable[WAVEFORM_SAW][count] = (((float)count / (float)nineTenths));
        }
        else
        {
            _waveformTable[WAVEFORM_SAW][count] = 1.0f - (((float)count - nineTenths) / (TABLESIZE / 20));
        }
		_waveformTable[WAVEFORM_SINC][count] = fabs(_waveformTable[WAVEFORM_SINE][count]);
		if( count < (TABLESIZE/2))
		{
			_waveformTable[WAVEFORM_TRIANGLE][count] = (-1.0f + (2.0f * ((float)count*2.0f / (float)TABLESIZE)));
		}
		else
		{
			_waveformTable[WAVEFORM_TRIANGLE][count] = (1.0f - (2.0f * ((float)((count - (TABLESIZE/2.0f)) * 2.0f) / (float)TABLESIZE)));
		}
		if( count < (TABLESIZE/4))
		{
			_waveformTable[WAVEFORM_PULSE][count] = 1.0f;
		}
		else
		{
			_waveformTable[WAVEFORM_PULSE][count] = 0.0f;
		}
		if( count < (TABLESIZE / 2))
		{
			_waveformTable[WAVEFORM_SQUARE][count] = 1.0f;
		}
		else
		{
			_waveformTable[WAVEFORM_SQUARE][count] = 0.0f;
		}
        if( count < (TABLESIZE /3))
        {
            _waveformTable[WAVEFORM_TRAPEZOID][count] = (float)count / ((float)TABLESIZE / 3.0f);
        }
        else if( count < (TABLESIZE * 2 / 3))
        {
            _waveformTable[WAVEFORM_TRAPEZOID][count] = 1.0f;
        }
        else
        {
            _waveformTable[WAVEFORM_TRAPEZOID][count] = ((float)count - ((float)TABLESIZE * 2.0f / 3.0f)) / ((float)TABLESIZE / 3.0f);
        }
	}
}

/**
* Gets the number of waveforms in the wave table.  Valid waveforms are from 0 to NUM_WAVEFORMS-1.
*/
int WaveTable::GetNumWaveforms()
{
    return NUM_WAVEFORMS;
}

/**
* Gets the name of a waveform based on its number.  Valid waveforms are from 0 to NUM_WAVEFORMS-1.
*/
const char* WaveTable::GetWaveformName(int number)
{
    switch( number )
    {
    case 0:
        return "Sine";
    case 1:
        return "Square";
    case 2:
        return "Saw";
    case 3:
        return "Triangle";
    case 4:
        return "Sinc";
    case 5:
        return "Pulse 25%";
    case 6:
        return "Trapezoid";
    default:
        return "Invalid - Index Out of Range";
    }
}