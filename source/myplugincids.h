//------------------------------------------------------------------------
// Copyright(c) 2022 SBM.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace sbm {
//------------------------------------------------------------------------
static const Steinberg::FUID kLatencySpyProcessorUID (0x1E7D76F8, 0x99C75B2C, 0xA3407151, 0x98574547);
static const Steinberg::FUID kLatencySpyControllerUID (0xE88CC3E0, 0xB67E58FF, 0xBC46932F, 0x15E7965A);

#define LatencySpyVST3Category "Fx"

//------------------------------------------------------------------------
} // namespace sbm
