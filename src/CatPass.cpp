#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Value.h"
#include <set>

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
        auto [listFrontInst, listNextInst, phiNodeInst] = isListLoop(loop, PDG);
        if (listFrontInst != nullptr && listNextInst != nullptr && phiNodeInst != nullptr) {

          // TODO for transformations
          // [DONE] make PHINode BE i
          // [DONE] remove our own outdated code
          // [DONE] remove dead code
          // benchmark (some point)

          std::set<Instruction*> instsToDelete = { phiNodeInst, listNextInst };

          auto voidPtr = PointerType::get(IntegerType::get(context, 8), 0);
          auto voidPtrPtr = PointerType::get(voidPtr, 0);
          ConstantInt* zero = ConstantInt::get(Type::getInt64Ty(context), 0);
          ConstantInt* one = ConstantInt::get(Type::getInt64Ty(context), 1);

          /* Inserting array transformation, call to size in preheader */
          auto preHeaderBlock = LS->getPreHeader()->getTerminator();
          std::vector<Value*> args;

          args.push_back(listFrontInst->getArgOperand(0));  
          auto structPtr = listFrontInst->getArgOperand(0)->getType();

          auto func_list_to_array = M.getOrInsertFunction("List_to_array", voidPtrPtr, structPtr).getCallee();
          CallInst* listToArrayInst = CallInst::Create(func_list_to_array, ArrayRef<Value*>(args), "ArrAy", preHeaderBlock);
          
          auto funcListSize = M.getOrInsertFunction("List_size", IntegerType::get(context, 64), structPtr).getCallee();
          CallInst* listSizeInst = CallInst::Create(funcListSize, ArrayRef<Value*>(args), "siZE", preHeaderBlock); 

          /* Inserting a new phiNode for I counter in the header */
          auto headerBlock = LS->getHeader()->getTerminator();

          auto phiNodeI = PHINode::Create(Type::getInt64Ty(context), 2, "phiI", phiNodeInst);
          phiNodeI->addIncoming(zero, LS->getPreHeader());

          auto cmpI = CmpInst::Create(Instruction::ICmp, CmpInst::ICMP_SLT, phiNodeI, listSizeInst, "cmpResult", headerBlock);

          /* Index into the array and get the current node here, in the header */
          auto gep = GetElementPtrInst::CreateInBounds(listToArrayInst, ArrayRef<Value*>({ phiNodeI }), "element", headerBlock);
          auto currValue = new LoadInst(voidPtr, gep, "curr", headerBlock);

          /* Add an increment to i in same block as Node_next */
          for (auto exitBlock : predecessors(LS->getHeader())) {
            if (exitBlock == LS->getPreHeader()) { continue; }

            auto nodeNextBlock = exitBlock->getTerminator();
            auto incI = BinaryOperator::Create(Instruction::Add, phiNodeI, one, "incI", nodeNextBlock);
            phiNodeI->addIncoming(incI, listNextInst->getParent());
          }

          /* Modifying the compare condition */
          if (auto branchInst = dyn_cast<BranchInst>(LS->getHeader()->getTerminator())) {
            if (auto cmpInst = dyn_cast<Instruction>(branchInst->getCondition())) {
              instsToDelete.insert(cmpInst);
            }
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
                  instsToDelete.insert(callInst);
                }
              }
            }
          }

          for (auto inst : instsToReplace) {
            inst->replaceAllUsesWith(currValue);
          }

          for (auto inst : instsToDelete) {
            inst->eraseFromParent();
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

    tuple<CallInst*, CallInst*, PHINode*> isListLoop(LoopDependenceInfo* loop, PDG* pdg) {
      auto LS = loop->getLoopStructure();
      auto sccManager = loop->getSCCManager();
      auto sccdag = sccManager->getSCCDAG();
      // errs() << "   New SCCDAG\n";

      auto phiNodePassed = false;
      auto cmpInstPassed = false;
      auto nodeNextPassed = false;

      CallInst* listFront;
      CallInst* nodeNext;
      PHINode* phiphi;

      auto topLevelNodes = sccdag->getTopLevelNodes();

      auto sccIterator = [&](SCC *scc) -> bool {
        // errs() << "   New SCC\n";
        for (auto node : topLevelNodes) {
          if (node->getT() == scc) { 
            // errs() << "Found top level node\n";
            break;
          }
          return false;
        }

        /*
          * Print the instructions that compose the SCC.
          */
        // errs() << "     Instructions:\n";
        auto mySCCIter = [&](Instruction *inst) mutable -> bool {
          // errs() << "       " << *inst << "\n";

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

      // errs() << "phi: " << phiNodePassed << "\n";
      // errs() << "cmp: " << cmpInstPassed << "\n";
      // errs() << "next: " << nodeNextPassed << "\n";

      
      if (phiNodePassed && cmpInstPassed && nodeNextPassed) {   

        // Condtion 1 satisfied - move on to condition 2
        // jk lol
        // jk not lol
        bool badDeps = false;
        auto dependenceIter = [&](Value* to, DGEdge<Value>* dep) -> bool {
          if (auto inst = dyn_cast<Instruction>(to)) {
            if (to != phiphi && !dep->isMemoryDependence() && LS->isIncluded(inst)) {
              badDeps = true;
              return false;
            }
          }          
          return false;
        };

        pdg->iterateOverDependencesFrom(nodeNext, false, true, true, dependenceIter);

        if (!badDeps) {
          errs() << "FOUND A LOOP\n";
          return {listFront, nodeNext, phiphi};
        }
      }

      return {nullptr, nullptr, nullptr};
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
