# CIE 1 - Problem 5: Smart Traffic Light Controller

**Course:** Embedded Processors Laboratory (EPL) [2625PC32 / 2626PC12]  
**Academic Year:** 2026–27 | **Class:** T.Y. B.Tech (E&TC)  
**Target Device:** Microchip PIC18F4550 (DIP-40)  
**Development Tools:** MPLAB IDE / MPLAB C18 v3.47 / XC8, Proteus 8 Professional  

---

## 1. Problem Statement
**Problem Statement No. 5:**  
*Smart Traffic Light Control.*

Design and implement an automated traffic signaling controller using a PIC18F4550 microcontroller. The system must cycle through the standard international traffic signal sequence:
- **Red Light (Stop):** Active for **5 seconds**
- **Green Light (Go):** Active for **5 seconds**
- **Yellow / Amber Light (Caution / Clear Junction):** Active for **2 seconds**

The sequence must repeat indefinitely in an autonomous loop with strict, non-overlapping timing intervals.

---

## 2. System Architecture & Finite State Machine (FSM)

### 2.1 State Transition Diagram
The traffic controller operates as a deterministic **Moore Finite State Machine**, where the output logic depends exclusively on the current operational state:

```mermaid
stateDiagram-v2
    direction LR
    [*] --> STATE_RED : System Power-up
    STATE_RED --> STATE_GREEN : 5000 ms Elapsed
    STATE_GREEN --> STATE_YELLOW : 5000 ms Elapsed
    STATE_YELLOW --> STATE_RED : 2000 ms Elapsed
```

| State Name | Active Color | PORTB Binary | PORTB Hex | Duration | Traffic Rule |
|:---:|:---:|:---:|:---:|:---:|:---|
| **STATE 1** | **RED** | `0000 0001` | **`0x01`** | $5000\text{ ms}$ (5s) | Complete stop before the stop line |
| **STATE 2** | **GREEN** | `0000 0100` | **`0x04`** | $5000\text{ ms}$ (5s) | Proceed through intersection safely |
| **STATE 3** | **YELLOW** | `0000 0010` | **`0x02`** | $2000\text{ ms}$ (2s) | Prepare to stop; clear the intersection |

### 2.2 LED Current-Limiting Resistor Design
Standard $5\text{ mm}$ indicator LEDs operate with nominal forward characteristics:
- Red LED: $V_{F} \approx 1.8\text{V}$ to $2.0\text{V}$, $I_{F} = 10\text{ mA}$
- Yellow LED: $V_{F} \approx 2.1\text{V}$, $I_{F} = 10\text{ mA}$
- Green LED: $V_{F} \approx 2.2\text{V}$, $I_{F} = 10\text{ mA}$

Using Ohm's Law with a $V_{CC} = 5.0\text{V}$ supply:
$$R_{limit} = \frac{V_{CC} - V_F}{I_F} = \frac{5.0\text{V} - 2.0\text{V}}{10\text{ mA}} = 300\,\Omega \implies \text{Standard } 330\,\Omega$$

---

## 3. Hardware Interfacing & Pinout Map

| PIC18F4550 Pin | Port Pin | Direction | Signal Type | Target Component | Electrical Circuit |
|:---:|:---:|:---:|:---:|:---:|:---|
| <code class="pin">Pin 33</code> | **RB0** | Digital Output | Active-HIGH | **Red LED** (Stop) | Anode via $330\,\Omega$ resistor; Cathode to GND |
| <code class="pin">Pin 34</code> | **RB1** | Digital Output | Active-HIGH | **Yellow LED** (Caution) | Anode via $330\,\Omega$ resistor; Cathode to GND |
| <code class="pin">Pin 35</code> | **RB2** | Digital Output | Active-HIGH | **Green LED** (Go) | Anode via $330\,\Omega$ resistor; Cathode to GND |
| <code class="pin">Pin 12 / 31</code> | **VSS** | Power Ground | $0\text{V}$ | Common Ground | LED Cathode return path |
| <code class="pin">Pin 11 / 32</code> | **VDD** | Power Supply | $+5.0\text{V}$ | Microcontroller Power | Regulated DC logic power rail |

---

## 4. Firmware Code & Detailed Annotation

