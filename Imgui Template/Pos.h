#pragma once
#include <cstdint>
#include "Facing.h"
#include <cassert>
#include <math.h>
#include "absl/hash/hash.h"
struct Pos {
public:
	int32_t x = 0, y = 0;
	uint64_t CoordToEncoded();
	static Pos EncodedToCoord(uint64_t input);
	static Facing BehindFacing(Facing);
	static Facing RelativeFacing(Facing, int);
	Pos FacingPosition(Facing facing);
	Pos BehindPosition(Facing facing);
	Pos RelativePosition(int32_t dx, int32_t dy);
	Pos RelativePosition(Pos pos);
	Pos ChunkPosition();
	uint64_t ChunkEncoded();
	Pos InChunkPosition();
	Pos operator>>(int other);
	Pos operator<<(int other);
	Pos operator+(Pos other);
	Pos operator-(Pos other);
	Pos operator*(float other);
	Pos operator/(float other);
	Pos operator*(int other);
	Pos operator/(int other);
	Pos operator%(int other);
	template <typename H>
	friend H AbslHashValue(H h, const Pos& p) {
		return H::combine(std::move(h), p.x, p.y);
	}
};

bool operator==(const Pos& posA, const Pos& posB);
bool operator!=(const Pos& posA, const Pos& posB);