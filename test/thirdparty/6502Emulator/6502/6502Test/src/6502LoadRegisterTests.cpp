#include <Muon/Muon.h>
#include "m6502.h"

struct M6502LoadRegisterTests {
public:	
	m6502::Mem mem;
	m6502::CPU cpu;

	void TestLoadRegisterImmediate( 
		m6502::Byte Opcode, 
		m6502::Byte m6502::CPU::* );

	void TestLoadRegisterZeroPage(
		m6502::Byte OpcodeToTest,
		m6502::Byte m6502::CPU::*RegisterToTest );

	void TestLoadRegisterZeroPageX(
		m6502::Byte OpcodeToTest,
		m6502::Byte m6502::CPU::*RegisterToTest );

	void TestLoadRegisterZeroPageY(
		m6502::Byte OpcodeToTest,
		m6502::Byte m6502::CPU::*RegisterToTest );

	void TestLoadRegisterAbsolute(
		m6502::Byte OpcodeToTest,
		m6502::Byte m6502::CPU::*RegisterToTest );

	void TestLoadRegisterAbsoluteX(
		m6502::Byte OpcodeToTest,
		m6502::Byte m6502::CPU::*RegisterToTest );

	void TestLoadRegisterAbsoluteY(
		m6502::Byte OpcodeToTest,
		m6502::Byte m6502::CPU::*RegisterToTest );

	void TestLoadRegisterAbsoluteYWhenCrossingPage(
		m6502::Byte OpcodeToTest,
		m6502::Byte m6502::CPU::*RegisterToTest );

	void TestLoadRegisterAbsoluteXWhenCrossingPage(
		m6502::Byte OpcodeToTest,
		m6502::Byte m6502::CPU::*RegisterToTest );
}; // M6502LoadRegisterTests

TEST_F_SETUP(M6502LoadRegisterTests) {
	muon->cpu.Reset( muon->mem );
}

TEST_F_TEARDOWN(M6502LoadRegisterTests){}

static void VerfifyUnmodifiedFlagsFromLoadRegister( 
	const m6502::CPU& cpu, 
	const m6502::CPU& CPUCopy )
{
	CHECK_EQ( cpu.Flag.C, CPUCopy.Flag.C );
	CHECK_EQ( cpu.Flag.I, CPUCopy.Flag.I );
	CHECK_EQ( cpu.Flag.D, CPUCopy.Flag.D );
	CHECK_EQ( cpu.Flag.B, CPUCopy.Flag.B );
	CHECK_EQ( cpu.Flag.V, CPUCopy.Flag.V );
}

TEST_F( M6502LoadRegisterTests, TheCPUDoesNothingWhenWeExecuteZeroCycles )
{
	//given:
	using namespace m6502;
	constexpr s32 NUM_CYCLES = 0;

	//when:
	s32 CyclesUsed = muon->cpu.Execute( NUM_CYCLES, muon->mem );

	//then:
	CHECK_EQ( CyclesUsed, 0 );
}

TEST_F( M6502LoadRegisterTests, CPUCanExecuteMoreCyclesThanRequestedIfRequiredByTheInstruction )
{
	// given:
	using namespace m6502;
	muon->mem[0xFFFC] = CPU::INS_LDA_IM;
	muon->mem[0xFFFD] = 0x84;
	CPU CPUCopy = muon->cpu;
	constexpr s32 NUM_CYCLES = 1;

	//when:
	s32 CyclesUsed = muon->cpu.Execute( NUM_CYCLES, muon->mem );

	//then:
	CHECK_EQ( CyclesUsed, 2 );
}

