//------------------------------------------------------------------------
// Copyright(c) 2022 SBM.
// 
// USE AT YOUR OWN RISK!
// THE AUTHOUR IS NOT RESPONSIBLE!
// 
//------------------------------------------------------------------------

#include <time.h>
#include <atomic>

#include "params.h"
#include "mypluginprocessor.h"
#include "myplugincids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include <pluginterfaces/vst/ivstprocesscontext.h>

struct ProcessContext;
using namespace Steinberg;
using namespace Steinberg::Vst;

namespace sbm
{
	// Prepered for some kind of thread safty..
	static std::atomic<int64> sFirstStartTime = 9223372036854775806;
	static std::atomic<float> sBiggestTimeSamples = -999999.9;

	//------------------------------------------------------------------------
	// LatencySpyProcessor
	//------------------------------------------------------------------------
	LatencySpyProcessor::LatencySpyProcessor()
	{
		//--- set the wanted controller for our processor
		setControllerClass(kLatencySpyControllerUID);
	}

	//------------------------------------------------------------------------
	LatencySpyProcessor::~LatencySpyProcessor()
	{}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LatencySpyProcessor::initialize(FUnknown* context)
	{
		// Here the Plug-in will be instanciated

		//---always initialize the parent-------
		tresult result = AudioEffect::initialize(context);
		// if everything Ok, continue
		if (result != kResultOk)
		{
			return result;
		}

		//--- create Audio IO ------
		addAudioInput(STR16("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
		addAudioOutput(STR16("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

		/* If you don't need an event bus, you can remove the next line */
		addEventInput(STR16("Event In"), 1);

		return kResultOk;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LatencySpyProcessor::terminate()
	{
		// Here the Plug-in will be de-instanciated, last possibility to remove some memory!

		//---do not forget to call parent ------
		return AudioEffect::terminate();
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LatencySpyProcessor::setActive(TBool state)
	{
		//--- called when the Plug-in is enable/disable (On/Off) -----
		return AudioEffect::setActive(state);
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LatencySpyProcessor::process(Vst::ProcessData& data)
	{
		//--- Calculate Latency and display..

		// If there is a processContext from the Host..
		if (data.processContext != NULL)
		{
			// Get the state (are we playing, stopping etc??..)
			uint32 state = data.processContext->state;

			// If the state is that we are playing or recording?
			if (state & Vst::ProcessContext::kPlaying || state & Vst::ProcessContext::kRecording)
			{
				// If this is the first run for this plugin, the test will be true..
				if (LatencySpyProcessor::mCalculatedLatency == -999999.9F)
				{
					// If our projectTimeSamples are bigger than the static sBiggestSamples
					// Then over write the old one with the new on from this pluign.
					// Here there should be some kind of locks! But this will do
					// Because all values will turn out fine when all plugins made their turn..
					if (sBiggestTimeSamples < data.processContext->projectTimeSamples)
						sBiggestTimeSamples = data.processContext->projectTimeSamples;

					// And save the reported ptojectTimeSamples,
					// because this will contains the latency compensation from the host..
					LatencySpyProcessor::mProjectTimeSamples = data.processContext->projectTimeSamples;

					// If this plugin has a smaller system Time than another (most likely not though)
					// then save this new one as the global static value also will need lock..
					if (clock() < sFirstStartTime)
						sFirstStartTime = clock();

					// And save this moment to the plugin's mSystemTime..
					LatencySpyProcessor::mSystemTime = clock();
				}

				// Create the "useless" return param..
				int32 i1 = 0;

				// Get the output params..
				IParameterChanges* pC = data.outputParameterChanges;

				// Initialize to be able to add patam data to the host..
				IParamValueQueue* qu = pC->addParameterData(kLatencyLblID, i1);

				// If possible..
				if (qu != NULL)
				{
					// One more "useless" return data..
					int32 i2 = 0;

					// Calculate the difference in "normal" count of samples reported minus this plugin's samples..
					LatencySpyProcessor::mCalculatedLatency = sBiggestTimeSamples - LatencySpyProcessor::mProjectTimeSamples;

					// Add this converted to millisecounds and normalize it before it's sent to the host..
					qu->addPoint(0, (LatencySpyProcessor::mCalculatedLatency / (data.processContext->sampleRate / 1000.0)) / 50000.0, i2);
				}

				// Initialize to be able to add param data to the host again..
				IParamValueQueue* qu2 = pC->addParameterData(kLatencyLblID2, i1);

				// If possible..
				if (qu2 != NULL)
				{
					// Again.. usesless..
					int32 i2 = 0;

					// Calculate the time difference from first reported plugin's start time and this plugin's (nano secounds)
					LatencySpyProcessor::mCalculatedSystemTimeLatency = LatencySpyProcessor::mSystemTime - sFirstStartTime;

					// Add this and normalize it before it's sent to the host,
					// which then will be sent to set the param in the plugin..
					qu2->addPoint(0, LatencySpyProcessor::mCalculatedSystemTimeLatency / 50000.0, i2);
				}
			}
			else
			{
				// If we have stopped playing, reset data..
				LatencySpyProcessor::mProjectTimeSamples = 0;
				LatencySpyProcessor::mCalculatedLatency = -999999.9;

				// This will be set by all instances
				// of the Latency Plugin but since they
				// never will do this simultainiously
				// a static non-thread safe way
				// will do for now.
				sBiggestTimeSamples = -999999.9;
				sFirstStartTime = 9223372036854775806;
			}
		}

		//--- First : Read inputs parameter changes-----------

		/*if (data.inputParameterChanges)
		{
			int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
			for (int32 index = 0; index < numParamsChanged; index++)
			{
				if (auto* paramQueue = data.inputParameterChanges->getParameterData (index))
				{
					Vst::ParamValue value;
					int32 sampleOffset;
					int32 numPoints = paramQueue->getPointCount ();
					switch (paramQueue->getParameterId ())
					{
					}
				}
			}
		}*/

		//--- Here you have to implement your processing

		if (data.numInputs == 0)
			return kResultOk;

		// This plugin does not need a mute functionallity..
		int32 numChannels = data.inputs[0].numChannels;
		Sample32** in = data.inputs[0].channelBuffers32;
		Sample32** out = data.outputs[0].channelBuffers32;

		for (int32 ch = 0; ch < numChannels; ch++)
		{
			Sample32* pIn = in[ch];
			Sample32* pOut = out[ch];
			Sample32 tmp;

			for (int32 i = 0; i < data.numSamples; i++)
			{
				tmp = *pIn;
				*pOut = tmp;
				pIn++;
				pOut++;
			}
		}

		return kResultOk;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LatencySpyProcessor::setupProcessing(Vst::ProcessSetup& newSetup)
	{
		//--- called before any processing ----
		return AudioEffect::setupProcessing(newSetup);
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LatencySpyProcessor::canProcessSampleSize(int32 symbolicSampleSize)
	{
		// by default kSample32 is supported
		if (symbolicSampleSize == Vst::kSample32)
			return kResultTrue;

		// disable the following comment if your processing support kSample64
		/* if (symbolicSampleSize == Vst::kSample64)
			return kResultTrue; */

		return kResultFalse;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LatencySpyProcessor::setState(IBStream* state)
	{
		// called when we load a preset, the model has to be reloaded
		IBStreamer streamer(state, kLittleEndian);

		return kResultOk;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LatencySpyProcessor::getState(IBStream* state)
	{
		// here we need to save the model
		IBStreamer streamer(state, kLittleEndian);

		return kResultOk;
	}

	//------------------------------------------------------------------------
} // namespace sbm
