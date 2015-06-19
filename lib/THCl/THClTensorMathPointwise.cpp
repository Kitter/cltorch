#include "THClTensorMath.h"
#include "THClGeneral.h"
//#include "THClBlas.h"
#include "THClTensorCopy.h"
//#include "THClTensorRandom.h"
#include "THClApply.h"
//#include "THClReduce.cuh"
#include "THClTensorMathPointwise.h"

#ifndef DIVUP
#define DIVUP(x, y) (((x) + (y) - 1) / (y))
#endif

#define IMPLEMENT_CL_TENSOR_BASIC_FUNC(NAME, CFUNC)                   \
  void THClTensor_##NAME(THClState* state, THClTensor* self_, THClTensor* src) { \
    THAssert(THClTensor_checkGPU(state, 2, self_, src));                \
    if (self_ == src) {                                                 \
      if (!THClTensor_pointwiseApply1(state, self_, TensorGenOp(#CFUNC))) { \
        THArgCheck(false, 2, CLTORCH_DIM_WARNING); \
      }                                                                 \
    } else {                                                            \
      THClTensor_resizeAs(state, self_, src);                         \
                                                                        \
      if (!THClTensor_pointwiseApply2(state, self_, src, TensorGenOp(#CFUNC))) { \
        THArgCheck(false, 2, CLTORCH_DIM_WARNING); \
      }                                                                 \
    }                                                                   \
                                                                        \
  }

IMPLEMENT_CL_TENSOR_BASIC_FUNC(log, native_log)
IMPLEMENT_CL_TENSOR_BASIC_FUNC(log1p, log1p)
IMPLEMENT_CL_TENSOR_BASIC_FUNC(exp, native_exp)
IMPLEMENT_CL_TENSOR_BASIC_FUNC(cos, native_cos)
IMPLEMENT_CL_TENSOR_BASIC_FUNC(acos, acos)
IMPLEMENT_CL_TENSOR_BASIC_FUNC(cosh, cosh)
IMPLEMENT_CL_TENSOR_BASIC_FUNC(sin, native_sin)
IMPLEMENT_CL_TENSOR_BASIC_FUNC(asin, asin)
IMPLEMENT_CL_TENSOR_BASIC_FUNC(sinh, sinh)
IMPLEMENT_CL_TENSOR_BASIC_FUNC(tan, native_tan)
IMPLEMENT_CL_TENSOR_BASIC_FUNC(pow, native_powr)
IMPLEMENT_CL_TENSOR_BASIC_FUNC(atan, atan)
IMPLEMENT_CL_TENSOR_BASIC_FUNC(tanh, tanh)
IMPLEMENT_CL_TENSOR_BASIC_FUNC(sqrt, native_sqrt)
IMPLEMENT_CL_TENSOR_BASIC_FUNC(ceil, ceil)
IMPLEMENT_CL_TENSOR_BASIC_FUNC(floor, floor)
IMPLEMENT_CL_TENSOR_BASIC_FUNC(abs, fabs)
IMPLEMENT_CL_TENSOR_BASIC_FUNC(round, round)
IMPLEMENT_CL_TENSOR_BASIC_FUNC(neg, -)

#undef IMPLEMENT_CL_TENSOR_BASIC_FUNC

void THClTensor_apply(THClState* state, THClTensor* self, char const *operation1) {
//  THAssert(THClTensor_checkGPU(state, 2, self, src));
//  if (self == src) {
    if (!THClTensor_pointwiseApply1(state, self, TensorGenOpFullInline1(operation1))) {
      THArgCheck(false, 2, CLTORCH_DIM_WARNING);
    }
//  } else {
//    THError("not implemented (though we could... :-) )");
//    THClTensor_resizeAs(state, self_, src);

//    if (!THClTensor_pointwiseApply2(state, self_, src, TensorGenOp(#CFUNC))) {
//      THArgCheck(false, 2, CLTORCH_DIM_WARNING);
//    }
//  }
}

void THClTensor_map(THClState* state, THClTensor* self, THClTensor *in1, char const *operation2) {
  THAssert(THClTensor_checkGPU(state, 2, self, in1));
//  if (self == src) {
    if (!THClTensor_pointwiseApply2(state, self, in1, TensorGenOpFullInline2(operation2))) {
      THArgCheck(false, 2, CLTORCH_DIM_WARNING);
    }
//  } else {
//    THError("not implemented (though we could... :-) )");
//    THClTensor_resizeAs(state, self_, src);

//    if (!THClTensor_pointwiseApply2(state, self_, src, TensorGenOp(#CFUNC))) {
//      THArgCheck(false, 2, CLTORCH_DIM_WARNING);
//    }
//  }
}

void THClTensor_cadd(THClState *state, THClTensor *self_, THClTensor* src1, float value, THClTensor *src2)
{
  THAssert(THClTensor_checkGPU(state, 3, self_, src1, src2));
  THArgCheck(THClTensor_nElement(state, src1) ==
             THClTensor_nElement(state, src2), 3, "sizes do not match");

  if (self_ == src1) {
    if (value == 1.0f) {
      // self += src2
      if (!THClTensor_pointwiseApply2(state, self_, src2, TensorAddOp())) {
        THArgCheck(false, 2, CLTORCH_DIM_WARNING);
      }
    } else {
      // self += value * src2
      if (!THClTensor_pointwiseApply2(state, self_, src2, TensorCAddOp(value))) {
        THArgCheck(false, 2, CLTORCH_DIM_WARNING);
      }
    }
  } else {
    THClTensor_resizeAs(state, self_, src1);

    if (value == 1.0f) {
      // self = src1 + src2
      if (!THClTensor_pointwiseApply3(state, self_, src1, src2, TensorAddOp())) {
        THArgCheck(false, 2, CLTORCH_DIM_WARNING);
      }
    } else {
      // self = src1 + value * src2
      if (!THClTensor_pointwiseApply3(state, self_, src1, src2, TensorCAddOp(value))) {
        THArgCheck(false, 2, CLTORCH_DIM_WARNING);
      }
    }
  }
}

void THClTensor_csub(THClState *state, THClTensor *self_, THClTensor* src1, float value, THClTensor *src2)
{
  THAssert(THClTensor_checkGPU(state, 3, self_, src1, src2));
  THArgCheck(THClTensor_nElement(state, src1) ==
             THClTensor_nElement(state, src2), 3, "sizes do not match");

  if (self_ == src1) {
    if (value == 1.0f) {
      // self += src2
      if (!THClTensor_pointwiseApply2(state, self_, src2, TensorSubOp())) {
        THArgCheck(false, 2, CLTORCH_DIM_WARNING);
      }
    } else {
      // self += value * src2
      if (!THClTensor_pointwiseApply2(state, self_, src2, TensorCSubOp(value))) {
        THArgCheck(false, 2, CLTORCH_DIM_WARNING);
      }
    }
  } else {
    THClTensor_resizeAs(state, self_, src1);

    if (value == 1.0f) {
      // self = src1 + src2
      if (!THClTensor_pointwiseApply3(state, self_, src1, src2, TensorSubOp())) {
        THArgCheck(false, 2, CLTORCH_DIM_WARNING);
      }
    } else {
      // self = src1 + value * src2
      if (!THClTensor_pointwiseApply3(state, self_, src1, src2, TensorCSubOp(value))) {
        THArgCheck(false, 2, CLTORCH_DIM_WARNING);
      }
    }
  }
}

void THClTensor_cmul(THClState *state, THClTensor *self_, THClTensor *src1, THClTensor *src2)
{
  THAssert(THClTensor_checkGPU(state, 3, self_, src1, src2));
  THArgCheck(THClTensor_nElement(state, src1) ==
             THClTensor_nElement(state, src2), 3, "sizes do not match");

  if (self_ == src1) {
    // self *= src2
    if (!THClTensor_pointwiseApply2(state, self_, src2, TensorMulOp())) {
      THArgCheck(false, 2, CLTORCH_DIM_WARNING);
    }
  } else {
    THClTensor_resizeAs(state, self_, src1);

    // self = src1 * src2
    if (!THClTensor_pointwiseApply3(state, self_, src1, src2, TensorMulOp())) {
      THArgCheck(false, 2, CLTORCH_DIM_WARNING);
    }
  }
}

