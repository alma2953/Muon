#include <Muon/Muon.h>
#include "m6502.h"

struct M6502ShiftsTests {
public:
	m6502::Mem mem;
	m6502::CPU cpu;
};

TEST_F_SETUP(M6502ShiftsTests) {
	muon->cpu.Reset( muon->mem );
}

TEST_F_TEARDOWN(M6502ShiftsTests){}


TEST_F( M6502ShiftsTests, ASLCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->cpu.A = 1;
	muon->mem[0xFF00] = CPU::INS_ASL;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 2 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );	
}

TEST_F( M6502ShiftsTests, ASLCanShiftANegativeValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->cpu.A = 0b11000010;
	muon->mem[0xFF00] = CPU::INS_ASL;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 0b10000100 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ASLZeroPageCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_ASL_ZP;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042] = 1;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042], 2 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ASLZeroPageCanShiftANegativeValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_ASL_ZP;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042] = 0b11000010;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042], 0b10000100 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ASLZeroPageXCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_ASL_ZPX;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042 + 0x10] = 1;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042 + 0x10], 2 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ASLZeroPageXCanShiftANegativeValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_ASL_ZPX;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042 + 0x10] = 0b11000010;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042 + 0x10], 0b10000100 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ASLAbsCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_ASL_ABS;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000] = 1;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000], 2 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ASLAbsCanShiftANegativeValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_ASL_ABS;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000] = 0b11000010;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000], 0b10000100 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ASLAbsXCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_ASL_ABSX;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000 + 0x10] = 1;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000 + 0x10], 2 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ASLAbsXCanShiftANegativeValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_ASL_ABSX;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000+0x10] = 0b11000010;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000 + 0x10], 0b10000100 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->cpu.A = 1;
	muon->mem[0xFF00] = CPU::INS_LSR;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 0 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRCanShiftAZeroIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->cpu.A = 8;
	muon->mem[0xFF00] = CPU::INS_LSR;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 4 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRZeroPageCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_LSR_ZP;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042] = 1;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042], 0 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRZeroPageCanShiftAZeroIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_LSR_ZP;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042] = 8;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042], 4 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRZeroPageXCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_LSR_ZPX;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042+ 0x10] = 1;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042+0x10], 0 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRZeroPageXCanShiftAZeroIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_LSR_ZPX;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042+ 0x10] = 8;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042+ 0x10], 4 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRAbsCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_LSR_ABS;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000] = 1;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000], 0 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRAbsCanShiftAZeroIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_LSR_ABS;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000] = 8;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000], 4 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRAbsXCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_LSR_ABSX;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000+0x10] = 1;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000 + 0x10], 0 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRAbsXCanShiftAZeroIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_LSR_ABSX;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000 + 0x10] = 8;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000 + 0x10], 4 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

 // ------------ ROL ----------------

TEST_F( M6502ShiftsTests, ROLCanShiftABitOutOfTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->cpu.A = 0;
	muon->mem[0xFF00] = CPU::INS_ROL;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 1 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLCanShiftABitIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->cpu.A = 0b10000000;
	muon->mem[0xFF00] = CPU::INS_ROL;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 0 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLCanShiftZeroWithNoCarry )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->cpu.A = 0;
	muon->mem[0xFF00] = CPU::INS_ROL;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 0 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLCanShiftAValueThatResultInANegativeValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = false;
	muon->cpu.A = 0b01110011;
	muon->mem[0xFF00] = CPU::INS_ROL;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 0b11100111 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

// ---------- Zero Page -------------

TEST_F( M6502ShiftsTests, ROLZeroPageCanShiftABitOutOfTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_ROL_ZP;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042] = 0;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042], 1 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLZeroPageCanShiftABitIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_ROL_ZP;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042] = 0b10000000;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042], 0 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLZeroPageCanShiftZeroWithNoCarry )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_ROL_ZP;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042] = 0;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042], 0 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLZeroPageCanShiftAValueThatResultInANegativeValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_ROL_ZP;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042] = 0b01110011;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042], 0b11100111 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

// ------------- Zero Page X --------------

