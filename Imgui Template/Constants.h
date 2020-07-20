#pragma once
#include <cstdint>
namespace GC
{
	static constexpr int32_t  FRAMERATE = 60;
	static constexpr int32_t  UPDATERATE = 20;
	static constexpr uint8_t cameraSpeed = 10;

	// World Constants
	static constexpr uint8_t  chunkShift = 5;
	static constexpr uint8_t  chunkMask = 31;
	static constexpr uint8_t  chunkSize = 32;
	static constexpr uint16_t chunkTileNum = 1024;

	static constexpr uint8_t tileSize = 32;
	static constexpr uint8_t halfTileSize = 16;
	static constexpr uint8_t tileShift = 5;
	static constexpr uint8_t tileMask = 31;

	static constexpr uint8_t halfItemSprite = 8;

	// GUI constants
	static constexpr uint8_t tooltipPadding = 6;
	static constexpr uint8_t hotbarSlotSize = 50;
	static constexpr uint8_t hotbarPadding = 4;

	// Game constants
	static constexpr uint8_t robotStrength = 6;
	static constexpr uint8_t maxSignalStrength = 32;
	static constexpr uint8_t startSignalStrength = 16;
	static constexpr uint8_t colorClassA = 1;
	static constexpr uint8_t colorClassB = 4;
}