//------------------------------------------------------------------------
// Copyright(c) 2022 SBM.
//------------------------------------------------------------------------

#include "params.h"
#include "myplugincontroller.h"
#include "myplugincids.h"
#include "vstgui/plugin-bindings/vst3editor.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

namespace sbm
{

	//------------------------------------------------------------------------
	// LatencySpyController Implementation
	//------------------------------------------------------------------------
	tresult PLUGIN_API LatencySpyController::initialize(FUnknown* context)
	{
		// Here the Plug-in will be instanciated

		//---do not forget to call parent ------
		tresult result = EditControllerEx1::initialize(context);
		if (result != kResultOk)
		{
			return result;
		}

		// Here you could register some parameters

		// Create the buffer latency label presenter..
		mPI = new ParameterInfo();
		swprintf(mPI->title, 20, L"%hs", "latencyLbl");
		swprintf(mPI->units, 20, L"%hs", "ms");
		mPI->flags = ParameterInfo::kCanAutomate;
		mPI->stepCount = 50000.0;
		mPI->id = kLatencyLblID;
		mPI->defaultNormalizedValue = 0;

		// Add it to the params collection..
		if (mPI != NULL)
			parameters.addParameter(new RangeParameter(*mPI, 0.0, 50000.0));

		// Create the actual time latency label presenter..
		mPI2 = new ParameterInfo();
		swprintf(mPI2->title, 20, L"%hs", "latencyLbl2");
		swprintf(mPI2->units, 20, L"%hs", "ms");
		mPI2->flags = ParameterInfo::kCanAutomate;
		mPI2->stepCount = 50000.0;
		mPI2->id = kLatencyLblID2;
		mPI2->defaultNormalizedValue = 0;

		// Add the second one too..
		if (mPI2 != NULL)
			parameters.addParameter(new RangeParameter(*mPI2, 0, 50000.0));

		return result;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LatencySpyController::terminate()
	{
		// Here the Plug-in will be de-instanciated, last possibility to remove some memory!

		if (mPI != NULL)
		{
			delete(mPI);
			mPI = NULL;
		}
		if (mPI2 != NULL)
		{
			delete(mPI2);
			mPI2 = NULL;
		}

		//---do not forget to call parent ------
		return EditControllerEx1::terminate();
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LatencySpyController::setComponentState(IBStream* state)
	{
		// Here you get the state of the component (Processor part)
		if (!state)
			return kResultFalse;

		return kResultOk;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LatencySpyController::setState(IBStream* state)
	{
		// Here you get the state of the controller

		return kResultTrue;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LatencySpyController::getState(IBStream* state)
	{
		// Here you are asked to deliver the state of the controller (if needed)
		// Note: the real state of your plug-in is saved in the processor

		return kResultTrue;
	}

	//------------------------------------------------------------------------
	IPlugView* PLUGIN_API LatencySpyController::createView(FIDString name)
	{
		// Here the Host wants to open your editor (if you have one)
		if (FIDStringsEqual(name, Vst::ViewType::kEditor))
		{
			// create your editor here and return a IPlugView ptr of it
			auto* view = new VSTGUI::VST3Editor(this, "view", "myplugineditor.uidesc");
			return view;
		}
		return nullptr;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LatencySpyController::setParamNormalized(Vst::ParamID tag, Vst::ParamValue value)
	{
		// called by host to update your parameters
		tresult result = EditControllerEx1::setParamNormalized(tag, value);
		return result;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LatencySpyController::getParamStringByValue(Vst::ParamID tag, Vst::ParamValue valueNormalized, Vst::String128 string)
	{
		// called by host to get a string for given normalized value of a specific parameter
		// (without having to set the value!)
		return EditControllerEx1::getParamStringByValue(tag, valueNormalized, string);
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LatencySpyController::getParamValueByString(Vst::ParamID tag, Vst::TChar* string, Vst::ParamValue& valueNormalized)
	{
		// called by host to get a normalized value from a string representation of a specific parameter
		// (without having to set the value!)
		return EditControllerEx1::getParamValueByString(tag, string, valueNormalized);
	}

	//------------------------------------------------------------------------
} // namespace sbm
