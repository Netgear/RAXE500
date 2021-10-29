#include <typedefs.h>

uint16 wl_srom_map_4365[590] = {
/* NOTE: */
/* BCM94366MC_1(p120) (dual band design)*/
/* 4366B1 */
/* PA values MUST be calibrated and refilled for the actual EEPROM/SROM-less design*/
/* MAC address are zero'ed */
/* CRC here is not recalculated, do not program this to EEPROM/SROM, this is for EEPROM/SROM-less use only */

#if 0 

/* 4366B1 */
/*  srom[000]:*/ 0x3801, 0x3000, 0x077b, 0x14e4, 0x0218, 0x1b7e, 0x0a00, 0x2bc4,
/*  srom[008]:*/ 0x2a64, 0x2964, 0x2c64, 0x3ce7, 0x480a, 0x113c, 0x2132, 0x3164,
/*  srom[016]:*/ 0x1852, 0x0005, 0x1f2e, 0x4df7, 0x8080, 0x000b, 0x8630, 0x0190,
/*  srom[024]:*/ 0x5f00, 0x41f6, 0x8630, 0x0191, 0x8300, 0x01eb, 0x9f01, 0x65f5,
/*  srom[032]:*/ 0x8200, 0xd800, 0x8010, 0x007c, 0x1f00, 0x0004, 0x8000, 0x200c,
/*  srom[040]:*/ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
/*  srom[048]:*/ 0x43c3, 0x8000, 0x0002, 0x0000, 0x3ff5, 0x1800, 0x0005, 0xffff,
/*  srom[056]:*/ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
/*  srom[064]:*/ 0x4d55, 0x1120, 0x1000, 0x1000, 0x0004, 0x0000, 0x0000, 0x0000,
/*  srom[072]:*/ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
/*  srom[080]:*/ 0x0f0f, 0x8585, 0x8547, 0x4747, 0x00ff, 0x1001, 0x0001, 0x78ff,
/*  srom[088]:*/ 0xffff, 0xffff, 0x9c40, 0xffff, 0x5a28, 0x7f7f, 0x6738, 0x6cd9,
/*  srom[096]:*/ 0x0000, 0xffff, 0x5b39, 0x0339, 0x0585, 0xa9bc, 0xa9bc, 0xa9bc,
/*  srom[104]:*/ 0xa9bc, 0x0862, 0x0042, 0x0005, 0x7777, 0x0e0e, 0x0e0e, 0x0e0e,
/*  srom[112]:*/ 0x0e0e, 0x6262, 0x6262, 0x6262, 0x6262, 0x6262, 0x6262, 0xffff,
/*  srom[120]:*/ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
/*  srom[128]:*/ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000,
/*  srom[136]:*/ 0x0000, 0x0000, 0x0000, 0xf8f8, 0xf8f8, 0xfefe, 0xfefe, 0xffff,
/*  srom[144]:*/ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
/*  srom[152]:*/ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
/*  srom[160]:*/ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x739c,
/*  srom[168]:*/ 0x2222, 0x2222, 0x2000, 0x5522, 0x2000, 0x5522, 0x2200, 0x0000,
/*  srom[176]:*/ 0x9400, 0xcc99, 0x9400, 0xcc99, 0x9400, 0xcc99, 0x0000, 0x0000,
/*  srom[184]:*/ 0x9400, 0xcc99, 0x9400, 0xcc99, 0x9400, 0xcc99, 0x0000, 0x0000,
/*  srom[192]:*/ 0x9400, 0xdd99, 0x9400, 0xdd99, 0x9400, 0xdd99, 0x0000, 0x0000,
/*  srom[200]:*/ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
/*  srom[208]:*/ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
/*  srom[216]:*/ 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0f12, 0xffff, 0xffff,
/*  srom[224]:*/ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
/*  srom[232]:*/ 0xffff, 0xffff, 0x9400, 0xdd99, 0x9400, 0xdd99, 0x9400, 0xdd99,
/*  srom[240]:*/ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x9400, 0xdd99, 0x9400,
/*  srom[248]:*/ 0xdd99, 0x9400, 0xdd99, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
/*  srom[256]:*/ 0x5050, 0x22ec, 0xb25b, 0x0000, 0x0000, 0xb3bb, 0x5050, 0x5050,
/*  srom[264]:*/ 0x20ca, 0xb655, 0x0000, 0x0000, 0x20d6, 0xb68e, 0x0000, 0x0000,
/*  srom[272]:*/ 0x211a, 0xb6db, 0x0000, 0x0000, 0x2156, 0xb6d3, 0x0000, 0x0000,
/*  srom[280]:*/ 0x21a0, 0xb691, 0x0000, 0x0000, 0x24de, 0xbf54, 0xf762, 0x0000,
/*  srom[288]:*/ 0x21bc, 0xc9ef, 0xf4a1, 0x0000, 0x21f8, 0xc827, 0xf54a, 0x0000,
/*  srom[296]:*/ 0x2260, 0xc74b, 0xf5e6, 0x0000, 0x225a, 0xc87f, 0xf556, 0x0000,
/*  srom[304]:*/ 0x227f, 0xc94b, 0xf4ff, 0x0000, 0x2821, 0xbca0, 0xf4fa, 0x0000,
/*  srom[312]:*/ 0x2842, 0xbab7, 0xf5e7, 0x0000, 0x287d, 0xbaa3, 0xf5f4, 0x0000,
/*  srom[320]:*/ 0x28cb, 0xb931, 0xf6c1, 0x0000, 0x2906, 0xb73e, 0xf7ae, 0x0000,
/*  srom[328]:*/ 0x5050, 0x21ca, 0xb46f, 0x0000, 0x0000, 0xb3bb, 0x5050, 0x5050,
/*  srom[336]:*/ 0x1ed0, 0xbc09, 0xfe76, 0x0000, 0x1ebb, 0xbcca, 0xfe40, 0x0000,
/*  srom[344]:*/ 0x1f79, 0xb8e9, 0x0000, 0x0000, 0x1fa8, 0xb891, 0x0000, 0x0000,
/*  srom[352]:*/ 0x1ff9, 0xb7f6, 0x0000, 0x0000, 0x2396, 0xbf44, 0xf89b, 0x0000,
/*  srom[360]:*/ 0x1f49, 0xcf85, 0xf37f, 0x0000, 0x1f09, 0xd0e0, 0xf2fd, 0x0000,
/*  srom[368]:*/ 0x203c, 0xca62, 0xf5bb, 0x0000, 0x20ce, 0xc6b3, 0xf717, 0x0000,
/*  srom[376]:*/ 0x212d, 0xc66d, 0xf6f4, 0x0000, 0x2419, 0xc613, 0xf3c0, 0x0000,
/*  srom[384]:*/ 0x23dc, 0xc5b3, 0xf41e, 0x0000, 0x256a, 0xbd46, 0xf746, 0x0000,
/*  srom[392]:*/ 0x262f, 0xb82c, 0xf972, 0x0000, 0x2682, 0xb653, 0xf9f6, 0x0000,
/*  srom[400]:*/ 0x5050, 0x22a3, 0xb292, 0x0000, 0x0000, 0xb3bb, 0x5050, 0x5050,
/*  srom[408]:*/ 0x1fb5, 0xba1e, 0xfe88, 0x0000, 0x1fb2, 0xba55, 0xfea5, 0x0000,
/*  srom[416]:*/ 0x2055, 0xb75c, 0x0000, 0x0000, 0x209c, 0xb6dc, 0x0000, 0x0000,
/*  srom[424]:*/ 0x20eb, 0xb630, 0x0000, 0x0000, 0x2498, 0xbe6d, 0xf7d3, 0x0000,
/*  srom[432]:*/ 0x1feb, 0xd159, 0xf1e4, 0x0000, 0x201a, 0xd00d, 0xf276, 0x0000,
/*  srom[440]:*/ 0x213d, 0xc9ca, 0xf504, 0x0000, 0x21a6, 0xc879, 0xf557, 0x0000,
/*  srom[448]:*/ 0x2245, 0xc593, 0xf65b, 0x0000, 0x2619, 0xc54b, 0xf210, 0x0000,
/*  srom[456]:*/ 0x2634, 0xc445, 0xf2a5, 0x0000, 0x2720, 0xbfc6, 0xf42b, 0x0000,
/*  srom[464]:*/ 0x2844, 0xb8a0, 0xf710, 0x0000, 0x2919, 0xb40b, 0xf88b, 0x0000,
/*  srom[472]:*/ 0x18c6, 0x6339, 0x6739, 0x6338, 0x6739, 0x6739, 0x0842, 0x0c63,
/*  srom[480]:*/ 0x0842, 0x0c62, 0x0c63, 0xffff, 0xffff, 0xffff, 0x0008, 0x0000,
/*  srom[488]:*/ 0x6f7b, 0x739c, 0x6f7b, 0x6f7b, 0x6f7c, 0x0001, 0x36bd, 0x0088,
/*  srom[496]:*/ 0x24ac, 0x24ac, 0x7ebd, 0x4888, 0x6cac, 0x6cac, 0x0000, 0x0000,
/*  srom[504]:*/ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x6f9b, 0x037b,
/*  srom[512]:*/ 0x5050, 0x2310, 0xb2b5, 0x0000, 0x0000, 0xb3bb, 0x5050, 0x5050,
/*  srom[520]:*/ 0x20d3, 0xb6ad, 0xfffc, 0x0000, 0x20bd, 0xb742, 0x0000, 0x0000,
/*  srom[528]:*/ 0x210e, 0xb76d, 0x0000, 0x0000, 0x2146, 0xb75d, 0x0000, 0x0000,
/*  srom[536]:*/ 0x21a8, 0xb711, 0x0000, 0x0000, 0x254f, 0xbc9a, 0xf89a, 0x0000,
/*  srom[544]:*/ 0x20ed, 0xce9d, 0xf2cc, 0x0000, 0x2143, 0xccbb, 0xf38f, 0x0000,
/*  srom[552]:*/ 0x21ba, 0xcad6, 0xf4b0, 0x0000, 0x2221, 0xc9b3, 0xf50e, 0x0000,
/*  srom[560]:*/ 0x222d, 0xca68, 0xf4d8, 0x0000, 0x277c, 0xc103, 0xf35b, 0x0000,
/*  srom[568]:*/ 0x27f5, 0xbd1f, 0xf4fd, 0x0000, 0x2869, 0xbcf3, 0xf506, 0x0000,
/*  srom[576]:*/ 0x281b, 0xbeef, 0xf489, 0x0000, 0x292b, 0xb852, 0xf724, 0x0000,
/*  srom[584]:*/ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x110d,
};

