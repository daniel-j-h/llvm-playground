#include <boost/program_options.hpp>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>


#include <cstdlib>

#include <string>
#include <iostream>
#include <stdexcept>


int main(int argc, char** argv) try {
  // Commandline Arguments
  boost::program_options::options_description optionDescription{"Options"};
  optionDescription.add_options()                                                      //
      ("help", "Help on Usage")                                                        //
      ("IRFile", boost::program_options::value<std::string>()->required(), "IR File"); //

  boost::program_options::variables_map variablesMap;
  store(boost::program_options::command_line_parser{argc, argv}.options(optionDescription).run(), variablesMap);

  if (variablesMap.count("help")) {
    std::cerr << optionDescription;
    return EXIT_FAILURE;
  }

  notify(variablesMap);

  const auto IRFile = variablesMap["IRFile"].as<std::string>();


  // LLVM Setup
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

  auto* fourtyTwo = builder.getInt32(42);
  auto* retInst = builder.CreateRet(fourtyTwo);

  if (llvm::verifyFunction(*fn, &llvm::errs()))
    return EXIT_FAILURE;

  if (llvm::verifyModule(module, &llvm::errs()))
    return EXIT_FAILURE;

  module.dump();

} catch (const std::exception& e) {
  llvm::errs() << "Error: " << e.what() << '\n';
  return EXIT_FAILURE;
}