TEST_F( M6502ShiftsTests, ROLZeroPageXCanShiftABitOutOfTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_ROL_ZPX;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042 + 0x10] = 0;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042+0x10], 1 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLZeroPageXCanShiftABitIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_ROL_ZPX;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042+0x10] = 0b10000000;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042 + 0x10], 0 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLZeroPageXCanShiftZeroWithNoCarry )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_ROL_ZPX;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042 + 0x10] = 0;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042 + 0x10], 0 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLZeroPageXCanShiftAValueThatResultInANegativeValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = false;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_ROL_ZPX;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042 + 0x10] = 0b01110011;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042 + 0x10], 0b11100111 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

// ------------- Absolute --------------

TEST_F( M6502ShiftsTests, ROLAbsoluteCanShiftABitOutOfTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_ROL_ABS;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000] = 0;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000], 1 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLAbsoluteCanShiftABitIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_ROL_ABS;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000] = 0b10000000;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000], 0 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLAbsoluteCanShiftZeroWithNoCarry )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_ROL_ABS;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000] = 0;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000], 0 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLAbsoluteCanShiftAValueThatResultInANegativeValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_ROL_ABS;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000] = 0b01110011;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000], 0b11100111 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

// ------------- Absolute X --------------

TEST_F( M6502ShiftsTests, ROLAbsoluteXCanShiftABitOutOfTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_ROL_ABSX;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000+0x10] = 0;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000 + 0x10], 1 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLAbsoluteXCanShiftABitIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_ROL_ABSX;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000 + 0x10] = 0b10000000;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000 + 0x10], 0 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLAbsoluteXCanShiftZeroWithNoCarry )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_ROL_ABSX;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000 + 0x10] = 0;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000 + 0x10], 0 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLAbsoluteXCanShiftAValueThatResultInANegativeValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = false;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_ROL_ABSX;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000 + 0x10] = 0b01110011;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000 + 0x10], 0b11100111 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

// --------------- ROR --------------------

TEST_F( M6502ShiftsTests, RORCanShiftTheCarryFlagIntoTheOperand )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = false;
	muon->cpu.A = 0;
	muon->mem[0xFF00] = CPU::INS_ROR;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 0b10000000 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, RORCanShiftAValueIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = false;
	muon->cpu.A = 1;
	muon->mem[0xFF00] = CPU::INS_ROR;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 0 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, RORCanRotateANumber )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->cpu.A = 0b01101101;
	muon->mem[0xFF00] = CPU::INS_ROR;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 0b10110110 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

// ZeroPage

TEST_F( M6502ShiftsTests, RORZeroPageCanShiftTheCarryFlagIntoTheOperand )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_ROR_ZP;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042] = 0;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042], 0b10000000 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, RORZeroPageCanShiftAValueIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_ROR_ZP;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042] = 1;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042], 0 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, RORZeroPageCanRotateANumber )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_ROR_ZP;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042] = 0b01101101;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042], 0b10110110 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

// Zero Page X

TEST_F( M6502ShiftsTests, RORZeroXPageCanShiftTheCarryFlagIntoTheOperand )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = false;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_ROR_ZPX;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042 + 0x10] = 0;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042 + 0x10], 0b10000000 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, RORZeroXPageCanShiftAValueIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = false;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_ROR_ZPX;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042+0x10] = 1;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042 + 0x10], 0 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, RORZeroXPageCanRotateANumber )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_ROR_ZPX;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042 + 0x10] = 0b01101101;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042 + 0x10], 0b10110110 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

// Absolute

TEST_F( M6502ShiftsTests, RORAbsolutePageCanShiftTheCarryFlagIntoTheOperand )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_ROR_ABS;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000] = 0;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000], 0b10000000 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, RORAbsolutePageCanShiftAValueIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_ROR_ABS;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000] = 1;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000], 0 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, RORAbsolutePageCanRotateANumber )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_ROR_ABS;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000] = 0b01101101;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000], 0b10110110 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

// Absolute X

TEST_F( M6502ShiftsTests, RORAbsoluteXPageCanShiftTheCarryFlagIntoTheOperand )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = false;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_ROR_ABSX;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000 + 0x10] = 0;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000 + 0x10], 0b10000000 );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, RORAbsoluteXPageCanShiftAValueIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = false;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_ROR_ABSX;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000+0x10] = 1;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000 + 0x10], 0 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, RORAbsoluteXPageCanRotateANumber )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_ROR_ABSX;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000 + 0x10] = 0b01101101;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000 + 0x10], 0b10110110 );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}