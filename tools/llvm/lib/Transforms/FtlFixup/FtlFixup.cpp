//===- FtlFixup ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Pass.h"
#include "llvm/IR/Attributes.h"
#include "llvm/Support/raw_ostream.h"

#include <set>
#include <string>
#include <utility>

using namespace llvm;

namespace {
    // FtlFixup - Mutate the apply function as needed
    struct FtlFixup : public FunctionPass {
        static char ID;

        FtlFixup() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override {
            if (F.hasFnAttribute("ftl_wasm_entry") || F.getName().equals("apply")) {
                Function *wasm_ctors = (Function *) F.getParent()->getOrInsertFunction("__wasm_call_ctors",
                                                                                       AttributeList{},
                                                                                       Type::getVoidTy(F.getContext()));
                Function *wasm_dtors = (Function *) F.getParent()->getOrInsertFunction("__cxa_finalize",
                                                                                       AttributeList{},
                                                                                       Type::getVoidTy(F.getContext()),
                                                                                       Type::getInt32Ty(
                                                                                               F.getContext()));

                IRBuilder<> builder(&F.getEntryBlock());
                builder.SetInsertPoint(&(F.getEntryBlock().front()));

                CallInst *wasm_ctor_call = builder.CreateCall(wasm_ctors, {}, "");
                if (const Function *F_ = dyn_cast<Function>(wasm_ctors->stripPointerCasts()))
                    wasm_ctor_call->setCallingConv(F_->getCallingConv());

                for (Function::iterator bb = F.begin(); bb != F.end(); bb++) {
                    if (isa<ReturnInst>((*bb).getTerminator())) {
                        builder.SetInsertPoint((*bb).getTerminator());
                        // for now just call with null
                        CallInst *wasm_dtor_call = builder.CreateCall(wasm_dtors, {Constant::getNullValue(
                                Type::getInt32Ty(F.getContext()))}, "");
                        if (const Function *F_ = dyn_cast<Function>(wasm_dtors->stripPointerCasts()))
                            wasm_dtor_call->setCallingConv(F_->getCallingConv());
                    }
                }

                return true;
            }
            return false;
        }
    };
}

char FtlFixup::ID = 0;
static RegisterPass<FtlFixup> X("ftl_fixup", "Fractal Fixup");

static void registerFtlFunctionPass(const PassManagerBuilder &, legacy::PassManagerBase &PM) {
    PM.add(new FtlFixup());
}

static RegisterStandardPasses RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible, registerFtlFunctionPass);