void M6502LoadRegisterTests::TestLoadRegisterImmediate( 
	m6502::Byte OpcodeToTest,  
	m6502::Byte m6502::CPU::*RegisterToTest )
{
	// given:
	using namespace m6502;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x84;

	//when:
	CPU CPUCopy = cpu;
	s32 CyclesUsed = cpu.Execute( 2, mem );

	//then:
	CHECK_EQ( cpu.*RegisterToTest, 0x84 ); 
	CHECK_EQ( CyclesUsed, 2 );
	CHECK_FALSE( cpu.Flag.Z );
	CHECK_TRUE( cpu.Flag.N );
	VerfifyUnmodifiedFlagsFromLoadRegister( cpu, CPUCopy );
}

TEST_F( M6502LoadRegisterTests, LDAImmediateCanLoadAValueIntoTheARegister )
{
	using namespace m6502;
	muon->TestLoadRegisterImmediate( CPU::INS_LDA_IM, &CPU::A );
}

TEST_F( M6502LoadRegisterTests, LDXImmediateCanLoadAValueIntoTheXRegister )
{
	using namespace m6502;
	muon->TestLoadRegisterImmediate( CPU::INS_LDX_IM, &CPU::X );
}

TEST_F( M6502LoadRegisterTests, LDYImmediateCanLoadAValueIntoTheYRegister )
{
	using namespace m6502;
	muon->TestLoadRegisterImmediate( CPU::INS_LDY_IM, &CPU::Y );
}

void M6502LoadRegisterTests::TestLoadRegisterZeroPage(
	m6502::Byte OpcodeToTest,
	m6502::Byte m6502::CPU::*RegisterToTest )
{
	// given:
	using namespace m6502;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x42;
	mem[0x0042] = 0x37;

	//when:
	CPU CPUCopy = cpu;
	s32 CyclesUsed = cpu.Execute( 3, mem );

	//then:
	CHECK_EQ( cpu.*RegisterToTest, 0x37 );
	CHECK_EQ( CyclesUsed, 3 );
	CHECK_FALSE( cpu.Flag.Z );
	CHECK_FALSE( cpu.Flag.N );
	VerfifyUnmodifiedFlagsFromLoadRegister( cpu, CPUCopy );
}

TEST_F( M6502LoadRegisterTests, LDAZeroPageCanLoadAValueIntoTheARegister )
{
	using namespace m6502;
	muon->TestLoadRegisterZeroPage( CPU::INS_LDA_ZP, &CPU::A );
}

TEST_F( M6502LoadRegisterTests, LDXZeroPageCanLoadAValueIntoTheXRegister )
{
	using namespace m6502;
	muon->TestLoadRegisterZeroPage( CPU::INS_LDX_ZP, &CPU::X );
}

TEST_F( M6502LoadRegisterTests, LDYZeroPageCanLoadAValueIntoTheYRegister )
{
	using namespace m6502;
	muon->TestLoadRegisterZeroPage( CPU::INS_LDY_ZP, &CPU::Y );
}

TEST_F( M6502LoadRegisterTests, LDAImmediateCanAffectTheZeroFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.A = 0x44;
	muon->mem[0xFFFC] = CPU::INS_LDA_IM;
	muon->mem[0xFFFD] = 0x0;
	CPU CPUCopy = muon->cpu;

	//when:
	muon->cpu.Execute( 2, muon->mem );

	//then:
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	VerfifyUnmodifiedFlagsFromLoadRegister( muon->cpu, CPUCopy );
}

void M6502LoadRegisterTests::TestLoadRegisterZeroPageX(
	m6502::Byte OpcodeToTest,
	m6502::Byte m6502::CPU::*RegisterToTest )
{
	// given:
	using namespace m6502;
	cpu.X = 5;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x42;
	mem[0x0047] = 0x37;
	CPU CPUCopy = cpu;

	//when:
	s32 CyclesUsed = cpu.Execute( 4, mem );

	//then:
	CHECK_EQ( cpu.*RegisterToTest, 0x37 );
	CHECK_EQ( CyclesUsed, 4 );
	CHECK_FALSE( cpu.Flag.Z );
	CHECK_FALSE( cpu.Flag.N );
	VerfifyUnmodifiedFlagsFromLoadRegister( cpu, CPUCopy );
}