```c
#include <p18f4550.h>

// Microcontroller Configuration Directives
#pragma config FOSC = HS    // High-Speed External Crystal Oscillator (8 MHz)
#pragma config WDT = OFF    // Watchdog Timer Disabled (prevents watchdog reset during 5s delays)
#pragma config LVP = OFF    // Low-Voltage ICSP Disabled (dedicates RB5 to general I/O)

// Software delay loop calibrated for ~1 ms per count at 8 MHz clock
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 165; j++); // 165 iterations ≈ 1 ms at Fosc = 8 MHz
    }
}

void main(void) {
    // Configure all pins of PORTB as digital outputs (0 = Output)
    TRISB = 0x00;
    
    // Ensure all lights are turned OFF on initial boot
    PORTB = 0x00;
    
    while(1) { // Infinite traffic control loop
        
        // ----------------------------------------------------
        // Step 1: Red Light ON (RB0 = 1, RB1 = 0, RB2 = 0)
        // Binary: 0000 0001 = 0x01
        // ----------------------------------------------------
        PORTB = 0x01; 
        delay_ms(5000); // Hold RED signal for 5 seconds
        
        // ----------------------------------------------------
        // Step 2: Green Light ON (RB0 = 0, RB1 = 0, RB2 = 1)
        // Binary: 0000 0100 = 0x04
        // ----------------------------------------------------
        PORTB = 0x04; 
        delay_ms(5000); // Hold GREEN signal for 5 seconds
        
        // ----------------------------------------------------
        // Step 3: Yellow Light ON (RB0 = 0, RB1 = 1, RB2 = 0)
        // Binary: 0000 0010 = 0x02
        // ----------------------------------------------------
        PORTB = 0x02; 
        delay_ms(2000); // Hold YELLOW caution signal for 2 seconds
    }
}
```

---

## 5. Proteus Simulation Setup Instructions

1. Launch **Proteus 8 Professional** and open `Smart_Traffic_Light.pdsprj`.
2. Component list:
   - `PIC18F4550`
   - `LED-RED`, `LED-YELLOW`, `LED-GREEN`
   - `RES` (Resistors set to $330\,\Omega$)
3. Wiring:
   - Connect `RB0` to $330\,\Omega$ resistor $\to$ Anode of `LED-RED`.
   - Connect `RB1` to $330\,\Omega$ resistor $\to$ Anode of `LED-YELLOW`.
   - Connect `RB2` to $330\,\Omega$ resistor $\to$ Anode of `LED-GREEN`.
   - Connect the cathodes of all three LEDs to **GROUND**.
4. Double-click the **PIC18F4550** in Proteus:
   - Set **Processor Clock Frequency** to `8MHz`.
   - Set **Program File** by browsing to `Q2.hex` (or `main.hex`).
5. Click **Play** to run simulation.
6. Verify the timing cycle:
   - Red illuminates alone for 5 seconds.
   - Green illuminates alone for 5 seconds.
   - Yellow illuminates alone for 2 seconds.
   - Cycle restarts seamlessly at Red.

---

## 6. Comprehensive Viva Voce & Oral Examination Guide

### Level 1: Fundamental / Conceptual Questions ("What & Why")

#### Q1.1: What is a Finite State Machine (FSM), and why is it used to model traffic lights?
**Answer:**  
A Finite State Machine is a computational model consisting of a finite number of well-defined states, transitions between those states, and specific actions. Traffic light control is an ideal application because:
1. Only one valid operational state (e.g., Red, Green, or Yellow) can be active on any single approach lane at any instant.
2. Transitions occur strictly in an established sequence governed by predetermined time intervals or sensor events.
3. It prevents catastrophic illegal states, such as displaying Red and Green simultaneously.

#### Q1.2: Why is the sequencing order Red $\to$ Green $\to$ Yellow $\to$ Red, rather than Red $\to$ Yellow $\to$ Green?
**Answer:**  
In international traffic safety conventions:
- **Red** orders vehicles to halt.
- **Green** grants right of way.
- **Yellow (Amber)** is an exclusive transition phase alerting moving drivers that Green has ended and Red is imminent. It allows drivers already inside the intersection to clear safely, and oncoming drivers to brake smoothly without skidding. Placing Yellow before Green would encourage dangerous early acceleration before cross-traffic has cleared.

