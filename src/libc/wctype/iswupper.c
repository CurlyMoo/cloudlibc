// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wctype.h>

#include "wctype_impl.h"

int iswupper(wint_t wc) {
  static const uint32_t extents[] = {
      0x00020819, 0x00060016, 0x0006c006, 0x00080000, 0x00081000, 0x00082000,
      0x00083000, 0x00084000, 0x00085000, 0x00086000, 0x00087000, 0x00088000,
      0x00089000, 0x0008a000, 0x0008b000, 0x0008c000, 0x0008d000, 0x0008e000,
      0x0008f000, 0x00090000, 0x00091000, 0x00092000, 0x00093000, 0x00094000,
      0x00095000, 0x00096000, 0x00097000, 0x00098000, 0x00099000, 0x0009a000,
      0x0009b000, 0x0009c800, 0x0009d800, 0x0009e800, 0x0009f800, 0x000a0800,
      0x000a1800, 0x000a2800, 0x000a3800, 0x000a5000, 0x000a6000, 0x000a7000,
      0x000a8000, 0x000a9000, 0x000aa000, 0x000ab000, 0x000ac000, 0x000ad000,
      0x000ae000, 0x000af000, 0x000b0000, 0x000b1000, 0x000b2000, 0x000b3000,
      0x000b4000, 0x000b5000, 0x000b6000, 0x000b7000, 0x000b8000, 0x000b9000,
      0x000ba000, 0x000bb000, 0x000bc001, 0x000bd800, 0x000be800, 0x000c0801,
      0x000c2000, 0x000c3001, 0x000c4802, 0x000c7003, 0x000c9801, 0x000cb002,
      0x000ce001, 0x000cf801, 0x000d1000, 0x000d2000, 0x000d3001, 0x000d4800,
      0x000d6000, 0x000d7001, 0x000d8802, 0x000da800, 0x000db801, 0x000de000,
      0x000e2001, 0x000e3801, 0x000e5001, 0x000e6800, 0x000e7800, 0x000e8800,
      0x000e9800, 0x000ea800, 0x000eb800, 0x000ec800, 0x000ed800, 0x000ef000,
      0x000f0000, 0x000f1000, 0x000f2000, 0x000f3000, 0x000f4000, 0x000f5000,
      0x000f6000, 0x000f7000, 0x000f8801, 0x000fa000, 0x000fb002, 0x000fd000,
      0x000fe000, 0x000ff000, 0x00100000, 0x00101000, 0x00102000, 0x00103000,
      0x00104000, 0x00105000, 0x00106000, 0x00107000, 0x00108000, 0x00109000,
      0x0010a000, 0x0010b000, 0x0010c000, 0x0010d000, 0x0010e000, 0x0010f000,
      0x00110000, 0x00111000, 0x00112000, 0x00113000, 0x00114000, 0x00115000,
      0x00116000, 0x00117000, 0x00118000, 0x00119000, 0x0011d001, 0x0011e801,
      0x00120800, 0x00121803, 0x00124000, 0x00125000, 0x00126000, 0x00127000,
      0x001b8000, 0x001b9000, 0x001bb000, 0x001bf800, 0x001c3000, 0x001c4002,
      0x001c6000, 0x001c7001, 0x001c8810, 0x001d1808, 0x001e7800, 0x001ec000,
      0x001ed000, 0x001ee000, 0x001ef000, 0x001f0000, 0x001f1000, 0x001f2000,
      0x001f3000, 0x001f4000, 0x001f5000, 0x001f6000, 0x001f7000, 0x001fa000,
      0x001fb800, 0x001fc801, 0x001fe832, 0x00230000, 0x00231000, 0x00232000,
      0x00233000, 0x00234000, 0x00235000, 0x00236000, 0x00237000, 0x00238000,
      0x00239000, 0x0023a000, 0x0023b000, 0x0023c000, 0x0023d000, 0x0023e000,
      0x0023f000, 0x00240000, 0x00245000, 0x00246000, 0x00247000, 0x00248000,
      0x00249000, 0x0024a000, 0x0024b000, 0x0024c000, 0x0024d000, 0x0024e000,
      0x0024f000, 0x00250000, 0x00251000, 0x00252000, 0x00253000, 0x00254000,
      0x00255000, 0x00256000, 0x00257000, 0x00258000, 0x00259000, 0x0025a000,
      0x0025b000, 0x0025c000, 0x0025d000, 0x0025e000, 0x0025f000, 0x00260001,
      0x00261800, 0x00262800, 0x00263800, 0x00264800, 0x00265800, 0x00266800,
      0x00268000, 0x00269000, 0x0026a000, 0x0026b000, 0x0026c000, 0x0026d000,
      0x0026e000, 0x0026f000, 0x00270000, 0x00271000, 0x00272000, 0x00273000,
      0x00274000, 0x00275000, 0x00276000, 0x00277000, 0x00278000, 0x00279000,
      0x0027a000, 0x0027b000, 0x0027c000, 0x0027d000, 0x0027e000, 0x0027f000,
      0x00280000, 0x00281000, 0x00282000, 0x00283000, 0x00284000, 0x00285000,
      0x00286000, 0x00287000, 0x00288000, 0x00289000, 0x0028a000, 0x0028b000,
      0x0028c000, 0x0028d000, 0x0028e000, 0x0028f000, 0x00290000, 0x00291000,
      0x00292000, 0x00293000, 0x00294000, 0x00295000, 0x00296000, 0x00297000,
      0x00298825, 0x00850025, 0x00863800, 0x00866800, 0x009d0055, 0x00f00000,
      0x00f01000, 0x00f02000, 0x00f03000, 0x00f04000, 0x00f05000, 0x00f06000,
      0x00f07000, 0x00f08000, 0x00f09000, 0x00f0a000, 0x00f0b000, 0x00f0c000,
      0x00f0d000, 0x00f0e000, 0x00f0f000, 0x00f10000, 0x00f11000, 0x00f12000,
      0x00f13000, 0x00f14000, 0x00f15000, 0x00f16000, 0x00f17000, 0x00f18000,
      0x00f19000, 0x00f1a000, 0x00f1b000, 0x00f1c000, 0x00f1d000, 0x00f1e000,
      0x00f1f000, 0x00f20000, 0x00f21000, 0x00f22000, 0x00f23000, 0x00f24000,
      0x00f25000, 0x00f26000, 0x00f27000, 0x00f28000, 0x00f29000, 0x00f2a000,
      0x00f2b000, 0x00f2c000, 0x00f2d000, 0x00f2e000, 0x00f2f000, 0x00f30000,
      0x00f31000, 0x00f32000, 0x00f33000, 0x00f34000, 0x00f35000, 0x00f36000,
      0x00f37000, 0x00f38000, 0x00f39000, 0x00f3a000, 0x00f3b000, 0x00f3c000,
      0x00f3d000, 0x00f3e000, 0x00f3f000, 0x00f40000, 0x00f41000, 0x00f42000,
      0x00f43000, 0x00f44000, 0x00f45000, 0x00f46000, 0x00f47000, 0x00f48000,
      0x00f49000, 0x00f4a000, 0x00f4f000, 0x00f50000, 0x00f51000, 0x00f52000,
      0x00f53000, 0x00f54000, 0x00f55000, 0x00f56000, 0x00f57000, 0x00f58000,
      0x00f59000, 0x00f5a000, 0x00f5b000, 0x00f5c000, 0x00f5d000, 0x00f5e000,
      0x00f5f000, 0x00f60000, 0x00f61000, 0x00f62000, 0x00f63000, 0x00f64000,
      0x00f65000, 0x00f66000, 0x00f67000, 0x00f68000, 0x00f69000, 0x00f6a000,
      0x00f6b000, 0x00f6c000, 0x00f6d000, 0x00f6e000, 0x00f6f000, 0x00f70000,
      0x00f71000, 0x00f72000, 0x00f73000, 0x00f74000, 0x00f75000, 0x00f76000,
      0x00f77000, 0x00f78000, 0x00f79000, 0x00f7a000, 0x00f7b000, 0x00f7c000,
      0x00f7d000, 0x00f7e000, 0x00f7f000, 0x00f84007, 0x00f8c005, 0x00f94007,
      0x00f9c007, 0x00fa4005, 0x00fac800, 0x00fad800, 0x00fae800, 0x00faf800,
      0x00fb4007, 0x00fc4007, 0x00fcc007, 0x00fd4007, 0x00fdc004, 0x00fe4004,
      0x00fec003, 0x00ff4004, 0x00ffc004, 0x01093000, 0x01095001, 0x01099000,
      0x010b000f, 0x010c1800, 0x0125b019, 0x0160002e, 0x01630000, 0x01631002,
      0x01633800, 0x01634800, 0x01635800, 0x01636803, 0x01639000, 0x0163a800,
      0x0163f002, 0x01641000, 0x01642000, 0x01643000, 0x01644000, 0x01645000,
      0x01646000, 0x01647000, 0x01648000, 0x01649000, 0x0164a000, 0x0164b000,
      0x0164c000, 0x0164d000, 0x0164e000, 0x0164f000, 0x01650000, 0x01651000,
      0x01652000, 0x01653000, 0x01654000, 0x01655000, 0x01656000, 0x01657000,
      0x01658000, 0x01659000, 0x0165a000, 0x0165b000, 0x0165c000, 0x0165d000,
      0x0165e000, 0x0165f000, 0x01660000, 0x01661000, 0x01662000, 0x01663000,
      0x01664000, 0x01665000, 0x01666000, 0x01667000, 0x01668000, 0x01669000,
      0x0166a000, 0x0166b000, 0x0166c000, 0x0166d000, 0x0166e000, 0x0166f000,
      0x01670000, 0x01671000, 0x01675800, 0x01676800, 0x01679000, 0x05320000,
      0x05321000, 0x05322000, 0x05323000, 0x05324000, 0x05325000, 0x05326000,
      0x05327000, 0x05328000, 0x05329000, 0x0532a000, 0x0532b000, 0x0532c000,
      0x0532d000, 0x0532e000, 0x0532f000, 0x05330000, 0x05331000, 0x05332000,
      0x05333000, 0x05334000, 0x05335000, 0x05336000, 0x05340000, 0x05341000,
      0x05342000, 0x05343000, 0x05344000, 0x05345000, 0x05346000, 0x05347000,
      0x05348000, 0x05349000, 0x0534a000, 0x0534b000, 0x0534c000, 0x0534d000,
      0x05391000, 0x05392000, 0x05393000, 0x05394000, 0x05395000, 0x05396000,
      0x05397000, 0x05399000, 0x0539a000, 0x0539b000, 0x0539c000, 0x0539d000,
      0x0539e000, 0x0539f000, 0x053a0000, 0x053a1000, 0x053a2000, 0x053a3000,
      0x053a4000, 0x053a5000, 0x053a6000, 0x053a7000, 0x053a8000, 0x053a9000,
      0x053aa000, 0x053ab000, 0x053ac000, 0x053ad000, 0x053ae000, 0x053af000,
      0x053b0000, 0x053b1000, 0x053b2000, 0x053b3000, 0x053b4000, 0x053b5000,
      0x053b6000, 0x053b7000, 0x053bc800, 0x053bd800, 0x053be801, 0x053c0000,
      0x053c1000, 0x053c2000, 0x053c3000, 0x053c5800, 0x053c6800, 0x053c8000,
      0x053c9000, 0x053cb000, 0x053cc000, 0x053cd000, 0x053ce000, 0x053cf000,
      0x053d0000, 0x053d1000, 0x053d2000, 0x053d3000, 0x053d4000, 0x053d5003,
      0x053d8004, 0x053db000, 0x07f90819, 0x08200027, 0x08640032, 0x08c5001f,
  };
  return iswctype_extents(wc, extents, __arraycount(extents));
}
