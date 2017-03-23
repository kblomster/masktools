#ifndef __Mt_Deflate_H__
#define __Mt_Deflate_H__

#include "../morphologic.h"

namespace Filtering { namespace MaskTools { namespace Filters { namespace Morphologic { namespace Deflate {

extern Processor *deflate_c;
extern Processor *deflate_sse2;
extern Processor *deflate_asse2;

/* 16 bit */
extern StackedProcessor *deflate_stacked_c;
extern Processor16 *deflate_native_c;

extern Processor32 *deflate32_c;

class Deflate : public Morphologic::MorphologicFilter
{
public:
  Deflate(const Parameters &parameters, CpuFlags cpuFlags) : Morphologic::MorphologicFilter(parameters, (CpuFlags)cpuFlags)
  {
    int _bits_per_pixel = bit_depths[C];
    bool _isStacked = parameters["stacked"].toBool();
    if (_isStacked)
      _bits_per_pixel = 16;
    /* add the processors */
    if (_bits_per_pixel == 8) {
      processors.push_back(Filtering::Processor<Processor>(deflate_c, Constraint(CPU_NONE, 1, 1, 1, 1), 0));
      processors.push_back(Filtering::Processor<Processor>(deflate_sse2, Constraint(CPU_SSE2, MODULO_NONE, MODULO_NONE, ALIGNMENT_NONE, 16), 1));
      processors.push_back(Filtering::Processor<Processor>(deflate_asse2, Constraint(CPU_SSE2, MODULO_NONE, MODULO_NONE, ALIGNMENT_16, 16), 2));
    }
    else if (_isStacked) {
      stackedProcessors.push_back(Filtering::Processor<StackedProcessor>(deflate_stacked_c, Constraint(CPU_NONE, 1, 1, 1, 1), 0));
    }
    else if (_bits_per_pixel <= 16) {
      processors16.push_back(Filtering::Processor<Processor16>(deflate_native_c, Constraint(CPU_NONE, 1, 1, 1, 1), 0));
    }
    else {
      processors32.push_back(Filtering::Processor<Processor32>(deflate32_c, Constraint(CPU_NONE, 1, 1, 1, 1), 0));
    }
  }

  static Signature Deflate::filter_signature()
  {
    Signature signature = "mt_deflate";

    signature.add(Parameter(TYPE_CLIP, ""));
    signature.add(Parameter(TYPE_FLOAT, "thY")); // overwritten to default 255..65535 in morphologic.h
    signature.add(Parameter(TYPE_FLOAT, "thC"));
    signature.add(Parameter(false, "stacked"));
    signature.add(Parameter(String("i8"), "paramscale")); // like in expressions + none

    return add_defaults(signature);
  }
};

} } } } } // namespace Deflate, Morphologic, Filter, MaskTools, Filtering

#endif