#### Q1.3: What is the purpose of current-limiting resistors connected to each LED?
**Answer:**  
LEDs have low dynamic forward resistance once conducting. A direct connection to a $+5\text{V}$ digital output pin would draw excess current ($> 80\text{ mA}$), far exceeding the PIC18F absolute maximum pin rating of $25\text{ mA}$. A $330\,\Omega$ resistor limits the current to approximately $10\text{ mA}$, protecting the internal output transistors and preventing LED thermal burnout.

---

### Level 2: Easy / Direct Implementation Questions

#### Q2.1: Explain why `PORTB = 0x04;` turns ON the Green LED and turns OFF the others.
**Answer:**  
Hexadecimal `0x04` translates to binary `0000 0100`:
- Bit 0 (`RB0` - Red) = `0` $\to$ Red LED OFF
- Bit 1 (`RB1` - Yellow) = `0` $\to$ Yellow LED OFF
- Bit 2 (`RB2` - Green) = `1` $\to$ Green LED ON  
Assigning a full-byte literal (`PORTB = 0x04`) writes all 8 port bits simultaneously in a single instruction cycle, ensuring atomic transition with zero unwanted overlap between lights.

#### Q2.2: What happens if you forget `#pragma config WDT = OFF`?
**Answer:**  
The **Watchdog Timer (WDT)** is an on-chip RC oscillator counter designed to reset the microcontroller if software hangs or enters an infinite deadlock. The nominal WDT timeout period is roughly $4\text{ ms}$ to $131\text{ ms}$ (depending on prescalers). If `WDT = ON`, the 5-second `delay_ms(5000)` blocking loop will fail to clear the watchdog timer with `ClrWdt()`. Consequently, the watchdog will overflow and forcefully reset the microcontroller every few milliseconds, causing the Red LED to flicker continuously without ever reaching the Green state.

#### Q2.3: How does the software delay function generate 5 seconds?
**Answer:**  
The `delay_ms(unsigned int ms)` function executes a nested loop. The inner loop executes 165 times, taking approximately 1 millisecond at $8\text{ MHz}$ ($0.5\,\mu\text{s}$ per instruction cycle). When called with `delay_ms(5000)`, the outer loop iterates 5,000 times, producing a total delay of $5000 \times 1\text{ ms} = 5,000\text{ ms} = 5.0\text{ seconds}$.

---

### Level 3: Moderate / Practical Circuit & System Design Questions

#### Q3.1: Why are software busy-wait loops (`delay_ms`) considered unacceptable in commercial smart traffic controllers?
**Answer:**  
A software delay loop completely monopolizes the CPU in a continuous decrement cycle (`while(j--)`). During these 5 seconds:
1. The microcontroller cannot read pedestrian pushbuttons.
2. It cannot process inductive loop vehicle sensors or radar traffic cameras.
3. It cannot respond to emergency vehicle sirens or central dispatch commands.  
Commercial systems use **hardware timer interrupts** or **Real-Time Operating Systems (RTOS)** with non-blocking timers.

#### Q3.2: How would you expand this single-road light into a 4-Way Traffic Intersection (12 LEDs total)?
**Answer:**  
A 4-way intersection requires 4 sets of Red/Yellow/Green signals (12 LEDs total):
- **North-South (NS) Road:** 1 Red, 1 Yellow, 1 Green (3 pins)
- **East-West (EW) Road:** 1 Red, 1 Yellow, 1 Green (3 pins)
Since opposite directions (North and South) mirror each other, we only require **6 GPIO pins** in total:
- Pins `RB0-RB2` control North-South (both North and South signal heads wired in parallel or driven via buffer ICs).
- Pins `RB3-RB5` control East-West (both East and West signal heads).
State progression:
1. **State 1:** NS Green (`RB2=1`), EW Red (`RB3=1`) for 15s.
2. **State 2:** NS Yellow (`RB1=1`), EW Red (`RB3=1`) for 3s.
3. **State 3:** NS Red (`RB0=1`), EW Green (`RB5=1`) for 15s.
4. **State 4:** NS Red (`RB0=1`), EW Yellow (`RB4=1`) for 3s.

