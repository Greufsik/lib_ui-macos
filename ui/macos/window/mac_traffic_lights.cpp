// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include "ui/macos/window/mac_traffic_lights.h"

#include "ui/abstract_button.h"
#include "ui/effects/animations.h"
#include "ui/integration.h"
#include "ui/macos/icons/mac_icon_metrics.h"
#include "ui/macos/motion/mac_motion.h"
#include "ui/macos/theme/mac_theme.h"
#include "ui/painter.h"
#include "styles/style_widgets.h"

#include <QtGui/QPainter>
#include <QtGui/QPainterPath>

namespace Ui::Platform {
namespace {

class MacTrafficLightButton final : public AbstractButton {
public:
	MacTrafficLightButton(
		QWidget *parent,
		MacTheme::TrafficLight kind,
		Fn<void()> hoverChanged);

	void applyState(bool active, bool maximized, int height);
	void setGroupHovered(bool hovered);

	[[nodiscard]] MacTheme::TrafficLight kind() const {
		return _kind;
	}

protected:
	void paintEvent(QPaintEvent *e) override;
	void enterEventHook(QEnterEvent *e) override;
	void leaveEventHook(QEvent *e) override;

private:
	void paintGlyph(QPainter &p, const QRectF &circle) const;

	MacTheme::TrafficLight _kind;
	Fn<void()> _hoverChanged;
	Ui::Animations::Simple _hover;
	bool _active = true;
	bool _maximized = false;
	bool _groupHovered = false;

};

MacTrafficLightButton::MacTrafficLightButton(
	QWidget *parent,
	MacTheme::TrafficLight kind,
	Fn<void()> hoverChanged)
: AbstractButton(parent)
, _kind(kind)
, _hoverChanged(std::move(hoverChanged)) {
	setPointerCursor(false);
	setFixedSize(MacTheme::TrafficLightSlot(), st::defaultWindowTitle.height);
}

void MacTrafficLightButton::applyState(
		bool active,
		bool maximized,
		int height) {
	_active = active;
	_maximized = maximized;
	const auto size = QSize(MacTheme::TrafficLightSlot(), height);
	if (this->size() != size) {
		setFixedSize(size);
	}
	update();
}

void MacTrafficLightButton::setGroupHovered(bool hovered) {
	if (_groupHovered == hovered) {
		return;
	}
	_groupHovered = hovered;
	const auto from = _hover.value(hovered ? 0. : 1.);
	_hover.start(
		[=] { update(); },
		from,
		hovered ? 1. : 0.,
		MacMotion::kHoverDuration,
		MacMotion::HoverEasing());
}

void MacTrafficLightButton::enterEventHook(QEnterEvent *e) {
	AbstractButton::enterEventHook(e);
	if (_hoverChanged) {
		_hoverChanged();
	}
}

void MacTrafficLightButton::leaveEventHook(QEvent *e) {
	AbstractButton::leaveEventHook(e);
	if (_hoverChanged) {
		_hoverChanged();
	}
}

void MacTrafficLightButton::paintGlyph(
		QPainter &p,
		const QRectF &circle) const {
	const auto amount = _hover.value(_groupHovered ? 1. : 0.);
	if (amount <= 0.) {
		return;
	}
	const auto inset = circle.width() * 0.28;
	const auto glyph = circle.adjusted(inset, inset, -inset, -inset);
	p.save();
	p.setOpacity(amount);
	p.setBrush(Qt::NoBrush);
	p.setPen(QPen(
		MacTheme::TrafficLightGlyph(),
		std::max(1., float64(style::ConvertScale(MacIcons::kStrokeTrafficLight))),
		Qt::SolidLine,
		Qt::RoundCap,
		Qt::RoundJoin));
	switch (_kind) {
	case MacTheme::TrafficLight::Close: {
		p.drawLine(glyph.topLeft(), glyph.bottomRight());
		p.drawLine(glyph.topRight(), glyph.bottomLeft());
	} break;
	case MacTheme::TrafficLight::Minimize: {
		const auto y = glyph.center().y();
		p.drawLine(QPointF(glyph.left(), y), QPointF(glyph.right(), y));
	} break;
	case MacTheme::TrafficLight::Zoom: {
		p.setBrush(MacTheme::TrafficLightGlyph());
		p.setPen(Qt::NoPen);
		const auto mid = glyph.center();
		if (_maximized) {
			QPainterPath inward;
			inward.moveTo(mid.x(), glyph.top());
			inward.lineTo(glyph.right(), glyph.top());
			inward.lineTo(glyph.right(), mid.y());
			inward.closeSubpath();
			inward.moveTo(mid.x(), glyph.bottom());
			inward.lineTo(glyph.left(), glyph.bottom());
			inward.lineTo(glyph.left(), mid.y());
			inward.closeSubpath();
			p.drawPath(inward);
		} else {
			QPainterPath outward;
			outward.moveTo(glyph.right(), glyph.top());
			outward.lineTo(glyph.right(), mid.y());
			outward.lineTo(mid.x(), glyph.top());
			outward.closeSubpath();
			outward.moveTo(glyph.left(), glyph.bottom());
			outward.lineTo(glyph.left(), mid.y());
			outward.lineTo(mid.x(), glyph.bottom());
			outward.closeSubpath();
			p.drawPath(outward);
		}
	} break;
	}
	p.restore();
}

void MacTrafficLightButton::paintEvent(QPaintEvent *e) {
	Painter p(this);
	p.setRenderHint(QPainter::Antialiasing);

	const auto diameter = MacTheme::TrafficLightDiameter();
	const auto circle = QRectF(
		(width() - diameter) / 2.,
		(height() - diameter) / 2.,
		diameter,
		diameter);
	const auto amount = _hover.value(_groupHovered ? 1. : 0.);
	auto fill = _active
		? MacTheme::TrafficLightFill(_kind)
		: MacTheme::Blend(
			MacTheme::TrafficLightInactiveFill(),
			MacTheme::TrafficLightFill(_kind),
			amount);
	auto border = _active
		? MacTheme::TrafficLightBorder(_kind)
		: MacTheme::Blend(
			MacTheme::TrafficLightInactiveBorder(),
			MacTheme::TrafficLightBorder(_kind),
			amount);
	if (isDown()) {
		fill = MacTheme::TrafficLightPressed(fill);
		border = MacTheme::TrafficLightPressed(border);
	}

	p.setPen(Qt::NoPen);
	p.setBrush(QColor(0, 0, 0, _active ? 36 : 16));
	p.drawEllipse(circle.translated(0, style::ConvertScale(1) / 2.));

	p.setPen(QPen(border, 1));
	p.setBrush(fill);
	p.drawEllipse(circle);

	paintGlyph(p, circle);
}

class MacTrafficLightButtons final : public AbstractTitleButtons {
public:
	MacTrafficLightButtons() = default;