void M6502LoadRegisterTests::TestLoadRegisterZeroPageY(
	m6502::Byte OpcodeToTest,
	m6502::Byte m6502::CPU::*RegisterToTest )
{
	// given:
	using namespace m6502;
	cpu.Y = 5;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x42;
	mem[0x0047] = 0x37;
	CPU CPUCopy = cpu;

	//when:
	s32 CyclesUsed = cpu.Execute( 4, mem );

	//then:
	CHECK_EQ( cpu.*RegisterToTest, 0x37 );
	CHECK_EQ( CyclesUsed, 4 );
	CHECK_FALSE( cpu.Flag.Z );
	CHECK_FALSE( cpu.Flag.N );
	VerfifyUnmodifiedFlagsFromLoadRegister( cpu, CPUCopy );
}

TEST_F( M6502LoadRegisterTests, LDAZeroPageXCanLoadAValueIntoTheARegister )
{
	using namespace m6502;
	muon->TestLoadRegisterZeroPageX( CPU::INS_LDA_ZPX, &CPU::A );
}

TEST_F( M6502LoadRegisterTests, LDXZeroPageYCanLoadAValueIntoTheXRegister )
{
	using namespace m6502;
	muon->TestLoadRegisterZeroPageY( CPU::INS_LDX_ZPY, &CPU::X );
}

TEST_F( M6502LoadRegisterTests, LDYZeroPageXCanLoadAValueIntoTheYRegister )
{
	using namespace m6502;
	muon->TestLoadRegisterZeroPageX( CPU::INS_LDY_ZPX, &CPU::Y );
}

