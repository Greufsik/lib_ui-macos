// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include "base/assertion.h"
#include "base/basic_types.h"
#include "styles/palette.h"
#include "ui/style/style_core_scale.h"

#include <QtGui/QColor>

#include <algorithm>

// Central macOS presentation tokens for the Windows port.
// Geometry values are in points at 100% interface scale.
// style::ConvertScale keeps them aligned with tdesktop DPI / scale.

namespace MacTheme {

[[nodiscard]] inline int Px(int points) {
	return style::ConvertScale(points);
}

[[nodiscard]] inline QColor Blend(QColor from, QColor to, float64 amount) {
	const auto t = std::clamp(amount, 0., 1.);
	return QColor(
		int(from.red() + (to.red() - from.red()) * t),
		int(from.green() + (to.green() - from.green()) * t),
		int(from.blue() + (to.blue() - from.blue()) * t),
		int(from.alpha() + (to.alpha() - from.alpha()) * t));
}

[[nodiscard]] inline bool ChromeIsDark() {
	return st::windowBg->c.lightness() < 128;
}

// Product name drawn in the Windows window shell. The rest of the
// Telegram branding stays until the app is renamed.
inline constexpr wchar_t kShellName[] = L"YangeGram";

// Title bar. Tall enough that the lights sit inside the rounded corner.
inline constexpr int kTitleBarHeight = 38;

// Traffic lights. First circle starts 20pt from the window edge, with an
// 8pt gap, matching the macOS reference inset. The slot is wider than the
// circle so leftSkip + centering lands on that origin.
inline constexpr int kTrafficLightDiameter = 13;
inline constexpr int kTrafficLightSlot = 21;
inline constexpr int kTrafficLightOriginX = 16;
inline constexpr int kTrafficLightOutline = 2;

inline constexpr int kCornerRadiusSmall = 6;
inline constexpr int kCornerRadiusMedium = 10;
inline constexpr int kCornerRadiusLarge = 12;
inline constexpr int kWindowCornerRadius = 12;

// Icon rail from Telegram for macOS, not the wide named chat list.
inline constexpr int kSidebarWidth = 72;
inline constexpr int kChatRowHeight = 70;
inline constexpr int kAvatarSize = 50;
inline constexpr int kAvatarOrigin = 10;
inline constexpr int kSearchHeight = 30;
inline constexpr int kChatHeaderHeight = 52;

[[nodiscard]] inline int SidebarWidth() {
	return Px(kSidebarWidth);
}

[[nodiscard]] inline int ChatRowHeight() {
	return Px(kChatRowHeight);
}

[[nodiscard]] inline int AvatarSize() {
	return Px(kAvatarSize);
}

[[nodiscard]] inline int TitleBarHeight() {
	return Px(kTitleBarHeight);
}

[[nodiscard]] inline int TrafficLightDiameter() {
	return Px(kTrafficLightDiameter);
}

[[nodiscard]] inline int TrafficLightSlot() {
	return Px(kTrafficLightSlot);
}

[[nodiscard]] inline int TrafficLightOriginX() {
	return Px(kTrafficLightOriginX);
}

[[nodiscard]] inline int CornerRadiusMedium() {
	return Px(kCornerRadiusMedium);
}

[[nodiscard]] inline QColor TitleBarBackground(bool active) {
	const auto dark = ChromeIsDark();
	if (dark) {
		return active ? QColor(28, 28, 30) : QColor(36, 36, 38);
	}
	return active ? QColor(246, 246, 246) : QColor(236, 236, 236);
}

[[nodiscard]] inline QColor TitleBarSeparator() {
	return ChromeIsDark()
		? QColor(255, 255, 255, 24)
		: QColor(0, 0, 0, 28);
}

[[nodiscard]] inline QColor SidebarBackground() {
	return ChromeIsDark() ? QColor(28, 28, 30) : QColor(255, 255, 255);
}

enum class TrafficLight {
	Close,
	Minimize,
	Zoom,
};

[[nodiscard]] inline QColor TrafficLightFill(TrafficLight light) {
	switch (light) {
	case TrafficLight::Close: return QColor(255, 95, 86);
	case TrafficLight::Minimize: return QColor(255, 189, 46);
	case TrafficLight::Zoom: return QColor(40, 200, 64);
	}
	Unexpected("Traffic light in MacTheme::TrafficLightFill.");
}

[[nodiscard]] inline QColor TrafficLightBorder(TrafficLight light) {
	switch (light) {
	case TrafficLight::Close: return QColor(226, 70, 64);
	case TrafficLight::Minimize: return QColor(224, 161, 0);
	case TrafficLight::Zoom: return QColor(26, 171, 46);
	}
	Unexpected("Traffic light in MacTheme::TrafficLightBorder.");
}

[[nodiscard]] inline QColor TrafficLightInactiveFill() {
	return ChromeIsDark() ? QColor(68, 68, 70) : QColor(220, 220, 220);
}

[[nodiscard]] inline QColor TrafficLightInactiveBorder() {
	return ChromeIsDark() ? QColor(52, 52, 54) : QColor(198, 198, 198);
}

[[nodiscard]] inline QColor TrafficLightGlyph() {
	return QColor(0, 0, 0, 150);
}

[[nodiscard]] inline QColor TrafficLightPressed(QColor fill) {
	return fill.darker(118);
}

} // namespace MacTheme
