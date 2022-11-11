#pragma once

class UsefulFunc {
public:
	// inverse square root from Quake III
	static float InvSqrt(float x) {
		union {
			float f;
			int i;
		} tmp;
		tmp.f = x;
		tmp.i = 0x5f3759df - (tmp.i >> 1);
		float y = tmp.f;
		return y * (1.5f - 0.5f * x * y * y);
	}

	// approximation of atan2 from 
	// https://math.stackexchange.com/questions/1098487/atan2-faster-approximation
	static float atan2(float y, float x) {
		float absx = std::abs(x);
		float absy = std::abs(y);
		float a = std::min(absx, absy) / std::max(absx, absy);
		float s = a * a;
		float r = ((-0.0464964749f * s + 0.15931422f) * s - 0.327622764f) * s * a + a;
		if (absy > absx) r = (float)M_PI_2 - r;
		if (x < 0) r = (float)M_PI - r;
		if (y < 0) r = -r;
		return r;
	}
};