TEST_F( M6502LoadRegisterTests, LDAZeroPageXCanLoadAValueIntoTheARegisterWhenItWraps )
{
	// given:
	using namespace m6502;
	muon->cpu.X = 0xFF;
	muon->mem[0xFFFC] = CPU::INS_LDA_ZPX;
	muon->mem[0xFFFD] = 0x80;
	muon->mem[0x007F] = 0x37;

	//when:
	CPU CPUCopy = muon->cpu;
	s32 CyclesUsed = muon->cpu.Execute( 4, muon->mem );

	//then:
	CHECK_EQ( muon->cpu.A, 0x37 );
	CHECK_EQ( CyclesUsed, 4 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	VerfifyUnmodifiedFlagsFromLoadRegister( muon->cpu, CPUCopy );
}

void M6502LoadRegisterTests::TestLoadRegisterAbsolute(
	m6502::Byte OpcodeToTest,
	m6502::Byte m6502::CPU::*RegisterToTest )
{
	// given:
	cpu.Flag.Z = cpu.Flag.N = true;
	using namespace m6502;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;	//0x4480
	mem[0x4480] = 0x37;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = cpu;

	//when:
	s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

	//then:
	CHECK_EQ( cpu.*RegisterToTest, 0x37 );
	CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
	CHECK_FALSE( cpu.Flag.Z );
	CHECK_FALSE( cpu.Flag.N );
	VerfifyUnmodifiedFlagsFromLoadRegister( cpu, CPUCopy );
}

TEST_F( M6502LoadRegisterTests, LDAAbsoluteCanLoadAValueIntoTheARegister )
{
	using namespace m6502;
	muon->TestLoadRegisterAbsolute( CPU::INS_LDA_ABS, &CPU::A );
}

TEST_F( M6502LoadRegisterTests, LDXAbsoluteCanLoadAValueIntoTheXRegister )
{
	using namespace m6502;
	muon->TestLoadRegisterAbsolute( CPU::INS_LDX_ABS, &CPU::X );
}

TEST_F( M6502LoadRegisterTests, LDYAbsoluteCanLoadAValueIntoTheYRegister )
{
	using namespace m6502;
	muon->TestLoadRegisterAbsolute( CPU::INS_LDY_ABS, &CPU::Y );
}

void M6502LoadRegisterTests::TestLoadRegisterAbsoluteX(
	m6502::Byte OpcodeToTest,
	m6502::Byte m6502::CPU::*RegisterToTest )
{
	// given:
	cpu.Flag.Z = cpu.Flag.N = true;
	using namespace m6502;
	cpu.X = 1;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;	//0x4480
	mem[0x4481] = 0x37;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = cpu;

	//when:
	s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

	//then:
	CHECK_EQ( cpu.*RegisterToTest, 0x37 );
	CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
	CHECK_FALSE( cpu.Flag.Z );
	CHECK_FALSE( cpu.Flag.N );
	VerfifyUnmodifiedFlagsFromLoadRegister( cpu, CPUCopy );
}

void M6502LoadRegisterTests::TestLoadRegisterAbsoluteY(
	m6502::Byte OpcodeToTest,
	m6502::Byte m6502::CPU::*RegisterToTest )
{
	// given:
	using namespace m6502;
	cpu.Flag.Z = cpu.Flag.N = true;
	cpu.Y = 1;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;	//0x4480
	mem[0x4481] = 0x37;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = cpu;

	//when:
	s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

	//then:
	CHECK_EQ( cpu.*RegisterToTest, 0x37 );
	CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
	CHECK_FALSE( cpu.Flag.Z );
	CHECK_FALSE( cpu.Flag.N );
	VerfifyUnmodifiedFlagsFromLoadRegister( cpu, CPUCopy );
}

TEST_F( M6502LoadRegisterTests, LDAAbsoluteXCanLoadAValueIntoTheARegister )
{
	using namespace m6502;
	muon->TestLoadRegisterAbsoluteX( CPU::INS_LDA_ABSX, &CPU::A );
}

TEST_F( M6502LoadRegisterTests, LDXAbsoluteYCanLoadAValueIntoTheXRegister )
{
	using namespace m6502;
	muon->TestLoadRegisterAbsoluteY( CPU::INS_LDX_ABSY, &CPU::X );
}

TEST_F( M6502LoadRegisterTests, LDYAbsoluteXCanLoadAValueIntoTheYRegister )
{
	using namespace m6502;
	muon->TestLoadRegisterAbsoluteX( CPU::INS_LDY_ABSX, &CPU::Y );
}

void M6502LoadRegisterTests::TestLoadRegisterAbsoluteXWhenCrossingPage(
	m6502::Byte OpcodeToTest,
	m6502::Byte m6502::CPU::*RegisterToTest )
{
	// given:
	using namespace m6502;
	cpu.X = 0x1;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0xFF;
	mem[0xFFFE] = 0x44;	//0x44FF
	mem[0x4500] = 0x37;	//0x44FF+0x1 crosses page boundary!
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	//when:
	s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

	//then:
	CHECK_EQ( cpu.*RegisterToTest, 0x37 );
	CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
	CHECK_FALSE( cpu.Flag.Z );
	CHECK_FALSE( cpu.Flag.N );
	VerfifyUnmodifiedFlagsFromLoadRegister( cpu, CPUCopy );
}

TEST_F( M6502LoadRegisterTests, LDAAbsoluteXCanLoadAValueIntoTheARegisterWhenItCrossesAPageBoundary )
{
	using namespace m6502;
	muon->TestLoadRegisterAbsoluteXWhenCrossingPage( CPU::INS_LDA_ABSX, &CPU::A );
}

TEST_F( M6502LoadRegisterTests, LDYAbsoluteXCanLoadAValueIntoTheYRegisterWhenItCrossesAPageBoundary )
{
	using namespace m6502;
	muon->TestLoadRegisterAbsoluteXWhenCrossingPage( CPU::INS_LDY_ABSX, &CPU::Y );
}

TEST_F( M6502LoadRegisterTests, LDAAbsoluteYCanLoadAValueIntoTheARegister )
{
	using namespace m6502;
	muon->TestLoadRegisterAbsoluteY( CPU::INS_LDA_ABSY, &CPU::A );
}

void M6502LoadRegisterTests::TestLoadRegisterAbsoluteYWhenCrossingPage(
	m6502::Byte OpcodeToTest,
	m6502::Byte m6502::CPU::*RegisterToTest )
{
	// given:
	using namespace m6502;
	cpu.Y = 0x1;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0xFF;
	mem[0xFFFE] = 0x44;	//0x44FF
	mem[0x4500] = 0x37;	//0x44FF+0x1 crosses page boundary!
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	//when:
	s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

	//then:
	CHECK_EQ( cpu.*RegisterToTest, 0x37 );
	CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
	CHECK_FALSE( cpu.Flag.Z );
	CHECK_FALSE( cpu.Flag.N );
	VerfifyUnmodifiedFlagsFromLoadRegister( cpu, CPUCopy );
}

TEST_F( M6502LoadRegisterTests, LDAAbsoluteYCanLoadAValueIntoTheARegisterWhenItCrossesAPageBoundary )
{
	using namespace m6502;
	muon->TestLoadRegisterAbsoluteYWhenCrossingPage( CPU::INS_LDA_ABSY, &CPU::A );
}

TEST_F( M6502LoadRegisterTests, LDXAbsoluteYCanLoadAValueIntoTheXRegisterWhenItCrossesAPageBoundary )
{
	using namespace m6502;
	muon->TestLoadRegisterAbsoluteYWhenCrossingPage( CPU::INS_LDX_ABSY, &CPU::X );
}

TEST_F( M6502LoadRegisterTests, LDAIndirectXCanLoadAValueIntoTheARegister )
{
	// given:
	using namespace m6502;
	muon->cpu.Flag.Z = muon->cpu.Flag.N = true;
	muon->cpu.X = 0x04;
	muon->mem[0xFFFC] = CPU::INS_LDA_INDX;
	muon->mem[0xFFFD] = 0x02;
	muon->mem[0x0006] = 0x00;	//0x2 + 0x4
	muon->mem[0x0007] = 0x80;	
	muon->mem[0x8000] = 0x37;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	//when:
	s32 CyclesUsed = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	//then:
	CHECK_EQ( muon->cpu.A, 0x37 );
	CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	VerfifyUnmodifiedFlagsFromLoadRegister( muon->cpu, CPUCopy );
}

TEST_F( M6502LoadRegisterTests, LDAIndirectYCanLoadAValueIntoTheARegister )
{
	// given:
	using namespace m6502;
	muon->cpu.Flag.Z = muon->cpu.Flag.N = true;
	muon->cpu.Y = 0x04;
	muon->mem[0xFFFC] = CPU::INS_LDA_INDY;
	muon->mem[0xFFFD] = 0x02;
	muon->mem[0x0002] = 0x00;	
	muon->mem[0x0003] = 0x80;
	muon->mem[0x8004] = 0x37;	//0x8000 + 0x4
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = muon->cpu;

	//when:
	s32 CyclesUsed = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	//then:
	CHECK_EQ( muon->cpu.A, 0x37 );
	CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	VerfifyUnmodifiedFlagsFromLoadRegister( muon->cpu, CPUCopy );
}

TEST_F( M6502LoadRegisterTests, LDAIndirectYCanLoadAValueIntoTheARegisterWhenItCrossesAPage )
{
	// given:
	using namespace m6502;
	muon->cpu.Y = 0x1;
	muon->mem[0xFFFC] = CPU::INS_LDA_INDY;
	muon->mem[0xFFFD] = 0x05;
	muon->mem[0x0005] = 0xFF;
	muon->mem[0x0006] = 0x80;
	muon->mem[0x8100] = 0x37;	//0x80FF + 0x1
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	//when:
	s32 CyclesUsed = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	//then:
	CHECK_EQ( muon->cpu.A, 0x37 );
	CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	VerfifyUnmodifiedFlagsFromLoadRegister( muon->cpu, CPUCopy );
}
