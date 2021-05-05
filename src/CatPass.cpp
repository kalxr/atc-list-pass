#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Value.h"

#include "Noelle.hpp"

using namespace llvm::noelle ;

namespace {

  struct CAT : public ModulePass {
    static char ID; 

    CAT() : ModulePass(ID) {}

    bool doInitialization (Module &M) override {
      return false;
    }

    bool runOnModule (Module &M) override {
      LLVMContext &context = M.getContext();

      /*
       * Fetch NOELLE
       */
      auto& noelle = getAnalysis<Noelle>();

      /*
       * Use NOELLE
       */
      // auto fm = noelle.getFunctionsManager();
      // auto mainF = fm->getEntryFunction();

      auto loopStructures = noelle.getLoopStructures();
      auto PDG = noelle.getProgramDependenceGraph();


      for (auto LS : *loopStructures) {
        auto [listFrontInst, listNextInst] = isListLoop(LS);
        if (listFrontInst != nullptr && listNextInst != nullptr) {

          auto terminator = LS->getPreHeader()->getTerminator();
          std::vector<Value*> args;

          auto voidPtr = PointerType::get(IntegerType::get(context, 8), 0);
          auto voidPtrPtr = PointerType::get(voidPtr, 0);
          auto structPtr = PointerType::get(StructType::get(context, "List"), 0);

          // Doesn't work yet
          auto func = M.getOrInsertFunction("List_to_array", voidPtrPtr, structPtr);

          // Does work if we call List_to_array in sample.c
          auto func_list_to_array = M.getFunction("List_to_array");
          args.push_back(llvm::cast<Value>(listFrontInst->getArgOperand(0)));          
          CallInst* listToArrayInst = CallInst::Create(func_list_to_array, ArrayRef<Value*>(args), "ArrAy", terminator);
          
          auto funcListSize = M.getFunction("List_size");
          CallInst* listSizeInst = CallInst::Create(funcListSize, ArrayRef<Value*>(args), "siZE", terminator);

          auto initI = new AllocaInst(IntegerType::get(context, 64), 0, "iiIi", terminator); 

          ConstantInt* zero = ConstantInt::get(Type::getInt64Ty(context), 0);
          auto setIZero = new StoreInst(zero, initI, terminator);

          terminator = LS->getHeader()->getTerminator();
          auto loadI = new LoadInst(IntegerType::get(context, 64), initI, "giraffe", terminator);
          auto cmpI = CmpInst::Create(Instruction::ICmp, CmpInst::ICMP_SLT, loadI, listSizeInst, "cmpResult", terminator);

          // Index into the array and get the current node here
          // %30 = load i32, i32* %5, align 4 (load i)
          // %31 = sext i32 %30 to i64 (something??)
          // %32 = getelementptr inbounds i8*, i8** %29, i64 %31 (get pointer to node in array)
          std::vector<Value*> indicies = { loadI };
          auto gep = GetElementPtrInst::CreateInBounds(listToArrayInst, ArrayRef<Value*>(indicies), "element", terminator);
          // // %33 = load i8*, i8** %32, align 8 (load current node)
          auto curr = new LoadInst(voidPtr, gep, "curr", terminator);


          for (auto exitBlock : predecessors(LS->getHeader())) {
            if (exitBlock == LS->getPreHeader()) { continue; }

            terminator = exitBlock->getTerminator();
            auto loadI = new LoadInst(IntegerType::get(context, 64), initI, "loadI", terminator);

            ConstantInt* one = ConstantInt::get(Type::getInt64Ty(context), 1);
            auto incI = BinaryOperator::Create(Instruction::Add, loadI, one, "incI", terminator);

            auto storeI = new StoreInst(incI, initI, terminator);
          }

          /* Modifying the compare condition */
          if (auto branchInst = dyn_cast<BranchInst>(LS->getHeader()->getTerminator())) {
            branchInst->setCondition(cmpI);
          }

          std::vector<Instruction*> instsToReplace;
          for (auto bb : LS->getBasicBlocks()) {
            for (auto &inst : *bb) {
              if (auto callInst = dyn_cast<CallInst>(&inst)) {

                // perhaps just remove these Node_get with a condition
                if (callInst->getCalledFunction()->getName() == "Node_get") {
                  auto targetNode = callInst->getArgOperand(0);
                  instsToReplace.push_back(callInst);
                }
              }
            }
          }

          for (auto inst : instsToReplace) {
            inst->replaceAllUsesWith(curr);
          }
        }
      }


      return false;
    }

