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


#ifndef LP_BLD_INIT_H
#define LP_BLD_INIT_H


#include "pipe/p_compiler.h"
#include "lp_bld.h"
#include <llvm-c/ExecutionEngine.h>


struct gallivm_state
{
   LLVMModuleRef module;
   LLVMExecutionEngineRef engine;
   LLVMTargetDataRef target;
   LLVMPassManagerRef passmgr;
   LLVMContextRef context;
   LLVMBuilderRef builder;
};


void
lp_build_init(void);


extern void
lp_func_delete_body(LLVMValueRef func);


void
gallivm_garbage_collect(struct gallivm_state *gallivm);


typedef void (*garbage_collect_callback_func)(void *cb_data);

void
gallivm_register_garbage_collector_callback(garbage_collect_callback_func func,
                                            void *cb_data);

void
gallivm_remove_garbage_collector_callback(garbage_collect_callback_func func,
                                          void *cb_data);


struct gallivm_state *
gallivm_create(void);

void
gallivm_destroy(struct gallivm_state *gallivm);


extern LLVMValueRef
lp_build_load_volatile(LLVMBuilderRef B, LLVMValueRef PointerVal,
                       const char *Name);


#endif /* !LP_BLD_INIT_H */
