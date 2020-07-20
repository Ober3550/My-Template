#include "Pos.h"
#include "MyMod.h"
#include "Constants.h"

uint64_t Pos::CoordToEncoded()
{
	return (uint64_t(this->y) << 32) | uint32_t(this->x);
}
Pos Pos::EncodedToCoord(uint64_t input)
{
	return Pos{ int32_t(input & 0xFFFFFFFF), int32_t(input >> 32) };
}
Pos Pos::FacingPosition(Facing facing)
{
	facing = Facing(MyMod(facing, 4));
	switch (facing)
	{
	case north:
		return RelativePosition(0, -1);
	case east:
		return RelativePosition(1, 0);
	case south:
		return RelativePosition(0, 1);
	case west:
		return RelativePosition(-1, 0);
	default:
		assert(false);
		return *this;
	}
}
Pos Pos::BehindPosition(Facing facing)
{
	switch (facing)
	{
	case north:
		return RelativePosition(0, 1);
	case east:
		return RelativePosition(-1, 0);
	case south:
		return RelativePosition(0, -1);
	case west:
		return RelativePosition(1, 0);
	default:
		assert(false);
		return *this;
	}
}
Pos Pos::RelativePosition(int32_t dx, int32_t dy)
{
	Pos result{ (this->x + dx),(this->y + dy) };
	return result;
}
Pos Pos::RelativePosition(Pos other)
{
	return RelativePosition(other.x, other.y);
}
Pos Pos::ChunkPosition()
{
	Pos result{ this->x >> GC::chunkShift, this->y >> GC::chunkShift };
	return result;
}
uint64_t Pos::ChunkEncoded()
{
	return ChunkPosition().CoordToEncoded();
}
Pos Pos::InChunkPosition()
{
	Pos result{ this->x & GC::chunkMask,this->y & GC::chunkMask };
	return result;
}
bool operator==(const Pos& posA, const Pos& posB)
{
	if (posB.x == posA.x && posB.y == posA.y)
		return true;
	else
		return false;
}
bool operator!=(const Pos& posA, const Pos& posB)
{
	return !(posA == posB);
}
Pos Pos::operator>>(int shift)
{
	return Pos{ this->x >> shift, this->y >> shift };
}
Pos Pos::operator<<(int shift)
{
	return Pos{ this->x << shift, this->y << shift };
}
Pos Pos::operator+(Pos other)
{
	return Pos{ this->x + other.x, this->y + other.y };
}
Pos Pos::operator-(Pos other)
{
	return Pos{ this->x - other.x, this->y - other.y };
}
Facing Pos::BehindFacing(Facing input)
{
	return Facing(((int)input + 2) & 3);
}
Facing Pos::RelativeFacing(Facing input, int relative)
{
	return Facing(((int)input + relative) & 3);
}
Pos Pos::operator*(float other)
{
	return Pos{ int(this->x * other), int(this->y * other) };
}
Pos Pos::operator/(float other)
{
	return Pos{ int(round(float(this->x) / other)), int(round(float(this->y) / other)) };
}
Pos Pos::operator*(int other)
{
	return Pos{ int(this->x * other), int(this->y * other) };
}
Pos Pos::operator/(int other)
{
	return Pos{ int(this->x / other), int(this->y / other) };
}
Pos Pos::operator%(int other)
{
	return Pos{ int(MyMod(this->x,other)), int(MyMod(this->y,other)) };
}