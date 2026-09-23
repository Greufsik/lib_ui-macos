// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include "ui/effects/animation_value.h"

#include <QtCore/QEasingCurve>
#include <crl/crl_time.h>

// Durations are in milliseconds and match the short macOS control feedback
// used by traffic lights, menus and hover states.

namespace MacMotion {

inline constexpr crl::time kHoverDuration = 120;
inline constexpr crl::time kPressDuration = 80;
inline constexpr crl::time kPopoverDuration = 180;
inline constexpr crl::time kModalDuration = 220;
inline constexpr crl::time kNavigationDuration = 280;
inline constexpr crl::time kPanelDuration = 240;

[[nodiscard]] inline anim::transition HoverEasing() {
	return anim::easeOutCubic;
}

[[nodiscard]] inline anim::transition PopoverEasing() {
	return anim::easeOutCubic;
}

[[nodiscard]] inline anim::transition NavigationEasing() {
	return anim::sineInOut;
}

[[nodiscard]] inline QEasingCurve HoverCurve() {
	return QEasingCurve(QEasingCurve::OutCubic);
}

} // namespace MacMotion