#else

/* 4366C0 */
/*  srom[000]:*/ 0x3801, 0x3000, 0x077b, 0x14e4, 0x0218, 0x1b7e, 0x0a00, 0x2bc4,
/*  srom[008]:*/ 0x2a64, 0x2964, 0x2c64, 0x3ce7, 0x480a, 0x113c, 0x2132, 0x3164,
/*  srom[016]:*/ 0x1852, 0x0005, 0x1f2e, 0x4df7, 0x8080, 0x000b, 0x8630, 0x0190,
/*  srom[024]:*/ 0x5f00, 0x41f6, 0x8630, 0x0191, 0x8300, 0x01eb, 0x9f01, 0x65f5,
/*  srom[032]:*/ 0x8200, 0xd800, 0x8010, 0x007c, 0x1f00, 0x0004, 0x8000, 0x200c,
/*  srom[040]:*/ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
/*  srom[048]:*/ 0x43c3, 0x8000, 0x0002, 0x0000, 0x3ff7, 0x1800, 0x0005, 0xffff,
/*  srom[056]:*/ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
/*  srom[064]:*/ 0x4d55, 0x1140, 0x1000, 0x1000, 0x0004, 0x0000, 0x0000, 0x0000,
/*  srom[072]:*/ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
/*  srom[080]:*/ 0x0f0f, 0x0202, 0x0202, 0x0202, 0x00ff, 0x1001, 0x0001, 0x78ff,
/*  srom[088]:*/ 0xffff, 0xffff, 0x9c40, 0xffff, 0x5a28, 0x7f7f, 0x6738, 0x6cd9,
/*  srom[096]:*/ 0x0000, 0xffff, 0x5b39, 0x0339, 0x0202, 0x0017, 0x292b, 0x5a3f,
/*  srom[104]:*/ 0xffff, 0x0862, 0x0042, 0x0005, 0xcaca, 0x8484, 0x8484, 0x8484,
/*  srom[112]:*/ 0x8484, 0xb8b8, 0xb8b8, 0xb8b8, 0xb8b8, 0xb8b8, 0xb8b8, 0xffff,
/*  srom[120]:*/ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
/*  srom[128]:*/ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x3838,
/*  srom[136]:*/ 0x3838, 0x3e3e, 0x3e3e, 0x3e3e, 0x3e3e, 0x3e3e, 0x3e3e, 0xffff,
/*  srom[144]:*/ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
/*  srom[152]:*/ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
/*  srom[160]:*/ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x739c,
/*  srom[168]:*/ 0x2222, 0x2222, 0x4220, 0x8866, 0x4220, 0x8866, 0x6422, 0x0000,
/*  srom[176]:*/ 0x9640, 0x2eca, 0x9640, 0x2eca, 0x9640, 0x2eca, 0x271a, 0xae45,
/*  srom[184]:*/ 0x9640, 0x2eca, 0x9640, 0x2eca, 0x9640, 0x2eca, 0xae43, 0xbc4b,
/*  srom[192]:*/ 0xa640, 0x40ec, 0xa640, 0x40ec, 0xa640, 0x40ec, 0xc652, 0x0000,
/*  srom[200]:*/ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
/*  srom[208]:*/ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
/*  srom[216]:*/ 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0f12, 0xffff, 0xffff,
/*  srom[224]:*/ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
/*  srom[232]:*/ 0xffff, 0xffff, 0xa640, 0x40ec, 0xa640, 0x40ec, 0xa640, 0x40ec,
/*  srom[240]:*/ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xa640, 0x40ec, 0xa640,
/*  srom[248]:*/ 0x40ec, 0xa640, 0x40ec, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
/*  srom[256]:*/ 0x5050, 0x239a, 0xb25b, 0x0000, 0x0000, 0xb3bb, 0x5050, 0x5050,
/*  srom[264]:*/ 0x216d, 0xb655, 0x0000, 0x0000, 0x217b, 0xb68e, 0x0000, 0x0000,
/*  srom[272]:*/ 0x2178, 0xb6db, 0x0000, 0x0000, 0x2156, 0xb6d3, 0x0000, 0x0000,
/*  srom[280]:*/ 0x21e8, 0xb691, 0x0000, 0x0000, 0x24de, 0xbf54, 0xf762, 0x0000,
/*  srom[288]:*/ 0x2253, 0xc9ef, 0xf4a1, 0x0000, 0x22ac, 0xc827, 0xf54a, 0x0000,
/*  srom[296]:*/ 0x2304, 0xc74b, 0xf5e6, 0x0000, 0x22e0, 0xc87f, 0xf556, 0x0000,
/*  srom[304]:*/ 0x22d7, 0xc94b, 0xf4ff, 0x0000, 0x28c3, 0xbca0, 0xf4fa, 0x0000,
/*  srom[312]:*/ 0x2908, 0xbab7, 0xf5e7, 0x0000, 0x2908, 0xbaa3, 0xf5f4, 0x0000,
/*  srom[320]:*/ 0x28cb, 0xb931, 0xf6c1, 0x0000, 0x29ba, 0xb73e, 0xf7ae, 0x0000,
/*  srom[328]:*/ 0x5050, 0x236d, 0xb46f, 0x0000, 0x0000, 0xb3bb, 0x5050, 0x5050,
/*  srom[336]:*/ 0x2047, 0xbc09, 0xfe76, 0x0000, 0x201e, 0xbcca, 0xfe40, 0x0000,
/*  srom[344]:*/ 0x20b2, 0xb8e9, 0x0000, 0x0000, 0x20ed, 0xb891, 0x0000, 0x0000,
/*  srom[352]:*/ 0x219f, 0xb7f6, 0x0000, 0x0000, 0x2396, 0xbf44, 0xf89b, 0x0000,
/*  srom[360]:*/ 0x20e5, 0xcf85, 0xf37f, 0x0000, 0x20ae, 0xd0e0, 0xf2fd, 0x0000,
/*  srom[368]:*/ 0x21bd, 0xca62, 0xf5bb, 0x0000, 0x225e, 0xc6b3, 0xf717, 0x0000,
/*  srom[376]:*/ 0x2290, 0xc66d, 0xf6f4, 0x0000, 0x2595, 0xc613, 0xf3c0, 0x0000,
/*  srom[384]:*/ 0x2584, 0xc5b3, 0xf41e, 0x0000, 0x26bd, 0xbd46, 0xf746, 0x0000,
/*  srom[392]:*/ 0x262f, 0xb82c, 0xf972, 0x0000, 0x2864, 0xb653, 0xf9f6, 0x0000,
/*  srom[400]:*/ 0x5050, 0x2366, 0xb292, 0x0000, 0x0000, 0xb3bb, 0x5050, 0x5050,
/*  srom[408]:*/ 0x210f, 0xba1e, 0xfe88, 0x0000, 0x20f8, 0xba55, 0xfea5, 0x0000,
/*  srom[416]:*/ 0x21a7, 0xb75c, 0x0000, 0x0000, 0x221d, 0xb6dc, 0x0000, 0x0000,
/*  srom[424]:*/ 0x2274, 0xb630, 0x0000, 0x0000, 0x2498, 0xbe6d, 0xf7d3, 0x0000,
/*  srom[432]:*/ 0x218b, 0xd159, 0xf1e4, 0x0000, 0x21ae, 0xd00d, 0xf276, 0x0000,
/*  srom[440]:*/ 0x2316, 0xc9ca, 0xf504, 0x0000, 0x2359, 0xc879, 0xf557, 0x0000,
/*  srom[448]:*/ 0x243c, 0xc593, 0xf65b, 0x0000, 0x2775, 0xc54b, 0xf210, 0x0000,
/*  srom[456]:*/ 0x27cb, 0xc445, 0xf2a5, 0x0000, 0x2884, 0xbfc6, 0xf42b, 0x0000,
/*  srom[464]:*/ 0x2844, 0xb8a0, 0xf710, 0x0000, 0x2b04, 0xb40b, 0xf88b, 0x0000,
/*  srom[472]:*/ 0x18c6, 0x6339, 0x6739, 0x6338, 0x6739, 0x6739, 0x0842, 0x0c63,
/*  srom[480]:*/ 0x0842, 0x0c62, 0x0c63, 0xffff, 0xffff, 0xffff, 0x0008, 0x0000,
/*  srom[488]:*/ 0x6f7b, 0x739c, 0x6f7b, 0x6f7b, 0x6f7c, 0x0001, 0x36bd, 0x0088,
/*  srom[496]:*/ 0x24ac, 0x24ac, 0x7ebd, 0x4888, 0x6cac, 0x6cac, 0x0000, 0x0000,
/*  srom[504]:*/ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x6f9b, 0x037b,
/*  srom[512]:*/ 0x5050, 0x2354, 0xb2b5, 0x0000, 0x0000, 0xb3bb, 0x5050, 0x5050,
/*  srom[520]:*/ 0x2173, 0xb6ad, 0xfffc, 0x0000, 0x2177, 0xb742, 0x0000, 0x0000,
/*  srom[528]:*/ 0x2168, 0xb76d, 0x0000, 0x0000, 0x21d9, 0xb75d, 0x0000, 0x0000,
/*  srom[536]:*/ 0x2213, 0xb711, 0x0000, 0x0000, 0x254f, 0xbc9a, 0xf89a, 0x0000,
/*  srom[544]:*/ 0x220c, 0xce9d, 0xf2cc, 0x0000, 0x220e, 0xccbb, 0xf38f, 0x0000,
/*  srom[552]:*/ 0x2233, 0xcad6, 0xf4b0, 0x0000, 0x22bb, 0xc9b3, 0xf50e, 0x0000,
/*  srom[560]:*/ 0x22d0, 0xca68, 0xf4d8, 0x0000, 0x2833, 0xc103, 0xf35b, 0x0000,
/*  srom[568]:*/ 0x28db, 0xbd1f, 0xf4fd, 0x0000, 0x290f, 0xbcf3, 0xf506, 0x0000,
/*  srom[576]:*/ 0x281b, 0xbeef, 0xf489, 0x0000, 0x29ea, 0xb852, 0xf724, 0x0000,
/*  srom[584]:*/ 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x720d,
};

#endif