#include "twSizePolicy.h"

namespace TwinkleGraphics {
SizePolicy::SizePolicy(Policy horPolicy, Policy verPolicy)
    : _horPolicy(horPolicy), _verPolicy(verPolicy) {}

SizePolicy::~SizePolicy() {}

void SizePolicy::SetHorizontalPolicy(Policy policy) { _horPolicy = policy; }

void SizePolicy::SetVerticalPolicy(Policy policy) { _verPolicy = policy; }

void SizePolicy::SetHeightForWidth(bool dependent) {
  _heightDependWidth = dependent;
}

void SizePolicy::SetHorizontalStretch(float stretch) { _horStretch = stretch; }

void SizePolicy::SetVerticalStretch(float stretch) { _verStretch = stretch; }

} // namespace TwinkleGraphics
