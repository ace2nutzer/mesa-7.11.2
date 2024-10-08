/**************************************************************************
 *
 * Copyright 2009 VMware, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL VMWARE AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#include "util/u_memory.h"
#include "util/u_pointer.h"
#include "util/u_string.h"
#include "util/u_format.h"
#include "util/u_format_tests.h"
#include "util/u_format_s3tc.h"

#include "gallivm/lp_bld.h"
#include "gallivm/lp_bld_debug.h"
#include "gallivm/lp_bld_format.h"
#include "gallivm/lp_bld_init.h"

#include "lp_test.h"


void
write_tsv_header(FILE *fp)
{
   fprintf(fp,
           "result\t"
           "format\n");

   fflush(fp);
}


static void
write_tsv_row(FILE *fp,
              const struct util_format_description *desc,
              boolean success)
{
   fprintf(fp, "%s\t", success ? "pass" : "fail");

   fprintf(fp, "%s\n", desc->name);

   fflush(fp);
}


typedef void
(*fetch_ptr_t)(void *unpacked, const void *packed,
               unsigned i, unsigned j);


static LLVMValueRef
add_fetch_rgba_test(struct gallivm_state *gallivm, unsigned verbose,
                    const struct util_format_description *desc,
                    struct lp_type type)
{
   char name[256];
   LLVMContextRef context = gallivm->context;
   LLVMModuleRef module = gallivm->module;
   LLVMBuilderRef builder = gallivm->builder;
   LLVMPassManagerRef passmgr = gallivm->passmgr;
   LLVMTypeRef args[4];
   LLVMValueRef func;
   LLVMValueRef packed_ptr;
   LLVMValueRef offset = LLVMConstNull(LLVMInt32TypeInContext(context));
   LLVMValueRef rgba_ptr;
   LLVMValueRef i;
   LLVMValueRef j;
   LLVMBasicBlockRef block;
   LLVMValueRef rgba;

   util_snprintf(name, sizeof name, "fetch_%s_%s", desc->short_name,
                 type.floating ? "float" : "unorm8");

   args[0] = LLVMPointerType(lp_build_vec_type(gallivm, type), 0);
   args[1] = LLVMPointerType(LLVMInt8TypeInContext(context), 0);
   args[3] = args[2] = LLVMInt32TypeInContext(context);

   func = LLVMAddFunction(module, name,
                          LLVMFunctionType(LLVMVoidTypeInContext(context),
                                           args, Elements(args), 0));
   LLVMSetFunctionCallConv(func, LLVMCCallConv);
   rgba_ptr = LLVMGetParam(func, 0);
   packed_ptr = LLVMGetParam(func, 1);
   i = LLVMGetParam(func, 2);
   j = LLVMGetParam(func, 3);

   block = LLVMAppendBasicBlockInContext(context, func, "entry");
   LLVMPositionBuilderAtEnd(builder, block);

   rgba = lp_build_fetch_rgba_aos(gallivm, desc, type,
                                  packed_ptr, offset, i, j);

   LLVMBuildStore(builder, rgba, rgba_ptr);

   LLVMBuildRetVoid(builder);

   if (LLVMVerifyFunction(func, LLVMPrintMessageAction)) {
      LLVMDumpValue(func);
      abort();
   }

   LLVMRunFunctionPassManager(passmgr, func);

   if (verbose >= 1) {
      LLVMDumpValue(func);
   }

   return func;
}


PIPE_ALIGN_STACK
static boolean
test_format_float(struct gallivm_state *gallivm, unsigned verbose, FILE *fp,
                  const struct util_format_description *desc)
{
   LLVMValueRef fetch = NULL;
   LLVMExecutionEngineRef engine = gallivm->engine;
   fetch_ptr_t fetch_ptr;
   PIPE_ALIGN_VAR(16) float unpacked[4];
   boolean first = TRUE;
   boolean success = TRUE;
   unsigned i, j, k, l;
   void *f;

   fetch = add_fetch_rgba_test(gallivm, verbose, desc, lp_float32_vec4_type());

   f = LLVMGetPointerToGlobal(engine, fetch);
   fetch_ptr = (fetch_ptr_t) pointer_to_func(f);

#if 0
   if (verbose >= 2) {
      lp_disassemble(f);
   }
#endif

   for (l = 0; l < util_format_nr_test_cases; ++l) {
      const struct util_format_test_case *test = &util_format_test_cases[l];

      if (test->format == desc->format) {

         if (first) {
            printf("Testing %s (float) ...\n",
                   desc->name);
            first = FALSE;
         }

         for (i = 0; i < desc->block.height; ++i) {
            for (j = 0; j < desc->block.width; ++j) {
               boolean match;

               memset(unpacked, 0, sizeof unpacked);

               fetch_ptr(unpacked, test->packed, j, i);

               match = TRUE;
               for(k = 0; k < 4; ++k)
                  if (fabs((float)test->unpacked[i][j][k] - unpacked[k]) > FLT_EPSILON)
                     match = FALSE;

               if (!match) {
                  printf("FAILED\n");
                  printf("  Packed: %02x %02x %02x %02x\n",
                         test->packed[0], test->packed[1], test->packed[2], test->packed[3]);
                  printf("  Unpacked (%u,%u): %f %f %f %f obtained\n",
                         j, i,
                         unpacked[0], unpacked[1], unpacked[2], unpacked[3]);
                  printf("                  %f %f %f %f expected\n",
                         test->unpacked[i][j][0],
                         test->unpacked[i][j][1],
                         test->unpacked[i][j][2],
                         test->unpacked[i][j][3]);
                  success = FALSE;
               }
            }
         }
      }
   }

   if (!success) {
      if (verbose < 1) {
         LLVMDumpValue(fetch);
      }
   }

   LLVMFreeMachineCodeForFunction(engine, fetch);
   LLVMDeleteFunction(fetch);

   if(fp)
      write_tsv_row(fp, desc, success);

   return success;
}


PIPE_ALIGN_STACK
static boolean
test_format_unorm8(struct gallivm_state *gallivm,
                   unsigned verbose, FILE *fp,
                   const struct util_format_description *desc)
{
   LLVMValueRef fetch = NULL;
   fetch_ptr_t fetch_ptr;
   uint8_t unpacked[4];
   boolean first = TRUE;
   boolean success = TRUE;
   unsigned i, j, k, l;
   void *f;

   fetch = add_fetch_rgba_test(gallivm, verbose, desc, lp_unorm8_vec4_type());

   f = LLVMGetPointerToGlobal(gallivm->engine, fetch);
   fetch_ptr = (fetch_ptr_t) pointer_to_func(f);

#if 0
   if (verbose >= 2) {
      lp_disassemble(f);
   }
#endif

   for (l = 0; l < util_format_nr_test_cases; ++l) {
      const struct util_format_test_case *test = &util_format_test_cases[l];

      if (test->format == desc->format) {

         if (first) {
            printf("Testing %s (unorm8) ...\n",
                   desc->name);
            first = FALSE;
         }

         for (i = 0; i < desc->block.height; ++i) {
            for (j = 0; j < desc->block.width; ++j) {
               boolean match;

               memset(unpacked, 0, sizeof unpacked);

               fetch_ptr(unpacked, test->packed, j, i);

               match = TRUE;
               for(k = 0; k < 4; ++k) {
                  int error = float_to_ubyte(test->unpacked[i][j][k]) - unpacked[k];
                  if (error < 0)
                     error = -error;
                  if (error > 1)
                     match = FALSE;
               }

               if (!match) {
                  printf("FAILED\n");
                  printf("  Packed: %02x %02x %02x %02x\n",
                         test->packed[0], test->packed[1], test->packed[2], test->packed[3]);
                  printf("  Unpacked (%u,%u): %02x %02x %02x %02x obtained\n",
                         j, i,
                         unpacked[0], unpacked[1], unpacked[2], unpacked[3]);
                  printf("                  %02x %02x %02x %02x expected\n",
                         float_to_ubyte(test->unpacked[i][j][0]),
                         float_to_ubyte(test->unpacked[i][j][1]),
                         float_to_ubyte(test->unpacked[i][j][2]),
                         float_to_ubyte(test->unpacked[i][j][3]));
                  success = FALSE;
               }
            }
         }
      }
   }

   if (!success)
      LLVMDumpValue(fetch);

   LLVMFreeMachineCodeForFunction(gallivm->engine, fetch);
   LLVMDeleteFunction(fetch);

   if(fp)
      write_tsv_row(fp, desc, success);

   return success;
}




static boolean
test_one(struct gallivm_state *gallivm,
         unsigned verbose, FILE *fp,
         const struct util_format_description *format_desc)
{
   boolean success = TRUE;

   if (!test_format_float(gallivm, verbose, fp, format_desc)) {
     success = FALSE;
   }

   if (!test_format_unorm8(gallivm, verbose, fp, format_desc)) {
     success = FALSE;
   }

   return success;
}


boolean
test_all(struct gallivm_state *gallivm, unsigned verbose, FILE *fp)
{
   enum pipe_format format;
   boolean success = TRUE;

   util_format_s3tc_init();

   for (format = 1; format < PIPE_FORMAT_COUNT; ++format) {
      const struct util_format_description *format_desc;

      format_desc = util_format_description(format);
      if (!format_desc) {
         continue;
      }

      /*
       * TODO: test more
       */

      if (format_desc->colorspace == UTIL_FORMAT_COLORSPACE_ZS) {
         continue;
      }

      if (format_desc->layout == UTIL_FORMAT_LAYOUT_S3TC &&
          !util_format_s3tc_enabled) {
         continue;
      }

      if (!test_one(gallivm, verbose, fp, format_desc)) {
           success = FALSE;
      }
   }

   return success;
}


boolean
test_some(struct gallivm_state *gallivm, unsigned verbose, FILE *fp,
          unsigned long n)
{
   return test_all(gallivm, verbose, fp);
}


boolean
test_single(struct gallivm_state *gallivm, unsigned verbose, FILE *fp)
{
   printf("no test_single()");
   return TRUE;
}
