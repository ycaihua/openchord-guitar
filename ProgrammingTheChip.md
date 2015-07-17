# Introduction #

In order to get firmware on the chip, there are a couple methods. This article will go over programming the chip via both the default USB bootloader as well as ISP programming.

# Using the default USB bootloader #

If you've purchased a kit from us, then the chip that you bought comes pre-programmed with both the latest OpenChord firmware, but also with CChristian Starkjohann's [BootloadHID](http://www.obdev.at/products/vusb/bootloadhid.html).  This allows for easy programming via USB.

In order to program the guitar, first, unplug the guitar.  Then, while holding the "Plus" button, plug the guitar into your PC - The guitar should show up in windows as BootloadHID. (On older versions of the bootloader, you must continue to hold the button while you program the guitar, otherwise the guitar will go into normal guitar mode. This has been fixed in the newer code (February 2010))

That will get your PC to recognize the guitar as BootloadHID.  Now we need to upload the firmware onto the chip. In windows, you can use the Mario Steiner's [HIDBootFlash program](http://vusb.wikidot.com/project:hidbootflash) or the command-line tools which unfortunately must be built from source code on your system.  Just point whichever tool you're using to the .hex file you've either downloaded or compiled, and you're good to go.

Also, a note about the binaries - unfortunately, the circuit boards
have undergone some changes over the life of the product.  Most
importantly, with the May 2010 board revision, the pin mapping changed.
Be sure you know which board you have, so you can download the proper
binary.

Additionally, a few of the very first boards came with a 16mHz crystal,
instead of the current 12mHz crystal.  As of May 2010, we're no longer
compiling binaries for those automatically, in order to reduce confusion.
If you have one of those boards, either e-mail us and we'll compile you
a binary, or we'd be happy to send you a 12mHz crystal you can swap in
your board, free of charge.

# Using an external AVR programmer #

If you're starting with your own raw chip, then you'll need an AVR programmer.  There are many options available, but a simple ISP programmer like the AVRISP mkII does the job.  The PCB has a spot for you to solder on a 2x3 block of 0.1" header pins for a 6-pin ISP header. Also, if you've got a programmer then installing the bootloader onto your chip becomes optional if you're trying to save space, but it's still convenient.

I'll assume that if you've got a programmer, you can get .hex files onto a chip by yourself, so you can either download the pre-compiled binaries, or compile the source yourself.  The easiest way will be to use the .ELF files, since they already have the fuse settings all set up, along with the default EEPROM set up.  This will just install the USB bootloader, and you'll have to use that to load your code.  Alternatively, you can install the guitar firmware onto the chip, then have no bootloader, saving a few KB's of space for code. Finally, if you want, you can install the bootloader from the .HEX file as well, although if you do, make sure to also set the lock bits to prevent SPM or LPM in the boot section, otherwise the bootloader will overwrite itself and be unusable.