//
//  i8080.hpp
//  orion
//
//  Created by Артём Оконечников on 29.10.2020.
//

#ifndef i8080_hpp
#define i8080_hpp

#include <stdio.h>
#include <cstdint>
#include <string>
#include <vector>

#include "Status.hpp"

class Bus;

class i8080
{
public:
    
    i8080();
    ~i8080() { };
    
private:
    
    uint8_t  reg[8];            // Registers        (B C D E H L M A)
    uint8_t * pairs[4];         // Pairs
                                // 0x00 - B & C
                                // 0x01 - D & E
                                // 0x02 - H & L
                                // 0x03 - SP
    
    uint8_t  op      = 0x00;    // Operation code
    uint8_t  cycles  = 0x00;    // Cycle counter

    uint16_t sp      = 0x0000;  // Stack pointer
    uint16_t pc      = 0x0000;  // Program counter
    
    uint16_t address = 0x0000;
    
    Status   sr;                // Status register
    
private:
    
    enum
    {
        B,
        C,
        D,
        E,
        H,
        L,
        M, // Memory
        A  // Accumulator
    };
    
    enum
    {
        BC,
        DE,
        HL,
        SP
    };
    
    enum
    {
        SS  = 0x07,
        DD  = 0x38
    };
    
    uint16_t readpair(uint8_t index);
    void writepair(const uint8_t & index, const uint16_t & data);
    void mutatepair(const uint8_t & index, std::function<void(uint16_t &)> mutator);
    
// Bus communication
private:
    
    Bus * bus = nullptr;
    
    uint8_t read(uint16_t address);
    void write (uint16_t address, uint8_t data);
    
private:
    
    void IMP(); // Implied
    void IMM(); // Immediate D8/D16
    void IND(); // Indirect
    void DIR(); // Direct
    void HLM(); // Direct HL
    
private:
    
    // Move, Load, Store
    
    uint8_t MOVRR ();
    uint8_t MOVMR ();
    uint8_t MOVRM ();
    uint8_t MVIR  ();
    uint8_t MVIM  ();
    uint8_t LXI   ();
    uint8_t STAX  ();
    uint8_t LDAX  ();
    uint8_t STA   ();
    uint8_t LDA   ();
    uint8_t SHLD  ();
    uint8_t LHLD  ();
    uint8_t XCHG  ();
    
    
    // Stack operations
    
    uint8_t PUSHB ();
    uint8_t PUSHD ();
    uint8_t PUSHH ();
    uint8_t PUSH  ();  // PSW
    uint8_t POPB  ();
    uint8_t POPD  ();
    uint8_t POPH  ();
    uint8_t POP   ();  // PSW
    uint8_t XTHL  ();
    uint8_t SPHL  ();

    // Jump
    
    uint8_t JMP  ();
    uint8_t JC   ();
    uint8_t JNC  ();
    uint8_t JZ   ();
    uint8_t JNZ  ();
    uint8_t JP   ();
    uint8_t JM   ();
    uint8_t JPE  ();
    uint8_t JPO  ();
    uint8_t PCHL ();
    
    // Call
    
    uint8_t CALL ();
    uint8_t CC   ();
    uint8_t CNC  ();
    uint8_t CZ   ();
    uint8_t CNZ  ();
    uint8_t CP   ();
    uint8_t CM   ();
    uint8_t CPE  ();
    uint8_t CPO  ();
    
    // Return
    
    uint8_t RET  ();
    uint8_t RC   ();
    uint8_t RNC  ();
    uint8_t RZ   ();
    uint8_t RNZ  ();
    uint8_t RP   ();
    uint8_t RM   ();
    uint8_t RPE  ();
    uint8_t RPO  ();
    
    // Restart
    
    uint8_t RST  ();
    
    // Increment and decrement
    
    uint8_t INRR ();
    uint8_t INRM ();
    uint8_t DCRR ();
    uint8_t DCRM ();
    uint8_t INX  ();
    uint8_t DCX  ();

    // Add
    
    uint8_t ADDR ();
    uint8_t ADDM ();
    uint8_t ADDR (uint8_t carry);
    uint8_t ADDM (uint8_t carry);
    uint8_t ADCR ();
    uint8_t ADCM ();
    uint8_t ADI  ();
    uint8_t ACI  ();
    uint8_t DAD  ();
    
    // Substract
    
    uint8_t SUBR ();
    uint8_t SUBM ();
    uint8_t SBBR ();
    uint8_t SBBM ();
    uint8_t SUI  ();
    uint8_t SBI  ();
    
    // Logical
    
    uint8_t ANAR ();
    uint8_t XRAR ();
    uint8_t ORAR ();
    uint8_t CMPR ();
    uint8_t ANAM ();
    uint8_t XRAM ();
    uint8_t ORAM ();
    uint8_t CMPM ();
    uint8_t ANI  ();
    uint8_t XRI  ();
    uint8_t ORI  ();
    uint8_t CPI  ();
    
    // Rotate
    
    uint8_t RLC  ();
    uint8_t RRC  ();
    uint8_t RAL  ();
    uint8_t RAR  ();
    
    // Special
    
    uint8_t CMA  ();
    uint8_t STC  ();
    uint8_t CMC  ();
    uint8_t DAA  ();
    
    // I/O
    
    uint8_t IN   ();
    uint8_t OUT  ();
    
    // Control
    
    uint8_t EI   ();
    uint8_t DI   ();
    uint8_t NOP  ();
    uint8_t HLT  ();
    uint8_t XXX  ();
    
private:
    
    struct Command
    {
        std::string name;
        uint8_t cycles = 0x00;
        
        uint8_t (i8080::*operate) (void) = nullptr;
        void    (i8080::*addrmod) (void) = nullptr;
    };

    std::vector<Command> lookup;
    
public:
    
    void clock();
    void connect(Bus * bus);
    void execute(int clock);
};

#endif /* i8080_hpp */
