#ifndef ALLOWANCEMASKS
#define ALLOWANCEMASKS

#define ALLOW_NONE  0x0
#define ALLOW_FOOT  0x1
#define ALLOW_BIKE  0x2
#define ALLOW_CAR   0x4
#define ALLOW_ALL   0x7

typedef enum Medium {CAR=0x4, FOOT=0x1, BIKE=0x2} TravelMedium;

#endif // ALLOWANCEMASKS