#### Q3.3: How would you incorporate a Pedestrian Crossing Pushbutton into this design?
**Answer:**  
1. Connect an active-low pushbutton with an external $10\text{ k}\Omega$ pull-up resistor to external interrupt pin `INT0` (`RB0` or relocated to `RB4`).
2. Configure `INT0` for a falling-edge trigger:
   ```c
   INTCONbits.INT0IE = 1; // Enable INT0 interrupt
   INTCON2bits.INTEDG0 = 0; // Trigger on falling edge (button press)
   ```
3. In the Interrupt Service Routine (ISR), set a global flag `pedestrian_requested = 1`.
4. The main state machine checks this flag and abbreviates the vehicle Green phase smoothly, introducing a Pedestrian Walk phase (Vehicle Red + Pedestrian Green) with acoustic beeping.

---

### Level 4: Tough / In-Depth & Troubleshooting Questions

#### Q4.1: How would you design an Emergency Vehicle Priority Override using hardware interrupts?
**Answer:**  
1. Connect an optical strobe sensor (detecting $14\text{ Hz}$ emergency vehicle flashes) or an RF receiver module to high-priority interrupt pin `INT1` (`RB1` or `PORTB` pin).
2. Configure `INTCON3bits.INT1IP = 1;` for High-Priority Interrupt.
3. When triggered, the ISR immediately forces:
   - Current active Green light to transition to Yellow for $2\text{ s}$ clearance, then to Red.
   - Emergency route signal transitions immediately to Green.
   - All other conflicting directions are latched into solid Red.
4. An emergency hold timer maintains the corridor until the vehicle clears, then returns the FSM smoothly to its pre-interruption state.

#### Q4.2: How would you implement this entire traffic controller using Timer 1 interrupts without any software blocking loops?
**Answer:**  
1. Configure **Timer 1** with a 1:8 prescaler to generate an interrupt every $100\text{ ms}$ (10 Hz tick).
2. Maintain a global tick counter and an FSM state variable:
   ```c
   enum TrafficState { STATE_RED, STATE_GREEN, STATE_YELLOW } current_state;
   unsigned int state_timer_ticks = 0;

   void timer1_isr(void) {
       if(PIR1bits.TMR1IF) {
           PIR1bits.TMR1IF = 0;
           TMR1H = RELOAD_H; TMR1L = RELOAD_L;
           state_timer_ticks++;
           
           switch(current_state) {
               case STATE_RED:
                   PORTB = 0x01;
                   if(state_timer_ticks >= 50) { // 50 * 100ms = 5.0s
                       current_state = STATE_GREEN;
                       state_timer_ticks = 0;
                   }
                   break;
               case STATE_GREEN:
                   PORTB = 0x04;
                   if(state_timer_ticks >= 50) { // 5.0s
                       current_state = STATE_YELLOW;
                       state_timer_ticks = 0;
                   }
                   break;
               case STATE_YELLOW:
                   PORTB = 0x02;
                   if(state_timer_ticks >= 20) { // 20 * 100ms = 2.0s
                       current_state = STATE_RED;
                       state_timer_ticks = 0;
                   }
                   break;
           }
       }
   }
   ```
3. The foreground `main()` loop remains non-blocked and can monitor serial diagnostic telemetry or external sensors.

#### Q4.3: In real municipal traffic installations, what happens if the microcontroller experiences a firmware crash or hardware lockup?
**Answer:**  
Commercial traffic intersection controllers incorporate an independent hardware supervisory circuit called a **Conflict Monitor Unit (CMU)** or **Malfunction Management Unit (MMU)**:
1. **Watchdog Timer (WDT) Recovery:** If the MCU hangs in software, the on-chip WDT resets the chip into a known initialization state.
2. **Hardware MMU Relay Interlock:** If the hardware detects simultaneous conflicting Green signals (e.g. North Green and East Green both ON due to short circuit or software bug), the external MMU forcefully trips an electromechanical relay that disconnects the MCU and puts the entire intersection into **All-Way Flashing Amber (Yellow) Mode** powered by a hardware multivibrator, warning all drivers to treat the intersection as a 4-way stop.
