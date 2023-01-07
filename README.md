# I2C-Rtc-Project

**Project goal**:
implementation of communication with the Real-Time Clock connected via I2C bus.

**Parameters**:

• In the Cube IDE configure I2C1 on pins PB8, PB9
• RTC I2C device address is 208 (0xD0)
• Write 7 bytes via I2C to RTC, starting from memory address 0:
    ◦ {0,0,0,1,1,1,1}
• Read 7 bytes from the RTC via I2C, starting from memory address 0
• Print the first read byte every second and see how it changes

• With the datasheets folder, locate and open the RTC's datasheet: DS1307.pdf
• Go to Table 2. Timekeeper Registers on page 8
• Understand the format of the 8 bytes you are reading from the RTC
• Implement an ADT called DateTime that holds all RTC data fields
• Poll the RTC for data every second
• parse the read bytes from RTC into DateTime, and print them in human-readable format ("YY:MM:DD:d:hh:mm: ss")

• replace the weekday to be a string ("Sunday", "Monday", etc.) instead of a number
• set the YY:MM:DD:d:hh:mm: ss via UART, using strtok and atoi function

**Hardware setup**:

On the attached shield the LEDs are not connected to the corresponding pins. So we need to join them.
Using provided connectors connect holes on the boards marked as L1 and L2 to pins 5 & 6 of the CN5 digital connector (see page 35 of Nucleo data sheet) PA5 & PA6.

• Implement UART commands that switch LEDs ON and OFF
• Save the LEDs state in RTC memory addresses 0x10 (16) and 0x18 (24)
• On program startup read the RTC memory to detect the LEDs' state and switch the LEDs correspondingly.
