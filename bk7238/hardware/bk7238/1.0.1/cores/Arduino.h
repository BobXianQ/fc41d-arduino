#include "ql_hal.h"
#include "stdlib_noniso.h"

void delay(uint32_t ms);
unsigned long millis();
unsigned long  micros();


typedef uint8_t byte;
typedef bool boolean;
typedef unsigned int word;


#ifdef __cplusplus
#include <algorithm>
#include <cmath>
#include "WCharacter.h"
#include "WString.h"
#include "Stream.h"
#include "Printable.h"
#include "Print.h"
#include "IPAddress.h"
#include "Client.h"
#include "Server.h"
#include "arduino_udp.h"
#include "HardwareSerial.h"

using std::abs;
using std::isinf;
using std::isnan;
using std::max;
using std::min;
using std::round;


void setup(void);
void loop(void);

long random(long);
long random(long, long);
void randomSeed(unsigned long);


#endif
