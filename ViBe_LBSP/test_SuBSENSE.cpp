#include "BackgroundSubtractorSuBSENSE.h"
#include "DistanceUtils.h"
#include "RandUtils.h"
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>

/*
 *
 * Intrinsic parameters for our method are defined here; tuning these for better
 * performance should not be required in most cases -- although improvements in
 * very specific scenarios are always possible.
 *
 */
//! defines the threshold value(s) used to detect long-term ghosting and trigger the fast edge-based absorption heuristic
#define GHOSTDET_D_MAX (0.010f) // defines 'negligible' change here
#define GHOSTDET_S_MIN (0.995f) // defines the required minimum local foreground saturation value
//! parameter used to scale dynamic distance threshold adjustments ('R(x)')
#define FEEDBACK_R_VAR (0.01f)
//! parameters used to adjust the variation step size of 'v(x)'
#define FEEDBACK_V_INCR  (1.000f)
#define FEEDBACK_V_DECR  (0.100f)
//! parameters used to scale dynamic learning rate adjustments  ('T(x)')
#define FEEDBACK_T_DECR  (0.2500f)
#define FEEDBACK_T_INCR  (0.5000f)
#define FEEDBACK_T_LOWER (2.0000f)
#define FEEDBACK_T_UPPER (256.00f)
//! parameters used to define 'unstable' regions, based on segm noise/bg dynamics and local dist threshold values
#define UNSTABLE_REG_RATIO_MIN (0.100f)
#define UNSTABLE_REG_RDIST_MIN (3.000f)
//! parameters used to scale the relative LBSP intensity threshold used for internal comparisons
#define LBSPDESC_NONZERO_RATIO_MIN (0.100f)
#define LBSPDESC_NONZERO_RATIO_MAX (0.500f)
//! parameters used to define model reset/learning rate boosts in our frame-level component
#define FRAMELEVEL_MIN_COLOR_DIFF_THRESHOLD  (m_nMinColorDistThreshold/2)
#define FRAMELEVEL_ANALYSIS_DOWNSAMPLE_RATIO (8)

// local define used to display debug information
#define DISPLAY_SUBSENSE_DEBUG_INFO 0
// local define used to specify the default frame size (320x240 = QVGA)
#define DEFAULT_FRAME_SIZE cv::Size(320,240)
// local define used to specify the color dist threshold offset used for unstable regions
#define STAB_COLOR_DIST_OFFSET (m_nMinColorDistThreshold/5)
// local define used to specify the desc dist threshold offset used for unstable regions
#define UNSTAB_DESC_DIST_OFFSET (m_nDescDistThresholdOffset)

static const size_t s_nColorMaxDataRange_1ch = UCHAR_MAX;
static const size_t s_nDescMaxDataRange_1ch = LBSP::DESC_SIZE*8;
static const size_t s_nColorMaxDataRange_3ch = s_nColorMaxDataRange_1ch*3;
static const size_t s_nDescMaxDataRange_3ch = s_nDescMaxDataRange_1ch*3;

BackgroundSubtractorSuBSENSE::BackgroundSubtractorSuBSENSE(	 float fRelLBSPThreshold
															,size_t nDescDistThresholdOffset
															,size_t nMinColorDistThreshold
															,size_t nBGSamples
															,size_t nRequiredBGSamples
															,size_t nSamplesForMovingAvgs)
	:	 BackgroundSubtractorLBSP(fRelLBSPThreshold)
		,m_nMinColorDistThreshold(nMinColorDistThreshold)
		,m_nDescDistThresholdOffset(nDescDistThresholdOffset)
		,m_nBGSamples(nBGSamples)
		,m_nRequiredBGSamples(nRequiredBGSamples)
		,m_nSamplesForMovingAvgs(nSamplesForMovingAvgs)
		,m_fLastNonZeroDescRatio(0.0f)
		,m_bLearningRateScalingEnabled(true)
		,m_fCurrLearningRateLowerCap(FEEDBACK_T_LOWER)
		,m_fCurrLearningRateUpperCap(FEEDBACK_T_UPPER)
		,m_nMedianBlurKernelSize(m_nDefaultMedianBlurKernelSize)
		,m_bUse3x3Spread(true) {
	CV_Assert(m_nBGSamples>0 && m_nRequiredBGSamples<=m_nBGSamples);
	CV_Assert(m_nMinColorDistThreshold>=STAB_COLOR_DIST_OFFSET);
}

BackgroundSubtractorSuBSENSE::~BackgroundSubtractorSuBSENSE() {
	if(m_aPxIdxLUT)
		delete[] m_aPxIdxLUT;
	if(m_aPxInfoLUT)
	    delete[] m_aPxInfoLUT;
}








void BackgroundSubtractorSuBSENSE::refreshModel(float fSamplesRefreshFrac, bool bForceFGUpdate) {

	// == refresh
	CV_Assert(m_bInitialized);
	CV_Assert(fSamplesRefreshFrac>0.0f && fSamplesRefreshFrac<=1.0f);
	const size_t nModelsToRefresh = fSamplesRefreshFrac<1.0f?(size_t)(fSamplesRefreshFrac*m_nBGSamples):m_nBGSamples;//ÎÊÌâÒ»
	const size_t nRefreshStartPos = fSamplesRefreshFrac<1.0f?rand()%m_nBGSamples:0;
	for(size_t nModelIter=0; nModelIter<m_nTotRelevantPxCount; ++nModelIter) {


	}






}