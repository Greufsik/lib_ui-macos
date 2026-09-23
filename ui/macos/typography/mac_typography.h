// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

// Point sizes and weights approximating San Francisco metrics.
// The actual family is resolved at runtime from fonts that exist on Windows
// (Segoe UI Variable / Segoe UI) so layout does not depend on SF being installed.

namespace MacTypography {

inline constexpr int kCaption = 11;
inline constexpr int kBody = 13;
inline constexpr int kCallout = 14;
inline constexpr int kTitle = 15;
inline constexpr int kHeadline = 17;

inline constexpr int kWeightRegular = 400;
inline constexpr int kWeightMedium = 510;
inline constexpr int kWeightSemibold = 590;
inline constexpr int kWeightBold = 700;

inline constexpr int kBodyLineHeight = 16;
inline constexpr int kTitleLineHeight = 20;

inline constexpr int kTrackingBody = 0;

} // namespace MacTypography
