#ifndef _BINARYCONSTANT_H_
#define _BINARYCONSTANT_H_

#include "memlocdata.h"

// This all needs to go away. Exactly how, I don't yet know
void createBinaryConstantDataTypes(Trace * t);
extern const DataType * datatype_u8_le;
extern const DataType * datatype_s8_le;

extern const DataType * datatype_u16_le;
extern const DataType * datatype_s16_le;

extern const DataType * datatype_u32_le;
extern const DataType * datatype_s32_le;

extern const DataType * datatype_u64_le;
extern const DataType * datatype_s64_le;

#endif
