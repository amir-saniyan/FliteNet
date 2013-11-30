#pragma once

#include <stddef.h>
#include <flite.h>

extern "C"
{
	cst_voice* register_cmu_time_awb(const char* voxdir);
	cst_voice* register_cmu_us_awb(const char* voxdir);
	cst_voice* register_cmu_us_kal(const char* voxdir);
	cst_voice* register_cmu_us_kal16(const char* voxdir);
	cst_voice* register_cmu_us_rms(const char* voxdir);
	cst_voice* register_cmu_us_slt(const char* voxdir);

	void unregister_cmu_time_awb(cst_voice* v);
	void unregister_cmu_us_awb(cst_voice* v);
	void unregister_cmu_us_kal(cst_voice* v);
	void unregister_cmu_us_kal16(cst_voice* v);
	void unregister_cmu_us_rms(cst_voice* v);
	void unregister_cmu_us_slt(cst_voice* v);
}

#include "WavePlayer.h"
#include "Utility.h"
#include "FliteSpeaker.h"
#include "FliteWave.h"
#include "FliteClass.h"