	object_ptr<AbstractButton> create(
		not_null<QWidget*> parent,
		TitleControl control,
		const style::WindowTitle &st) override;
	void updateState(
		bool active,
		bool maximized,
		const style::WindowTitle &st) override;
	void notifySynteticOver(TitleControl control, bool over) override;

private:
	void syncHover();
	[[nodiscard]] MacTrafficLightButton *button(TitleControl control) const;

	QPointer<MacTrafficLightButton> _minimize;
	QPointer<MacTrafficLightButton> _maximizeRestore;
	QPointer<MacTrafficLightButton> _close;
	bool _active = true;
	bool _maximized = false;

};

object_ptr<AbstractButton> MacTrafficLightButtons::create(
		not_null<QWidget*> parent,
		TitleControl control,
		const style::WindowTitle &st) {
	const auto kind = [&] {
		switch (control) {
		case TitleControl::Minimize: return MacTheme::TrafficLight::Minimize;
		case TitleControl::Maximize: return MacTheme::TrafficLight::Zoom;
		case TitleControl::Close: return MacTheme::TrafficLight::Close;
		}
		Unexpected("Control in MacTrafficLightButtons::create.");
	}();
	auto result = object_ptr<MacTrafficLightButton>(
		parent,
		kind,
		[=] { syncHover(); });
	const auto raw = result.data();
	raw->applyState(_active, _maximized, st.height);
	switch (control) {
	case TitleControl::Minimize: _minimize = raw; break;
	case TitleControl::Maximize: _maximizeRestore = raw; break;
	case TitleControl::Close: _close = raw; break;
	case TitleControl::Unknown: break;
	}
	return result;
}

void MacTrafficLightButtons::updateState(
		bool active,
		bool maximized,
		const style::WindowTitle &st) {
	_active = active;
	_maximized = maximized;
	const auto apply = [&](MacTrafficLightButton *button, const QString &name) {
		if (!button) {
			return;
		}
		button->applyState(active, maximized, st.height);
		button->setAccessibleName(name);
	};
	apply(_minimize, Integration::Instance().phraseMinimize());
	apply(
		_maximizeRestore,
		maximized
			? Integration::Instance().phraseRestore()
			: Integration::Instance().phraseMaximize());
	apply(_close, Integration::Instance().phraseButtonClose());
}

void MacTrafficLightButtons::notifySynteticOver(TitleControl control, bool over) {
	if (const auto raw = button(control)) {
		raw->setSynteticOver(over);
	}
	syncHover();
}

MacTrafficLightButton *MacTrafficLightButtons::button(TitleControl control) const {
	switch (control) {
	case TitleControl::Minimize: return _minimize;
	case TitleControl::Maximize: return _maximizeRestore;
	case TitleControl::Close: return _close;
	case TitleControl::Unknown: return nullptr;
	}
	return nullptr;
}

void MacTrafficLightButtons::syncHover() {
	const auto hovered = (_minimize && _minimize->isOver())
		|| (_maximizeRestore && _maximizeRestore->isOver())
		|| (_close && _close->isOver());
	if (_minimize) {
		_minimize->setGroupHovered(hovered);
	}
	if (_maximizeRestore) {
		_maximizeRestore->setGroupHovered(hovered);
	}
	if (_close) {
		_close->setGroupHovered(hovered);
	}
}

} // namespace

std::unique_ptr<AbstractTitleButtons> CreateMacTrafficLightButtons() {
	return std::make_unique<MacTrafficLightButtons>();
}

} // namespace Ui::Platform