    bool isNodePointer(Value* v) {
      auto vType = v->getType();
      if (auto vPointerType = dyn_cast<llvm::PointerType>(vType)) { 
        auto structName = vPointerType->getElementType()->getStructName();
        if (structName != "struct.NodeData") {
          return false;
        }
      } else {
        return false;
      } 

      return true;
    }

    tuple<CallInst*, CallInst*> isListLoop(LoopStructure* LS) {
      /*
        * Print the first instruction the loop executes.
        */
      // auto LS = loop->getLoopStructure();
      auto entryInst = LS->getEntryInstruction();
      errs() << "Loop " << *entryInst << "\n";

      auto header = LS->getHeader();
      CallInst* listFrontInst;
      CallInst* listNextInst;

      for (auto &inst : *header) {
        errs() << inst << "\n";

        if (auto cmpInst = dyn_cast<CmpInst>(&inst)) {
          if (cmpInst->getPredicate() == llvm::CmpInst::ICMP_NE) {
            Value* lhs = cmpInst->getOperand(0);
            Value* rhs = cmpInst->getOperand(1);

            // Verify types of the comparison 
            if (!((isNodePointer(lhs) && isa<llvm::ConstantPointerNull>(rhs)) || 
                 (isNodePointer(rhs) && isa<llvm::ConstantPointerNull>(lhs)))) {
              return {nullptr, nullptr};
            }

            // Get variable for Node in comparison
            Instruction* inst;
            if (Instruction* nodeInst = dyn_cast<Instruction>(lhs)) {
              inst = nodeInst;
            }
            if (Instruction* nodeInst = dyn_cast<Instruction>(rhs)) {
              inst = nodeInst;
            }

            bool definedByListFront = false;
            bool isUpdatedWithNext = false;
            if (auto loadInst = dyn_cast<LoadInst>(inst)) {
              auto pointerOperand = loadInst->getPointerOperand();

              for (auto user : pointerOperand->users()) {
                if (auto storeInst = dyn_cast<StoreInst>(user)) {
                  auto storedValue = storeInst->getOperand(0);
                  
                  if (auto listFrontCall = dyn_cast<CallInst>(storedValue)) {
                    if (listFrontCall->getCalledFunction()->getName() == "List_front") {
                      definedByListFront = true;
                      listFrontInst = listFrontCall;
                      errs() << "lookee here1    " << *listFrontCall << "\n";
                      errs() << "lookee here2    " << *listFrontInst << "\n";
                    }  

                    if (listFrontCall->getCalledFunction()->getName() == "Node_next") {
                      listNextInst = listFrontCall;
                      isUpdatedWithNext = true;
                    } 
                  }
                }
              }
            }

            if (!definedByListFront || !isUpdatedWithNext) {
              return {nullptr, nullptr};
            }
          }
          else return {nullptr, nullptr};
        }
      }

      errs() << "found the pattern\n";
      return {listFrontInst, listNextInst};
    }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.addRequired<Noelle>();
    }
  };

}

// Next there is code to register your pass to "opt"
char CAT::ID = 0;
static RegisterPass<CAT> X("CAT", "Simple user of the Noelle framework");

// Next there is code to register your pass to "clang"
static CAT * _PassMaker = NULL;
static RegisterStandardPasses _RegPass1(PassManagerBuilder::EP_OptimizerLast,
    [](const PassManagerBuilder&, legacy::PassManagerBase& PM) {
        if(!_PassMaker){ PM.add(_PassMaker = new CAT());}}); // ** for -Ox
static RegisterStandardPasses _RegPass2(PassManagerBuilder::EP_EnabledOnOptLevel0,
    [](const PassManagerBuilder&, legacy::PassManagerBase& PM) {
        if(!_PassMaker){ PM.add(_PassMaker = new CAT()); }}); // ** for -O0
