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
      auto loops = noelle.getLoops();
      auto PDG = noelle.getProgramDependenceGraph();

      for (auto loop : *loops) {
        auto LS = loop->getLoopStructure();
        auto [listFrontInst, listNextInst] = isListLoop(loop, PDG);
        if (listFrontInst != nullptr && listNextInst != nullptr) {

          // TODO for transformations
          // make PHINode BE i
          // remove our own outdated code
          // remove dead code
          // benchmark (some point)

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

          // Right now we are just blindly substituting all Node_gets with the array curr node
          // TODO: How do we figure out that the node in the Node_get is actually the curr node??

          // only modify instructions that are reachable from the SSCDAG
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

    // Use the same condition when you're turning the list into an array

    // For now just manually traversing up the tree to see if the node in the null check is 
    // 1. Defined by listFront
    // 2. Updated with Node next
    // TOOD: How to make this more extensible?? Memory alias? generic traversal?

    // SCCDAG of 1, 2, 3, 5
    // 0 is front

    // 1 is phi with 2 entries (front, next) - and this is a source of the DAG
    // 2 the compare
    // 3 the break
    // 4 is body of the loop
    // 5 call to next
    // LoopDependenceInfo can give you the SCCDAG
    // data dependence between 0

    // 0 and 5 MUST alias (or data dependence- probably this)
    // no MAY data dependence between 0 or 5 AND any instruction of the loop

    tuple<CallInst*, CallInst*> isListLoop(LoopDependenceInfo* loop, PDG* pdg) {
      auto LS = loop->getLoopStructure();
      auto sccManager = loop->getSCCManager();
      auto sccdag = sccManager->getSCCDAG();
      errs() << "   New SCCDAG\n";

      auto phiNodePassed = false;
      auto cmpInstPassed = false;
      auto nodeNextPassed = false;

      CallInst* listFront;
      CallInst* nodeNext;
      PHINode* phiphi;

      auto topLevelNodes = sccdag->getTopLevelNodes();

      auto sccIterator = [&](SCC *scc) -> bool {
        errs() << "   New SCC\n";
        for (auto node : topLevelNodes) {
          if (node->getT() == scc) { 
            errs() << "Found top level node\n";
            break;
          }
          return false;
        }

        /*
          * Print the instructions that compose the SCC.
          */
        errs() << "     Instructions:\n";
        auto mySCCIter = [&](Instruction *inst) mutable -> bool {
          errs() << "       " << *inst << "\n";

          // Make sure phi node has 2 args to List_front and Node_get
          if (auto phiNode = dyn_cast<PHINode>(inst)) {
            if (phiNode->getNumIncomingValues() != 2) {
              return false;
            }

            std::string firstName;
            std::string secondName;
            auto firstCallInst = dyn_cast<CallInst>(phiNode->getIncomingValue(0));
            auto secondCallInst = dyn_cast<CallInst>(phiNode->getIncomingValue(1));
            if (firstCallInst) {
              firstName = firstCallInst->getCalledFunction()->getName();
            }
            if (secondCallInst) {
              secondName = secondCallInst->getCalledFunction()->getName();
            }
            if (firstName == "List_front" && secondName == "Node_next") {
              listFront = firstCallInst;
              nodeNext = secondCallInst;
            }
            else if (firstName == "Node_next" && secondName == "List_front") {
              nodeNext = firstCallInst;
              listFront = secondCallInst;
            }
            else return false;

            // Condition 2
            if (nodeNext->getArgOperand(0) != phiNode) {
              return false;
            }

            phiNodePassed = true;
            phiphi = phiNode;

          }

          // Compare Inst check
          if (auto cmpInst = dyn_cast<CmpInst>(inst)) {
            if (cmpInst->getPredicate() == llvm::CmpInst::ICMP_NE) {

              Value* lhs = cmpInst->getOperand(0);
              Value* rhs = cmpInst->getOperand(1);

              // Verify types of the comparison 
              if (!((isNodePointer(lhs) && isa<llvm::ConstantPointerNull>(rhs)) || 
                  (isNodePointer(rhs) && isa<llvm::ConstantPointerNull>(lhs)))) {
                return false;
              }
              cmpInstPassed = true;              
            }          
          }

          // Node_next check
          if (auto callInst = dyn_cast<CallInst>(inst)) {
            if (callInst->getCalledFunction()->getName() != "Node_next") {
              return false;
            }
            nodeNextPassed = true;
          }

          return false;
        };
        scc->iterateOverInstructions(mySCCIter);

        return false;
      };

      sccdag->iterateOverSCCs(sccIterator);

      errs() << "phi: " << phiNodePassed << "\n";
      errs() << "cmp: " << cmpInstPassed << "\n";
      errs() << "next: " << nodeNextPassed << "\n";

      
      if (phiNodePassed && cmpInstPassed && nodeNextPassed) {   

        // Condtion 1 satisfied - move on to condition 2
        // jk lol
        // jk not lol
        bool badDeps = false;
        auto dependenceIter = [&](Value* to, DGEdge<Value>* dep) -> bool {
          if (auto inst = dyn_cast<Instruction>(to)) {
            if (to != phiphi && !dep->isMemoryDependence() && LS->isIncluded(inst)) {
              errs() << dep->toString() << "\n" << *to << "\n\n";
              badDeps = true;
              return false;
            }
          }          
          return false;
        };

        errs() << "Gonna iterate to" << *listFront << "\n";
        pdg->iterateOverDependencesFrom(nodeNext, false, true, true, dependenceIter);

        if (!badDeps) {
          errs() << "FOUND A LOOP\n";
          return {listFront, nodeNext};
        }
      }

      return {nullptr, nullptr};
    }

    // tuple<CallInst*, CallInst*> isListLoop(LoopDependenceInfo* loop) {
    //   /*
    //     * Print the first instruction the loop executes.
    //     */
    //   auto LS = loop->getLoopStructure();

    //   auto entryInst = LS->getEntryInstruction();
    //   errs() << "Loop " << *entryInst << "\n";

    //   auto header = LS->getHeader();
    //   CallInst* listFrontInst;
    //   CallInst* listNextInst;

    //   for (auto &inst : *header) {
    //     errs() << inst << "\n";

    //     if (auto cmpInst = dyn_cast<CmpInst>(&inst)) {
    //       if (cmpInst->getPredicate() == llvm::CmpInst::ICMP_NE) {
    //         Value* lhs = cmpInst->getOperand(0);
    //         Value* rhs = cmpInst->getOperand(1);

    //         // Verify types of the comparison 
    //         if (!((isNodePointer(lhs) && isa<llvm::ConstantPointerNull>(rhs)) || 
    //              (isNodePointer(rhs) && isa<llvm::ConstantPointerNull>(lhs)))) {
    //           return {nullptr, nullptr};
    //         }

    //         // Get variable for Node in comparison
    //         Instruction* inst;
    //         if (Instruction* nodeInst = dyn_cast<Instruction>(lhs)) {
    //           inst = nodeInst;
    //         }
    //         if (Instruction* nodeInst = dyn_cast<Instruction>(rhs)) {
    //           inst = nodeInst;
    //         }

           
    //         bool definedByListFront = false;
    //         bool isUpdatedWithNext = false;
    //         if (auto loadInst = dyn_cast<LoadInst>(inst)) {
    //           auto pointerOperand = loadInst->getPointerOperand();

    //           for (auto user : pointerOperand->users()) {
    //             if (auto storeInst = dyn_cast<StoreInst>(user)) {
    //               auto storedValue = storeInst->getOperand(0);
                  
    //               if (auto listFrontCall = dyn_cast<CallInst>(storedValue)) {
    //                 if (listFrontCall->getCalledFunction()->getName() == "List_front") {
    //                   definedByListFront = true;
    //                   listFrontInst = listFrontCall;
    //                   errs() << "lookee here1    " << *listFrontCall << "\n";
    //                   errs() << "lookee here2    " << *listFrontInst << "\n";
    //                 }  

    //                 if (listFrontCall->getCalledFunction()->getName() == "Node_next") {
    //                   listNextInst = listFrontCall;
    //                   isUpdatedWithNext = true;
    //                 } 
    //               }
    //             }
    //           }
    //         }

    //         if (!definedByListFront || !isUpdatedWithNext) {
    //           return {nullptr, nullptr};
    //         }
    //       }
    //       else return {nullptr, nullptr};
    //     }
    //   }

    //   errs() << "found the pattern\n";
    //   return {listFrontInst, listNextInst};
    // }

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
