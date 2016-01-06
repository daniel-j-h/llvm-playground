#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

#include <cstdlib>


int main(int /*argc*/, char** /*argv*/) {
  auto& context = llvm::getGlobalContext();
  llvm::Module module{"TopModule", context};

  llvm::IRBuilder<> builder{context};

  // fn :: unit -> int32
  llvm::Type* fnReturnTy = llvm::Type::getInt32Ty(context);
  llvm::FunctionType* fnTy = llvm::FunctionType::get(fnReturnTy, false);
  const auto fnLinkage = llvm::Function::ExternalLinkage;
  const char* fnName = "fn";

  llvm::Function* fn = llvm::Function::Create(fnTy, fnLinkage, fnName, &module);

  const char* fnBBName = "fnBasicBlock";
  auto* fnBasicBlock = llvm::BasicBlock::Create(context, fnBBName, fn);

  builder.SetInsertPoint(fnBasicBlock);

  (void)builder.CreateRet(builder.getInt32(42));

  if (llvm::verifyFunction(*fn, &llvm::errs()))
    return EXIT_FAILURE;

  if (llvm::verifyModule(module, &llvm::errs()))
    return EXIT_FAILURE;

  module.dump();
}
