//------------------------------------------------------------------------
// Copyright(c) 2022 SBM.
//------------------------------------------------------------------------

#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

namespace sbm
{

	//------------------------------------------------------------------------
	//  LatencySpyProcessor
	//------------------------------------------------------------------------
	class LatencySpyProcessor : public Steinberg::Vst::AudioEffect
	{
	public:
		LatencySpyProcessor();
		~LatencySpyProcessor() SMTG_OVERRIDE;

		// Create function
		static Steinberg::FUnknown* createInstance(void* /*context*/)
		{
			return (Steinberg::Vst::IAudioProcessor*)new LatencySpyProcessor;
		}

		//--- ---------------------------------------------------------------------
		// AudioEffect overrides:
		//--- ---------------------------------------------------------------------
		/** Called at first after constructor */
		Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown* context) SMTG_OVERRIDE;

		/** Called at the end before destructor */
		Steinberg::tresult PLUGIN_API terminate() SMTG_OVERRIDE;

		/** Switch the Plug-in on/off */
		Steinberg::tresult PLUGIN_API setActive(Steinberg::TBool state) SMTG_OVERRIDE;

		/** Will be called before any process call */
		Steinberg::tresult PLUGIN_API setupProcessing(Steinberg::Vst::ProcessSetup& newSetup) SMTG_OVERRIDE;

		/** Asks if a given sample size is supported see SymbolicSampleSizes. */
		Steinberg::tresult PLUGIN_API canProcessSampleSize(Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;

		/** Here we go...the process call */
		Steinberg::tresult PLUGIN_API process(Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;

		/** For persistence */
		Steinberg::tresult PLUGIN_API setState(Steinberg::IBStream* state) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API getState(Steinberg::IBStream* state) SMTG_OVERRIDE;

		//------------------------------------------------------------------------
	protected:

		float mProjectTimeSamples = 0;
		float mCalculatedLatency = -999999.9; // This value is enough we are only supporting up to 50 sec.
		Steinberg::int64 mSystemTime = 0;
		Steinberg::int64 mCalculatedSystemTimeLatency = 0;
	};

	//------------------------------------------------------------------------
} // namespace sbm
