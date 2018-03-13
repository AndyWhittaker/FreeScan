# FreeScan
FreeScan is a ALDL Scan Tool for various GM based ALDL vehicles

## Background
When I owned my Lotus Esprit GT3 MY1997, I was concerned with the lack of diagnostic scan tools available. Therefore, I took apart the ECU, unplugged the MemCal, read in the binary code, disassembled it with IDA Pro and found out how the diagnostics worked.

What helped was having a friendly dealer who offered me a few supervised hours sniffing the protocol conversation with the car's ECU while certain diagnostic tasks were performed.

The result was FreeScan was born and can be seen [on my website HERE](https://www.andywhittaker.com/ecu/freescan/)

## I want to try this right now
In this era of pre-OBD-II protocols, each manufacturer has their own way of talking and interfacing to the ECU. Whereas OBD-II is a 12V protocol, ALDL is a 5V protocol. They also use their own type of interface connector too with a non-standard 8197 baud signalling speed.

This project is what kick-started my (ECUFix)(https://www.ecufix.com/shop/) business where years later I did the same thing to my Audi S4's ECU which was somewhat more complicated... Anyway, I used to sell ALDL interfaces but you could still hack one of the [USB FTDI FT232R ISO OBD-II / VAGCOM / Fiat ECU Scan / IAW Scan2](https://www.ecufix.com/shop/index.php?main_page=product_info&cPath=1&products_id=182) OBD-II interfaces to give you the +-5V signalling required - oh and it's USB too since trying to find an RS232 port on a computer is very difficult nowadays.

